#include "ds1307.h"

DateTime_TypeDef CurrentDateTime;
uint8_t DS1307_Read[7], DS1307_Write[7];

extern I2C_HandleTypeDef hi2c1;

uint8_t BCD2DEC(uint8_t bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0f);
}

uint8_t DEC2BCD(uint8_t dec)
{
	return (dec / 10) << 4 | ( dec % 10);
}

void DS1307_ReadData(void){
   HAL_I2C_Mem_Read_IT(&hi2c1, DS1307_Address << 1, 0, I2C_MEMADD_SIZE_8BIT, DS1307_Read, 7);
}

void DS1307_WriteData(void){
   HAL_I2C_Mem_Write_IT(&hi2c1, DS1307_Address << 1, 0, I2C_MEMADD_SIZE_8BIT, DS1307_Write, 7);
}


void DS1307_SetDefaultDateTime(void){
	DS1307_Write[0] = DEC2BCD(12);
	DS1307_Write[1] = DEC2BCD(12);
	DS1307_Write[2] = DEC2BCD(12);
	DS1307_Write[3] = DEC2BCD(12);
	DS1307_Write[4] = DEC2BCD(12);
	DS1307_Write[5] = DEC2BCD(12);
	DS1307_Write[6] = DEC2BCD(18);
	DS1307_WriteData();
}
void DS1307_SetDateTime(DateTime_TypeDef SetupDateTime){
    DS1307_Write[0] = DEC2BCD(SetupDateTime.Second);
    DS1307_Write[1] = DEC2BCD(SetupDateTime.Minute);
    DS1307_Write[2] = DEC2BCD(SetupDateTime.Hour);
    DS1307_Write[3] = DEC2BCD(SetupDateTime.Day);
    DS1307_Write[4] = DEC2BCD(SetupDateTime.Date);
    DS1307_Write[5] = DEC2BCD(SetupDateTime.Month);
    DS1307_Write[6] = DEC2BCD(SetupDateTime.Year);
    DS1307_WriteData();
}

void DS1307_GetDateTime(DateTime_TypeDef *DateTime){
   DateTime->Second = CurrentDateTime.Second;
   DateTime->Minute = CurrentDateTime.Minute;
   DateTime->Hour = CurrentDateTime.Hour;
   DateTime->Day = CurrentDateTime.Day;
   DateTime->Date = CurrentDateTime.Date;
   DateTime->Month = CurrentDateTime.Month;
   DateTime->Year = CurrentDateTime.Year;
}

void DS1307_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
  if(hi2c->Instance==DS1307_I2C_Instance)
	{
	  CurrentDateTime.Second=BCD2DEC(DS1307_Read[0]);
	  CurrentDateTime.Minute=BCD2DEC(DS1307_Read[1]);
	  CurrentDateTime.Hour=BCD2DEC(DS1307_Read[2]);
	  CurrentDateTime.Day=BCD2DEC(DS1307_Read[3]);
	  CurrentDateTime.Date=BCD2DEC(DS1307_Read[4]);
	  CurrentDateTime.Month=BCD2DEC(DS1307_Read[5]);
	  CurrentDateTime.Year=BCD2DEC(DS1307_Read[6]);
	}   
}

void DS1307_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==DS1307_SET_PIN)
	{
	  DS1307_SetDefaultDateTime();
	}
}

