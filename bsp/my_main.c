 #include "my_main.h"

uint8_t led_sta=0x00;
char text[30];
char uart_tx[50];
char uart_rx[50];
extern struct Bkeys bkey[];
char password[3]={'1','2','3'};
uint8_t eep_num;


uint32_t time500ms;

void LED_Disp(uint8_t dsLED)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);//所有LED熄灭
	HAL_GPIO_WritePin(GPIOC,dsLED<<8,GPIO_PIN_RESET);//左移8位，控制C8-15引脚，值为1的点亮
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);//开锁存器
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

float adc_read_os(ADC_HandleTypeDef *hadc)
{
	uint16_t adc_val;
	float adc_f;
	HAL_ADC_Start(hadc);
	adc_val=HAL_ADC_GetValue(hadc);
	adc_f=adc_val*3.3f/65536.0f;
	return adc_f;
	
}
float adc_read(ADC_HandleTypeDef *hadc)
{
	uint16_t adc_val;
	float adc_f;
	HAL_ADC_Start(hadc);
	adc_val=HAL_ADC_GetValue(hadc);
	adc_f=adc_val*3.3f/4096.0f;
	return adc_f;
	
}

void LED_Change(uint8_t num,uint8_t sta) //1~8
{
	uint8_t pos=0x01<<(num-1);
	led_sta=(led_sta&(~pos))|(pos*sta);
	LED_Disp(led_sta);
}

void setup()
{
	HAL_TIM_Base_Start_IT(&htim6);
//	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2); 
//	//TIM15_CH1输入演示
//	HAL_TIM_IC_Start(&htim15,TIM_CHANNEL_1);
//	HAL_TIM_IC_Start(&htim15,TIM_CHANNEL_2);
	
	HAL_UARTEx_ReceiveToIdle_IT(&huart1,(uint8_t *)uart_rx,50);
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);
	
	LED_Disp(0x00);
	//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);//解锁锁存器
	//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
	
	
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	I2CInit();
	eep_num=eeprom_read(8);

	time500ms=uwTick;
}

void loop()
{
	static uint16_t temp;
	float frq1=0;
	float duty1=0;
	//frq1=1000000.0f/(HAL_TIM_ReadCapturedValue(&htim15,TIM_CHANNEL_1)+1);
	//duty1=((HAL_TIM_ReadCapturedValue(&htim15,TIM_CHANNEL_2)+1)*100.0f/(HAL_TIM_ReadCapturedValue(&htim15,TIM_CHANNEL_1)+1));
	float adc_data_PB15;
	
	sprintf(text,"   FQR:%.2fHz      ",frq1);
	LCD_DisplayStringLine(Line1,(uint8_t *)text);
	sprintf(text,"   DUTY:%.2f%%     ",duty1);
	LCD_DisplayStringLine(Line2,(uint8_t *)text);
	//第五个LED常亮，第一个闪烁
	/*led_sta=(led_sta&0xfe)|0x01;
	LED_Disp(led_sta);
	HAL_Delay(500);
	led_sta=(led_sta&0xfe)|0x00;
	LED_Disp(led_sta);
	HAL_Delay(500);*/
	
	/*LED_Change(2,1);
	HAL_Delay(500);
	LED_Change(2,0);
	HAL_Delay(500);*/
	
	/*if(uwTick-time500ms>500)
	{
		static uint8_t LED_sta=0;
		LED_Change(2,LED_sta);
		LED_sta=!LED_sta;
		time500ms=uwTick;
	}*/
	adc_data_PB15=adc_read_os(&hadc2);
	sprintf(text,"    PB15:%.4f    ",adc_data_PB15);
	LCD_DisplayStringLine(Line5,(uint8_t *)text);	
	
	//sprintf(uart_tx,"PB15:%.4fV   \r\n ",adc_data_PB15);
	//HAL_UART_Transmit(&huart1,(uint8_t *)uart_tx,strlen(uart_tx),50);
	sprintf(text,"    EEP:%d    ",eep_num);
	LCD_DisplayStringLine(Line6,(uint8_t *)text);	
	
	if(bkey[1].short_flag==1)
	{
		sprintf(text,"    KEY1_Down    ");
		LCD_DisplayStringLine(Line8,(uint8_t *)text);
		//__HAL_TIM_SET_AUTORELOAD(&htim2,250-1);
		//__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,100);
		bkey[1].short_flag=0;
	}
	if(bkey[2].short_flag==1)
	{
		eep_num++;
		eeprom_write(8,eep_num);
		sprintf(text,"    KEY2_Down    ");
		LCD_DisplayStringLine(Line8,(uint8_t *)text);
		bkey[2].short_flag=0;
	}
   	
	
	if(bkey[1].long_flag==1)
	{
		sprintf(text,"    KEY1_long    %d    ",temp);
		LCD_DisplayStringLine(Line8,(uint8_t *)text);
		temp++;
	}
	if(bkey[2].long_flag==1)
	{
		sprintf(text,"    KEY2_long    %d    ",temp);
		LCD_DisplayStringLine(Line8,(uint8_t *)text);
		temp++;
	}
	/*if(bkey[1].long_flag==1)
	{
		sprintf(text,"    KEY1_long       ");
		LCD_DisplayStringLine(Line8,(uint8_t *)text);
		bkey[1].long_flag=0;
	}
	if(bkey[2].long_flag==1)
	{
		sprintf(text,"    KEY2_long      ");
		LCD_DisplayStringLine(Line8,(uint8_t *)text);
		bkey[2].long_flag=0;
	}*/
	if(bkey[1].double_flag==1)
	{
		sprintf(text,"    KEY1_double       ");
		LCD_DisplayStringLine(Line8,(uint8_t *)text);
		bkey[1].double_flag=0;
	}
	if(bkey[2].double_flag==1)
	{
		sprintf(text,"    KEY2_double      ");
		LCD_DisplayStringLine(Line8,(uint8_t *)text);
		bkey[2].double_flag=0;
	}
	/*sprintf(text,"    number:%d    ",14);
	LCD_DisplayStringLine(Line8,(uint8_t *)text);
	
	LCD_SetBackColor(Yellow);
	sprintf(text,"    T:%d                  ",30);
	LCD_DisplayStringLine(Line5,(uint8_t *)text);
	
	LCD_SetBackColor(Black);
	sprintf(text,"    X:A01    ");
	LCD_DisplayStringLine(Line6,(uint8_t *)text);*/
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)
	key_serv_double();
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	char old_password[10];
	char new_password[10];
	
	sscanf(uart_rx,"%3s-%3s",old_password,new_password);
	if(strcmp(old_password,password)==0)
	{
		for(int i=0;i<3;i++)
		{
			password[i]=new_password[i];
		}
		sprintf(text,"    new:%s    ",password);
		LCD_DisplayStringLine(Line9,(uint8_t *)text);
	}
	else
	{
		sprintf(text,"    FAIL    ");
		LCD_DisplayStringLine(Line9,(uint8_t *)text);
	}
	HAL_UARTEx_ReceiveToIdle_IT(&huart1,(uint8_t *)uart_rx,50);
}

