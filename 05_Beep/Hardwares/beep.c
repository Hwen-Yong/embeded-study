#include "beep.h"
#include "tim.h"

#define REST 0
#define C4 262 // 低音频率
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

#define C5 524 // 中音频率
#define D5 587
#define E5 659
#define F5 698
#define G5 784
#define A5 880
#define B5 988

#define C6 1047 // 高音频率
#define D6 1175
#define E6 1419
#define F6 1497
#define G6 1568
#define A6 1760
#define B6 1976

// 曲子的BPM（速度）
#define BPM 96

typedef struct Note
{
    // 频率
    uint16_t note_name;
    float beats;
} Note;

Node song[] = {
    {REST, 1}, {REST, 1}, {G5, 0.5}, {G5, 0.5}, {A5, 1}, {G5, 1}, {C6, 1}, {B5, 1}};

void Play_Song()
{
    int i = 0;
    int len = sizeof(song) / sizeof(Note);
    for (i = 0; i <= len; i++)
    {
        // 播放音符
        Play_Node(song[i]);
    }
}

Play_Node(Note note)
{
    // 一拍的时间(us)
    float beat_time = 1000000 * 60 / BPM;

    // 计算音符持续时间
    float duration = beat_time / note.beats;

    // 休止符，空一拍
    if (note.note_name == REST)
    {
        HAL_Delay_us(duration);
        return;
    }

    // 计算一个音符的周期(arr的值)
    float arr = 1000000 / note.note_name;

    //设置PWM频率（输出指定频率的声音）
    __HAL_TIM_SET_AUTORELOAD(htim4, arr);

    //持续播放音符到指定时间
    HAL_Delay_us(duration);

    // 关闭定时器4
    __HAL_TIM_SET_AUTORELOAD(htim4, 0);
}
