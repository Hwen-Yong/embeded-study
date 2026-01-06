#include "stm32f10x.h"

int main()
{
	
	//1、配置系统时钟(可省略,默认HSI为时钟来源)
	
	//2、打开APB2的时钟 
	//将RCC_APB2ENR寄存器的值设置为0x00000010
	RCC->APB2ENR = RCC_APB2ENR_IOPCEN;
	
	//3、配置GPIO
	
	//将GPIOC设置为推挽输出模式，最大速度50Mhz
	GPIOC->CRH = 0x00300000;
	
	
	//PC13输出低电平
	GPIOC->ODR = 0x0000;
	while(1)
	{
	
	}
}