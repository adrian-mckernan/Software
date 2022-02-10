
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
  * COPYRIGHT(c) 2020 STMicroelectronics
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
#include "stm32f0xx_hal.h"
#include "dac.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "Handles.h"
#include "MC_DAT_15R5A.h"
#include "protocol.h"
#include "LTC6946.h"
#include "LT_SPI.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint32_t DAC_Value;
uint32_t temp1;
uint32_t temp2 ;
uint32_t temp3 ;
uint8_t UARTbuff[4];
uint8_t serial_RX = 0;
int pktType;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
int checkPacket(void);
void SetPhase(uint32_t buffer);
void SetPower(uint32_t buffer);
void WriteReg(uint32_t buffer);
void SetMute(uint32_t buffer);
void Board_init(void);

uint32_t Read_LTC6946Reg(uint8_t address);
uint32_t Write_LTC6946Reg(uint8_t address, uint8_t Reg1, uint8_t Reg2);
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
  MX_DAC1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

HAL_GPIO_WritePin(DEBUG1_GPIO_Port, DEBUG1_Pin, GPIO_PIN_SET);

/* Initalise to 0V start the DAC */

	HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1,DAC_ALIGN_12B_R,0);
	 HAL_GPIO_WritePin(DEBUG1_GPIO_Port, DEBUG1_Pin, GPIO_PIN_RESET);
	
	// turn on LNA
	HAL_GPIO_WritePin(DEBUG2_GPIO_Port, DEBUG2_Pin, GPIO_PIN_SET);
	DAC_Value = 0;
	
	
	
	
// start a UART interrupt

	HAL_UART_Receive_IT(&huart1,UARTbuff,4);

Board_init();



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		


	if (serial_RX){
				
				serial_RX = 0;
		

				pktType = checkPacket();
				
	//			if (pktType > 0){
	//				HAL_GPIO_WritePin(TEST1_GPIO_PORT ,TEST1_GPIO_PIN,GPIO_PIN_SET);
	//				HAL_Delay(2);
	//				HAL_GPIO_WritePin(TEST1_GPIO_PORT ,TEST1_GPIO_PIN,GPIO_PIN_RESET);
	//			}
				// set up Interrupt again			
				HAL_UART_Receive_IT(&huart1,UARTbuff,4);				
			}		
		
		
		
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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
int checkPacket(void)
	{
		
// first check if it is valid packet and if not exit with error
//		1 wrong start value
//		2 wrong end value
//		3 wrong NODE
		
				uint32_t buffer;

			buffer = UARTbuff[0] | UARTbuff[1] << 8 | UARTbuff[2] << 16 | UARTbuff[3] << 24; 

			if ((buffer & startMask)  != startVal)	
			{
				// invalid packet
				return(1);
			}

			if ((buffer & endMask) !=  endVal)
			{
				// invalid packet
				return(2);
			}

			 temp1 = buffer & nodeMask;
			temp2 = NODE << nodeLoc;
			 
			if (((buffer & nodeMask)  !=  (NODE << nodeLoc)) && (buffer & nodeMask)  !=  0 )
			{
				temp3 = temp1 + temp2;
				// wrong node do nothing
				return(3);
			}
			temp3 = temp1 + temp1;
			
			// Read Register
				if ((buffer & funcLowMask) ==  RegReadPKT ){
					return(4);
				}
			
			// Write Register
				if ((buffer & funcLowMask) ==  RegWritePKT ){
					WriteReg(buffer);
					return(4);
				}
				
				
				// function packet
				if ((buffer & funcLowMask) ==  funPKT ){
					// this is a function packet but which one
					
					// check for initalisation packet
					if ((buffer & funcHighMask) ==  initalisePKT )
					{			
						Board_init();
						
						return(5);				
					}	
				
					// check for packet to change phase
					if ((buffer & funcHighMask) ==  phasePKT )
					{
						//adjustPhase
						SetPhase(buffer);
						return(6);
					}
					
					// check for attenuation packet
					if ((buffer & funcHighMask) ==  powerPKT)
					{
						// convert 32 bit number to 8 bit
						SetPower(buffer);	
					
						return(7);
					}
					// check for attenuation packet
					if ((buffer & funcHighMask) ==  mutePKT)
					{
						// convert 32 bit number to 8 bit
						SetMute(buffer);	
					
						return(7);
					}
					
				} else {
					return(4);
				}
			
			
			

			return(0);
		}

		
void SetPower(uint32_t buffer){
	uint8_t Power;
	uint32_t temp;
	
	temp = buffer & valueMask;
	temp = temp >> valueLoc;	
	Power = 	temp & 0x1F;	
	SetAttenuator(Power);
	
}		

void SetMute(uint32_t buffer){
	uint8_t Power;
	uint32_t temp;
	
	temp = buffer & valueMask;
	temp = temp >> valueLoc;	
	Power = 	temp & 0x1F;
	if (Power > 0) {	
		HAL_GPIO_WritePin(DEBUG2_GPIO_Port, DEBUG2_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(DEBUG2_GPIO_Port, DEBUG2_Pin, GPIO_PIN_RESET);
	}
	
}	

void SetPhase(uint32_t buffer){
	
	uint32_t DACCode;
	DACCode = buffer & valueMask;
	DACCode  = DACCode >> valueLoc;
	HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1,DAC_ALIGN_12B_R,DACCode);
}



uint32_t Read_LTC6946Reg(uint8_t address){
	return(2);
}


void WriteReg(uint32_t buffer){

	uint8_t tx[3], rx[3];
	uint8_t address, Reg1, Reg2;
	
	address  = buffer & funcHighMask >> funcHighLoc;
	Reg1 = (buffer & valueMask >> valueLoc) & 0x0F;
	Reg1 = (buffer & valueMask >> (valueLoc +8)) & 0x0F ;
	
	
	tx[0] = address << 1;
	tx[1] = Reg1;
	tx[2] = Reg2;
	spi_transfer_block(tx,rx,3);

}
	

uint32_t Write_LTC6946Reg(uint8_t address, uint8_t Reg1, uint8_t Reg2){
	
	uint8_t tx[3], rx[3];
	tx[0] = address << 1;
	tx[1] = Reg1;
	tx[2] = Reg2;
	spi_transfer_block(tx,rx,3);
	
	return(0);
}

void Board_init(){
	
	SetPhase(0);
	SetAttenuator(ATTN_0DBM);
	LTC6946_init();
	LTC6946_initRegs();

	
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	serial_RX = 1;
	
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
