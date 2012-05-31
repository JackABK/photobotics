/*
 * photoserver.cpp
 *
 *  Created on: 2012-05-28
 *      Author: steve
 */
#include <stdint.h>


extern	int initialisePWM(uint16_t , uint8_t  );
extern	int initialiseSocket(void);

int main(int argc, char* argv[]) {

	initialisePWM(100,50);
	initialiseSocket();


}

