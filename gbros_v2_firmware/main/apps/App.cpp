/*
 * App.cpp
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#include "App.h"


bool App::isRunning() {
	return running;
}

void App::start() {
	running=true;
}

void App::stop(){
	running=false;
}

void App::displayOff(){
	dispOff=true;
}
void App::displayOn(){
	dispOff=false;
}

bool App::isDisplayOff(){
	return dispOff;
}
