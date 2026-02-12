/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Compile_Data.h"
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

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

uint8_t Uart_rx1_buf[rx1_buf_len] ;
uint8_t Uart_rx1_buf_tmp[3] ;
int rx1_State ;
int rx1_buf_count;
int rx1_buf_count_tmp;
int rx1_Receive_complete;

uint8_t Uart_rx3_buf[rx3_buf_len] ;
uint8_t Uart_rx3_buf_tmp[3] ;
int rx3_State ;
int rx3_buf_count;
int rx3_buf_count_tmp;
int rx3_Receive_complete;

uint8_t POL_Sens_Date[5] ;

uint8_t EB_Mode_tmp[4] ;
uint8_t EB_Mode[1] ;

uint8_t EB_Type, EB_Com , EB_Com_St;

#define EBC_Data_Num    1024
uint8_t EBC_Data[EBC_Data_Num][12] ;
uint8_t EBC_ARK = 0x06;
uint8_t EBC_NARK = 0x15;

int EBC_Data_Cur_Num = 0;


int Timer_1S = 0;
int Timer_2S = 0;
int Timer_4S = 0;

extern int BC_STOP_MODE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

///////////////Start timer interrupt operation function///////////////////

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM1){
	  HAL_IWDG_Refresh(&hiwdg);
  }

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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  MX_USB_DEVICE_Init();
  MX_USART1_UART_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */

  for(int i=0;i<EBC_Data_Num;i++){
    for(int j=0;j<12;j++){
      EBC_Data[i][j] = 0;
    }
  }
  EB_Mode_tmp[0] =  ~(HAL_GPIO_ReadPin(ADDR_DIP0_GPIO_Port, ADDR_DIP0_Pin)) & 0x01;
  EB_Mode_tmp[1] =  ~(HAL_GPIO_ReadPin(ADDR_DIP1_GPIO_Port, ADDR_DIP1_Pin)) & 0x01;
  EB_Mode_tmp[2] =  ~(HAL_GPIO_ReadPin(ADDR_DIP2_GPIO_Port, ADDR_DIP2_Pin)) & 0x01;
  EB_Mode_tmp[3] =  ~(HAL_GPIO_ReadPin(ADDR_DIP3_GPIO_Port, ADDR_DIP3_Pin)) & 0x01;


  EB_Mode[0] = (EB_Mode_tmp[3] << 3)|(EB_Mode_tmp[2] << 2)|(EB_Mode_tmp[1] << 1)|(EB_Mode_tmp[0] << 0);
  EB_Type = EB_Mode[0];

  /* huart1 RX Interrupt  Enable */
  /* Process Unlocked */
  __HAL_UNLOCK(&huart1);
  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_PE);
  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);


  /* huart3 RX Interrupt  Enable */
  /* Process Unlocked */
  __HAL_UNLOCK(&huart3);
  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_PE);
  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_ERR);
  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);

  HAL_TIM_OC_Start_IT(&htim1,TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim2,TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim3,TIM_CHANNEL_1);

  POL_Sens_Date[0] = 0x02;
  POL_Sens_Date[1] = 0x50;
  POL_Sens_Date[2] = 0x4F;
  POL_Sens_Date[3] = 0x4C;
  POL_Sens_Date[4] = 0x03;

  Compile_Date();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	    __HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
	    /* Enable the UART Data Register not empty Interrupt */
	    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

	    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	    __HAL_UART_ENABLE_IT(&huart3, UART_IT_ERR);
	    /* Enable the UART Data Register not empty Interrupt */
	    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);


	    if(rx3_Receive_complete == 1){
	      SW_Com();
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 8000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 800;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 8000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  huart1.Init.BaudRate = 115200;
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RUN_LED_GPIO_Port, RUN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RS485_RE_Pin|RS485_DE_Pin|RS485_BRC_RE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RS485_BRC_DE_GPIO_Port, RS485_BRC_DE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : RUN_LED_Pin */
  GPIO_InitStruct.Pin = RUN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RUN_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ADDR_DIP3_Pin ADDR_DIP2_Pin ADDR_DIP1_Pin */
  GPIO_InitStruct.Pin = ADDR_DIP3_Pin|ADDR_DIP2_Pin|ADDR_DIP1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : RS485_RE_Pin RS485_DE_Pin RS485_BRC_RE_Pin */
  GPIO_InitStruct.Pin = RS485_RE_Pin|RS485_DE_Pin|RS485_BRC_RE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ADDR_DIP0_Pin */
  GPIO_InitStruct.Pin = ADDR_DIP0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ADDR_DIP0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RS485_BRC_DE_Pin */
  GPIO_InitStruct.Pin = RS485_BRC_DE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS485_BRC_DE_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


#define tx_buf_len 12
uint8_t Uart_tx_buf[tx_buf_len] ;

#define tx_Q_buf_len 18
uint8_t Uart_tx_Q_buf[tx_Q_buf_len] ;

void SW_Com(void){

  // CRC 체크

  int i,Uart3_crc , EBC_Data_CRC, Uart_crc;

  // 전송할 중계기 정보를 확인한다.

  Uart3_crc = 0;

  for(i = 0; i <rx3_buf_count_tmp - 5 ; i++){
    Uart3_crc = Uart3_crc ^ Uart_rx3_buf[i+2];
  }

  if(Uart3_crc == Uart_rx3_buf[rx3_buf_count_tmp - 3]){

    if(Uart_rx3_buf[4] == 0x42){        // EB board: '0x42'

      if(Uart_rx3_buf[3] == 0x51){

        Uart_tx_Q_buf[0] = 0x53;    //S
        Uart_tx_Q_buf[1] = 0x54;    //T
        Uart_tx_Q_buf[2] = 0x42;    //B
        Uart_tx_Q_buf[3] = 0x52;    //R
        Uart_tx_Q_buf[4] = 0x01;    //address
        Uart_tx_Q_buf[5] = 0x0A;    //version 10 -> 1.0
        Uart_tx_Q_buf[6] = 0x00;
        Uart_tx_Q_buf[7] = 0x00;
        Uart_tx_Q_buf[8] = 0x00;
        Uart_tx_Q_buf[9] = 0x00;
        Uart_tx_Q_buf[10] = 0x00;
        Uart_tx_Q_buf[11] = 0x00;
        Uart_tx_Q_buf[12] = 0x00;
        Uart_tx_Q_buf[13] = 0x00;   //dummy1
        Uart_tx_Q_buf[14] = 0x00;   //dummy2
        Uart_tx_Q_buf[15] = 0x00;   //CRC
        Uart_tx_Q_buf[16] = 0x45;   //E
        Uart_tx_Q_buf[17] = 0x44;   //D

        Uart_tx_Q_buf[14] = EB_Com_St;

        Uart_crc = 0;

        for(i = 0; i <tx_buf_len - 5 ; i++){
          Uart_crc = Uart_crc ^ Uart_tx_Q_buf[i+2];
        }
        Uart_tx_Q_buf[15] = Uart_crc;

        HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_SET);
        HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_SET);

        if(HAL_UART_Transmit(&huart3, Uart_tx_Q_buf, sizeof(Uart_tx_Q_buf), 1000)!= HAL_OK)
            {
              Error_Handler();
            }


        HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_RESET);

      }

      else if(Uart_rx3_buf[3] == 0x53){

        EBC_Data[EBC_Data_Cur_Num][0] = 0x02;
        EBC_Data[EBC_Data_Cur_Num][1] = Uart_rx3_buf[6];
        EBC_Data[EBC_Data_Cur_Num][2] = Uart_rx3_buf[7];
        EBC_Data[EBC_Data_Cur_Num][3] = 0x2D;
        EBC_Data[EBC_Data_Cur_Num][4] = Uart_rx3_buf[8];
        EBC_Data[EBC_Data_Cur_Num][5] = Uart_rx3_buf[9];
        EBC_Data[EBC_Data_Cur_Num][6] = 0x2D;
        EBC_Data[EBC_Data_Cur_Num][7] = Uart_rx3_buf[10];
        EBC_Data[EBC_Data_Cur_Num][8] = Uart_rx3_buf[11];
        EBC_Data[EBC_Data_Cur_Num][9] = Uart_rx3_buf[12];
        EBC_Data[EBC_Data_Cur_Num][11] = 0x03;

        EBC_Data_CRC = 0;
        /*
        for(i = 0; i < 10; i++){
          EBC_Data_CRC = EBC_Data_CRC + EBC_Data[EBC_Data_Cur_Num][i];
        }

        EBC_Data_CRC = EBC_Data_CRC + EBC_Data[EBC_Data_Cur_Num][1];
        */
        for(int j = 6; j < 12 ; j++){
          if(Uart_rx3_buf[j] == 0){
            Uart_rx3_buf[j] = 0x30;
          }
        }

        for(int j = 0; j < 12 ; j++){
          EBC_Data_CRC = EBC_Data_CRC + EBC_Data[EBC_Data_Cur_Num][j];
        }

        EBC_Data_CRC = EBC_Data_CRC % 16 + 0x30;
        EBC_Data[EBC_Data_Cur_Num][10] = (char)EBC_Data_CRC;

        if(Uart_rx3_buf[14] == 0x01){

          EBC_Data_Cur_Num++;
          EB_Com_St = 0;

        }
        else if(Uart_rx3_buf[14] == 0x00){

           EB_Com_St = 1;
           if(EB_Com == 0){
             EB_Com_St = Start_EB_Com();
           }

          EB_Com = 0;
        }
      }
      else if(Uart_rx3_buf[3] == 0x56){
    	  SUB_Com_SW_V();
      }

    }
    else{
      Send_P_Data();
    }
  }

  rx3_Receive_complete = 0;

  for(i=0;i<rx3_buf_len;i++){
    Uart_rx3_buf[i] = 0;
  }
}

int Start_EB_Com(void){

  uint8_t Uart_tx_buf[12];
  int i, j , k , l;

  int EB_state;

  EBC_Data_Cur_Num++;
  EB_state = 1;
  EB_Com = 1;

  if(EB_Type == SinWha_Type){

    for(i=0;i<EBC_Data_Cur_Num;i++){

      for(j = 0;j<12;j++){
        Uart_tx_buf[j] = EBC_Data[i][j];
      }

      for(l=0;l<3;l++){

        rx1_Receive_complete = 0;
        HAL_GPIO_WritePin(RS485_BRC_DE_GPIO_Port , RS485_BRC_DE_Pin , GPIO_PIN_SET);
        HAL_GPIO_WritePin(RS485_BRC_RE_GPIO_Port , RS485_BRC_RE_Pin , GPIO_PIN_SET);

        if(HAL_UART_Transmit(&huart1, Uart_tx_buf, sizeof(Uart_tx_buf), 1000)!= HAL_OK)
          {
            Error_Handler();
          }

        HAL_GPIO_WritePin(RS485_BRC_DE_GPIO_Port , RS485_BRC_DE_Pin , GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RS485_BRC_RE_GPIO_Port , RS485_BRC_RE_Pin , GPIO_PIN_RESET);

        for(k = 0; k < 50;k++){
          if(rx1_Receive_complete == 1){
            break;
          }

          Wate_EB_Com(1);
        }

        if(Uart_rx1_buf_tmp[0] == EBC_ARK){
          EB_state = 1;
          break;
        }
        else if(Uart_rx1_buf_tmp[0] == EBC_NARK){
          EB_state = 2;
        }
        else{
          EB_state = 2;
        }
        Wate_EB_Com(500);
      }

      if(EB_state == 2){
        break;
      }
    }


  }
  else if(EB_Type == DongBang_Type){

    for(i=0;i<EBC_Data_Cur_Num;i++){

      for(j = 0;j<12;j++){
        Uart_tx_buf[j] = EBC_Data[i][j];
      }

      HAL_GPIO_WritePin(RS485_BRC_DE_GPIO_Port , RS485_BRC_DE_Pin , GPIO_PIN_SET);
      HAL_GPIO_WritePin(RS485_BRC_RE_GPIO_Port , RS485_BRC_RE_Pin , GPIO_PIN_SET);

      if(HAL_UART_Transmit(&huart1, Uart_tx_buf, sizeof(Uart_tx_buf), 1000)!= HAL_OK)
        {
          Error_Handler();
      }

      HAL_GPIO_WritePin(RS485_BRC_DE_GPIO_Port , RS485_BRC_DE_Pin , GPIO_PIN_RESET);
      HAL_GPIO_WritePin(RS485_BRC_RE_GPIO_Port , RS485_BRC_RE_Pin , GPIO_PIN_RESET);

      Wate_EB_Com(500);
    }

    EB_state = 0;

  }
  else if(EB_Type == Coex_Type){
    for(i=0;i<EBC_Data_Cur_Num;i++){

      for(j = 0;j<12;j++){
        Uart_tx_buf[j] = EBC_Data[i][j];
      }

      HAL_GPIO_WritePin(RS485_BRC_DE_GPIO_Port , RS485_BRC_DE_Pin , GPIO_PIN_SET);
      HAL_GPIO_WritePin(RS485_BRC_RE_GPIO_Port , RS485_BRC_RE_Pin , GPIO_PIN_SET);

      if(HAL_UART_Transmit(&huart1, Uart_tx_buf, sizeof(Uart_tx_buf), 1000)!= HAL_OK)
        {
          Error_Handler();
      }

      HAL_GPIO_WritePin(RS485_BRC_DE_GPIO_Port , RS485_BRC_DE_Pin , GPIO_PIN_RESET);
      HAL_GPIO_WritePin(RS485_BRC_RE_GPIO_Port , RS485_BRC_RE_Pin , GPIO_PIN_RESET);

      Wate_EB_Com(1000);
    }

    EB_state = 0;
  }


  for(i=0;i<EBC_Data_Cur_Num;i++){
    for(j = 0;j<12;j++){
        EBC_Data[i][j] = 0;
    }
  }
  EBC_Data_Cur_Num = 0;

  return EB_state;
}

void Wate_EB_Com(int wate_time){

  int i;

  for(i=0; i<wate_time;i++){

    if(rx3_Receive_complete == 1){
        SW_Com();
    }
    HAL_Delay(1);
  }
}

void Send_P_Data(void){

  if(Timer_4S == 1){
    HAL_GPIO_WritePin(RS485_BRC_DE_GPIO_Port , RS485_BRC_DE_Pin , GPIO_PIN_SET);
    HAL_GPIO_WritePin(RS485_BRC_RE_GPIO_Port , RS485_BRC_RE_Pin , GPIO_PIN_SET);

    if(HAL_UART_Transmit(&huart1, POL_Sens_Date, sizeof(POL_Sens_Date), 1000)!= HAL_OK)
    {
      Error_Handler();
    }

    HAL_GPIO_WritePin(RS485_BRC_DE_GPIO_Port , RS485_BRC_DE_Pin , GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RS485_BRC_RE_GPIO_Port , RS485_BRC_RE_Pin , GPIO_PIN_RESET);
  }


  Timer_4S = 0;
}


#define Sub_MCC_R_length      15

void SUB_Com_SW_V(void){

	uint8_t Uart_crc;

    Uart_tx_buf[0] = 0x53;    //S
    Uart_tx_buf[1] = 0x54;    //T
    Uart_tx_buf[2] = 0x53;    //S
    Uart_tx_buf[3] = 0x76;    //v
    Uart_tx_buf[4] = 0x42;    //MCC Ebc
    Uart_tx_buf[5] = 1;    //address
    Uart_tx_buf[6] = F_Version_Year;
    Uart_tx_buf[7] = F_Version_Month;
    Uart_tx_buf[8] = F_Version_Day;
    Uart_tx_buf[9] = F_Version_Hour;
    Uart_tx_buf[10] = F_Version_Min;
    Uart_tx_buf[11] = F_Version_Sec;
    Uart_tx_buf[12] = 0x00;   //CRC
    Uart_tx_buf[13] = 0x45;   //E
    Uart_tx_buf[14] = 0x44;   //D


    for(int i = 0; i <Sub_MCC_R_length - 5 ; i++){
      Uart_crc = Uart_crc ^ Uart_tx_buf[i+2];
    }
    Uart_tx_buf[Sub_MCC_R_length-3] = Uart_crc;


    HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_SET);
    HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_SET);

    HAL_Delay(1);
    if(HAL_UART_Transmit(&huart3, Uart_tx_buf, Sub_MCC_R_length, 1000)!= HAL_OK)
        {
          Error_Handler();
        }

    HAL_Delay(1);

    HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_RESET);

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
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
