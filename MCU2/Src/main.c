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
#include <stdbool.h>
#include "CH395.h"
#include "Lib485.h"
#include "Delay.h"
#include "Flash_EEPROM.h"
#include "FS.h"

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
IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// PINS
const PIN_typedef BCD1_0 = {BCD1_0_GPIO_Port, BCD1_0_Pin};
const PIN_typedef BCD1_1 = {BCD1_1_GPIO_Port, BCD1_1_Pin};
const PIN_typedef BCD1_2 = {BCD1_2_GPIO_Port, BCD1_2_Pin};
const PIN_typedef BCD2_0 = {BCD2_0_GPIO_Port, BCD2_0_Pin};
const PIN_typedef BCD2_1 = {BCD2_1_GPIO_Port, BCD2_1_Pin};
const PIN_typedef BCD2_2 = {BCD2_2_GPIO_Port, BCD2_2_Pin};
const PIN_typedef RW485 = {RW485_GPIO_Port, RW485_Pin};
AntennaSelector_typedef Selector[N_SELECTORS];
// Saved data on EEPROM
SavedData_typedef SavedData;
// TODO: antenna labels non-volatile on FLASH
uint8_t IP[4] = {192, 168, 4, 1};
uint16_t port = 80;
// flags
volatile BOOL flag_PHY_reconn = false;
volatile BOOL flag_IP_conflict = false;
volatile BOOL flag_CH395_ready = false;
const char HTTP_STR_HELLO[] = "Hello\r\n";
// web config
char ant_labels[MAX_LEN_ANT_LABELS] = {0}; // antenna labels
//extern const uint8_t FS_test_buffer[];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

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
	uint8_t i;
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
  // init 485
  begin_serial485(p485, &huart2, RW485, 1);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  // Check EEPROM
  EEPROM.base_addr = (uint8_t*)EEPROM_BASE_ADDR;
  EEPROM.size = 400;
  EEPROM_ReadBytes(&EEPROM, (uint8_t*)&SavedData, sizeof(SavedData_typedef));
  if(SavedData.EEPROM_valid_ID != EEPROM_VALID_BYTE)
  {
	  // create a new EEPROM image
	  for(i=0; i<NUM_ANTENNA; ++i)
	  {
		  strcpy(SavedData.ant_labels[i], "Ant");
		  SavedData.ant_labels[i][3] = '1' + i;
		  SavedData.ant_labels[i][4] = 0;
	  }
	  SavedData.EEPROM_valid_ID = EEPROM_VALID_BYTE;
	  EEPROM_WriteBytes(&EEPROM, (uint8_t*)&SavedData, sizeof(SavedData_typedef));
	  EEPROM_ReadBytes(&EEPROM, (uint8_t*)&SavedData, sizeof(SavedData_typedef));
  }
  DEBUG_LOG("Self checking...\n");
//   Check FS
  FS_begin(&FS, FS_BASE_ADDR);
  i =  FS_exists(&FS, "/index.html");
  i = FS_exists(&FS, "/b.f");

  // USART
//  begin_serial485(p485, &huart1, RW485, SCHED_INTERVAL / 1000);
//  HAL_UART_Receive_IT(&huart1, p485->rx_buffer, 1);
  // don't forget to override the callback function of USART1
//   Check ch395
	Delay_ms(350); // wait for CH395 being ready from power on
	for(i=0; i<0xff;++i)
	{
		if(CH395CMDCheckExist(i) != (uint8_t)~i)
		{
			DEBUG_LOG("CH395 self check error!\n");
			break;
		}
	}
	  HAL_IWDG_Refresh(&hiwdg); // feed dog
RESET_CH395:
	reset_CH395();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // handle CH395 interrupt flags
	  if(flag_PHY_reconn && !flag_CH395_ready) goto RESET_CH395;
	  HAL_IWDG_Refresh(&hiwdg); // feed dog
	  if(flag_CH395_ready && ch395.RX_received)
	  {
		  HTTPHandle(&ch395);
	  }
	  while(HAL_GPIO_ReadPin(CH395_INT_GPIO_Port, CH395_INT_Pin) == GPIO_PIN_RESET)
		  interrupt_CH395();
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Window = 2047;
  hiwdg.Init.Reload = 2047;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, SPI1_CS_Pin|BCD2_2_Pin|BCD2_1_Pin|BCD2_0_Pin 
                          |BCD1_2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RW485_Pin|BCDM2_2_Pin|BCDM2_1_Pin|BCDM2_0_Pin 
                          |BCD1_1_Pin|BCD1_0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PF0 PF1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : CH395_INT_Pin */
  GPIO_InitStruct.Pin = CH395_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(CH395_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_CS_Pin BCD2_2_Pin BCD2_1_Pin BCD2_0_Pin 
                           BCD1_2_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin|BCD2_2_Pin|BCD2_1_Pin|BCD2_0_Pin 
                          |BCD1_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RW485_Pin BCDM2_2_Pin BCDM2_1_Pin BCDM2_0_Pin 
                           BCD1_1_Pin BCD1_0_Pin */
  GPIO_InitStruct.Pin = RW485_Pin|BCDM2_2_Pin|BCDM2_1_Pin|BCDM2_0_Pin 
                          |BCD1_1_Pin|BCD1_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BCDM1_2_Pin */
  GPIO_InitStruct.Pin = BCDM1_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BCDM1_2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BCDM1_1_Pin BCDM1_0_Pin */
  GPIO_InitStruct.Pin = BCDM1_1_Pin|BCDM1_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	onReceived_serial485(p485);
//}

void reset_CH395()
{
  flag_CH395_ready = false;
  ch395.RX_received = 0;
  ch395.SOCK_responding = -1;
  ch395.TX_available = 0xff;
  ch395.socket_connected = 0;
	CH395CMDReset();
	Delay_ms(100); // wait for CH395 being ready from power on
	// initialize CH395 GPIO settings to INPUT/PULL DOWN
//	CH395WriteGPIOAddr(GPIO_DIR_REG, 0);
//	CH395WriteGPIOAddr(GPIO_PU_REG, 0);
//	CH395WriteGPIOAddr(GPIO_PD_REG, 0xff);
	CH395SetBuffer();
  // initialize TCP server
  flag_CH395_ready = CH395TCPServerStart(*(uint32_t*)IP, port);
  //CH395SetBuffer();
  flag_PHY_reconn= false;
  flag_IP_conflict = false;
}

void interrupt_CH395()
{
	uint8_t sock_int_status, i;
	uint16_t glob_int_status;
	//BEGIN_INT_CH395:
	// read global int status
	//glob_int_status = CH395CMDGetGlobIntStatus();
	glob_int_status = CH395CMDGetGlobIntStatus_ALL();
//	if(glob_int_status == 0)
//	{
//		reset_CH395();
//		Delay_ms(300);
//	}
	if(glob_int_status & GINT_STAT_UNREACH)
	{
		uint8_t unreach[8];
		CH395CMDGetUnreachIPPT(unreach);
		reset_CH395();
	}
//	if(glob_int_status & GINT_STAT_IP_CONFLI)
//	{
//		flag_IP_conflict = true;
//	}
	if(glob_int_status & GINT_STAT_DHCP)
	{
		CH395GetDHCPStatus();
	}
	if(glob_int_status & GINT_STAT_PHY_CHANGE)
	{
		switch(CH395CMDGetPHYStatus())
		{
			case PHY_DISCONN:
				flag_CH395_ready = false;
				break;
			default:
				flag_PHY_reconn = true;
		}
	}
	glob_int_status >>= 4; // get socket interrupt status
	if(!glob_int_status) goto END_INT_CH395;
	// handle SOCKET interrupts
	for(i=1; i<NUM_SOCKETS; ++i)
	{
		glob_int_status >>= 1;
		if(glob_int_status & 1) // the LSB of glob_int_status indicates SOCK#i interrupt status
		{
			sock_int_status = CH395GetSocketInt(i);
			if(sock_int_status & SINT_STAT_SENBUF_FREE) // Send buffer free
			{
				ch395.TX_available |= (1 << i);
			}
			if(sock_int_status & SINT_STAT_RECV) // data received on SOCK i
			{
				/*
				 * This section of code reads at most CH395_SIZE_BUFFER-1 bytes from PHY buffer
				 * and discards the rest of the received data
				 */
				HTTPRequestParseState* pS = parseStates + i - 1;
				uint16_t len = CH395GetRecvLength(i);
				CH395GetRecvData(i, (len < CH395_SIZE_BUFFER-1)?(len):(len = CH395_SIZE_BUFFER-1), (uint8_t*)ch395.buffer);
				ch395.buffer[len] = 0; // terminate the recved text stream
				resetHTTPParseState(pS);
				pS->sock_index = i;
				if(parse_http(pS, ch395.buffer))
				{
					ch395.RX_received |= (1 << i); // mark received flag
					if(ch395.SOCK_responding < 0) // put this socket into responding slot if the slot is empty
						ch395.SOCK_responding = i;
				}
				CH395ClearRecvBuf(i);
			}
			if(sock_int_status & SINT_STAT_CONNECT) // SOCK i connected
			{
				ch395.socket_connected |= (1 << i);
				ch395.TX_available |= (1 << i);
			}
			if(sock_int_status & SINT_STAT_DISCONNECT) // SOCK i disconnected
			{
				ch395.socket_connected &= ~(1<<i);
				ch395.RX_received &= ~(1<<i);
				ch395.TX_available |= (1 << i);
				if(ch395.SOCK_responding == i) ch395.SOCK_responding = -1; // release the responding socket flag
			}
//			if(sock_int_status == 0)
//			{
//				reset_CH395();
//				Delay_ms(100);
//				goto END_INT_CH395;
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
		uint8_t val = ~Selector[i].sel;
		HAL_GPIO_WritePin(pin0.group, pin0.pin, val & 1);
		HAL_GPIO_WritePin(pin1.group, pin1.pin, (val >> 1) & 1);
		HAL_GPIO_WritePin(pin2.group, pin2.pin, (val >> 2) & 1);
	}
}

uint8_t get_Antenna() //3-0:SEL1[2:0] 7-4:SEL2[2:0] GPIOs are Low Effective
{
	uint8_t res = 0;
#ifdef BCDM_ON_CH395
	uint8_t d;
	d = CH395ReadGPIOAddr(GPIO_IN_REG); // read CH395 GPIO
	res |= (d >> BCDM1_0) & 1;// bit 0: BCDM1_0
	res |= ((d >> BCDM1_1) & 1) << 1;// bit 1: BCDM1_1
	res |= ((d >> BCDM1_2) & 1) << 2;// bit 2: BCDM1_2
	res |= ((d >> BCDM2_0) & 1) << 4;// bit 0: BCDM2_0
	res |= ((d >> BCDM2_1) & 1) << 5;// bit 1: BCDM1_1
	res |= ((d >> BCDM2_2) & 1) << 6;// bit 1: BCDM1_1
#else
	res |= !HAL_GPIO_ReadPin(BCDM1_0_GPIO_Port, BCDM1_0_Pin); // BCDM1[0]
	res |= !HAL_GPIO_ReadPin(BCDM1_1_GPIO_Port, BCDM1_1_Pin) << 1; // BCDM1[1]
	res |= !HAL_GPIO_ReadPin(BCDM1_2_GPIO_Port, BCDM1_2_Pin) << 2; // BCDM1[2]
	res |= !HAL_GPIO_ReadPin(BCDM2_0_GPIO_Port, BCDM2_0_Pin) << 4; // BCDM2[0]
	res |= !HAL_GPIO_ReadPin(BCDM2_1_GPIO_Port, BCDM2_1_Pin) << 5; // BCDM2[1]
	res |= !HAL_GPIO_ReadPin(BCDM2_2_GPIO_Port, BCDM2_2_Pin) << 6; // BCDM2[2]
#endif
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
