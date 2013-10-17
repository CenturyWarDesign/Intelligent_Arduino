/*
 * light.cpp
 *
 *  Created on: 2013-10-16
 *      Author: wanhin
 */
#include "light.h"

Light::Light(int temid) {
	interid = temid;
	status = false;
}

Light::~Light(void) {
}

bool Light::getStatus() {
	return status;
}

void Light::open() {
	status = true;
}

void Light::close() {
	status = false;
}

void Light::clear() {
	delay = 0;
}


void Light::setdelay(int sec) {
	delay = sec;
}
int Light::getInter() {
	return interid;
}
