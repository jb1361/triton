/*
 Name:		triton-orbit.ino
 Created:	6/18/2019 6:38:32 PM
 Author:	Justin
*/


#include <NeoSWSerial.h>
#include <NMEAGPS.h>
#include <SPI.h>
#include <RH_ASK.h>

//GPS pins
//rx - pin 10
//tx - pin 11
NeoSWSerial gps_port(10, 11);
NMEAGPS     gps;

//RF transmit/receive pins
//rx (receiver) - pin 8
//tx (transmitter) - pin 9
RH_ASK driver(2000, 8, 9, NULL);

void setup() {
	Serial.begin(9600);
	Serial.println(F("Starting Triton"));
	gps_port.begin(9600);
	if (!driver.init()) {
		Serial.println(F("Radio Head driver failed"));
		TransmitData(F("Radio Head driver failed"));
	}
}

void loop() {
	ReceiveData();
	delay(1);
}

void GetGPSData() {
	if (gps.available(gps_port))
	{
		gps_fix fix = gps.read();

		if (fix.valid.location)
		{
			String satalliteCount = "Satellite Count: " + fix.satellites;
			String latitude = "Latitude: " + String(fix.latitude(), 6);
			String longitude = "Longitude: " + String(fix.longitude(), 6);
			String speed = "Speed Kmph: " + String(fix.speed_kph());
			String altitude = "Altitude(meters): " + String(fix.altitude());
			String time = "Timestamp : " + fix.dateTime.date;
			TransmitData(satalliteCount);
			TransmitData(latitude);
			TransmitData(longitude);
			TransmitData(speed);
			TransmitData(altitude);
			TransmitData(time);
		}
		else {
			TransmitData("Location not valid, waiting for more satallites.");
			TransmitData("Satellites found: " + fix.satellites);
		}
	}
	if (!gps.available(gps_port)) {
		TransmitData("Satellites found: " + gps.read().satellites);
		TransmitData("Finding Satellites...");
	}
}

void TransmitData(String data) {
	const char* msg = data.c_str();
	driver.send((uint8_t*)msg, strlen(msg));
	driver.waitPacketSent();
}

void ReceiveData() {
	uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
	uint8_t buflen = sizeof(buf);
	if (driver.recv(buf, &buflen)) {
		String data;
		for (int i = 0; i < buflen; i++)
		{
			data += (char)buf[i];
		}
		ParseCommand(data);
	}
}

void ParseCommand(String command) {
	if (command == "gps")
		Serial.println("Received GPS telemetry command.");
		GetGPSData();
}
