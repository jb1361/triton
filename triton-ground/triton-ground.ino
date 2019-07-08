/*
 Name:		triton-ground.ino
 Created:	6/20/2019 8:08:45 PM
 Author:	Justin
*/


#include <SPI.h>
#include <RH_ASK.h>

#define RF_RX_RECEIVER_PIN 8
#define RF_TX_TRANSMITTER_PIN 9


RH_ASK driver(2000, RF_RX_RECEIVER_PIN, RF_TX_TRANSMITTER_PIN, NULL);

void setup() {
	Serial.begin(9600);
	Serial.println("Starting Reciever");
	if (!driver.init()) {
		Serial.println("Radio Head driver failed");
	}
	Serial.println("Starting CLI");
	cli_init();
}

void loop() {
	run_cli();
	ReceiveData();
	delay(1);
}

void ReceiveData() {
	uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
	uint8_t buflen = sizeof(buf);
	if(driver.recv(buf, &buflen)) {
		String data;
		for (int i = 0; i < buflen; i++)
		{
			data += (char)buf[i];
		}
		Serial.println(data);
	}
}

void TransmitData(String data) {
	const char* msg = data.c_str();
	driver.send((uint8_t*)msg, strlen(msg));
	driver.waitPacketSent();
}
