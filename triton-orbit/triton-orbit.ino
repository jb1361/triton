/*
 Name:		triton-orbit.ino
 Created:	6/18/2019 6:38:32 PM
 Author:	Justin
*/

#include <NeoSWSerial.h>
#include <NMEAGPS.h>
#include <SPI.h>
#include <RH_ASK.h>

#define FLYWHEEL_PIN 3   
#define RF_RX_TRANSMITTER_PIN 8
#define RF_TX_RECIEVER_PIN 9
#define GPS_RX_PIN 10
#define GPS_TX_PIN 11

RH_ASK driver(2000, RF_RX_TRANSMITTER_PIN, RF_TX_RECIEVER_PIN, NULL);
NeoSWSerial gps_port(GPS_RX_PIN, GPS_TX_PIN);
NMEAGPS     gps;

bool flywheelActive = false;

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
	if (command == "gps") {
		Serial.println("Received GPS telemetry command.");
		GetGPSData();
	}
	else if (command == "activateFlywheel") {
		Serial.println("Activating Flywheel.");
		ActivateFlywheel();
	}
	else if (command == "deactivateFlywheel") {
		Serial.println("Deactivating Flywheel.");
		DeactivateFlywheel();
	}
	else if (command == "flywheelStatus") {
		Serial.println("Sending flywheel Status.");
		GetFlywheelStatus();
	}
}
void GetFlywheelStatus() {
	if (flywheelActive)
		TransmitData("Flywheel is active.");
	else
		TransmitData("Flywheel is not active.");
}

void ActivateFlywheel() {

	flywheelActive = true;
}

void DeactivateFlywheel() {

	flywheelActive = false;
}
