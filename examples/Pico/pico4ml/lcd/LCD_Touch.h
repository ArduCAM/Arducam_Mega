/*****************************************************************************
 * | File      	:	LCD_Touch.h
 * | Author      :   Waveshare team
 * | Function    :	LCD Touch Pad Driver and Draw
 * | Info        :
 *   Image scanning
 *      Please use progressive scanning to generate images or fonts
 *----------------
 * |	This version:   V1.0
 * | Date        :   2017-08-16
 * | Info        :   Basic version
 *
 ******************************************************************************/
#ifndef __LCD_TOUCH_H_
#define __LCD_TOUCH_H_

// #include "LCD_GUI.h"
// #include "pico/float.h"
#include "pico/stdlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gui_func.h"
#ifdef __cplusplus
extern "C" {
#endif

#define TP_IRQ_PIN    8
#define TP_CS_PIN     9

#define TP_PRESS_DOWN 0x80
#define TP_PRESSED    0x40

typedef uint16_t POINT;
typedef uint16_t LCD_SCAN_DIR;
typedef uint16_t COLOR;
// Touch screen structure
typedef struct {
    POINT Xpoint0;
    POINT Ypoint0;
    POINT Xpoint;
    POINT Ypoint;
    uint8_t chStatus;
    uint8_t chType;
    int16_t iXoff;
    int16_t iYoff;
    float fXfac;
    float fYfac;
    // Select the coordinates of the XPT2046 touch \
	  screen relative to what scan direction
    LCD_SCAN_DIR TP_Scan_Dir;
} TP_DEV;

// Brush structure
typedef struct {
    POINT Xpoint;
    POINT Ypoint;
    COLOR Color;
    // DOT_PIXEL DotPixel;
} TP_DRAW;

void touch_cs_init(uint8_t pin);
void touch_irq_init(uint8_t pin, int irq_flag);
void touch_cs_high(uint16_t pin);
void tpuch_cs_low(uint8_t pin);
void TP_GetAdFac(void);
// void TP_Adjust(void);
// void TP_Dialog(void);
// void TP_DrawBoard(void);
void TP_Init(LCD_SCAN_DIR Lcd_ScanDir);
int select_touch(UI_LOG_MSG_T *select_win, int lenght);

#ifdef __cplusplus
}
#endif
#endif
