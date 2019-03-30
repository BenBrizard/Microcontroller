
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "LCD.h"
#include <math.h>
#include "Part_1.c"
#include "Part_2.c"
#include "Part_3.c"


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//-------------- -----------------------------------PART 1

uint32_t captures[2]={0};//temps enregistré des captures
// Interrupts flags - Begin
volatile uint8_t captureDone=0;
volatile uint8_t flag_half_second =0;
volatile uint8_t flag_tempo = 0;
volatile uint8_t DMA_done=0;
uint8_t DMA_state = 0;
int note_buffer = 0;

uint32_t notes_musique;
const uint16_t ZELDA_LUT_SIZE=128;
const uint16_t MARIO_LUT_SIZE=304;
const int8_t ZELDA_LUT[]={
21,14,10,7,19,14,10,7,18,14,10,7,19,14,10,7,19,12,9,5,17,12,9,5,16,12,9,5,
17,12,9,5,17,10,7,4,16,10,7,4,15,10,7,4,16,10,7,4,16,9,5,2,14,9,5,2,13,9,5,2,
14,9,5,2,21,14,10,7,19,14,10,7,18,14,10,7,19,14,10,7,22,15,12,9,21,15,12,9,
20,15,12,9,21,15,12,9,24,14,10,7,22,14,10,7,21,14,10,7,22,14,10,7,21,10,7,4,19,10,7,4,
17,10,7,4,16,10,7,4
};
const int8_t MARIO_LUT[]={
16,16,-1,16,-1,12,16,-1,19,-1,-1,-1,7,-1,-1,12,-1,-1,7,-1,-1,4,-1,-1,9,-1,11,-1,10,9,-1,7,
16,-1,19,21,-1,17,19,-1,16,-1,12,-1,14,11,-1,-1,19,18,17,14,-1,16,-1,7,9,12,-1,9,12,14,-1,19,
18,17,14,-1,16,-1,24,-1,24,24,-1,-1,-1,-1,-1,19,18,17,14,-1,16,-1,7,9,12,-1,9,12,14,-1,-1,15,-1,-1,
14,-1,-1,12,-1,-1,-1,-1,-1,12,12,-1,12,-1,12,14,-1,16,12,-1,9,7,-1,-1,-1,12,12,-1,12,-1,12,14,16,-1,-1,-1,-1,-1,
12,12,-1,12,-1,12,14,-1,16,12,-1,9,7,-1,-1,16,16,-1,16,-1,12,16,-1,19,-1,-1,-1,7,-1,-1,-1,12,-1,-1,7,-1,-1,4,-1,-1,9,-1,
11,-1,10,9,-1,7,16,-1,19,21,-1,17,19,-1,16,-1,12,-1,14,11,-1,-1,12,-1,-1,7,-1,-1,4,-1,-1,9,-1,11,-1,10,9,-1,7,16,-1,19,21,-1,17,
19,-1,16,-1,12,-1,14,11,-1,-1,-1,16,12,-1,7,-1,-1,7,-1,9,17,-1,17,9,-1,11,-1,21,-1,21,-1,21,19,-1,17,-1,16,12,-1,9,7,-1,-1,
16,12,-1,7,-1,-1,7,-1,-1,9,17,-1,17,9,-1,-1,11,-1,17,-1,17,-1,17,-1,16,-1,14,-1,12,7,-1,4,0,-1,-1,-1,-1,-1
};
// Interrupts flags - End

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef* hdac);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_DAC_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	LCD_Begin();

	
	HAL_TIM_Base_Start_IT(&htim4);	
	HAL_TIM_IC_Start_DMA(&htim3, TIM_CHANNEL_3, (uint32_t*) captures, 2);
	HAL_TIM_Base_Start_IT(&htim2);
	
	DerroulementMenu(); // Affichage du menu d'accueil
	update_tab(to_process, &setting1);//on demarre les ping-pong buffer
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)to_process,DMA_SIZE, DAC_ALIGN_12B_R);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		if(setting1.Mode == MODE0 || setting1.Mode == MODE1 || setting1.Mode == MODE2){
			if(captureDone==1)//chaque 1ms
			{//pour que note[0] change, on doit attendre 10 ms, donc on ne vérifie que si it=0, car cest apres les 10ms que note est update
				captureDone=0;
				note_buffer = note[0];//keep track of the last note
				refresh_distance();//update la distance 
				if( it==0 && note[0]>=0 && note_buffer != note[0])
				{
					play_note(note[0], setting1.Instrument);//change the note output by DMA
					
					if(setting1.Mode != MODE1) 
					{
							iterateur_adsr = 0;//on recommence l'enveloppe (on rejoue la note du début)
							update_tab(to_write, &setting1);
						
					
							if(DMA_state == 0)
							{
								// Si le DMA est à OFF, on le restart avec la bonne note que le sensor vient de compute
								//on veut le to_wite, on vient de le compute...
								uint32_t * temp =to_write;//temp : celui quon vient de compute
								to_write=to_process;
								to_process=temp;
								HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)to_process,DMA_SIZE, DAC_ALIGN_12B_R);
								update_tab(to_write, &setting1);
								DMA_state = 1;// DMA est à ON
							}
					}
				}
				// we are changing note. Restart the adsr iterator and stop the DMA (stop playing the last
				// note and compute the ping-pong buffer for the new one
				else if(note[0]<0) 
				{
					iterateur_adsr = 0;
					DMA_state = 0; // DMA est à OFF
				}
				
		}
			
			if(flag_half_second==1)
			{
				flag_half_second=0;
				switch (setting1.Mode){
					case MODE0:
					showNoteMode0(&position02);
					break;
					
					case MODE1:
						showNoteMode1(&position1);
					break;
					
					case MODE2:
						showNoteMode2(&position02);
					break;
				}
			}
		}
		else if(setting1.Mode == MODEMARIO || setting1.Mode == MODEZELDA){
			if(setting1.Mode == MODEMARIO){
				if(flag_tempo==1)
				{
					flag_tempo = 0;
					setting1.Instrument = 0;
					play_note(MARIO_LUT[notes_musique],4);
					iterateur_adsr = 0;
					notes_musique++;					
					if(notes_musique == MARIO_LUT_SIZE) notes_musique = 0;
				}
			}
			
			else if(setting1.Mode == MODEZELDA){
				if(flag_tempo==1)
				{
					flag_tempo = 0;
					play_note(ZELDA_LUT[notes_musique],4);
					iterateur_adsr = 0;
					notes_musique++;					
					if(notes_musique == ZELDA_LUT_SIZE) notes_musique = 0;
				}
			}
		}
	
		if(DMA_done==1)
		{
			if(to_write==dac_buffer_ping)
			{
				to_write=dac_buffer_pong;
				to_process=dac_buffer_ping;

			}
			else
			{
				to_write=dac_buffer_ping;
				to_process=dac_buffer_pong;

			}
			if(note[0]>=0|| DMA_state!=0) 
			{
				HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)to_process,DMA_SIZE, DAC_ALIGN_12B_R);
				update_tab(to_write, &setting1);
			}
			DMA_done=0;
		
		}
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

//-----------------------------------------------------------------------------PART1

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

 
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		
			captureDone = 1;//chaque fois que le input capture a détecté 2 edges
	}

 
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if(htim->Instance == TIM2)
	{	
		static uint8_t local_time=0;
		static uint8_t tempo = 0;
		local_time++;
		tempo++;
		if(local_time >= 5)
		{	
			local_time =0;
			flag_half_second = 1;//chaque 0,5 seconde on refresh l'écran
		}	
		if(tempo >= 2)
		{	
			flag_tempo = 1;
			tempo = 0;
		}	
	}
}

//--------------------------------------------------------------------------------------------PART2

void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef* hdac)
{

		DMA_done=1;//quand le dma est arrivé au bout du tableau pour feeder le DAC
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
