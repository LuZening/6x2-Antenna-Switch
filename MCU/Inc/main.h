/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PIN.h"
#include "CH395.H"
#include "CH395CMD.H"
#include "FS.h"
#include "HTTPServer.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef struct
{
	PIN_typedef PIN_BCD0;
	PIN_typedef PIN_BCD1;
	PIN_typedef PIN_BCD2;
	uint8_t sel; // the number of selected antenna 0...7
} AntennaSelector_typedef;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#ifdef DEBUG
//#define DEBUG_LOG(...) printf(__VA_ARGS__)
#define DEBUG_LOG(...) while(0)
#else
#define DEBUG_LOG(...) while(0)
#endif
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SCS_Pin GPIO_PIN_4
#define SCS_GPIO_Port GPIOA
#define CH395_INT_Pin GPIO_PIN_1
#define CH395_INT_GPIO_Port GPIOB
#define CH395_INT_EXTI_IRQn EXTI0_1_IRQn
/* USER CODE BEGIN Private defines */
#define __CH395_
#define __ON_BOARD_
#define CH395_INT_Pin GPIO_PIN_1
#define CH395_INT_GPIO_Port GPIOB
#define CH395_INT_EXTI_IRQn EXTI0_1_IRQn
#define NUM_ANTENNA 6
#define NUM_TRANCEIVERS 2
#define MAX_LEN_ANT_LABEL 15
#define MAX_LEN_ANT_LABELS ((MAX_LEN_ANT_LABEL + 1) * NUM_ANTENNA)
#define EEPROM_VALID_BYTE 0xAA
#define N_SELECTORS NUM_TRANCEIVERS
#define BCD2INT(D0,D1,D2) (D2<<2 | D1<<1 | D0);
#define BCDM1_0 2
#define BCDM1_1 1
#define BCDM1_2 0
#define BCDM2_0 4
#define BCDM2_1 6
#define BCDM2_2 5
#define FS_BASE_ADDR 0x08004400 // starting after the first 16KB flash bank
// Saved data on EEPROM
typedef struct
{
	uint8_t EEPROM_valid_ID;
	char ant_labels[NUM_ANTENNA][MAX_LEN_ANT_LABEL];
} SavedData_typedef;
extern SavedData_typedef SavedData;

// CH395 available sockets
// CH395 interrupt handler
void interrupt_CH395();
void switch_Antenna(uint8_t A, uint8_t B);
uint8_t get_Antenna();
extern AntennaSelector_typedef Selector[];
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
