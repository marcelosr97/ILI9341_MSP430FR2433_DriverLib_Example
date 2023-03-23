/**
 * @file LcdIf.h
 * @brief LCD Interface header file.
 * @author Marcelo Simbana (https://github.com/marcelosr97)
 * @date 02/02/2023
 */

#ifndef LCDIF_H_
#define LCDIF_H_

#include "LcdIf_cfg.h"
#include "delay.h"
#include "typedef.h"
#include "driverlib.h"
#include "Board.h"

/******************************************************************************/
/*                          DEFINES                                           */
/******************************************************************************/

/* COMMAND / DATA */
#define COMMAND 0U
#define DATA 1U

/* COMMAND LIST */
#define POWER_CONTROL_A 0xCBU
#define POWER_CONTROL_B 0xCFU
#define DRIVER_TIMING_CONTROL_A 0xE8U
#define DRIVER_TIMING_CONTROL_B 0xEAU
#define POWER_ON_SQUENCE_CONTROL 0xEDU
#define PUMP_RATIO_CONTROL 0xF7U
#define POWER_CONTROL_1 0xC0U
#define POWER_CONTROL_2 0xC1U
#define VCOM_CONTROL_1 0xC5U
#define VCOM_CONTROL_2 0xC7U
#define MEMORY_ACCESS_CONTROL 0x36U
#define COLMOD_PIXEL_FORMAT_SET 0x3AU
#define FRAME_RATE_CONTROL_NORMAL 0xB1U
#define DISPLAY_FUNCTION_CONTROL 0xB6U
#define ENABLE_3G 0xF2U
#define GAMMA_SET 0x26U
#define POSITIVE_GAMMA_CORRECTION 0xE0U
#define NEGATIVE_GAMMA_CORRECTION 0xE1U
#define SLEEP_OUT 0x11U
#define DISPLAY_OFF 0x28U
#define DISPLAY_ON 0x29U
#define MEMORY_WRITE 0x2CU
#define COLUMN_ADDRESS_SET 0x2AU
#define PAGE_ADDRESS_SET 0x2BU

/* TOUCH */
#define READ_X_REF_OFF 0xD1 // PD1 = 0, PD0 = 1 Reference is on and ADC is off
#define READ_Y_REF_OFF 0x91 // PD1 = 0, PD0 = 1 Reference is on and ADC is off
#define READ_Y_PWR_DOWN 0x90 // PD1 = 0, PD0 = 1 Power-Down between conversions

/* COLORS RGB565 */
#define WHITE 0xFFFFU
#define BLACK 0x0000U
#define BLUE 0xF800U
#define GREEN 0x0400U
#define RED 0x001FU
#define YELLOW 0xFFE0U
#define SILVER 0xC618U
#define BRED 0XF81FU
#define GRED 0XFFE0U
#define GBLUE 0X07FFU
#define MAGENTA 0xF81FU
#define CYAN 0x7FFFU
#define BROWN 0X79C0U
#define BRRED 0XFC07U
#define GRAY 0X8430U
#define TRANSP 0xF81FU

/* 12 BITS ADC CONVERTER MAX VALUE */
#define ADC_CONVERTER_MAX_VALUE 0x0FFFU

/******************************************************************************/
/*                          DATA TYPES                                        */
/******************************************************************************/

/**
 * @brief LCD Interface return type.
 *
 */
typedef enum
{
    LCDIF_OK = 0,
    LCDIF_NOT_OK,
    LCDIF_OVERFLOW
} LcdIf_ReturnType;

/******************************************************************************/
/*                 			GLOBAL FUNCTION DEFINITIONS      	              */
/******************************************************************************/

/**
 * @brief Lcd Interface Initialization. It initializes the display ILI9341 with
 * a default configuration.
 *
 */

void LcdIf_Init(void);

/**
 * @brief Fill the screen with a given color.
 *
 * @param t_color Color in RGB565.
 */
void LcdIf_FillScreen(uint16 t_color);

/**
 * @brief Draw the rectangle given for two points.
 *
 * @param t_x1 X coordinate of the first point.
 * @param t_y1 Y coordinate of the first point.
 * @param t_x2 X coordinate of the second point.
 * @param t_y2 Y coordinate of the second point.
 * @param t_color Color in RGB565.
 *
 */

LcdIf_ReturnType LcdIf_DrawRectangle(uint16 t_x1, uint16 t_y1, uint16 t_x2,
                                     uint16 t_y2, uint16 t_color);

/**
 * @brief Draw a line given by two points.
 *
 * @param t_x1 X coordinate of the first point.
 * @param t_y1 Y coordinate of the first point.
 * @param t_x2 X coordinate of the second point.
 * @param t_y2 Y coordinate of the second point.
 * @param t_color Color in RGB565.
 * @return
 *
 */

LcdIf_ReturnType LcdIf_DrawLine(uint16 t_x1, uint16 t_y1, uint16 t_x2, uint16 t_y2,
                                uint16 t_color);

/**
 * @brief Draw a point in the specific coordinates.
 *
 * @param t_x X coordinate of the point.
 * @param t_y Y coordinate of the point.
 * @param t_color Color in RGB565.
 * @return
 *
 */

LcdIf_ReturnType LcdIf_DrawPoint(uint16 t_x, uint16 t_y, uint16 t_color);

/**
 * @brief Draw a char in the specific coordinates.
 *
 * @param t_x X coordinate of the char.
 * @param t_y Y coordinate of the char.
 * @param t_ptrStr Pointer to the char.
 * @param t_fColor Foreground color in RGB565.
 * @param t_bColor Background color in RGB565.
 * @return
 *
 */

LcdIf_ReturnType LcdIf_DrawChar(uint16 t_x, uint16 t_y, const uint8 *t_ptrStr,
                                uint16 t_fColor, uint16 t_bColor);

/**
 * @brief Draw a string in the specific coordinates.
 *
 * @param t_x X coordinate of the char.
 * @param t_y Y coordinate of the char.
 * @param t_color Color in RGB565.
 * @return
 *
 */

LcdIf_ReturnType LcdIf_DrawStr(uint16 x, uint16 y, const uint8 *t_ptrStr, uint16 t_fColor,
                               uint16 t_bColor);

LcdIf_ReturnType LcdIf_ReadXY(uint16 *t_ptrX, uint16 *t_ptrY);

#endif /* LCD_H_ */
