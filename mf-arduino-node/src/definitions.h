// COMMANDS
// UCOM_[] is command from PyCom
#define UCOM_START			0xF0
#define UCOM_GETDATA		0xA0
#define UCOM_SOLENOIDOFF	0xA1
#define UCOM_SOLENOIDON		0xA2

// DCOM_[] is commmand to seesaw
#define DCOM_TEST				0xFF
#define DCOM_UID(i)				(0x10 + i*0x10)
#define DCOM_MOISTUREA(i) 		(0x11 + i*0x10)
#define DCOM_MOISTUREB(i) 		(0x12 + i*0x10)
#define DCOM_TEMPERATUREA(i)	(0x13 + i*0x10)
#define DCOM_TEMPERATUREB(i)	(0x14 + i*0x10)
#define DCOM_TEMPERATUREC(i)	(0x15 + i*0x10)
#define DCOM_TEMPERATURED(i)	(0x16 + i*0x10)
#define DCOM_ONLINE(i) 			(0x17 + i*0x10)
#define DCOM_SOLENOIDON			0x90
#define DCOM_SOLENOIDOFF		0x91

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
