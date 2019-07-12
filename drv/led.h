/*
led.h

MIT License

Copyright (c) 2016 Yongqian Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef __LED_H__
#define __LED_H__

#include "base.h"

// LED模式枚举
typedef enum {
    LED_MODE_NORMAL,    // 普通模式（亮/灭）
    LED_MODE_FLASH,     // 闪烁模式

    LED_MODE_MAX
} led_mode_e;


// LED模式
static led_mode_e s_led_mode;
// 上次更新时间(ms)
static uint32_t s_led_last_time;
// LED计时(ms)
static uint16_t s_led_timer;
// LED闪烁周期(ms)
static uint16_t s_led_period;
// LED每闪烁周期点亮时间(ms)
static uint16_t s_led_pulse;
// LED闪烁次数
static uint16_t s_led_cycle_cnt;


void led_init(void);

void led_update(void);

void led_light(BOOL is_light);

void led_flash(uint16_t period, uint16_t pulse, uint16_t cycles);

#endif //__LED_H__
