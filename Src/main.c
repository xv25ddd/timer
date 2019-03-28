/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
#include "i2c-lcd.h"
#include "var.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LCD_CLEAR  lcd_send_cmd(0x01);
#define LCD_LINE_2 lcd_send_cmd (0xc0);
////////////////////////////////////////////////////////////////////////
#define GRA_LOW HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);//group A
#define GRB_LOW HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
#define GRC_LOW HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
#define GRD_LOW HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
//
#define GRA_HIGH HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
#define GRB_HIGH HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
#define GRC_HIGH HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
#define GRD_HIGH HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
//
#define GRA_TOGGLE HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
#define GRB_TOGGLE HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
#define GRC_TOGGLE HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
#define GRD_TOGGLE HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
//
#define GR1 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);//group 1
#define GR2 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
#define GR3 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13);
#define GR4 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14);
////////////////////////////////////////////////////////////////////////
#define R1_HIGH HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET);
#define R2_HIGH HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_SET);
#define R3_HIGH HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
#define R4_HIGH HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//
#define R1_LOW HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);
#define R2_LOW HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_RESET);
#define R3_LOW HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
#define R4_LOW HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//
#define R1_TOGGLE HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_0);
#define R2_TOGGLE HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_1);
#define R3_TOGGLE HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
#define R4_TOGGLE HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
////////////////////////////////////////////////////////////////////////

//======================================//
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim14;

/* USER CODE BEGIN PV */
  int in=0;
  int out=0;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
int _tim14=0;
int _tim14_1=0;
int hours=0;int minutes=0;int seconds=0;
bool check_clock=0;
int releys_data[4][5];//releys settings. [00]:[00]-[00]:[00]|[0]<-mode
//char menu_string[9][16];//max symbols 16
  struct string{
    char string[20];
  };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM14_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */




   int gr_out=0;//group out
   int gr_in=0;
char matrix_keypad(){
  //gr_out=0; gr_in=0;

  for(int i=0;i<4;i++){
    switch(i){
    case 0: 
      GRA_HIGH;
      gr_out=i+1;
      break;
    case 1: 
      GRB_HIGH;
      gr_out=i+1;
      break;
    case 2: 
      GRC_HIGH;
      gr_out=i+1;
      break;
    case 3: 
      GRD_HIGH;
      gr_out=i+1;
      break;
    default: 
      //gr_out=0;
      break;
    }

    //
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)==1){
      gr_in=1;
      break;
    }
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)==1){
      gr_in=2;
      break;
    }
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13)==1){
      gr_in=3;
      break;
    }
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14)==1){
      gr_in=4;
      break;
    }
    //
    GRA_LOW; GRB_LOW; GRC_LOW; GRD_LOW;
    //gr_out=0; gr_in=0;
  }
  GRA_LOW; GRB_LOW; GRC_LOW; GRD_LOW;
  //

  if(gr_out==1&&gr_in==1)return '1';  
  else if(gr_out==1&&gr_in==2)return '2';
  else if(gr_out==1&&gr_in==3)return '3';
  else if(gr_out==1&&gr_in==4)return 'A';
  //
  else if(gr_out==2&&gr_in==1)return '4';
  else if(gr_out==2&&gr_in==2)return '5';
  else if(gr_out==2&&gr_in==3)return '6';
  else if(gr_out==2&&gr_in==4)return 'B';
  //
  else if(gr_out==3&&gr_in==1)return '7';
  else if(gr_out==3&&gr_in==2)return '8';
  else if(gr_out==3&&gr_in==3)return '9';
  else if(gr_out==3&&gr_in==4)return 'C';
  //
  else if(gr_out==4&&gr_in==1)return '*';
  else if(gr_out==4&&gr_in==2)return '0';
  else if(gr_out==4&&gr_in==3)return '#';
  else if(gr_out==4&&gr_in==4)return 'D';

  gr_out=0; gr_in=0;

  //
 return 0;
}
//MENU//
void menu(struct string *menu_string, int size){
  int counter=0;
  int menu_coord_x=0;
  int menu_coord_y=0;
  bool while_active=1;
  //
  
  LCD_CLEAR;
  //lcd_send_cmd(0x02);
  lcd_send_string(menu_string[0].string);
  while (while_active==1){
    
    //while(matrix_keypad()==0);
    
    if(matrix_keypad()!=0){
     if(matrix_keypad()=='A'){
        counter--;
        if(counter<0)counter=size;
        LCD_CLEAR;
        lcd_send_string(menu_string[counter].string);
        HAL_Delay(400);
      }
      else if(matrix_keypad()=='B'){
        counter++;
        if(counter>size)counter=0;
        LCD_CLEAR;
        lcd_send_string(menu_string[counter].string);
        HAL_Delay(400);
      }
      else if(matrix_keypad()=='C'){
        while_active=0;
      }
      else if(matrix_keypad()=='D'){     
        
        
      }
    }
  }
}

//RELE//
struct string str_1[10];
void rele_settings(struct string *str_,int size){
 // str_[0].string = "";
  
  
  lcd_send_string(str_[0].string);
  
  
  if(matrix_keypad()!=0){
  
    
  }
  
}



void rele_check(){
  
}





/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
 
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM14_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  GRA_LOW; GRB_LOW; GRC_LOW; GRD_LOW;
 // HAL_TIM_Base_Start(&htim3);
 // HAL_TIM_Base_Start_IT(&htim3);
  //=========================================//
  for(int i=0;i<4;i++){
    for(int o=0;o<5;o++){
      releys_data[i][o]=0;
    }
  }
  //menus create//
  struct string str[2]={{"hello"},{"world"}};
  struct string main_menu[10]={
    {"   releys settings"},
    {"   time set"},
    {"   menu 3"},
    {"   menu 4"},
    {"   menu 5"},
    {"   menu 6"},
    {"   menu 7"},
    {"   menu 8"},
    {"   menu 9"},
    {"   menu 10"}

  };
  //end menus create//
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //char str[]="++++";
  //////////////////////
  //init matrix keypad//
  //////////////////////
 
  //////////////////////
  //struct string str={"hello world"};
  HAL_TIM_Base_Start(&htim14);
  HAL_TIM_Base_Start_IT(&htim14);

  char buff_char[5];
  char buff_char_1=0;
  char *_buff_char_1 = &buff_char_1;
  //struct string str={"hello world"};
  //HAL_Delay(1);


  
  
  lcd_init(); 
//  lcd_send_cmd(0xC0);
    
  
  lcd_send_string(str[0].string);
  HAL_Delay(500);
  LCD_CLEAR;
  HAL_Delay(50);
  lcd_send_string(str[1].string);
  HAL_Delay(500);
  LCD_CLEAR;



 // lcd_send_string("   main menu");
  //HAL_Delay(500);
  //menu(main_menu, 9);
  //lcd_send_cmd(0x08);
  //lcd_send_cmd(0x00);
  
  while (1)
  {
    if((matrix_keypad())>0){
      buff_char_1 = matrix_keypad();
      //lcd_send_string(_buff_char_1);
      ///////
      //KEY//
      ///////
      if(buff_char_1=='*'){
        lcd_send_string("=====");
        HAL_Delay(500);
        LCD_CLEAR;
        
      }
      //
      else if(buff_char_1=='A'){
        LCD_CLEAR;
        lcd_send_string("   enter_menu");
        HAL_Delay(500);
        menu(main_menu, 9);
      }
      //
      else if(buff_char_1=='B'){
        LCD_CLEAR;
        lcd_send_string("   enter_menu");
        HAL_Delay(500);
        menu(main_menu, 9);
      }
      //
      else if(buff_char_1=='#'){
        
        /*
        HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
        
        hours = sTime.Hours;
        minutes = sTime.Minutes;
        seconds = sTime.Seconds;
        */
        LCD_LINE_2;
        sprintf(buff_char, "%d", hours);
        lcd_send_string(buff_char);
        lcd_send_string(":");
        sprintf(buff_char, "%d", minutes);
        lcd_send_string(buff_char);
        lcd_send_string(":");
        sprintf(buff_char, "%d", seconds);
        lcd_send_string(buff_char);
        lcd_send_string("|");
        sprintf(buff_char, "%d", _tim14);
        lcd_send_string(buff_char);
        lcd_send_string("|");
      }
      ///////////
      //end KEY//
      ///////////
      gr_out=0; gr_in=0;
      //HAL_Delay(500);
      LCD_CLEAR;
    }
    HAL_Delay(100);
      
      //gr_out=0; gr_in=0;     
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /**Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 311;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /**Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 47999;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 1000;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PF0 PF1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA6 PA13 PA14 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
