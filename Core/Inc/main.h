/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RUN_LED_Pin GPIO_PIN_15
#define RUN_LED_GPIO_Port GPIOC
#define ADDR_DIP3_Pin GPIO_PIN_2
#define ADDR_DIP3_GPIO_Port GPIOB
#define RS485_RE_Pin GPIO_PIN_12
#define RS485_RE_GPIO_Port GPIOB
#define RS485_DE_Pin GPIO_PIN_13
#define RS485_DE_GPIO_Port GPIOB
#define ADDR_DIP2_Pin GPIO_PIN_14
#define ADDR_DIP2_GPIO_Port GPIOB
#define ADDR_DIP1_Pin GPIO_PIN_15
#define ADDR_DIP1_GPIO_Port GPIOB
#define ADDR_DIP0_Pin GPIO_PIN_8
#define ADDR_DIP0_GPIO_Port GPIOA
#define RS485_BRC_DE_Pin GPIO_PIN_15
#define RS485_BRC_DE_GPIO_Port GPIOA
#define RS485_BRC_RE_Pin GPIO_PIN_4
#define RS485_BRC_RE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define RUN_LED_Pin GPIO_PIN_15
#define RUN_LED_GPIO_Port GPIOC
#define ADDR_DIP3_Pin GPIO_PIN_2
#define ADDR_DIP3_GPIO_Port GPIOB
#define RS485_RE_Pin GPIO_PIN_12
#define RS485_RE_GPIO_Port GPIOB
#define RS485_DE_Pin GPIO_PIN_13
#define RS485_DE_GPIO_Port GPIOB
#define ADDR_DIP2_Pin GPIO_PIN_14
#define ADDR_DIP2_GPIO_Port GPIOB
#define ADDR_DIP1_Pin GPIO_PIN_15
#define ADDR_DIP1_GPIO_Port GPIOB
#define ADDR_DIP0_Pin GPIO_PIN_8
#define ADDR_DIP0_GPIO_Port GPIOA
#define RS485_BRC_DE_Pin GPIO_PIN_15
#define RS485_BRC_DE_GPIO_Port GPIOA
#define RS485_BRC_RE_Pin GPIO_PIN_4
#define RS485_BRC_RE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define DongBang_Type   1
#define SinWha_Type     0
#define Coex_Type       8


extern int Timer_1S;
extern int Timer_2S;
extern int Timer_4S;

extern int BC_STOP_MODE;

#define rx1_buf_len 40
extern uint8_t Uart_rx1_buf[rx1_buf_len] ;
extern uint8_t Uart_rx1_buf_tmp[3] ;
extern int rx1_State ;
extern int rx1_buf_count;
extern int rx1_buf_count_tmp;
extern int rx1_Receive_complete;

#define rx3_buf_len 40
extern uint8_t Uart_rx3_buf[rx3_buf_len] ;
extern uint8_t Uart_rx3_buf_tmp[3] ;
extern int rx3_State ;
extern int rx3_buf_count;
extern int rx3_buf_count_tmp;
extern int rx3_Receive_complete;

void SW_Com(void);
int Start_EB_Com(void);
void Wate_EB_Com(int wate_time);
void Send_P_Data(void);

void SUB_Com_SW_V(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
