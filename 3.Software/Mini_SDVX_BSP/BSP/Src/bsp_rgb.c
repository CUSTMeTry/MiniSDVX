#include "bsp_rgb.h"

volatile uint16_t send_Buf[NUM];
volatile uint16_t send_BufK[NUM];
extern uint8_t keyIsPress[9];

// 启动DMA载入数据
void WS_Load(void)
{
	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *)send_Buf, NUM);
}

// 启动DMA载入数据
void WS_LoadK(void)
{
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t *)send_BufK, NUM_K);
}

// 关闭所有LED灯
void WS_CloseAll(void)
{
	uint16_t i;

	for (i = WAIT_TIME; i < PIXEL_NUM * 24; i++)
		send_Buf[i] = WS0; // 写入逻辑0的占空比

	for (i = PIXEL_NUM * 24; i < NUM; i++)
		send_Buf[i] = 0; // 占空比比为0，全为低电平

	WS_Load();
}

// 关闭所有LED灯
void WS_CloseAllK(void)
{
	uint16_t i;

	for (i = WAIT_TIME; i < PIXEL_NUM_K * 24; i++)
		send_BufK[i] = WS0; // 写入逻辑0的占空比

	for (i = PIXEL_NUM_K * 24; i < NUM_K; i++)
		send_BufK[i] = 0; // 占空比比为0，全为低电平

	WS_LoadK();
}
/**
 * @brief 全部led灯设置成一样的亮度，其中RGB分别设置亮度
 * WS2812的写入顺序是GRB，高位在前面
 */
void WS_WriteAll_RGB(uint8_t n_R, uint8_t n_G, uint8_t n_B)
{
	uint16_t i, j;
	uint8_t dat[24];

	// 将RGB数据进行转换
	for (i = 0; i < 8; i++)
	{
		dat[i] = ((n_G & 0x80) ? WS1 : WS0);
		n_G <<= 1;
	}
	for (i = 0; i < 8; i++)
	{
		dat[i + 8] = ((n_R & 0x80) ? WS1 : WS0);
		n_R <<= 1;
	}
	for (i = 0; i < 8; i++)
	{
		dat[i + 16] = ((n_B & 0x80) ? WS1 : WS0);
		n_B <<= 1;
	}

	for (i = 0; i < PIXEL_NUM; i++)
	{
		for (j = 0; j < 24; j++)
		{
			send_Buf[i * 24 + j] = dat[j];
		}
	}

	for (i = PIXEL_NUM * 24; i < NUM; i++)
	{
		send_Buf[i] = 0; // 占空比比为0，全为低电平
	}

	WS_Load();
}

uint32_t WS281x_Color(uint8_t red, uint8_t green, uint8_t blue)
{
	return green << 16 | red << 8 | blue;
}

/**
 * @brief  Set the color of No.n light in 24bit-format .
 * @note   None
 * @param  n: No.n lamp light, ranges from 0 to (PIXEL_NUM-1)
 * @param  GRBColor: color value of No.n light in 24bit-format(8 bit G + 8 bit R +8 bit B)
 * @retval None
 */
void WS281x_SetPixelColor(uint16_t n, uint32_t GRBColor)
{
	uint8_t i;
	if (n < PIXEL_NUM)
	{
		for (i = 0; i < 24; ++i)
		{
			send_Buf[WAIT_TIME + 24 * n + i] = (((GRBColor << i) & 0X800000) ? WS1 : WS0);
		}
	}
}

void WS281x_SetPixelColorK(uint16_t n, uint32_t GRBColor)
{
	uint8_t i;
	if (n < PIXEL_NUM_K)
	{
		for (i = 0; i < 24; ++i)
		{
			send_BufK[WAIT_TIME + 24 * n + i] = (((GRBColor << i) & 0X800000) ? WS1 : WS0);
		}
	}
}

/**
 * @brief  Set the color of No.n light in RGB-format .
 * @note   None
 * @param  n: No.n lamp light, ranges from 0 to (PIXEL_NUM-1)
 * @param  red: red color value of No.n light in RGB-format
 * @param  green: greeen color value of No.n light in RGB-format
 * @param  blue: blue color value of No.n light in RGB-format
 * @retval None
 */
void WS281x_SetPixelRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t i;

	if (n < PIXEL_NUM)
	{
		for (i = 0; i < 24; ++i)
		{
			send_Buf[WAIT_TIME + 24 * n + i] = (((WS281x_Color(red, green, blue) << i) & 0X800000) ? WS1 : WS0);
		}
	}
}

void WS281x_SetPixelRGBK(uint16_t n, uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t i;

	if (n < PIXEL_NUM_K)
	{
		for (i = 0; i < 24; ++i)
		{
			send_BufK[WAIT_TIME + 24 * n + i] = (((WS281x_Color(red, green, blue) << i) & 0X800000) ? WS1 : WS0);
		}
	}
}

uint32_t Wheel(uint8_t WheelPos)
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return WS281x_Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170)
	{
		WheelPos -= 85;
		return WS281x_Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return WS281x_Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(uint8_t wait)
{
	uint32_t timestamp = HAL_GetTick();
	uint16_t i;
	static uint8_t j;
	static uint32_t next_time = 0;

	uint32_t flag = 0;
	if (next_time < wait)
	{
		if ((uint64_t)timestamp + wait - next_time > 0)
			flag = 1;
	}
	else if (timestamp > next_time)
	{
		flag = 1;
	}
	if (flag) // && (timestamp - next_time < wait*5))
	{
		j++;
		next_time = timestamp + wait;
		for (i = 0; i < PIXEL_NUM; i++)
		{
			WS281x_SetPixelColor(i, Wheel((i + j) & 255));
		}
        WS_Load();
    }
}

void rainbowCycle(uint8_t wait)
{
    static uint8_t j = 0;
    static uint32_t next_time = 0;
    static uint32_t timestamp = 0;
    timestamp = HAL_GetTick();
    uint16_t i;

	if ((timestamp > next_time)) // && (timestamp - next_time < wait*5))
	{
		j++;
		next_time = timestamp + wait;
		for (i = 0; i < PIXEL_NUM; i++)
		{
			WS281x_SetPixelColor(i, Wheel(((i * 256 / (PIXEL_NUM)) + j) & 255));
		}
        WS_Load();
    }
}

void rgbIInit(){

}

void soloShow(uint8_t wait)
{
    static uint8_t j = 0;
    static uint32_t next_time = 0;
    static uint32_t timestamp = 0;
    timestamp = HAL_GetTick();
    uint16_t i;
	if ((timestamp > next_time))
	{
		if (j > PIXEL_NUM)
		{
			j = 0;
		}
		next_time = timestamp + wait;
		for (i = 0; i < PIXEL_NUM; i++)
		{
			WS281x_SetPixelColor(i, 0);
		}
		WS281x_SetPixelColor(j, WS281x_Color(j * 3, 0,  200 - j * 3));
        j++;
        WS_Load();
    }
}

void blinkWithKey(uint8_t wait)
{

	uint32_t timestamp = HAL_GetTick();
	uint16_t i;
	static uint8_t j;
	static uint32_t next_time = 0;
	static uint8_t blinkWithKeyTimeTick[9] = {0};

	static uint8_t loop = 0;
	if (loop == 0)
		next_time = timestamp;
	loop = 1; //首次调用初始化

	if ((timestamp > next_time)) // && (timestamp - next_time < wait*5))
	{
		next_time = timestamp + wait;
		for (uint8_t i = 0; i < 9; i++)
		{
			if (keyIsPress[i])
			{
				blinkWithKeyTimeTick[i] = 0xcc;
			}
			else if(blinkWithKeyTimeTick[i] > 7)
			{
				blinkWithKeyTimeTick[i]-=8;
			}else{
				blinkWithKeyTimeTick[i] = 0;
			}
			WS281x_SetPixelRGBK(i, 0, 0, blinkWithKeyTimeTick[i]);
		}
	}
	WS_LoadK();
}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	//    HAL_TIM_PWM_Stop_DMA(&htim4,TIM_CHANNEL_4);
	//    HAL_TIM_PWM_Stop_DMA(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_3); // PA8
}
