/*
 * photoserver.cpp
 *
 *  Created on: 2012-05-28
 *      Author: steve
 */

extern	int initialisePWM(bool);
extern	int initialiseSocket(void);

int main(int argc, char* argv[]) {

	initialisePWM(true);
	initialiseSocket();


}

