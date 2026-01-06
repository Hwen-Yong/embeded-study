#include "key.h"
#include "main.h"
#include "tim.h"

static uint8_t key0_state = 0; // 按键状态变量

void Key_Scan(void)
{
    // 判断按键是否第一次按下
    if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET)
    {
        // 使用定时器完成消抖

        // 重新将定时器的时间设置为15ms
        __HAL_TIM_SET_AUTORELOAD(&htim2, 150 - 1);
        __HAL_TIM_SET_PRESCALER(&htim2, 7200 - 1);

        HAL_TIM_Base_Start_IT(&htim2);
    }

    // 判断按键是否释放
    if (key0_state && HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_SET)
    {
        // 说明按键释放
        key0_state = 0;

        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
        // 停止定时器
        HAL_TIM_Base_Stop_IT(&htim2);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        if (!key0_state && HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET)
        {
            // 更新按键状态为按下
            key0_state = 1;
        }
    }
}