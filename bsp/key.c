#include "key.h"

struct Bkeys bkey[5]={0,0,0,0};

uint8_t key_read(void)
{
	     if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0) return 1;
	else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0) return 2;
	else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==0) return 3;
	else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0) return 4;
	else return 0;
}

void key_serv(void)
{
	uint8_t key_sta=key_read();
	if(key_sta!=0)
	{
		bkey[key_sta].age++;
		if(bkey[key_sta].age>1)
		{ 
			bkey[key_sta].short_flag=1;
		}
	}
	else 
	{
		for(int i=0;i<5;i++)
		{
			bkey[i].age=0;
		}
	}
}


void key_serv_long(void)
{
	uint8_t key_sta=key_read();
	if(key_sta!=0)
	{
		bkey[key_sta].age++;
		if(bkey[key_sta].age>1)
		{ 
			bkey[key_sta].press=1;
		}
	}
	else 
	{
		for(int i=0;i<5;i++)
		{
			if(bkey[i].press==1&&bkey[i].long_flag==0)
			{
				bkey[i].short_flag=1;
			}
			
			bkey[i].press=0;
			bkey[i].age=0;
			bkey[i].long_flag=0;
		}
	}
	if(bkey[key_sta].age>200)
	{
		bkey[key_sta].long_flag=1;
	}
}

void key_serv_long_release(void)
{
	uint8_t key_sta=key_read();
	if(key_sta!=0)
	{
		bkey[key_sta].age++;
		if(bkey[key_sta].age>1)
		{ 
			bkey[key_sta].press=1;
		}
	}
	else 
	{
		for(int i=0;i<5;i++)
		{
			if(bkey[i].age>70)
			{
				bkey[i].long_flag=1;
			}
			if(bkey[i].press==1&&bkey[i].long_flag==0)
			{
				bkey[i].short_flag=1;
			}
			bkey[i].press=0;
			bkey[i].age=0;
		}
	}
	
}

void key_serv_double(void)
{
	uint8_t key_sta=key_read();
	if(key_sta!=0)
	{
		bkey[key_sta].age++;
		if(bkey[key_sta].age>1)
		{ 
			bkey[key_sta].press=1;
		}
	}
	else 
	{
		for(int i=0;i<5;i++)
		{
			if(bkey[i].double_ageEN==1&&bkey[i].press==1)
			{
				bkey[i].double_flag=1;
				bkey[i].press=0;
				bkey[i].double_ageEN=0;
				
			}
			
			if(bkey[i].press==1&&bkey[i].long_flag==0)
			{
				bkey[i].double_ageEN=1;
			}
			if(bkey[i].double_ageEN==1)
			{
				bkey[i].double_age++;
			}
			if(bkey[i].double_ageEN==1&&bkey[i].double_age>200)
			{
				bkey[i].short_flag=1;
				bkey[i].double_age=0;
				bkey[i].double_ageEN=0;
			}
			bkey[i].press=0;
			bkey[i].age=0;
			bkey[i].long_flag=0;
		}
	}
	if(bkey[key_sta].age>200)
	{
		bkey[key_sta].long_flag=1;
	}
}
