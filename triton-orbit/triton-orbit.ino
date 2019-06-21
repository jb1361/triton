/*
 Name:		triton-orbit.ino
 Created:	6/18/2019 6:38:32 PM
 Author:	Justin
*/


#include <SPI.h>
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <RH_ASK.h>

SoftwareSerial serial_connection(10, 11);
TinyGPSPlus gps;

RH_ASK driver(2000, NULL, 12, NULL);

void setup() {
	Serial.begin(9600);
	serial_connection.begin(9600);
	Serial.println("GPS Start");
	Serial.println("Starting Transmitter");
	if (!driver.init()) {
		Serial.println("Radio Head driver failed");
	}
}

void loop() {
	while (serial_connection.available())
	{
		gps.encode(serial_connection.read());
	}
	if (gps.location.isUpdated())
	{
		String data = "Satellite Count: " + gps.satellites.value();
		data += ", Latitude: " + String(gps.location.lat(), floor(sizeof(gps.location.lat())));
		data += ", Longitude: " + String(gps.location.lng(), floor(sizeof(gps.location.lng())));
		data += ", Speed MPH: " + String(gps.speed.mph(),floor(sizeof(gps.speed.mph())));
		data += ", Altitude Feet : " + (String)gps.altitude.meters();
		data += ", Timestamp : " + gps.time.value();
		const char* msg = data.c_str();
		driver.send((uint8_t*)msg, strlen(msg));
		driver.waitPacketSent();
	}
	else {
		//Serial.println("Satellite Count: " + gps.satellites.value());
		/*String data = "Satellite Count: " + gps.satellites.value();
		Serial.println(data);
		const char* msg = data.c_str();
		driver.send((uint8_t*)msg, strlen(msg));
		driver.waitPacketSent();*/
		const char* msg = "Finding Sattallites...";
		driver.send((uint8_t*)msg, strlen(msg));
		driver.waitPacketSent();
	}
	
	delay(500);
}
