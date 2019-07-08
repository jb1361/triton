/*
 Name:		triton-orbit.ino
 Created:	6/18/2019 6:38:32 PM
 Author:	Justin
*/

#pragma once
#include <NeoSWSerial.h>
#include <NMEAGPS.h>
#include <SPI.h>
#include <RH_ASK.h>
#include <MPU9250.h>

#define FLYWHEEL_PIN 2   
#define RF_RX_RECEIVER_PIN 8
#define RF_TX_TRANSMITTER_PIN 9
#define GPS_RX_PIN 10
#define GPS_TX_PIN 11

RH_ASK driver(2000, RF_RX_RECEIVER_PIN, RF_TX_TRANSMITTER_PIN, NULL);
NeoSWSerial gps_port(GPS_RX_PIN, GPS_TX_PIN);
NMEAGPS     gps;
MPU9250 IMU(Wire, 0x68);

bool flywheelActive = false;
int gyroscopeStatus;

void setup() {
	Serial.begin(9600);
	Serial.println(F("Starting Triton."));
	gps_port.begin(9600);
	if (!driver.init()) {
		Serial.println(F("Radio Head driver failed."));
		TransmitData(F("Radio Head driver failed."));
	}
	
	//begin returns negative int on failure
	gyroscopeStatus = IMU.begin();
	if (gyroscopeStatus < 0) {
		Serial.println("GyroScope Failed to start.");
		Serial.print("Status: ");
		Serial.println(gyroscopeStatus);
		TransmitData("GyroScope Failed to start. Status: " + String(gyroscopeStatus));
	}

	pinMode(FLYWHEEL_PIN, OUTPUT);
	Serial.println(F("Triton Started."));
}

void loop() {
	ReceiveData();
	delay(1);
}

void TransmitData(String data) {
	Serial.println("Transmitting Data: " + data);
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
	Serial.println("Command Recieved: " + command);
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
	else if (command == "IMUData") {
		Serial.println("Sending IMU Data.");
		GetIMUData();
	}
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

void GetIMUData() {
	GetAccelerometerData();
	GetGyroscopeData();
	GetMagnetometerData();
	GetTemperature();
}

void GetAccelerometerData() {
	float accelX = IMU.getAccelX_mss();
	float accelY = IMU.getAccelY_mss();
	float accelZ = IMU.getAccelZ_mss();
	Serial.print("AccelX: ");
	Serial.print(accelX, 6);
	Serial.print("	");
	Serial.print("AccelY: ");
	Serial.print(accelY, 6);
	Serial.print("	");
	Serial.print("AccelZ: ");
	Serial.println(accelZ, 6);
	Serial.println();
	TransmitData("Accelerometer Data (x,y,z)");
	TransmitData(String(accelX) + "," + String(accelY) + "," + String(accelZ));
	Serial.println();
}

void GetGyroscopeData() {
	float gyroX = IMU.getGyroX_rads();
	float gyroY = IMU.getGyroY_rads();
	float gyroZ = IMU.getGyroZ_rads();
	Serial.print("GyroX: ");
	Serial.print(gyroX, 6);
	Serial.print("	");
	Serial.print("GyroY: ");
	Serial.print(gyroY, 6);
	Serial.print("	");
	Serial.print("GyroZ: ");
	Serial.println(gyroZ, 6);
	Serial.println();
	TransmitData("Gyroscope Data (x,y,z)");
	TransmitData(String(gyroX) + "," + String(gyroY) + "," + String(gyroZ));
	Serial.println();
}

void GetMagnetometerData() {
	float magX = IMU.getMagX_uT();
	float magY = IMU.getMagY_uT();
	float magZ = IMU.getMagZ_uT();
	Serial.print("MagX: ");
	Serial.print(magX, 6);
	Serial.print("	");
	Serial.print("MagY: ");
	Serial.print(magY, 6);
	Serial.print("	");
	Serial.print("MagZ: ");
	Serial.println(magZ, 6);
	TransmitData("Magnetometer Data (x,y,z)");
	TransmitData(String(magX) + "," + String(magY) + "," + String(magZ));
	Serial.println();
}

void GetTemperature() {
	float temp = IMU.getTemperature_C();
	Serial.print("Temperature in C: ");
	Serial.println(temp, 6);
	Serial.println();
	TransmitData("Temperature Celcius");
	TransmitData(String(temp));
	Serial.println();
}

void GetFlywheelStatus() {
	if (flywheelActive)
		TransmitData("Flywheel is active.");
	else
		TransmitData("Flywheel is not active.");
}

void ActivateFlywheel() {
	digitalWrite(FLYWHEEL_PIN, HIGH);
	flywheelActive = true;
}

void DeactivateFlywheel() {
	digitalWrite(FLYWHEEL_PIN, LOW);
	flywheelActive = false;
}

