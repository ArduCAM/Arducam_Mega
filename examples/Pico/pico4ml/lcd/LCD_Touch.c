/*****************************************************************************
 * | File      	:	LCD_Touch.c
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
#include "LCD_Touch.h"
#include "LCD_SPI_interface.h"
#include "hardware/watchdog.h"

// extern LCD_DIS sLCD_DIS;
// extern uint8_t id;
static TP_DEV sTP_DEV;
static TP_DRAW sTP_Draw;

void touch_cs_init(uint8_t pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, 1);
}

void TP_Scan_irq(uint gpio, uint32_t events)
{
    if (!gpio_get(TP_IRQ_PIN)) {
        // display_color(return_btn, YELLOW);
        watchdog_enable(10, 1);
        watchdog_enable_caused_reboot();
        watchdog_caused_reboot();
    }
}

void touch_irq_init(uint8_t pin, int irq_flag)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    if (irq_flag == true)
        gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &TP_Scan_irq);
}

void touch_cs_high(uint16_t pin)
{
    gpio_put(pin, 1);
}
void touch_cs_low(uint8_t pin)
{
    gpio_put(pin, 0);
}

/*******************************************************************************
function:
        Read the ADC of the channel
parameter:
    Channel_Cmd :	0x90: Read channel Y +, select the ADC resolution is 12
bits, set to differential mode 0xd0: Read channel x +, select the ADC resolution
is 12 bits, set to differential mode
*******************************************************************************/
static uint16_t TP_Read_ADC(uint8_t CMD)
{
    uint16_t Data = 0;

    // A cycle of at least 400ns.
    touch_cs_low(TP_CS_PIN);

    lcd_spi_transport_byte(CMD);
    sleep_us(200);

    //	dont write 0xff, it will block xpt2046
    // Data =  lcd_spi_transport_byte(0Xff);
    Data = lcd_spi_transport_byte(0X00);
    Data <<= 8; // 7bit
    Data |= lcd_spi_transport_byte(0X00);
    // Data =  lcd_spi_transport_byte(0Xff);
    Data >>= 3; // 5bit
    touch_cs_high(TP_CS_PIN);

    return Data;
}

/*******************************************************************************
function:
        Read the 5th channel value and exclude the maximum and minimum returns
the average parameter: Channel_Cmd :	0x90 :Read channel Y + 0xd0 :Read
channel x +
*******************************************************************************/
#define READ_TIMES 5 // Number of readings
#define LOST_NUM   1 // Discard value
static uint16_t TP_Read_ADC_Average(uint8_t Channel_Cmd)
{
    uint8_t i, j;
    uint16_t Read_Buff[READ_TIMES];
    uint16_t Read_Sum = 0, Read_Temp = 0;
    // LCD SPI speed = 3 MHz
    spi_set_baudrate(LCD_SPI_PORT, 3000000);
    // Read and save multiple samples
    for (i = 0; i < READ_TIMES; i++) {
        Read_Buff[i] = TP_Read_ADC(Channel_Cmd);
        sleep_us(200);
    }
    // LCD SPI speed = 18 MHz
    spi_set_baudrate(LCD_SPI_PORT, SPI0_BAUDRATE);
    // Sort from small to large
    for (i = 0; i < READ_TIMES - 1; i++) {
        for (j = i + 1; j < READ_TIMES; j++) {
            if (Read_Buff[i] > Read_Buff[j]) {
                Read_Temp    = Read_Buff[i];
                Read_Buff[i] = Read_Buff[j];
                Read_Buff[j] = Read_Temp;
            }
        }
    }

    // Exclude the largest and the smallest
    for (i = LOST_NUM; i < READ_TIMES - LOST_NUM; i++)
        Read_Sum += Read_Buff[i];

    // Averaging
    Read_Temp = Read_Sum / (READ_TIMES - 2 * LOST_NUM);

    return Read_Temp;
}

/*******************************************************************************
function:
        Read X channel and Y channel AD value
parameter:
    Channel_Cmd :	0x90 :Read channel Y +
                    0xd0 :Read channel x +
*******************************************************************************/
static void TP_Read_ADC_XY(uint16_t* pXCh_Adc, uint16_t* pYCh_Adc)
{
    *pXCh_Adc = TP_Read_ADC_Average(0xD0);
    *pYCh_Adc = TP_Read_ADC_Average(0x90);
}

/*******************************************************************************
function:
        2 times to read the touch screen IC, and the two can not exceed the
deviation, ERR_RANGE, meet the conditions, then that the correct reading,
otherwise the reading error. parameter: Channel_Cmd :	pYCh_Adc = 0x90 :Read
channel Y + pXCh_Adc = 0xd0 :Read channel x +
*******************************************************************************/
#define ERR_RANGE 50 // tolerance scope
static bool TP_Read_TwiceADC(uint16_t* pXCh_Adc, uint16_t* pYCh_Adc)
{
    uint16_t XCh_Adc1, YCh_Adc1, XCh_Adc2, YCh_Adc2;

    // Read the ADC values Read the ADC values twice
    TP_Read_ADC_XY(&XCh_Adc1, &YCh_Adc1);
    sleep_us(10);
    TP_Read_ADC_XY(&XCh_Adc2, &YCh_Adc2);
    sleep_us(10);

    // The ADC error used twice is greater than ERR_RANGE to take the average
    if (((XCh_Adc2 <= XCh_Adc1 && XCh_Adc1 < XCh_Adc2 + ERR_RANGE) ||
         (XCh_Adc1 <= XCh_Adc2 && XCh_Adc2 < XCh_Adc1 + ERR_RANGE)) &&
        ((YCh_Adc2 <= YCh_Adc1 && YCh_Adc1 < YCh_Adc2 + ERR_RANGE) ||
         (YCh_Adc1 <= YCh_Adc2 && YCh_Adc2 < YCh_Adc1 + ERR_RANGE))) {
        *pXCh_Adc = (XCh_Adc1 + XCh_Adc2) / 2;
        *pYCh_Adc = (YCh_Adc1 + YCh_Adc2) / 2;
        return true;
    }

    // The ADC error used twice is less than ERR_RANGE returns failed
    return false;
}

static bool TP_Read_5xADC(uint16_t* pXCh_Adc, uint16_t* pYCh_Adc)
{
    uint16_t Ch_Adc[5][2] = {0};
    uint16_t *XCh_Adc_Max = &Ch_Adc[0][0], *YCh_Adc_Max = &Ch_Adc[0][1];
    uint16_t *XCh_Adc_Min = &Ch_Adc[0][0], *YCh_Adc_Min = &Ch_Adc[0][1];

    // Read the ADC values Read the ADC values twice
    for (size_t i = 0; i < 5; i++) {
        TP_Read_ADC_XY(&Ch_Adc[i][0], &Ch_Adc[i][1]);
        if (*XCh_Adc_Max < Ch_Adc[i][0])
            XCh_Adc_Max = &Ch_Adc[i][0];
        if (*YCh_Adc_Max < Ch_Adc[i][1])
            YCh_Adc_Max = &Ch_Adc[i][1];
        if (*XCh_Adc_Min > Ch_Adc[i][0])
            XCh_Adc_Min = &Ch_Adc[i][0];
        if (*YCh_Adc_Min < Ch_Adc[i][1])
            YCh_Adc_Min = &Ch_Adc[i][1];
        *pXCh_Adc += Ch_Adc[i][0];
        *pYCh_Adc += Ch_Adc[i][1];
        sleep_us(10);
    }

    *pXCh_Adc = (*pXCh_Adc - *XCh_Adc_Max - *XCh_Adc_Min) / 3;
    *pYCh_Adc = (*pYCh_Adc - *YCh_Adc_Max - *YCh_Adc_Min) / 3;

    // The ADC error used twice is less than ERR_RANGE returns failed
    return true;
}

/*******************************************************************************
function:
        Calculation
parameter:
        chCoordType:
                    1 : calibration
                    0 : relative position
*******************************************************************************/
static uint8_t TP_Scan(uint8_t chCoordType)
{
    // In X, Y coordinate measurement, IRQ is disabled and output is low
    if (!gpio_get(TP_IRQ_PIN)) { // Press the button to press
        // Read the physical coordinates
        if (chCoordType) {
            TP_Read_TwiceADC(&sTP_DEV.Xpoint, &sTP_DEV.Ypoint);
            // Read the screen coordinates
        } else if (TP_Read_5xADC(&sTP_DEV.Xpoint, &sTP_DEV.Ypoint)) {
            // if (LCD_2_8 == id) {
            sTP_Draw.Ypoint = sTP_DEV.fXfac * sTP_DEV.Xpoint + sTP_DEV.iXoff;
            sTP_Draw.Xpoint = sTP_DEV.fYfac * sTP_DEV.Ypoint + sTP_DEV.iYoff;
            // }
        }
        if (0 == (sTP_DEV.chStatus & TP_PRESS_DOWN)) { // Not being pressed
            sTP_DEV.chStatus = TP_PRESS_DOWN | TP_PRESSED;
            sTP_DEV.Xpoint0  = sTP_DEV.Xpoint;
            sTP_DEV.Ypoint0  = sTP_DEV.Ypoint;
        }
    } else {
        if (sTP_DEV.chStatus & TP_PRESS_DOWN) { // 0x80
            sTP_DEV.chStatus &= ~(1 << 7);      // 0x00
        } else {
            sTP_DEV.Xpoint0 = 0;
            sTP_DEV.Ypoint0 = 0;
            sTP_DEV.Xpoint  = 0xffff;
            sTP_DEV.Ypoint  = 0xffff;
        }
    }

    return (sTP_DEV.chStatus & TP_PRESS_DOWN);
}

static uint8_t Touch_Scan(void)
{
    // In X, Y coordinate measurement, IRQ is disabled and output is low
    if (!gpio_get(TP_IRQ_PIN)) { // Press the button to press
                                 // Read the physical coordinates
        TP_Read_5xADC(&sTP_DEV.Xpoint, &sTP_DEV.Ypoint);
        sTP_Draw.Ypoint = sTP_DEV.fXfac * sTP_DEV.Xpoint + sTP_DEV.iXoff;
        sTP_Draw.Xpoint = sTP_DEV.fYfac * sTP_DEV.Ypoint + sTP_DEV.iYoff;
        if (0 == (sTP_DEV.chStatus & TP_PRESS_DOWN)) { // Not being pressed
            sTP_DEV.chStatus = TP_PRESS_DOWN | TP_PRESSED;
            sTP_DEV.Xpoint0  = sTP_DEV.Xpoint;
            sTP_DEV.Ypoint0  = sTP_DEV.Ypoint;
        }
    } else {
        if (sTP_DEV.chStatus & TP_PRESS_DOWN) { // 0x80
            sTP_DEV.chStatus &= ~(1 << 7);      // 0x00
        } else {
            sTP_DEV.Xpoint0 = 0;
            sTP_DEV.Ypoint0 = 0;
            sTP_DEV.Xpoint  = 0xffff;
            sTP_DEV.Ypoint  = 0xffff;
            sTP_Draw.Xpoint = 0;
            sTP_Draw.Ypoint = 0;
        }
    }

    return (sTP_DEV.chStatus & TP_PRESS_DOWN);
}
/*******************************************************************************
function:
        Draw Cross
parameter:
            Xpoint :	The x coordinate of the point
            Ypoint :	The y coordinate of the point
            Color  :	Set color
*******************************************************************************/
// static void TP_DrawCross(POINT Xpoint, POINT Ypoint, COLOR Color) {
//     GUI_DrawLine(Xpoint - 12, Ypoint, Xpoint + 12, Ypoint, Color, LINE_SOLID,
//                  DOT_PIXEL_1X1);
//     GUI_DrawLine(Xpoint, Ypoint - 12, Xpoint, Ypoint + 12, Color, LINE_SOLID,
//                  DOT_PIXEL_1X1);
//     GUI_DrawPoint(Xpoint, Ypoint, Color, DOT_PIXEL_2X2, DOT_FILL_AROUND);
//     GUI_DrawCircle(Xpoint, Ypoint, 6, Color, DRAW_EMPTY, DOT_PIXEL_1X1);
// }

/*******************************************************************************
function:
        The corresponding ADC value is displayed on the LC
parameter:
            (Xpoint0 ,Xpoint0):	The coordinates of the first point
            (Xpoint1 ,Xpoint1):	The coordinates of the second point
            (Xpoint2 ,Xpoint2):	The coordinates of the third point
            (Xpoint3 ,Xpoint3):	The coordinates of the fourth point
            hwFac	:	Percentage of error
*******************************************************************************/
// static void TP_ShowInfo(POINT Xpoint0, POINT Ypoint0, POINT Xpoint1,
//                         POINT Ypoint1, POINT Xpoint2, POINT Ypoint2,
//                         POINT Xpoint3, POINT Ypoint3, POINT hwFac) {
//     if (LCD_2_8 == id) {
//     } else {
//         sFONT *TP_Font = &Font16;
//         LENGTH TP_Dx = TP_Font->Width;

//         GUI_DrawRectangle(40, 160, 250, 270, WHITE, DRAW_FULL, DOT_PIXEL_1X1);

//         GUI_DisString_EN(40, 160, "x1", TP_Font, FONT_BACKGROUND, RED);
//         GUI_DisString_EN(40 + 100, 160, "y1", TP_Font, FONT_BACKGROUND, RED);

//         GUI_DisString_EN(40, 180, "x2", TP_Font, FONT_BACKGROUND, RED);
//         GUI_DisString_EN(40 + 100, 180, "y2", TP_Font, FONT_BACKGROUND, RED);

//         GUI_DisString_EN(40, 200, "x3", TP_Font, FONT_BACKGROUND, RED);
//         GUI_DisString_EN(40 + 100, 200, "y3", TP_Font, FONT_BACKGROUND, RED);

//         GUI_DisString_EN(40, 220, "x4", TP_Font, FONT_BACKGROUND, RED);
//         GUI_DisString_EN(40 + 100, 220, "y4", TP_Font, FONT_BACKGROUND, RED);

//         GUI_DisString_EN(40, 240, "fac is : ", TP_Font, FONT_BACKGROUND, RED);

//         GUI_DisNum(40 + 3 * TP_Dx, 160, Xpoint0, TP_Font, FONT_BACKGROUND, RED);
//         GUI_DisNum(40 + 3 * TP_Dx + 100, 160, Ypoint0, TP_Font, FONT_BACKGROUND,
//                    RED);

//         GUI_DisNum(40 + 3 * TP_Dx, 180, Xpoint1, TP_Font, FONT_BACKGROUND, RED);
//         GUI_DisNum(40 + 3 * TP_Dx + 100, 180, Ypoint1, TP_Font, FONT_BACKGROUND,
//                    RED);

//         GUI_DisNum(40 + 3 * TP_Dx, 200, Xpoint2, TP_Font, FONT_BACKGROUND, RED);
//         GUI_DisNum(40 + 3 * TP_Dx + 100, 200, Ypoint2, TP_Font, FONT_BACKGROUND,
//                    RED);

//         GUI_DisNum(40 + 3 * TP_Dx, 220, Xpoint3, TP_Font, FONT_BACKGROUND, RED);
//         GUI_DisNum(40 + 3 * TP_Dx + 100, 220, Ypoint3, TP_Font, FONT_BACKGROUND,
//                    RED);

//         GUI_DisNum(40 + 10 * TP_Dx, 240, hwFac, TP_Font, FONT_BACKGROUND, RED);
//     }
// }

/*******************************************************************************
function:
        Touch screen adjust
*******************************************************************************/
// void TP_Adjust(void) {
//     uint8_t cnt = 0;
//     uint16_t XYpoint_Arr[4][2];
//     uint32_t Dx, Dy;
//     uint16_t Sqrt1, Sqrt2;
//     float Dsqrt;

//     LCD_Clear(LCD_BACKGROUND);
//     GUI_DisString_EN(0, 60,
//                      "Please use the stylus click the cross"
//                      "on the screen. The cross will always move until"
//                      "the screen adjustment is completed.",
//                      &Font16, FONT_BACKGROUND, RED);

//     uint8_t Mar_Val = 12;
//     TP_DrawCross(Mar_Val, Mar_Val, RED);

//     sTP_DEV.chStatus = 0;
//     while (1) {
//         TP_Scan(1);
//         if ((sTP_DEV.chStatus & 0xC0) == TP_PRESSED) {
//             sTP_DEV.chStatus &= ~(1 << 6);
//             XYpoint_Arr[cnt][0] = sTP_DEV.Xpoint;
//             XYpoint_Arr[cnt][1] = sTP_DEV.Ypoint;
//             printf("X%d,Y%d = %d,%d\r\n", cnt, cnt, XYpoint_Arr[cnt][0],
//                    XYpoint_Arr[cnt][1]);
//             cnt++;
//             sleep_ms(200);

//             switch (cnt) {
//             case 1:
//                 // DEBUG("not touch TP_IRQ 2 = %d\r\n", GET_TP_IRQ);
//                 TP_DrawCross(Mar_Val, Mar_Val, WHITE);
//                 TP_DrawCross(sLCD_DIS.LCD_Dis_Column - Mar_Val, Mar_Val, RED);
//                 sleep_ms(200);
//                 break;
//             case 2:
//                 // DEBUG("not touch TP_IRQ 3 = %d\r\n", GET_TP_IRQ);
//                 TP_DrawCross(sLCD_DIS.LCD_Dis_Column - Mar_Val, Mar_Val, WHITE);
//                 TP_DrawCross(Mar_Val, sLCD_DIS.LCD_Dis_Page - Mar_Val, RED);
//                 sleep_ms(200);
//                 break;
//             case 3:
//                 // DEBUG("not touch TP_IRQ 4 = %d\r\n", GET_TP_IRQ);
//                 TP_DrawCross(Mar_Val, sLCD_DIS.LCD_Dis_Page - Mar_Val, WHITE);
//                 TP_DrawCross(sLCD_DIS.LCD_Dis_Column - Mar_Val,
//                              sLCD_DIS.LCD_Dis_Page - Mar_Val, RED);
//                 sleep_ms(200);
//                 break;
//             case 4:

//                 // 1.Compare the X direction
//                 Dx = abs((int16_t)(XYpoint_Arr[0][0] -
//                                    XYpoint_Arr[1][0])); // x1 - x2
//                 Dy = abs((int16_t)(XYpoint_Arr[0][1] -
//                                    XYpoint_Arr[1][1])); // y1 - y2
//                 Dx *= Dx;
//                 Dy *= Dy;
//                 Sqrt1 = sqrt(Dx + Dy);

//                 Dx = abs((int16_t)(XYpoint_Arr[2][0] -
//                                    XYpoint_Arr[3][0])); // x3 - x4
//                 Dy = abs((int16_t)(XYpoint_Arr[2][1] -
//                                    XYpoint_Arr[3][1])); // y3 - y4
//                 Dx *= Dx;
//                 Dy *= Dy;
//                 Sqrt2 = sqrt(Dx + Dy);

//                 Dsqrt = (float)Sqrt1 / Sqrt2;
//                 if (Dsqrt < 0.95 || Dsqrt > 1.05 || Sqrt1 == 0 || Sqrt2 == 0) {
//                     // DEBUG("Adjust X direction \r\n");
//                     cnt = 0;
//                     TP_ShowInfo(
//                         XYpoint_Arr[0][0], XYpoint_Arr[0][1], XYpoint_Arr[1][0],
//                         XYpoint_Arr[1][1], XYpoint_Arr[2][0], XYpoint_Arr[2][1],
//                         XYpoint_Arr[3][0], XYpoint_Arr[3][1], Dsqrt * 100);
//                     sleep_ms(1000);
//                     TP_DrawCross(sLCD_DIS.LCD_Dis_Column - Mar_Val,
//                                  sLCD_DIS.LCD_Dis_Page - Mar_Val, WHITE);
//                     TP_DrawCross(Mar_Val, Mar_Val, RED);
//                     continue;
//                 }

//                 // 2.Compare the Y direction
//                 Dx = abs((int16_t)(XYpoint_Arr[0][0] -
//                                    XYpoint_Arr[2][0])); // x1 - x3
//                 Dy = abs((int16_t)(XYpoint_Arr[0][1] -
//                                    XYpoint_Arr[2][1])); // y1 - y3
//                 Dx *= Dx;
//                 Dy *= Dy;
//                 Sqrt1 = sqrt(Dx + Dy);

//                 Dx = abs((int16_t)(XYpoint_Arr[1][0] -
//                                    XYpoint_Arr[3][0])); // x2 - x4
//                 Dy = abs((int16_t)(XYpoint_Arr[1][1] -
//                                    XYpoint_Arr[3][1])); // y2 - y4
//                 Dx *= Dx;
//                 Dy *= Dy;
//                 Sqrt2 = sqrt(Dx + Dy); //

//                 Dsqrt = (float)Sqrt1 / Sqrt2;
//                 if (Dsqrt < 0.95 || Dsqrt > 1.05) {
//                     // DEBUG("Adjust Y direction \r\n");
//                     cnt = 0;
//                     TP_ShowInfo(
//                         XYpoint_Arr[0][0], XYpoint_Arr[0][1], XYpoint_Arr[1][0],
//                         XYpoint_Arr[1][1], XYpoint_Arr[2][0], XYpoint_Arr[2][1],
//                         XYpoint_Arr[3][0], XYpoint_Arr[3][1], Dsqrt * 100);
//                     sleep_ms(1000);
//                     TP_DrawCross(sLCD_DIS.LCD_Dis_Column - Mar_Val,
//                                  sLCD_DIS.LCD_Dis_Page - Mar_Val, WHITE);
//                     TP_DrawCross(Mar_Val, Mar_Val, RED);
//                     continue;
//                 } //

//                 // 3.Compare diagonal
//                 Dx = abs((int16_t)(XYpoint_Arr[1][0] -
//                                    XYpoint_Arr[2][0])); // x1 - x3
//                 Dy = abs((int16_t)(XYpoint_Arr[1][1] -
//                                    XYpoint_Arr[2][1])); // y1 - y3
//                 Dx *= Dx;
//                 Dy *= Dy;
//                 Sqrt1 = sqrt(Dx + Dy); //;

//                 Dx = abs((int16_t)(XYpoint_Arr[0][0] -
//                                    XYpoint_Arr[3][0])); // x2 - x4
//                 Dy = abs((int16_t)(XYpoint_Arr[0][1] -
//                                    XYpoint_Arr[3][1])); // y2 - y4
//                 Dx *= Dx;
//                 Dy *= Dy;
//                 Sqrt2 = sqrt(Dx + Dy); //

//                 Dsqrt = (float)Sqrt1 / Sqrt2;
//                 if (Dsqrt < 0.95 || Dsqrt > 1.05) {
//                     printf("Adjust diagonal direction\r\n");
//                     cnt = 0;
//                     TP_ShowInfo(
//                         XYpoint_Arr[0][0], XYpoint_Arr[0][1], XYpoint_Arr[1][0],
//                         XYpoint_Arr[1][1], XYpoint_Arr[2][0], XYpoint_Arr[2][1],
//                         XYpoint_Arr[3][0], XYpoint_Arr[3][1], Dsqrt * 100);
//                     sleep_ms(1000);
//                     TP_DrawCross(sLCD_DIS.LCD_Dis_Column - Mar_Val,
//                                  sLCD_DIS.LCD_Dis_Page - Mar_Val, WHITE);
//                     TP_DrawCross(Mar_Val, Mar_Val, RED);
//                     continue;
//                 }

//                 // 4.Get the scale factor and offset
//                 // Get the scanning direction of the touch screen
//                 sTP_DEV.TP_Scan_Dir = sLCD_DIS.LCD_Scan_Dir;
//                 sTP_DEV.fXfac = 0;

//                 // According to the display direction to get
//                 // the corresponding scale factor and offset
//                 sTP_DEV.fYfac =
//                     (float)(sLCD_DIS.LCD_Dis_Column - 2 * Mar_Val) /
//                     (int16_t)(XYpoint_Arr[1][1] - XYpoint_Arr[0][1]);
//                 sTP_DEV.fXfac =
//                     (float)(sLCD_DIS.LCD_Dis_Page - 2 * Mar_Val) /
//                     (int16_t)(XYpoint_Arr[2][0] - XYpoint_Arr[0][0]);

//                 sTP_DEV.iXoff =
//                     (sLCD_DIS.LCD_Dis_Page -
//                      sTP_DEV.fXfac * (XYpoint_Arr[0][1] + XYpoint_Arr[1][1])) /
//                     2;
//                 sTP_DEV.iYoff =
//                     (sLCD_DIS.LCD_Dis_Column -
//                      sTP_DEV.fYfac * (XYpoint_Arr[0][0] + XYpoint_Arr[2][0])) /
//                     2;
//                 printf("sTP_DEV.fXfac = %f \r\n", sTP_DEV.fXfac);
//                 printf("sTP_DEV.fYfac = %f \r\n", sTP_DEV.fYfac);
//                 printf("sTP_DEV.iXoff = %d \r\n", sTP_DEV.iXoff);
//                 printf("sTP_DEV.iYoff = %d \r\n", sTP_DEV.iYoff);

//                 // 6.Calibration is successful
//                 LCD_Clear(LCD_BACKGROUND);
//                 GUI_DisString_EN(35, 110, "Touch Screen Adjust OK!", &Font16,
//                                  FONT_BACKGROUND, RED);
//                 sleep_ms(1000);
//                 LCD_Clear(LCD_BACKGROUND);
//                 return;
//                 // Exception handling,Reset  Initial value
//             default:
//                 cnt = 0;
//                 TP_DrawCross(sLCD_DIS.LCD_Dis_Column - Mar_Val,
//                              sLCD_DIS.LCD_Dis_Page - Mar_Val, WHITE);
//                 TP_DrawCross(Mar_Val, Mar_Val, RED);
//                 GUI_DisString_EN(40, 26, "TP Need readjust!", &Font16,
//                                  FONT_BACKGROUND, RED);
//                 break;
//             }
//         }
//     }
// }

/*******************************************************************************
function:
        Use the default calibration factor
*******************************************************************************/
void TP_GetAdFac(void)
{
    sTP_DEV.fXfac = 0.069722;
    sTP_DEV.fYfac = -0.092154;
    sTP_DEV.iXoff = -12;
    sTP_DEV.iYoff = 344;
    // sTP_DEV.fXfac = -0.066176;
    // sTP_DEV.fYfac = 0.089860;
    // sTP_DEV.iXoff = 260;
    // sTP_DEV.iYoff = -26;
}

/*******************************************************************************
function:
        Paint the Delete key and paint color choose area
*******************************************************************************/
// void TP_Dialog(void) {
//     LCD_Clear(LCD_BACKGROUND);
//     if (LCD_2_8 == id) {
//         GUI_DisString_EN(sLCD_DIS.LCD_Dis_Column - 60, 0, "CLEAR", &Font16, RED,
//                          BLUE);
//         GUI_DisString_EN(sLCD_DIS.LCD_Dis_Column - 120, 0, "AD", &Font16, RED,
//                          BLUE);
//         GUI_DrawRectangle(0, 0, 15, 15, BLUE, DRAW_FULL, DOT_PIXEL_1X1);
//         GUI_DrawRectangle(20, 0, 35, 15, GREEN, DRAW_FULL, DOT_PIXEL_1X1);
//         GUI_DrawRectangle(40, 0, 55, 15, RED, DRAW_FULL, DOT_PIXEL_1X1);
//         GUI_DrawRectangle(60, 0, 75, 15, YELLOW, DRAW_FULL, DOT_PIXEL_1X1);
//         GUI_DrawRectangle(80, 0, 95, 15, BLACK, DRAW_FULL, DOT_PIXEL_1X1);
//     }
// }

/*******************************************************************************
function:
        Draw Board
*******************************************************************************/
// void TP_DrawBoard(void) {
//     //	sTP_DEV.chStatus &= ~(1 << 6);
//     TP_Scan(0);
//     if (sTP_DEV.chStatus & TP_PRESS_DOWN) { // Press the button
//         // Horizontal screen
//         if (sTP_Draw.Xpoint < sLCD_DIS.LCD_Dis_Column &&
//             // Determine whether the law is legal
//             sTP_Draw.Ypoint < sLCD_DIS.LCD_Dis_Page) {
//             // Judgment is horizontal screen
//             if (sLCD_DIS.LCD_Dis_Column > sLCD_DIS.LCD_Dis_Page) {
//                 printf("horizontal x:%d,y:%d\n", sTP_Draw.Xpoint,
//                        sTP_Draw.Ypoint);
//                 GUI_DrawPoint(sTP_Draw.Xpoint, sTP_Draw.Ypoint, sTP_Draw.Color,
//                               DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
//                 GUI_DrawPoint(sTP_Draw.Xpoint + 1, sTP_Draw.Ypoint,
//                               sTP_Draw.Color, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
//                 GUI_DrawPoint(sTP_Draw.Xpoint, sTP_Draw.Ypoint + 1,
//                               sTP_Draw.Color, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
//                 GUI_DrawPoint(sTP_Draw.Xpoint + 1, sTP_Draw.Ypoint + 1,
//                               sTP_Draw.Color, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
//                 GUI_DrawPoint(sTP_Draw.Xpoint, sTP_Draw.Ypoint, sTP_Draw.Color,
//                               DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
//             }
//         }
//     }
// }

/*******************************************************************************
function:
        Touch pad initialization
*******************************************************************************/
void TP_Init(LCD_SCAN_DIR Lcd_ScanDir)
{
    touch_cs_high(TP_CS_PIN);

    sTP_DEV.TP_Scan_Dir = Lcd_ScanDir;

    TP_Read_ADC_XY(&sTP_DEV.Xpoint, &sTP_DEV.Ypoint);

    TP_GetAdFac();
}

int select_touch(UI_LOG_MSG_T* select_win, int lenght)
{
    Touch_Scan();
    if (sTP_DEV.chStatus & TP_PRESS_DOWN) {
        // if (sTP_Draw.Xpoint < sLCD_DIS.LCD_Dis_Column &&
        //     sTP_Draw.Ypoint < sLCD_DIS.LCD_Dis_Page)
        // {
        // printf("horizontal x:%d,y:%d\n", sTP_Draw.Xpoint, sTP_Draw.Ypoint);
        for (size_t i = 0; i < lenght; i++)
            if (select_win[i].x_start < sTP_Draw.Xpoint && select_win[i].x_end > sTP_Draw.Xpoint &&
                select_win[i].y_start < sTP_Draw.Ypoint && select_win[i].y_end > sTP_Draw.Ypoint)
                return i;
        // }
    }
    return -1;
}