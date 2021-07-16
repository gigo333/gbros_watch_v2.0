# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS 	"main.cpp" 
					"display/TFT/st7789.c" 
					"display/TFT/SPIDisplay.cpp"
					"display/TFT/fontx.c" 
					"display/touch/focaltech.cpp" 
					"display/touch/utils/software_i2c.c"
					"sensors/accel/bma.cpp"
					"sensors/accel/bma423.c"
					"sensors/accel/bma4.c"
					"sensors/RTC/pcf8563.cpp"
					"sensors/PMU/axp20x.cpp" 
					"sensors/utils/master_i2c.cpp"
					"apps/Menu.cpp"
					"apps/App.cpp"
					"apps/clock/Clock.cpp"
					"apps/acc/Acc.cpp"	)
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
