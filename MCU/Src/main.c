/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <CH395.h>
#include "Delay.h"
#include "Flash_EEPROM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
// PINS
const PIN_typedef BCD1_0 = {GPIOF, GPIO_PIN_0};
const PIN_typedef BCD1_1 = {GPIOA, GPIO_PIN_10};
const PIN_typedef BCD1_2 = {GPIOA, GPIO_PIN_9};
const PIN_typedef BCD2_0 = {GPIOA, GPIO_PIN_1};
const PIN_typedef BCD2_1 = {GPIOA, GPIO_PIN_0};
const PIN_typedef BCD2_2 = {GPIOF, GPIO_PIN_1};
AntennaSelector_typedef Selector[N_SELECTORS];
// Saved data on EEPROM
SavedData_typedef SavedData;
// TODO: antenna labels non-volatile on FLASH
uint8_t IP[4] = {192, 168, 4, 1};
uint16_t port = 80;
// flags
volatile BOOL flag_PHY_change;
volatile BOOL flag_IP_conflict;
volatile BOOL flag_CH395_ready = FALSE;
const char HTTP_STR_HELLO[] = "Hello\r\n";
// web config
char ant_labels[MAX_LEN_ANT_LABELS] = {0}; // antenna labels
//extern const uint8_t FS_test_buffer[];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_NVIC_Init(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	Selector[0].PIN_BCD0 = BCD1_0;
	Selector[0].PIN_BCD1 = BCD1_1;
	Selector[0].PIN_BCD2 = BCD1_2;
	Selector[0].sel = 0;
	Selector[1].PIN_BCD0 = BCD2_0;
	Selector[1].PIN_BCD1 = BCD2_1;
	Selector[1].PIN_BCD2 = BCD2_2;
	Selector[0].sel = 0;

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
  MX_SPI1_Init();
  MX_USART1_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
// Load EEPROM data
  EEPROM_ReadBytes(&EEPROM, (uint8_t*)&SavedData, sizeof(SavedData_typedef));
  if(SavedData.EEPROM_valid_ID != EEPROM_VALID_BYTE)
  {
	  // create a new EEPROM image
	  memset(SavedData.ant_labels, 0, sizeof(SavedData.ant_labels));
	  SavedData.EEPROM_valid_ID = EEPROM_VALID_BYTE;
	  EEPROM_WriteBytes(&EEPROM, (uint8_t*)&SavedData, sizeof(SavedData_typedef));
	  EEPROM_ReadBytes(&EEPROM, (uint8_t*)&SavedData, sizeof(SavedData_typedef));
  }
  DEBUG_LOG("Self checking...\n");

//   Check FS
  FS_begin(&FS, (uint32_t*)FS_BASE_ADDR);
  FSfile_typedef file = FS_open(&FS, "/a.txt");
//   Check ch395
  uint8_t i;
RESET_CH395:
	Delay_ms(300); // wait for CH395 being ready from power on`	q1was
	CH395CMDReset();
	Delay_ms(200);
  // initialize TCP server
	flag_CH395_ready = FALSE;
  flag_CH395_ready = CH395TCPServerStart(*(uint32_t*)IP, port);
  flag_PHY_change = FALSE;
  flag_IP_conflict = FALSE;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // handle CH395 interrupt flags
	  if(flag_PHY_change || flag_IP_conflict) goto RESET_CH395;
	  if(flag_CH395_ready && ch395.RX_received)
	  {
		  HTTPHandle(&ch395);
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI0_1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SCS_GPIO_Port, SCS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PF0 PF1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SCS_Pin */
  GPIO_InitStruct.Pin = SCS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SCS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CH395_INT_Pin */
  GPIO_InitStruct.Pin = CH395_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(CH395_INT_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void interrupt_CH395()
{
	uint8_t glob_int_status, sock_int_status, i;
	BEGIN_INT_CH395:
	// read global int status
	glob_int_status = CH395CMDGetGlobIntStatus();
//	if(glob_int_status & GINT_STAT_UNREACH)
//	{
//	}
//	if(glob_int_status & GINT_STAT_IP_CONFLI)
//	{
//		flag_IP_conflict = TRUE;
//	}
	if(glob_int_status & GINT_STAT_PHY_CHANGE)
	{
		flag_PHY_change = TRUE;
	}
	glob_int_status >>= 4; // get socket interrupt status
	if(!glob_int_status) goto END_INT_CH395;
	// handle SOCKET interrupts
	for(i=1; i<=NUM_SOCKETS; ++i)
	{
		glob_int_status >>= 1;
		if(glob_int_status & 1) // the LSB of glob_int_status indicates SOCK#i interrupt status
		{
			sock_int_status = CH395GetSocketInt(i);
			if(sock_int_status & SINT_STAT_SENBUF_FREE) // Send buffer free
			{
				ch395.TX_available |= (1 << i);
				HTTPRequestParseState* pS = parseStates +i -1;
				if(pS->response_stage == RESPONSE_CONTENT_REMAIN)
				{
					uint16_t len = pS->len_response_content_remain;
					CH395SendData(i, pS->response_content, ((len<MAX_SIZE_PACK)?(len):(len=MAX_SIZE_PACK)));
					// sent
					pS->len_response_content_remain -= len;
					pS->response_content += len; // move the cursor
					if(pS->len_response_content_remain == 0)
						resetHTTPParseState(pS);
					// TODO: multiple packages
				}
				else if(pS->response_stage == RESPONSE_PREPARED)
				{
					uint16_t max_len_content = MAX_SIZE_PACK - pS->len_response_header;
					uint16_t len_content_this_time = ((pS->len_response_content_remain < max_len_content)
							? (pS->len_response_content_remain)
									: (max_len_content));
					CH395StartSendingData(i, pS->len_response_header + len_content_this_time);
					CH395ContinueSendingData(pS->response_header, pS->len_response_header);
					CH395ContinueSendingData(pS->response_content, len_content_this_time);
					CH395Complete();
					pS->len_response_content_remain -= len_content_this_time;
					pS->response_content += len_content_this_time;
					if(pS->len_response_content_remain == 0) // all content completely sent this time
					{
						pS->response_stage = RESPONSE_NOT_PREPARED;
					}
					else // content remained to be sent next time
					{
						pS->response_stage = RESPONSE_CONTENT_REMAIN;
					}
				}
			}
//			if(sock_int_status & SINT_STAT_SEND_OK)
//			{
//			}
			if(sock_int_status & SINT_STAT_RECV)
			{
				HTTPRequestParseState* pS = parseStates + i - 1;
				uint16_t len = CH395GetRecvLength(i);
				CH395GetRecvData(i, (len < CH395_SIZE_BUFFER)?(len):(CH395_SIZE_BUFFER-1), ch395.buffer);
				resetHTTPParseState(pS);
				pS->sock_index = i;
				if(parse_http(pS, ch395.buffer))
				{
					ch395.RX_received |= (1 << i);
				}
				CH395ClearRecvBuf(i);
			}
			if(sock_int_status & SINT_STAT_CONNECT)
			{
				ch395.socket_connected |= (1 << i);
			}
			if(sock_int_status & SINT_STAT_DISCONNECT)
			{
				ch395.socket_connected &= ~(1<<i);
				ch395.RX_received &= ~(1<<i);
			}
//			if(sock_int_status & SINT_STAT_TIM_OUT)
//			{
//			}
		}
	}
	END_INT_CH395:
	return;
//	if(HAL_GPIO_ReadPin(CH395_INT_GPIO_Port, CH395_INT_Pin) == GPIO_PIN_RESET) goto BEGIN_INT_CH395;
}

void switch_Antenna(uint8_t A, uint8_t B)
{
	uint8_t i;
	Selector[0].sel = A;
	Selector[1].sel = B;
	if(A==B && A>0) return;
	for(i=0; i<N_SELECTORS; ++i)
	{
		PIN_typedef pin0 = Selector[i].PIN_BCD0;
		PIN_typedef pin1 = Selector[i].PIN_BCD1;
		PIN_typedef pin2 = Selector[i].PIN_BCD2;
		uint8_t val = Selector[i].sel;
		HAL_GPIO_WritePin(pin0.group, pin0.pin, val & 1);
		HAL_GPIO_WritePin(pin1.group, pin1.pin, (val >> 1) & 1);
		HAL_GPIO_WritePin(pin2.group, pin2.pin, (val >> 2) & 2);
	}
}

uint8_t get_Antenna() //0-3:SEL1 4-7:SEL2
{
	uint8_t d = CH395ReadGPIOAddr(GPIO_IN_REG); // read CH395 GPIO
	uint8_t res = 0;
	res |= (d >> BCDM1_0) & 1;// bit 0: BCDM1_0
	res |= ((d >> BCDM1_1) & 1) << 1;// bit 1: BCDM1_1
	res |= ((d >> BCDM1_2) & 1) << 2;// bit 2: BCDM1_2
	res |= ((d >> BCDM2_0) & 1) << 4;// bit 0: BCDM2_0
	res |= ((d >> BCDM2_1) & 1) << 5;// bit 1: BCDM1_1
	res |= ((d >> BCDM2_2) & 1) << 6;// bit 1: BCDM1_1
	return res;
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
