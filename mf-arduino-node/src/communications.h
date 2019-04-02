void mfan_setup();
void mfan_loop();
uint8_t mfan_issue(uint8_t address, uint8_t command);
uint8_t mfan_requestinfo(uint8_t address, uint8_t command, uint8_t *info);
void mfan_getstream(uint8_t address);
void mfan_printstream();
void mfan_senduart();
