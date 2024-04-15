#ifndef _KEY_H_
#define _KEY_H_

#include "my_main.h"
struct Bkeys
{
	uint8_t age;
	uint8_t long_flag;
	uint8_t short_flag;
	uint8_t press;
	
	uint8_t double_ageEN;
	uint8_t double_age;
	uint8_t double_flag;
};

uint8_t key_read(void);
void key_serv(void);
void key_serv_long(void);
void key_serv_long_release(void);
void key_serv_double(void);

#endif
