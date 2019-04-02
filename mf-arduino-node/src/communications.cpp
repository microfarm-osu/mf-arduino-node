#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include "communications.h"
#include "definitions.h"

#define DEBUG 0
#define SEND_TEST 1

MFAN_StreamData data; // General Purpose Unsafe Stream Data

void mfan_setup()
{
	Serial.begin(9600);

	#if DEBUG
		Serial.println("Starting Up!");
	#endif

	Wire.begin();
}

// TODO: Change to reactionary to PyCom instructions
void mfan_loop()
{
	// Wait for request for data from PyCom
	// Sends address of seesaw
	if(Serial.available() > 0)
	{
		int addr = Serial.read();
		if(addr >= 0x10 && addr <= 0x15)
		{
			#if DEBUG
				Serial.println("\n\n\n");
			#endif


			// TEST COMMANDS
			mfan_issue(addr, COMMAND_SOLENOIDON);

			#if SEND_TEST
				mfan_issue(addr, COMMAND_TEST);
			#endif

			mfan_issue(addr, COMMAND_SOLENOIDOFF);


			// ASK FOR INFO
			mfan_getstream(addr);

			#if DEBUG
				mfan_printstream();
			#endif

			mfan_senduart(); // Send Data to PyCom
			delay(100);
		}
	}
}

void mfan_senduart()
{
	Serial.write((char*)(&data), sizeof(MFAN_StreamData));
	Serial.println();
}

void mfan_getstream(uint8_t address)
{
	uint32_t 	info32;
	uint16_t 	info16;
	uint8_t 	info8;

	// RESET STREAM STRUCTURE
	data.streamID = address;
	data.seesaw_online = 0;
	data.seesaw_temperature = 0;
	for(int i = 0; i < 4; i++)
	{
		data.UID[i] = 0;
		data.sensor_online[i] = 0;
		data.moisture[i] = 0;
		data.temperature[i] = 0;
	}

	// OBTAIN STREAM DATA
	for(int i = 0; i < 4; i++)
	{
		// ONLINE SENSORS
		data.seesaw_online = mfan_requestinfo(address, COMMAND_ONLINE(i), &data.sensor_online[i]);
		if(!data.seesaw_online) return;
		if(!data.sensor_online[i]) continue;

		// UPDATE IDENTIFICATION
		data.seesaw_online = mfan_requestinfo(address, COMMAND_UID(i), &data.UID[i]);
		if(!data.seesaw_online) return;

		// MOISTURE MSB
		data.seesaw_online = mfan_requestinfo(address, COMMAND_MOISTUREA(i), &info8);
		if(!data.seesaw_online) return;
		info16 = ((uint16_t)info8) << 8;

		// MOISTURE LSB
		data.seesaw_online = mfan_requestinfo(address, COMMAND_MOISTUREB(i), &info8);
		if(!data.seesaw_online) return;
		info16 |= info8;
		data.moisture[i] = info16;

		// TEMPERATURE
		data.seesaw_online = mfan_requestinfo(address, COMMAND_TEMPERATUREA(i), &info8);
		if(!data.seesaw_online) return;
		info32 = ((uint32_t)info8) << 24;

		data.seesaw_online = mfan_requestinfo(address, COMMAND_TEMPERATUREB(i), &info8);
		if(!data.seesaw_online) return;
		info32 |= ((uint32_t)info8) << 16;

		data.seesaw_online = mfan_requestinfo(address, COMMAND_TEMPERATUREC(i), &info8);
		if(!data.seesaw_online) return;
		info32 |= ((uint32_t)info8) << 8;

		data.seesaw_online = mfan_requestinfo(address, COMMAND_TEMPERATURED(i), &info8);
		if(!data.seesaw_online) return;
		info32 |= ((uint32_t)info8);
		data.temperature[i] = info32;
	}
}

uint8_t mfan_issue(uint8_t address, uint8_t command)
{
	Wire.beginTransmission(address);
	Wire.write(command);
	Wire.endTransmission();
}

uint8_t mfan_requestinfo(uint8_t address, uint8_t command, uint8_t *info)
{
	mfan_issue(address, command);
	uint8_t available = Wire.requestFrom((int)address, 1);
	if(available) (*info) = Wire.read();
	return available;
}

void printHex(int num, int precision)
{
     char tmp[16];
     char format[128];

     sprintf(format, "0x%%.%dX", precision);

     sprintf(tmp, format, num);
     Serial.print(tmp);
}

#if DEBUG
void mfan_printstream()
{
	Serial.print("Seesaw "); printHex(data.streamID, 0); Serial.println(":");
	if(data.seesaw_online)
	{
		// Loop Soil SENSORS
		for(int i = 0; i < 4; i++)
		{
			if(data.sensor_online[i])
			{
				Serial.print("\tSoil Sensor "); Serial.print(i); Serial.print(" ("); printHex(FIRST_ADDRESS + i, 0); Serial.print(") UID "); Serial.print(data.UID[i]); Serial.println(":");
				Serial.print("\t\tTemperature:\t"); Serial.println((double)data.temperature[i]/(1UL << 16));
				Serial.print("\t\tMoisture:\t"); Serial.println(data.moisture[i]);
			}
			else
			{
				Serial.print("\tSoil Sensor "); Serial.print(i); Serial.print(" ("); printHex(FIRST_ADDRESS + i, 0); Serial.println(") Offline");
			}
		}
	}
	else
	{
		Serial.println("\tOffline");
	}
}
#endif
