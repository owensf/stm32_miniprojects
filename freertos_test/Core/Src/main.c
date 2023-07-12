/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define PERIPH_BASE (0x40000000UL)
// GPIOC is in AHB2
// Starting point of AHB2: 0x48000000
#define AHB2PERIPH_OFFSET (0x8000000UL)
#define AHB2PERIPH_BASE PERIPH_BASE + AHB2PERIPH_OFFSET
#define GPIOC_OFFSET (0x800U)
#define GPIOC_BASE AHB2PERIPH_BASE + GPIOC_OFFSET

// RCC is in AHB1
#define AHB1PERIPH_OFFSET (0X0020000UL)
#define AHB1PERIPH_BASE PERIPH_BASE + AHB1PERIPH_OFFSET
#define RCC_OFFSET (0X1000UL)
#define RCC_BASE AHB1PERIPH_BASE + RCC_OFFSET
// seems like there's only 1 AHB EN??
#define AHBEN_OFFSET (0x14UL)
#define RCC_AHBENR (*(volatile unsigned int *) (RCC_BASE + AHBEN_OFFSET))
// GPIOC EN: bit 19
#define GPIOCEN (1U << 19)
#define MODER_OFFSET (0x0UL)
#define GPIOC_MODER (*(volatile unsigned int *) (GPIOC_BASE + MODER_OFFSET))
/*
// for GPIOC_8, pin 8 = bit 16
(1U << 16) // set bit 16 to 1
&= ~(1U<<17) set bit 16 to 0*/

#define ODR_OFFSET 0x14UL
#define GPIOC_ORDR (*(volatile unsigned int *) (GPIOC_BASE + ODR_OFFSET))
// set pin 8 = bit 8
#define PIN8 (1U <<8)
#define LED_PIN PIN8


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
/* Definitions for blink01 */
osThreadId_t blink01Handle;
const osThreadAttr_t blink01_attributes = {
  .name = "blink01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for blink02 */
osThreadId_t blink02Handle;
const osThreadAttr_t blink02_attributes = {
  .name = "blink02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void startblink01(void *argument);
void startblink02(void *argument);

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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	RCC_AHBENR |= GPIOCEN;
	 /* 2. Set PC8 as output pin */
	GPIOC_MODER |= (1U << 16);
	GPIOC_MODER &= ~(1U << 17);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of blink01 */
  blink01Handle = osThreadNew(startblink01, NULL, &blink01_attributes);

  /* creation of blink02 */
  blink02Handle = osThreadNew(startblink02, NULL, &blink02_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_startblink01 */
/**
  * @brief  Function implementing the blink01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_startblink01 */
void startblink01(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
		 GPIOC_ORDR ^= LED_PIN;
	  osDelay(500);
  }
  osThreadTerminate(NULL);
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_startblink02 */
/**
* @brief Function implementing the blink02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startblink02 */
void startblink02(void *argument)
{
  /* USER CODE BEGIN startblink02 */
  /* Infinite loop */
  for(;;)
  {
	//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
	 GPIOC_ORDR ^= LED_PIN;
	  osDelay(600);
  }
  osThreadTerminate(NULL);
  /* USER CODE END startblink02 */
}

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
