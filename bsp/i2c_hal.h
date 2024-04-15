#ifndef __I2C_HAL_H
#define __I2C_HAL_H

#include "stm32g4xx_hal.h"

void I2CStart(void);
void I2CStop(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);
void I2CInit(void);
uint8_t eeprom_read(uint8_t addr);
void eeprom_write(uint8_t addr,uint8_t dat);
void eeprom_write16(uint8_t addr,uint8_t dat);
uint16_t eeprom_read16(uint8_t addr);
void eeprom_write_f(uint8_t addr,float dat);
float eeprom_read_f(uint8_t addr);
void mcp_write(uint8_t dat);

#endif
