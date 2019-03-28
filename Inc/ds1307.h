#ifndef __ds1307_h
#define __ds1307_h

#include "stm32f0xx_hal.h"
#include "main.h"

#define DS1307_Address 0x68
#define DS1307_I2C_Instance I2C1

#ifndef DS1307_SET_PORT
   #define DS1307_SET_PORT GPIOC
#endif

#ifndef DS1307_SET_PIN
   #define DS1307_SET_PIN GPIO_PIN_14
#endif

typedef struct{
   uint8_t Second, Minute, Hour, Day, Date, Month, Year;
}DateTime_TypeDef;

extern DateTime_TypeDef CurrentDateTime;
extern uint8_t DS1307_Read[7], DS1307_Write[7];

uint8_t BCD2DEC(uint8_t bcd);
uint8_t DEC2BCD(uint8_t dec);

void DS1307_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
void DS1307_EXTI_Callback(uint16_t GPIO_Pin);

void DS1307_WriteData(void);
void DS1307_ReadData(void);
void DS1307_SetDefaultDateTime(void);
void DS1307_SetDateTime(DateTime_TypeDef DateTime); 
void DS1307_GetDateTime(DateTime_TypeDef *DateTime);

#endif 
