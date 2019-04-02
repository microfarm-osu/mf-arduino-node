#define DEBUG 0
#define SEND_TEST 1

void mfan_setup();
void mfan_loop();

void mfan_waitforstart();
void mfan_waitforaddress();
void mfan_waitforcommand(uint8_t addr);

uint8_t mfan_issue(uint8_t address, uint8_t command);
uint8_t mfan_requestinfo(uint8_t address, uint8_t command, uint8_t *info);
void mfan_getstream(uint8_t address);
void mfan_senduart();

#if DEBUG
void mfan_printstream();
#endif
