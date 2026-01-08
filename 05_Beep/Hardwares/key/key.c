#include "key.h"

// 按键读到低电平时，证明被按下
#define KEY_PRESSED GPIO_PIN_RESET
#define KEY_UNPRESSED GPIO_PIN_SET

#define DELAY_TIME 100

typedef struct Key_TypeDef
{
    GPIO_TypeDef *GPIO_Port;
    uint16_t GPIO_Pin;
} Key_TypeDef;


Key_TypeDef keys[] = {{KEY0_GPIO_Port, KEY0_Pin}};

// 通过KEY_STATE的不同位来做判断
static int KEY_STATE = 0;
static uint32_t TICK = 0;

/**
 * @brief   按键扫描函数
 * 
 * @param   mode mode==1：不支持连续扫描 
 *              mode==2：支持连续扫描
 * @return  int 返回对应按键的编号，未按下返回-1 
 */
int key_scan(int mode)
{
    // 逐个遍历各个KEY
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
    {
        Key_TypeDef key = keys[i];
        // 第一次检测到按键被按下（判断第i位是否为0）
        if ((KEY_STATE >> i & 1) == 0 && HAL_GPIO_ReadPin(key.GPIO_Port, key.GPIO_Pin) == KEY_PRESSED)
        {
            // 第一次进入判断(刚刚按下),记录KEY_STATE（将第i位设置位0）
            KEY_STATE |= (1 << i);

            // 记录当前时间，用于完成消抖工作
            TICK = HAL_GetTick();
        }
        // 已经不是第一次按下,判断其有没有等待足够的时间完成消抖 (判断第i位是否为1）
        else if ((KEY_STATE >> i & 1) == 1 && (HAL_GetTick() - TICK >= DELAY_TIME))
        {
            // 消抖完成，改变KEY_STATE状态（将第i位设置位1）
            KEY_STATE &= ~(1 << i);
            // 非连续扫描模式，需要捕获上升沿
            if (mode == 1 && HAL_GPIO_ReadPin(key.GPIO_Port, key.GPIO_Pin) == KEY_UNPRESSED)
                return i;

            else if (mode == 2 && HAL_GPIO_ReadPin(key.GPIO_Port, key.GPIO_Pin) == KEY_PRESSED)
                return i;
        }
    }
    return -1;
}
