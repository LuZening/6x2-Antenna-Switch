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
#include <stdbool.h>
#include "PIN.h"
#include "CH395.H"
#include "CH395CMD.H"
#include "FS.h"
#include "HTTPServer.h"
#include "Lib485.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define N_BCD_PINS 3

typedef struct
{
	PIN_typedef PIN_BCDs[N_BCD_PINS];
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
#define CH395_INT_Pin GPIO_PIN_1
#define CH395_INT_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define XDISPLAY_Pin GPIO_PIN_10
#define XDISPLAY_GPIO_Port GPIOB
#define RW485_Pin GPIO_PIN_11
#define RW485_GPIO_Port GPIOB
#define BCDM2_2_Pin GPIO_PIN_12
#define BCDM2_2_GPIO_Port GPIOB
#define BCDM2_1_Pin GPIO_PIN_13
#define BCDM2_1_GPIO_Port GPIOB
#define BCDM2_0_Pin GPIO_PIN_14
#define BCDM2_0_GPIO_Port GPIOB
#define BCDM1_2_Pin GPIO_PIN_15
#define BCDM1_2_GPIO_Port GPIOB
#define BCDM1_1_Pin GPIO_PIN_8
#define BCDM1_1_GPIO_Port GPIOA
#define BCDM1_0_Pin GPIO_PIN_9
#define BCDM1_0_GPIO_Port GPIOA
#define BCD2_2_Pin GPIO_PIN_10
#define BCD2_2_GPIO_Port GPIOA
#define BCD2_1_Pin GPIO_PIN_11
#define BCD2_1_GPIO_Port GPIOA
#define BCD2_0_Pin GPIO_PIN_12
#define BCD2_0_GPIO_Port GPIOA
#define BCD1_2_Pin GPIO_PIN_15
#define BCD1_2_GPIO_Port GPIOA
#define BCD1_1_Pin GPIO_PIN_3
#define BCD1_1_GPIO_Port GPIOB
#define BCD1_0_Pin GPIO_PIN_4
#define BCD1_0_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define __CH395_
#define __ON_BOARD_
#define NUM_ANTENNA 6
#define NUM_TRANCEIVERS 2
#define MAX_LEN_ANT_LABEL 16
#define MAX_LEN_ANT_LABELS ((MAX_LEN_ANT_LABEL + 1) * NUM_ANTENNA)
#define N_SELECTORS NUM_TRANCEIVERS
#define BCD2INT(D0,D1,D2) (D2<<2 | D1<<1 | D0);
#define SCHED_INTERVAL 2000 // 2us
// Saved data on EEPROM
typedef struct
{
	uint8_t EEPROM_valid_ID;
	char ant_labels[NUM_ANTENNA][MAX_LEN_ANT_LABEL];
} SavedData_typedef;
extern SavedData_typedef SavedData;

// CH395 available sockets
// CH395 interrupt handler
void reset_CH395();
void interrupt_CH395();
//bool check_conflict(uint8_t *antnums, uint8_t n);
int8_t switch_Antenna(uint8_t *antnums, uint8_t n);
void get_Antenna_real_BCDs(uint8_t* antnums, uint8_t n);
void display_IP (bool start);
extern AntennaSelector_typedef Selector[];
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
