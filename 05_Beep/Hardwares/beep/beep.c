#include "beep.h"
#include "tim.h"

#define BEEP_TIM htim4
#define BEEP_TIM_CHANNEL TIM_CHANNEL_4

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

Note song[] = {
    {G4, 0.5}, {G4, 0.5}, {A5, 1}, {G5, 1}, {C6, 1}, {B5, 1}};

void Play_Note(Note note);

void Play_Song()
{
    int i = 0;
    int len = sizeof(song) / sizeof(Note);

    HAL_TIM_PWM_Start(&BEEP_TIM, BEEP_TIM_CHANNEL); // 启动定时器4的PWM输出
    for (i = 0; i < len; i++)
    {
        // 播放音符
        Play_Note(song[i]);
        HAL_Delay(20);
    }
}

void Play_Note(Note note)
{
    // 一拍的时间(us)
    uint32_t beat_time = 1000 * 60 / BPM;

    // 计算音符持续时间
    uint32_t duration = beat_time / note.beats;

    // 休止符，空一拍
    if (note.note_name == REST)
    {
        HAL_Delay(duration);
        return;
    }

    // 计算一个音符的周期(arr的值)
    uint32_t arr = 1000000 / note.note_name;

    // 设置PWM频率（输出指定频率的声音）
    __HAL_TIM_SET_AUTORELOAD(&BEEP_TIM, arr);
    __HAL_TIM_SET_COMPARE(&BEEP_TIM, BEEP_TIM_CHANNEL, arr / 2); // 占空比50%
    __HAL_TIM_SetCounter(&BEEP_TIM, 0);

    // 持续播放音符到指定时间
    HAL_Delay(duration - 20);
}