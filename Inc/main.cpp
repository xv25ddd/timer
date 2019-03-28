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
#include <string>
#include <cstring>
#include "i2c-lcd.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LCD_CLEAR  lcd_send_cmd(0x01);
#define LCD_LINE_2 lcd_send_cmd (0xc0);
#define LCD_CLEAR_LINE lcd_send_string("          ");
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
TIM_HandleTypeDef htim14;

/* USER CODE BEGIN PV */
  int in=0;
  int out=0;
  //
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    //
int _tim14=0;
//int _tim14_1=0;
//
int hours=0;int minutes=0;int seconds=0;
//
//bool check_clock=0;
//
char buff_char[20];
//char buff_char_1=0;
int buff_int=255;
//
enum{
  rele_1,
  rele_2,
  rele_3,
  rele_4
};
enum{
  rele_mode,  
  value_1,
  value_2,
  value_3,
  value_4
  
};

int settings_rele[rele_4+1][value_4+1];
bool on_off[4]={0,0,0,0};
const int max_rele=4;
//



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM14_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



   int gr_out=0;//group out
   int gr_in=0;
int matrix_keypad(){
  //gr_out=0; gr_in=0;
  //cross-scan matrix keypad
  /*
        1 2 3 4
        | | | |
    GRA - - - -
        | | | |
    GRB - - - -
        | | | |
    GRC - - - -
        | | | |
    GRD - - - -
    group A, B, C, D - up in course
    group 0, 1, 2, 3 - read in course when one of the groups is active
    if read result 0 - read next group
    look return values
  */
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
    GRA_LOW; GRB_LOW; GRC_LOW; GRD_LOW;//group A, B, C, D = LOW
    //gr_out=0; gr_in=0;
  }
  GRA_LOW; GRB_LOW; GRC_LOW; GRD_LOW;
  //
  //gr_out n gr_in - mandatory for zeroing
  if(gr_out==1&&gr_in==1){gr_out=0; gr_in=0;return 1;}
  else if(gr_out==1&&gr_in==2){gr_out=0; gr_in=0;return 2;}
  else if(gr_out==1&&gr_in==3){gr_out=0; gr_in=0;return 3;}
  else if(gr_out==1&&gr_in==4){gr_out=0; gr_in=0;return 10;}//A
  //
  else if(gr_out==2&&gr_in==1){gr_out=0; gr_in=0;return 4;}
  else if(gr_out==2&&gr_in==2){gr_out=0; gr_in=0;return 5;}
  else if(gr_out==2&&gr_in==3){gr_out=0; gr_in=0;return 6;}
  else if(gr_out==2&&gr_in==4){gr_out=0; gr_in=0;return 11;}//B
  //
  else if(gr_out==3&&gr_in==1){gr_out=0; gr_in=0;return 7;}
  else if(gr_out==3&&gr_in==2){gr_out=0; gr_in=0;return 8;}
  else if(gr_out==3&&gr_in==3){gr_out=0; gr_in=0;return 9;}
  else if(gr_out==3&&gr_in==4){gr_out=0; gr_in=0;return 12;}//C
  //
  else if(gr_out==4&&gr_in==1){gr_out=0; gr_in=0;return 14;}//*
  else if(gr_out==4&&gr_in==2){gr_out=0; gr_in=0;return 0;}
  else if(gr_out==4&&gr_in==3){gr_out=0; gr_in=0;return 15;}//#
  else if(gr_out==4&&gr_in==4){gr_out=0; gr_in=0;return 13;}//D
  gr_out=0; gr_in=0;

  //
 return 255;//if no one touched matrix_keypad
 /*
 when calling a function, you can ask - if(matrix_keypad()!=255)
  assign the result of a function in int variable and
  compare it with the key codes
*/
 
}
//MENU//

//
//system functions
int _atoi(char *string){
  int size = strlen(string);
//  int returned_value=0;
  int buff_to_atoi;
  int buff_to_atoi_1=0;
  for(int i=0;i<size;i++){
    if(string[i]=='0')buff_to_atoi=0;
    else if(string[i]=='1')buff_to_atoi=1;
    else if(string[i]=='2')buff_to_atoi=2;
    else if(string[i]=='3')buff_to_atoi=3;
    else if(string[i]=='4')buff_to_atoi=4;
    else if(string[i]=='5')buff_to_atoi=5;
    else if(string[i]=='6')buff_to_atoi=6;
    else if(string[i]=='7')buff_to_atoi=7;
    else if(string[i]=='8')buff_to_atoi=8;
    else if(string[i]=='9')buff_to_atoi=9;
    if(i==0)
      buff_to_atoi_1 = buff_to_atoi;
    else if(i<size)
      buff_to_atoi_1=buff_to_atoi_1*10+buff_to_atoi;
  }
  return buff_to_atoi_1;
}
//
int entering_num(int previous_value){
  LCD_CLEAR;
  lcd_send_string("enter_num");
  int return_val=0;
  while(1){
    HAL_Delay(250);
    if(matrix_keypad()!=255){
      buff_int=matrix_keypad();
      if(buff_int<10){
        return_val=
          return_val*10+buff_int;
        //
        LCD_CLEAR;
        lcd_send_string("   ");
        sprintf(buff_char, "%d", return_val);        
        lcd_send_string(buff_char);
      }
      else if(buff_int==12){
        return_val=0;
        return previous_value;
      }
      else if(buff_int==13){
        return return_val;
      }
    }
  }
}
//end system functions
void rele_settings(int num_rele);
void releys_check();
void set_time();

//RELE//




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
  /* USER CODE BEGIN 2 */
  GRA_LOW; GRB_LOW; GRC_LOW; GRD_LOW;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_TIM_Base_Start(&htim14);
  HAL_TIM_Base_Start_IT(&htim14);
  
  HAL_Delay(200);
  R1_HIGH;
  HAL_Delay(200);
  R2_HIGH;
  HAL_Delay(200);
  R3_HIGH;
  HAL_Delay(200);
  R4_HIGH;
  
  lcd_init(); 
  HAL_Delay(500);

  while (1)
  {
    if((matrix_keypad())!=255){
      buff_int = matrix_keypad();  
      ///////
      //KEY//
      ///////
      if(buff_int==10){
        //rele_settings(buff_int);
        LCD_CLEAR;        
      }
      //
      else if(buff_int==11){
        //rele_settings(buff_int);
        LCD_CLEAR;
      }
      else if(buff_int==14){
        set_time();
      }
      else if(buff_int<5&&buff_int>0){
        rele_settings(buff_int);
      }
      ///////////
      //end KEY//
      ///////////
      //gr_out=0; gr_in=0;
      //HAL_Delay(500);
      LCD_CLEAR;
    }//end of matrix_keypad!=255
    if(_tim14>5){
//      releys_check();
      _tim14=0;
      releys_check();
      LCD_CLEAR;
      lcd_send_string("   ");
      for(int i=0;i<4;i++){
        sprintf(buff_char, "%d", on_off[i]);
        lcd_send_string(buff_char);
        lcd_send_string("|");
      }
      LCD_LINE_2;
      /*
      DS1307_ReadData();
      for(int i=0;i<7;i++){
        sprintf(buff_char, "%d", DS1307_Read[i]);
        lcd_send_string(buff_char);
        lcd_send_string(":");
      }
*/
        sprintf(buff_char, "%d", hours);
        lcd_send_string(buff_char);
        lcd_send_string(":");
        sprintf(buff_char, "%d", minutes);
        lcd_send_string(buff_char);
        lcd_send_string(":");
        sprintf(buff_char, "%d", seconds);
        lcd_send_string(buff_char);

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
/////////
//MENUS//
/////////
//
void rele_settings(int num_rele){
  int counter_for_menu=0;
  num_rele = num_rele-1;
  while(1){
    HAL_Delay(250);
    buff_int=matrix_keypad();
    //
    LCD_CLEAR;
    //counter 0
    if(counter_for_menu==0){
      lcd_send_string("   rele mode");
      LCD_LINE_2;
      if(settings_rele[num_rele][rele_mode]==0){
        lcd_send_string("minute timer");
        //settings_rele[num_rele-1][rele_mode]=1;
      }
      else if(settings_rele[num_rele][rele_mode]==1){
        lcd_send_string("daily timer");
        //settings_rele[num_rele-1][rele_mode]=0;
      }
    }
    //
    else if(counter_for_menu==1){//value 1
      if(settings_rele[num_rele][rele_mode]==0){
        lcd_send_string("   minute range 1");
      }
      else if(settings_rele[num_rele][rele_mode]==1){
        lcd_send_string("   hour 1");
      }      
      LCD_LINE_2;
      sprintf(buff_char, "%d", settings_rele[num_rele][value_1]);
      lcd_send_string(buff_char);
    }
    else if(counter_for_menu==2){//value 2
      if(settings_rele[num_rele][rele_mode]==0){
        lcd_send_string("   minute range 2");
      }
      else if(settings_rele[num_rele][rele_mode]==1){
        lcd_send_string("   minute 1");
      }      
      LCD_LINE_2;
      sprintf(buff_char, "%d", settings_rele[num_rele][value_2]);
      lcd_send_string(buff_char);
    }
    else if(counter_for_menu==3){//value 3
      lcd_send_string("   hour 2");
      LCD_LINE_2;
      sprintf(buff_char, "%d", settings_rele[num_rele][value_3]);
      lcd_send_string(buff_char);
    }
    else if(counter_for_menu==4){//value 4
      lcd_send_string("   minute 2");
      LCD_LINE_2;
      sprintf(buff_char, "%d", settings_rele[num_rele][value_4]);
      lcd_send_string(buff_char);
    }
    
    //
    if(buff_int!=255){
      //
      if(buff_int==10){
        counter_for_menu--;
        if(counter_for_menu<0){
          if(settings_rele[num_rele][rele_mode]==0)
            counter_for_menu=2;
          else if(settings_rele[num_rele][rele_mode]==1)
            counter_for_menu=4;
        }
      }
      //
      else if(buff_int==11){
        counter_for_menu++;
        if(settings_rele[num_rele][rele_mode]==0&&
           counter_for_menu>2)counter_for_menu=0;
        else if(settings_rele[num_rele][rele_mode]==1&&
           counter_for_menu>4)counter_for_menu=0;
      }
      //
      else if(buff_int==12){
        break;
      }
      //
      else if(buff_int==13){
        if(counter_for_menu==0){
          if(settings_rele[num_rele][rele_mode]==0)
            settings_rele[num_rele][rele_mode]=1;
          else if(settings_rele[num_rele][rele_mode]==1)
            settings_rele[num_rele][rele_mode]=0;
        }
        //
        else if(counter_for_menu==1){
          settings_rele[num_rele][value_1]=entering_num(
                                                        settings_rele[num_rele][value_1]
                                                        );
        }
        else if(counter_for_menu==2){
          settings_rele[num_rele][value_2]=entering_num(
                                                        settings_rele[num_rele][value_2]
                                                        );
        }
        else if(counter_for_menu==3){
          settings_rele[num_rele][value_3]=entering_num(
                                                        settings_rele[num_rele][value_3]
                                                        );
        }
        else if(counter_for_menu==4){
          settings_rele[num_rele][value_4]=entering_num(
                                                        settings_rele[num_rele][value_4]
                                                        );
        }
        //
      }
      //
    }
  }
}
int buff_to_releys_check[4]={0,0,0,0};
void releys_check(){
  int summary_minute=hours*60+minutes,
      summary_minute_1=0,//WARNING!!!
      summary_minute_2=0,//used in daily timer!!!
      difference=0;
  
  for(int i=0;i<4;i++){
    if(settings_rele[i][rele_mode]==0){//minute timer
      if(buff_to_releys_check[i]==0)
          buff_to_releys_check[i]=summary_minute;
      difference=summary_minute-buff_to_releys_check[i];
      if(difference<1)difference=
        difference+1439;
      //
      if(on_off[i]==0){
        if(difference==settings_rele[i][value_1]){
          on_off[i]=1;
          buff_to_releys_check[i]=summary_minute;
        }        
      }
      else if(on_off[i]==1){
        if(difference==settings_rele[i][value_2]){
          on_off[i]=0;
          buff_to_releys_check[i]=summary_minute;
        }        
      }
    }//end minute timer
    //
    else if(settings_rele[i][rele_mode]==1){//daily timer      
      summary_minute_1=settings_rele[i][value_1]*60+
        settings_rele[i][value_2];
      summary_minute_2=settings_rele[i][value_3]*60+
        settings_rele[i][value_4];
      if(summary_minute>summary_minute_1&&
         summary_minute<summary_minute_2){
          on_off[i]=1;
      }
      else{
        on_off[i]=0;
      }
    }//end daily timer
    
  }
  if(on_off[0]==0)R1_HIGH;
  if(on_off[0]==1)R1_LOW;
  //
  if(on_off[1]==0)R2_HIGH;
  if(on_off[1]==1)R2_LOW;
  //
  if(on_off[2]==0)R3_HIGH;
  if(on_off[2]==1)R3_LOW;
  //
  if(on_off[3]==0)R4_HIGH;
  if(on_off[3]==1)R4_LOW;  
}
void set_time(){
  int counter=0; 
  while(1){
    HAL_Delay(250);
    buff_int=matrix_keypad();    
    LCD_CLEAR;
    if(counter==0){
      lcd_send_string("   hour");
      LCD_LINE_2;
      sprintf(buff_char, "%d", hours);
      lcd_send_string(buff_char);
    }
    else if(counter==1){
      lcd_send_string("   minute");
      LCD_LINE_2;
      sprintf(buff_char, "%d", minutes);
      lcd_send_string(buff_char);
    }
    if(buff_int!=255){
      if(buff_int==10){
        counter--;
        if(counter<0)counter=1;
      }
      else if(buff_int==11){
        counter++;
        if(counter>1)counter=0;
      }
      else if(buff_int==12){
        for(int i=0;i<4;i++)buff_to_releys_check[i]=0;        
        break;
      }
      else if(buff_int==13){
        if(counter==0)hours=entering_num(hours);
        else if(counter==1)minutes=entering_num(minutes);
      }
    }
  }
}

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
