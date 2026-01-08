#ifndef __KEY_H
#define __KEY_H

#include "main.h"

int key0_pressed();
int key1_pressed();

/**
 *  扫描按键，判断有无按键被按下
 * 如果有按键，则返回按键的值
 * @param mode: mode==1：不支持连续扫描 
 *              mode==2：支持连续扫描
 * @return -1: 没有按键被按下
 */
int key_scan(int mode);

#endif // !__KEY_H
