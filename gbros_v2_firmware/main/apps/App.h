/*
 * App.h
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#ifndef MAIN_APPS_APP_H_
#define MAIN_APPS_APP_H_

class App {
public:
	bool isRunning(void);
	void start(void);
	void stop(void);
	void displayOff(void);
	void displayOn(void);
	bool isDisplayOff(void);
private:
	bool running =false;
	bool dispOff = false;
};

#endif /* MAIN_APPS_APP_H_ */
