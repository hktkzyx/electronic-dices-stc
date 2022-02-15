#include <8051.h>
#include <compiler.h>
#include <stdlib.h>
#include <string.h>

#include "my_header.h"
#include "stc15wxx.h"

typedef unsigned char u8;
typedef unsigned int u16;

SBIT(rolling, 0xB0, 2);  // P3_2 K3 rolling key
SBIT(setting, 0xB0, 3);  // P3_3 k4 setting key

__code const u8 kNumCoding[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D,
                                  0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C,
                                  0x39, 0x5E, 0x79, 0x71};  // 0 to F

#define DICE_MAX_NUM 4
#define IDLE_TIME 30000
#define COUNT_OF(arr) (sizeof(arr) / sizeof(0 [arr]))
#define DEFAULT_DICES_NUM 2
#define DEFAULT_ENABLE 0x03

u8 encodings[DICE_MAX_NUM] = {kNumCoding[1]};
u8 enable = DEFAULT_ENABLE;  // LED position enable
u8 dices_num = DEFAULT_DICES_NUM;
u8 setting_status = 1,
   rolling_status = 1;  // Key status 0: pressed, 1: unpressed

void Init(void);
void SettingKey(void);
void RollingKey(void);

void main(void) {
    Init();
    while (1) {
        SettingKey();
        RollingKey();
    };
}

void Init(void) {
    TMOD = 0x00;                        // Set timer mode
    IT0 = 1, IT1 = 1;                   // Set outer interrupt mode
    TH0 = 0xFE, TL0 = 0x0C;             // 1 ms
    EA = 1, ET0 = 1, EX0 = 1, EX1 = 1;  // Enable interrupt
    TR0 = 1, AUXR |= 0x10;              // Start timer
    P3 = 0xFF;                          // Pull up P3 as input IO

    setting_status = 1, rolling_status = 1;
    dices_num = DEFAULT_DICES_NUM;
    memset(encodings, kNumCoding[1], DICE_MAX_NUM);
    enable = DEFAULT_ENABLE;
}

/**
 * @brief Process Setting Key info
 */
void SettingKey(void) {
    static u8 setting_backup = 1;
    if ((setting_backup == 0) &&
        (setting_status == 1))  // Setting key is released
    {
        dices_num = (dices_num == DICE_MAX_NUM) ? 1 : dices_num + 1;
        memset(encodings, kNumCoding[1], DICE_MAX_NUM);
        enable = 0;
        for (u8 i = 0; i < dices_num; ++i) {
            enable |= 1 << i;
        }
        setting_backup = 1;
    } else if ((setting_backup == 1) && (setting_status == 0)) {
        setting_backup = 0;
    }
}

/**
 * @brief Process Rolling Key info
 */
void RollingKey(void) {
    static u8 rolling_backup = 1;
    if ((rolling_backup == 1) &&
        (rolling_status == 0))  // Rolling key is pressed
    {
        for (u8 i = 0; i < DICE_MAX_NUM;
             ++i)  // Display "8" when pressing rolling
        {
            encodings[i] = kNumCoding[8];
            enable |= 1 << i;
        }
        rolling_backup = 0;
    } else if ((rolling_backup == 0) &&
               (rolling_status == 1))  // Rolling key is released
    {
        static u8 ever_rolled = 0;
        if (ever_rolled == 0)  // Update seed
        {
            u16 time_high = T2H, time_low = T2L;
            u16 seed = time_high << 8 | time_low;
            srand(seed);
            AUXR &= 0xEF;  // Close timer 1
            ever_rolled = 1;
        }
        EA = 0;
        enable = 0;
        for (u8 i = 0; i < dices_num; ++i) {
            int random_num = rand() % 6 + 1;
            encodings[i] = kNumCoding[random_num];
            enable |= 1 << i;
        }
        EA = 1;
        rolling_backup = 1;
    }
}

void KeysInspect(void);
void Display(const u8* pNumEncodings, u8 num_enable);
void SavePower(void);
void InterruptT0(void) __interrupt(1) {
    KeysInspect();
    Display(encodings, enable);
    SavePower();
}
void InterruptIT0(void) __interrupt(0) { nop(); }
void InterruptIT1(void) __interrupt(2) { nop(); }

/**
 * @brief Inspect keys' status
 */
void KeysInspect(void) {
    static u16 setting_buffer = 0xFFFF, rolling_buffer = 0xFFFF;
    setting_buffer = setting_buffer << 1 | setting;
    rolling_buffer = rolling_buffer << 1 | rolling;
    if (setting_buffer == 0x0000) {
        setting_status = 0;
    }
    if (setting_buffer == 0xFFFF) {
        setting_status = 1;
    }
    if (rolling_buffer == 0x0000) {
        rolling_status = 0;
    }
    if (rolling_buffer == 0xFFFF) {
        rolling_status = 1;
    }
}

/**
 * @brief Character to show.
 * @param [in] led_code The character encoding.
 */
void SendToShow(u8 num_encoding) {
    for (u8 i = 0; i < 8; i++) {
        P1_0 = num_encoding & 1;
        P1_1 = 0;
        P1_1 = 1;
        num_encoding = num_encoding >> 1;
    }
    P1_2 = 0;
    P1_2 = 1;
}

/**
 * @brief LED number display.
 */
void Display(const u8* pNumEncodings, u8 num_enable) {
    static u8 num_position = 0;
    SendToShow(0x00);               // LED消隐
    P1 = 0xC0 | num_position << 3;  // LED位选
    if (num_enable >> num_position & 1) {
        SendToShow(pNumEncodings[num_position]);
    }
    if (num_position < COUNT_OF(pNumEncodings)) {
        ++num_position;
    } else {
        num_position = 0;
    }
}

/**
 * @brief Power management.
 */
void SavePower(void) {
    static u16 idle_time = 0;
    if ((setting_status == 0) || (rolling_status == 0)) {
        idle_time = 0;
    } else {
        idle_time++;
    }
    if (idle_time >= IDLE_TIME) {
        SendToShow(0x00);
        idle_time = 0;
        PCON = 0x02;
        nop_four();
    }
}