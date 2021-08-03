#include "freertos/FreeRTOS.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"

#include "nvs_flash.h"

#include "driver/gpio.h"

/*#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"*/

#include "esp_spiffs.h"
#include "esp_vfs.h"

#include "display/touch/focaltech.h"
#include "display/TFT/SPIDisplay.h"

#include "sensors/accel/bma.h"
#include "sensors/PMU/axp20x.h"
#include "sensors/RTC/pcf8563.h"
#include "sensors/utils/master_i2c.h"

#include "apps/Menu.h"
#include "apps/App.h"
#include "apps/acc/Acc.h"
#include "apps/clock/Clock.h"
#include "apps/timer/Timer.h"


//static const char *TAG = "AXP20x";

#define AXP_ISR_GPIO                (gpio_num_t)CONFIG_AXPxxx_INTERRUPT_PIN  /*!< axp power chip interrupt Pin*/
#define GPIO_INPUT_PIN_SEL          (1ULL<<AXP_ISR_GPIO)

TaskHandle_t ISR = NULL;

AXP20X_Class axp;

App app;

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR axp_isr_handler(void *arg){
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
    vTaskResume(ISR);
}

static void axp_irq_init(){

    gpio_config_t io_conf;
    //enable interrupt
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //bit mask of the pins that you want to set
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //enable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    //change gpio intrrupt type for one pin
    gpio_set_intr_type(AXP_ISR_GPIO, GPIO_INTR_NEGEDGE);
    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(AXP_ISR_GPIO, axp_isr_handler, (void *) AXP_ISR_GPIO);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
}

static void SPIFF_init(){
		esp_vfs_spiffs_conf_t conf = {
			.base_path = "/spiffs",
			.partition_label = NULL,
			.max_files = 10,
			.format_if_mount_failed =true
		};

		// Use settings defined above to initialize and mount SPIFFS filesystem.
		// Note: esp_vfs_spiffs_register is anall-in-one convenience function.
		esp_err_t ret = esp_vfs_spiffs_register(&conf);

		if (ret != ESP_OK) {
			if (ret == ESP_FAIL) {
				printf("Failed to mount or format filesystem\n");
			} else if (ret == ESP_ERR_NOT_FOUND) {
				printf("Failed to find SPIFFS partition\n");
			} else {
				printf("Failed to initialize SPIFFS (%s)\n",esp_err_to_name(ret));
			}
			return;
		}

		size_t total = 0, used = 0;
		ret = esp_spiffs_info(NULL, &total,&used);
		if (ret != ESP_OK) {
			printf("Failed to get SPIFFS partition information (%s)\n",esp_err_to_name(ret));
		} else {
			printf("Partition size: total: %d, used: %d\n", total, used);
		}

}

/*static void connectWifi(){
	tcpip_adapter_init();
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
	wifi_config_t sta_config;
	wifi_sta_config_t sta={CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD}; // @suppress("Invalid arguments")
	sta.bssid_set=false;
	sta_config.sta=sta;
	ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
	ESP_ERROR_CHECK( esp_wifi_start() );
	ESP_ERROR_CHECK( esp_wifi_connect() );
}*/

static void axpTask(void *pvParameters){
	//! Enable all irq channel
    axp.enableIRQ(AXP202_ALL_IRQ, true);
    axp.clearIRQ();
    uint32_t io_num;
    while(1) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            if (io_num == AXP_ISR_GPIO) {
                axp.readIRQ();
                if (axp.isPEKShortPressIRQ()) {
                    //printf("AXP202 PEK key Click\n");
                    if (app.isDisplayOff()){
                    	app.displayOn();
                    	vTaskDelay(100 / portTICK_PERIOD_MS);
                    	axp.setPowerOutPut(AXP202_LDO2, AXP202_ON);
                    } else if(app.isNotifying()){
                    	app.stopNotify();
                	} else {
                    	app.stop();
                    }
                }
                axp.clearIRQ();
                vTaskSuspend(NULL);
            }
        }
    }
}

static void appTask(void *pvParameters){

	//Vibration motor
	gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIO_NUM_4, 0);

	//TFT initialization
	SPIDisplay tft;
	tft.begin();

	//Draw initial screen
	tft.FillScreen(BLACK);

	FILE *fp;
	uint16_t *buffer;
	fp = fopen("/spiffs/startImage.icon", "rb");
	buffer=(uint16_t *)malloc(240*sizeof(uint16_t));

	for(int i=0; i<240; i++){
		fread(buffer, 240*sizeof(uint16_t), 1, fp);
		tft.DrawMultiPixels(0 ,i , 240, buffer);
	}

	fclose(fp);
	free(buffer);

	//Enable TFT power from PMU
	axp.setPowerOutPut(AXP202_LDO2, AXP202_ON);

	//Touch initialization
	FocalTech_Class touch;
	touch = initTouch();

	//RTC initialization
	PCF8563_Class clock;
	clock.begin(twi_read, twi_write, 0x51);

	//Accelerometer initialization
	BMA423 accel;
	accel.begin(twi_read, twi_write, vTaskDelay,  0x19);

	//Applications initialization
	Menu menuApp(&app, tft, touch);
	Clock clockApp(&app, tft, axp, clock);
	Timer timerApp(&app, tft, axp, touch, clock);
	Acc accApp(&app, tft, axp, accel);

	//vTaskDelay(5000 / portTICK_PERIOD_MS);

	int selectedApp=0;
	while(1){
		selectedApp=menuApp.run();
		switch(selectedApp){
			case 1:
				clockApp.run();
				break;
			case 2:
				timerApp.run();
				break;
			case 3:
				accApp.run();
				break;
		}
	}
}

extern "C" {
    void app_main(void);
}

void app_main(void){
    nvs_flash_init();
    SPIFF_init();
    //connectWifi();

    //PMU (Power Managment Unit) initialization
	ESP_ERROR_CHECK(i2c_master_init());
	axp_irq_init();
	if (axp.begin(twi_read, twi_write)) {
		   printf("axp202 not initialized!");
	   }

	//Tasks initilization
    xTaskCreate(axpTask, "interrupts", 4096, NULL, 5, &ISR);
    xTaskCreate(appTask, "main task", 16*4096, NULL, 4, NULL);

}

/*static void tcpTask(void *pvParameters){ // @suppress("Unused static function")
    char hello[20] = "Hello from client!";
    char buffer[1024] = {0};
    int err, rec;
    int l;

    //gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    int level = 0;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while (true) {
        err=0;
        int sock = 0, valread;
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr("192.168.0.120");
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(1234);

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            printf("\n Socket creation error \n");
            err=1;
        }
         if (err==0 && connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0){
            printf("\nConnection Failed \n");
            err=1;
            close(sock);
        }
        if (!err){
			valread=1;
			while(valread>0){
				rec=0;
				while(rec<10240){
					if(10240-rec < 1024)
						l=10240-rec;
					else
						l=1024;
					valread = read( sock , buffer, l);
					printf("%d\n", valread);
					rec+=valread;
				}
				//printf("%s\n",buffer);
				printf("All received! %d\n", rec);
				send(sock , hello , strlen(hello) , 0 );
				printf("Hello message sent\n");
				vTaskDelay(1000 / portTICK_PERIOD_MS);
			}
            shutdown(sock, 0);
            close(sock);
        }

        //gpio_set_level(GPIO_NUM_4, level);
        level = !level;
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}*/

