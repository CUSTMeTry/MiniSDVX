/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#define USE_USBD_COMPOSITE
#include "key.h"
// #include "usbd_composite.h"
 #include "usbd_cdc_if.h"
 #include "usbd_custom_hid_if.h"
//#include "ARGB.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STEP 10;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t keyboardReport;
extern uint8_t keyboardReportNull;
extern uint16_t keyData;

int8_t mouseReport[5] = {};
uint8_t LCount;
uint8_t RCount;
uint8_t tick;
uint8_t readFlag;
uint8_t n;
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  LCount = 0;
  RCount = 0;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_DMA_Init();
  MX_TIM4_Init();
  MX_TIM1_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  keyboardInit();
  mouseReport[0] = 2;
  // HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // char str[] = "Hello World!\n";
    // CDC_Transmit_FS((uint8_t *)str, 13);
    // HAL_Delay(2000);
    if (keyboardBitDecode())
    {
      USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, &keyboardReport, 12);
      USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, &keyboardReportNull, 12);
    }
    HAL_Delay(1);
    // if (LCount != 0 || RCount != 0)
    // {
    //   mouseReport[2] = (int8_t)LCount * STEP;
    //   mouseReport[3] = (int8_t)RCount * STEP;
    //   USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)mouseReport, 5);
    //   LCount = 0;
    //   RCount = 0;
    // }
    // if(!(keyData & 0x01)){
    //   while(keyData & 0x01);
    //   n++;
    // }
    // if(!(keyData & (0x01 << 3))){
    //   while(keyData & (0x01 << 3));
    //   n--;
    // }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == ENC_LA_Pin)
  {
    if (HAL_GPIO_ReadPin(ENC_LA_GPIO_Port, ENC_LA_Pin))
    {
      if (HAL_GPIO_ReadPin(ENC_LB_GPIO_Port, ENC_LB_Pin))
        LCount--;
      else
        LCount++;
    }
    else
    {
      if (HAL_GPIO_ReadPin(ENC_LB_GPIO_Port, ENC_LB_Pin))
        LCount++;
      else
        LCount--;
    }
  }
  else if (GPIO_Pin == ENC_LB_Pin)
  {
    if (HAL_GPIO_ReadPin(ENC_LB_GPIO_Port, ENC_LB_Pin))
    {
      if (HAL_GPIO_ReadPin(ENC_LA_GPIO_Port, ENC_LA_Pin))
        LCount++;
      else
        LCount--;
    }
    else
    {
      if (HAL_GPIO_ReadPin(ENC_LA_GPIO_Port, ENC_LA_Pin))
        LCount--;
      else
        LCount++;
    }
  }
  else if (GPIO_Pin == ENC_RA_Pin)
  {
    if (HAL_GPIO_ReadPin(ENC_RA_GPIO_Port, ENC_RA_Pin))
    {
      if (HAL_GPIO_ReadPin(ENC_RB_GPIO_Port, ENC_RB_Pin))
        RCount--;
      else
        RCount++;
    }
    else
    {
      if (HAL_GPIO_ReadPin(ENC_RB_GPIO_Port, ENC_RB_Pin))
        RCount++;
      else
        RCount--;
    }
  }
  else if (GPIO_Pin == ENC_RB_Pin)
  {
    if (HAL_GPIO_ReadPin(ENC_RB_GPIO_Port, ENC_RB_Pin))
    {
      if (HAL_GPIO_ReadPin(ENC_RA_GPIO_Port, ENC_RA_Pin))
        RCount++;
      else
        RCount--;
    }
    else
    {
      if (HAL_GPIO_ReadPin(ENC_RA_GPIO_Port, ENC_RA_Pin))
        RCount--;
      else
        RCount++;
    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == (&htim3))
  {

    // WS281X_SetPixelColorRGB(n,WS281X_Color(n, 255 - n, 25 + n));
    // WS281X_ShowRGB();
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
