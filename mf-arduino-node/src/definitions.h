// COMMANDS
#define FIRST_ADDRESS	0x10
#define LAST_ADDRESS	0x15

#define COMMAND_TEST		0xFF

#define COMMAND_UID(i)			(0x10 + i*0x10)
#define COMMAND_MOISTUREA(i) 	(0x11 + i*0x10)
#define COMMAND_MOISTUREB(i) 	(0x12 + i*0x10)
#define COMMAND_TEMPERATUREA(i)	(0x13 + i*0x10)
#define COMMAND_TEMPERATUREB(i)	(0x14 + i*0x10)
#define COMMAND_TEMPERATUREC(i)	(0x15 + i*0x10)
#define COMMAND_TEMPERATURED(i)	(0x16 + i*0x10)
#define COMMAND_ONLINE(i) 		(0x17 + i*0x10)

#define COMMAND_SOLENOIDON	0x90
#define COMMAND_SOLENOIDOFF	0x91

typedef struct
{
	uint8_t streamID; // Basically the Seesaw I2C Address
	uint8_t UID[4];
	uint8_t seesaw_online;
	uint8_t sensor_online[4];
	uint16_t moisture[4];
	// Temperatures
	uint32_t temperature[4];
	uint32_t seesaw_temperature;
} MFAN_StreamData;
