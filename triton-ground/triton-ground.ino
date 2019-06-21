/*
 Name:		triton_ground.ino
 Created:	6/20/2019 8:08:45 PM
 Author:	Justin
*/


#include <SPI.h>
#include <RH_ASK.h>

RH_ASK driver(2000, 8, 9, 10);
int i = 0;

void setup() {
	Serial.begin(9600);
	Serial.println("Starting Reciever");
	if (!driver.init()) {
		Serial.println("Radio Head driver failed");
	}

}

// the loop function runs over and over again until power down or reset
void loop() {
	uint8_t buf[24];
	uint8_t buflen = sizeof(buf);
	//Serial.println("Waiting for data");
	//Serial.println(driver.rxGood());
	if(driver.recv(buf, &buflen)) {
		Serial.print((char*)buf);
		Serial.print(" : " + i);
		Serial.println();
		i++;
	}
	delay(1);
}
