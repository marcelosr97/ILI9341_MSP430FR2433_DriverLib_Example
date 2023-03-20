/**
  @file LcdIf.c
  @brief LCD Interface source file.
  @author Marcelo Simbana (https://github.com/marcelosr97)
  @date 02/02/2023
*/

#include "LcdIf.h"
#include "LcdIf_data.h"

/******************************************************************************/
/*                      LOCAL FUNCTIONS DECLARATIONS                          */
/******************************************************************************/

static void LcdIf_mHardwareReset(void);
static void LcdIf_mSendInstruction(uint8 t_type, uint8 t_value);
static void LcdIf_mSetRectangle(uint16 t_x1, uint16 t_y1, uint16 t_x2,
                                uint16 t_y2);
static void LcdIf_mSetCursor(uint16 t_x, uint16 t_y);
static void LcdIf_mSetColor(uint16 t_color);
static uint16 LcdIf_mRequestCoordinate(uint8 t_value);

/******************************************************************************/
/*                     STATIC FUNCTIONS DEFINITION                            */
/******************************************************************************/

static void LcdIf_mHardwareReset(void)
{
    // Just in case RESET pin is unstable
    GPIO_setOutputHighOnPin(LCD_RST_PORT, LCD_RST_PIN);
    delayMilliseconds(10);
    GPIO_setOutputLowOnPin(LCD_RST_PORT, LCD_RST_PIN);
    delayMilliseconds(10);
    GPIO_setOutputHighOnPin(LCD_RST_PORT, LCD_RST_PIN);
    delayMilliseconds(100);
}

static void LcdIf_mSendInstruction(uint8 t_type, uint8 t_value)
{
    if (!t_type)
    {
        GPIO_setOutputLowOnPin(LCD_DC_PORT, LCD_DC_PIN); // Command
    }
    else
    {
        GPIO_setOutputHighOnPin(LCD_DC_PORT, LCD_DC_PIN); // Data
    }

    // Write data condition for LCD
    GPIO_setOutputLowOnPin(LCD_CS_PORT, LCD_CS_PIN);
    GPIO_setOutputHighOnPin(TOUCH_CS_PORT, TOUCH_CS_PIN); 
    EUSCI_A_SPI_transmitData(EUSCI_A1_BASE, t_value); // Transmit
}

static void LcdIf_mSetRectangle(uint16 t_x1, uint16 t_y1, uint16 t_x2,
                                uint16 t_y2)
{
    LcdIf_mSendInstruction(COMMAND, COLUMN_ADDRESS_SET);
    LcdIf_mSendInstruction(DATA, t_x1 >> 8U);
    LcdIf_mSendInstruction(DATA, t_x1 & 0xFFU);
    LcdIf_mSendInstruction(DATA, t_x2 >> 8U);
    LcdIf_mSendInstruction(DATA, t_x2 & 0xFFU);

    LcdIf_mSendInstruction(COMMAND, PAGE_ADDRESS_SET);
    LcdIf_mSendInstruction(DATA, t_y1 >> 8U);
    LcdIf_mSendInstruction(DATA, t_y1 & 0xFFU);
    LcdIf_mSendInstruction(DATA, t_y2 >> 8U);
    LcdIf_mSendInstruction(DATA, t_y2 & 0xFFU);

    LcdIf_mSendInstruction(COMMAND, MEMORY_WRITE);
}

static void LcdIf_mSetCursor(uint16 t_x, uint16 t_y)
{
    LcdIf_mSendInstruction(COMMAND, COLUMN_ADDRESS_SET);
    LcdIf_mSendInstruction(DATA, t_x >> 8U);
    LcdIf_mSendInstruction(DATA, t_x & 0xFFU);

    LcdIf_mSendInstruction(COMMAND, PAGE_ADDRESS_SET);
    LcdIf_mSendInstruction(DATA, t_y >> 8U);
    LcdIf_mSendInstruction(DATA, t_y & 0xFFU);

    LcdIf_mSendInstruction(COMMAND, MEMORY_WRITE);
}

static void LcdIf_mSetColor(uint16 t_color)
{
    LcdIf_mSendInstruction(DATA, t_color >> 8);
    LcdIf_mSendInstruction(DATA, t_color & 0xFF);
}

static uint16 LcdIf_mRequestCoordinate(uint8 t_value)
{
    EUSCI_A_SPI_transmitData(EUSCI_A1_BASE, t_value); // Transmit
    return 0;
}

/******************************************************************************/
/*                      GLOBAL FUNCTIONS DEFINITIONS                          */
/******************************************************************************/

void LcdIf_Init(void)
{
    // Enable USCI_A1 TX interrupt
    EUSCI_A_SPI_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);
    // Hardware Reset
    LcdIf_mHardwareReset();

    // Power Control A
    LcdIf_mSendInstruction(COMMAND, POWER_CONTROL_A);
    LcdIf_mSendInstruction(DATA, 0x39);
    LcdIf_mSendInstruction(DATA, 0x2C);
    LcdIf_mSendInstruction(DATA, 0x00);
    LcdIf_mSendInstruction(DATA, 0x34);
    LcdIf_mSendInstruction(DATA, 0x02);

    // Power Control B
    LcdIf_mSendInstruction(COMMAND, POWER_CONTROL_B);
    LcdIf_mSendInstruction(DATA, 0x00U);
    LcdIf_mSendInstruction(DATA, 0xC1U);
    LcdIf_mSendInstruction(DATA, 0x30U);

    // Driver Timing Control A
    LcdIf_mSendInstruction(COMMAND, DRIVER_TIMING_CONTROL_A);
    LcdIf_mSendInstruction(DATA, 0x85U);
    LcdIf_mSendInstruction(DATA, 0x00U);
    LcdIf_mSendInstruction(DATA, 0x78U);

    // Driver Timing Control B
    LcdIf_mSendInstruction(COMMAND, DRIVER_TIMING_CONTROL_B);
    LcdIf_mSendInstruction(DATA, 0x00U);
    LcdIf_mSendInstruction(DATA, 0x00U);

    // Power On Sequence Control
    LcdIf_mSendInstruction(COMMAND, POWER_ON_SQUENCE_CONTROL);
    LcdIf_mSendInstruction(DATA, 0x64U);
    LcdIf_mSendInstruction(DATA, 0x03U);
    LcdIf_mSendInstruction(DATA, 0x12U);
    LcdIf_mSendInstruction(DATA, 0x81U);

    // Pump Ratio Control
    LcdIf_mSendInstruction(COMMAND, PUMP_RATIO_CONTROL);
    LcdIf_mSendInstruction(DATA, 0x20U);

    // Power Control 1
    LcdIf_mSendInstruction(COMMAND, POWER_CONTROL_1);
    LcdIf_mSendInstruction(DATA, 0x23U);

    // Power Control 1
    LcdIf_mSendInstruction(COMMAND, POWER_CONTROL_2);
    LcdIf_mSendInstruction(DATA, 0x10U);

    // VCOM Control 1
    LcdIf_mSendInstruction(COMMAND, VCOM_CONTROL_1);
    LcdIf_mSendInstruction(DATA, 0x3EU);
    LcdIf_mSendInstruction(DATA, 0x28U);

    // VCOM Control 2
    LcdIf_mSendInstruction(COMMAND, VCOM_CONTROL_2);
    LcdIf_mSendInstruction(DATA, 0x86U);

    // Memory Access Control
    LcdIf_mSendInstruction(COMMAND, MEMORY_ACCESS_CONTROL);
    LcdIf_mSendInstruction(DATA, 0x28U);

    // COLMOD: Pixel Format Set
    LcdIf_mSendInstruction(COMMAND, COLMOD_PIXEL_FORMAT_SET);
    LcdIf_mSendInstruction(DATA, 0x55U);

    // Frame Rate Control Normal
    LcdIf_mSendInstruction(COMMAND, FRAME_RATE_CONTROL_NORMAL);
    LcdIf_mSendInstruction(DATA, 0x00U);
    LcdIf_mSendInstruction(DATA, 0x18U);

    // Displat Function Control
    LcdIf_mSendInstruction(COMMAND, DISPLAY_FUNCTION_CONTROL);
    LcdIf_mSendInstruction(DATA, 0x08U);
    LcdIf_mSendInstruction(DATA, 0x82U);
    LcdIf_mSendInstruction(DATA, 0x27U);

    // Enable 3G
    LcdIf_mSendInstruction(COMMAND, ENABLE_3G);
    LcdIf_mSendInstruction(DATA, 0x00U);

    // Gamma Set
    LcdIf_mSendInstruction(COMMAND, GAMMA_SET);
    LcdIf_mSendInstruction(DATA, 0x01U);

    // Positive Gamma Correction
    LcdIf_mSendInstruction(COMMAND, POSITIVE_GAMMA_CORRECTION);
    LcdIf_mSendInstruction(DATA, 0x0FU);
    LcdIf_mSendInstruction(DATA, 0x31U);
    LcdIf_mSendInstruction(DATA, 0x2BU);
    LcdIf_mSendInstruction(DATA, 0x0CU);
    LcdIf_mSendInstruction(DATA, 0x0EU);
    LcdIf_mSendInstruction(DATA, 0x08U);
    LcdIf_mSendInstruction(DATA, 0x4EU);
    LcdIf_mSendInstruction(DATA, 0xF1U);
    LcdIf_mSendInstruction(DATA, 0x37U);
    LcdIf_mSendInstruction(DATA, 0x07U);
    LcdIf_mSendInstruction(DATA, 0x10U);
    LcdIf_mSendInstruction(DATA, 0x03U);
    LcdIf_mSendInstruction(DATA, 0x0EU);
    LcdIf_mSendInstruction(DATA, 0x09U);
    LcdIf_mSendInstruction(DATA, 0x00U);

    // Negative Gamma Correction
    LcdIf_mSendInstruction(COMMAND, NEGATIVE_GAMMA_CORRECTION);
    LcdIf_mSendInstruction(DATA, 0x00U);
    LcdIf_mSendInstruction(DATA, 0x0EU);
    LcdIf_mSendInstruction(DATA, 0x14U);
    LcdIf_mSendInstruction(DATA, 0x03U);
    LcdIf_mSendInstruction(DATA, 0x11U);
    LcdIf_mSendInstruction(DATA, 0x07U);
    LcdIf_mSendInstruction(DATA, 0x31U);
    LcdIf_mSendInstruction(DATA, 0xC1U);
    LcdIf_mSendInstruction(DATA, 0x48U);
    LcdIf_mSendInstruction(DATA, 0x08U);
    LcdIf_mSendInstruction(DATA, 0x0FU);
    LcdIf_mSendInstruction(DATA, 0x0CU);
    LcdIf_mSendInstruction(DATA, 0x31U);
    LcdIf_mSendInstruction(DATA, 0x36U);
    LcdIf_mSendInstruction(DATA, 0x0FU);

    // Sleep Out
    LcdIf_mSendInstruction(COMMAND, SLEEP_OUT);
    delayMilliseconds(120);

    // Display On
    LcdIf_mSendInstruction(COMMAND, DISPLAY_ON);

    // Memory write
    LcdIf_mSendInstruction(COMMAND, MEMORY_WRITE);

    // Enable USCI_A1 TX interrupt
    EUSCI_A_SPI_disableInterrupt(EUSCI_A1_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);
}

void LcdIf_FillScreen(uint16 t_color)
{
    // Draw display-size rectangle
    LcdIf_DrawRectangle(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, t_color);
}

LcdIf_ReturnType LcdIf_DrawRectangle(uint16 t_x1, uint16 t_y1, uint16 t_x2, uint16 t_y2,
                         uint16 t_color)
{
    LcdIf_ReturnType ret = LCDIF_OK;
    uint16 x = 0U;
    uint16 y = 0U;
    const uint16 rectWidth = t_x2 - t_x1 + 1;
    const uint16 rectHeight = t_y2 - t_y1 + 1;

    LcdIf_mSetRectangle(t_x1, t_y1, t_x2, t_y2);
    for (x = 0U; x < rectWidth; x++)
    {
        for (y = 0U; y < rectHeight; y++)
        {
            LcdIf_mSetColor(t_color);
        }
    }
    return ret;
}

LcdIf_ReturnType LcdIf_DrawLine(uint16 t_x1, uint16 t_y1, uint16 t_x2, uint16 t_y2,
                          uint16 t_color)
{
    LcdIf_ReturnType ret = LCDIF_OK;
    sint32 e = 0U;      // Error
    sint16 ax = 0U;     // x distance
    sint16 ay = 0U;     // y distance
    sint16 sx = 0U;     // x sign
    sint16 sy = 0U;     // y sign
    sint16 change = 0U; // Change
    sint16 d = 0U;      // Distance
    sint16 x = 0U;      // Coordinate x to draw
    sint16 y = 0U;      // Coordinate y to draw

    // Check overflow
    if ((t_x1 >= LCD_WIDTH) || (t_x2 >= LCD_WIDTH) || (t_y1 >= LCD_HEIGHT) ||
        (t_y2 >= LCD_HEIGHT))
    {
        ret = LCDIF_OVERFLOW;
    }
    else
    {
        // Bresenham algorithm
        x = t_x1;
        y = t_y1;

        // Absolute value of x distance
        ax = (sint16)t_x2 - (sint16)t_x1;
        ax = ax >= 0 ? ax : -ax;

        // Absolute value of y distance
        ay = (sint16)t_y2 - (sint16)t_y1;
        ay = ay >= 0 ? ay : -ay;

        // Sign of x slope
        if (ax == 0)
        {
            sx = 0;
        }
        else
        {
            sx = ((sint16)t_x2 - (sint16)t_x1) > 0 ? 1 : -1;
        }

        // Sign of y slope
        if (ay == 0)
        {
            sy = 0;
        }
        else
        {
            sy = ((sint16)t_y2 - (sint16)t_y1) > 0 ? 1 : -1;
        }

        // Line calculation parameters
        if (ay > ax)
        {
            sint32 tmp = 0U;
            tmp = ax;
            ax = ay;
            ay = tmp;
            change = 1;
        }
        else
        {
            change = 0;
        }

        // Error calculation
        e = 2 * ay - ax;

        for (d = 1; d <= ax; d++)
        {
            LcdIf_DrawPoint(x, y, t_color);
            if (e >= 0)
            {
                if (change == 1)
                {
                    x = x + sx;
                }
                else
                {
                    y = y + sy;
                }
                e = e - 2 * ax;
            }
            if (change == 1)
            {
                y = y + sy;
            }
            else
            {
                x = x + sx;
            }
            e = e + 2 * ay;
        }
    }
    return ret;
}

LcdIf_ReturnType LcdIf_DrawPoint(uint16 t_x, uint16 t_y, uint16 t_color)
{
    LcdIf_ReturnType ret = LCDIF_OK; // Return value

    if ((t_x >= LCD_WIDTH) || (t_y >= LCD_HEIGHT)) // Check overflow
    {
        ret = LCDIF_OVERFLOW;
    }
    else
    {
        LcdIf_mSetCursor(t_x, t_y);
        LcdIf_mSetColor(t_color);
    }
    return ret;
}

LcdIf_ReturnType LcdIf_DrawChar(uint16 t_x, uint16 t_y, const uint8 *t_ptrStr,
                          uint16 t_fColor, uint16 t_bColor)
{
    LcdIf_ReturnType ret = LCDIF_OK;
    uint8 wInd = 0;
    uint8 hInd = 0;
    for (wInd = 0; wInd < CHAR_WIDTH; wInd++) // Horizontal pixel writing
    {
        for (hInd = 0; hInd < CHAR_HEIGTH; hInd++) // Vertical pixel writing
        {
            // Check overflow
            if ((t_x >= LCD_WIDTH) || (t_y >= LCD_HEIGHT))
            {
                wInd = CHAR_WIDTH; // Exit extern loop
                ret = LCDIF_OVERFLOW;
                break;
            }
            else
            {
                // Current bit is "1", print in foreground color
                if (t_ptrStr[wInd] & (1 << hInd))
                {
                    LcdIf_DrawPoint(t_x + wInd, t_y + hInd, t_fColor);
                }
                // Current bit is "0", print in background color
                else if (t_bColor != TRANSP)
                {
                    LcdIf_DrawPoint(t_x + wInd, t_y + hInd, t_bColor);
                }
            }
        }
    }
    return ret;
}

LcdIf_ReturnType LcdIf_DrawStr(uint16 t_x, uint16 t_y, const uint8 *t_ptrStr,
                         uint16 t_fColor, uint16 t_bColor)
{
    LcdIf_ReturnType ret = LCDIF_OK;      // Ret value
    const uint8 *ptrStr = t_ptrStr; // Get pointer value
    uint8 curSpace = 0;             // Current char space

    if (ptrStr == ((void *)0)) // Check null pointer
    {
        ret = LCDIF_NOT_OK;
    }
    else
    {
        while ((*ptrStr != 0) && (t_x < LCD_WIDTH))
        {
            LcdIf_DrawChar(t_x, t_y, ascii_table[*ptrStr], t_fColor, t_bColor);
            // Check if background should be drawn
            if (t_bColor != TRANSP)
            {
                curSpace = CHAR_SPACE;
                for (curSpace = 0; curSpace <= 0; curSpace--)
                {
                    // Draw rectangle in background color
                    LcdIf_DrawRectangle(t_x - CHAR_SPACE, t_y, t_x - 1,
                                        t_y + CHAR_HEIGTH - 1, t_bColor);
                }
            }
            *ptrStr++;
            t_x += (CHAR_WIDTH + CHAR_SPACE);
        }
        // Check if background should be drawn for the last char
        if (t_bColor != TRANSP)
        {
            curSpace = CHAR_SPACE;
            for (curSpace = 0; curSpace <= 0; curSpace--)
            {
                // Draw rectangle in background color
                LcdIf_DrawRectangle(t_x - CHAR_SPACE, t_y, t_x - 1,
                                    t_y + CHAR_HEIGTH - 1, t_bColor);
            }
        }
    }
    return ret;
}

uint8 rxDataRaw[7];
LcdIf_ReturnType LcdIf_ReadXY(uint16* t_ptrX, uint16* t_ptrY)
{
    uint16 x = 0;
    uint16 y = 0;
    // Transmission CS condition
    GPIO_setOutputLowOnPin(TOUCH_CS_PORT, TOUCH_CS_PIN);
    GPIO_setOutputHighOnPin(LCD_CS_PORT, LCD_CS_PIN);
    // Enable  USCI_A1 TX interrupt
    EUSCI_A_SPI_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_SPI_RECEIVE_INTERRUPT);
    (void) LcdIf_mRequestCoordinate(READ_X);
    (void) LcdIf_mRequestCoordinate(READ_X);
    (void) LcdIf_mRequestCoordinate(READ_X);
    delayMicroseconds(20);
    // Transmission CS condition done
    GPIO_setOutputHighOnPin(TOUCH_CS_PORT, TOUCH_CS_PIN);
    // Disable RX interrupt
    EUSCI_A_SPI_disableInterrupt(EUSCI_A1_BASE, EUSCI_A_SPI_RECEIVE_INTERRUPT);

    *t_ptrX = x;
    *t_ptrY = y;

    return LCDIF_OK;
}
