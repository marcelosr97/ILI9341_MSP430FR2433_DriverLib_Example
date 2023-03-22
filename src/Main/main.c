/**
  @file main.c
  @brief main file for LcdIf Project.
  @author Marcelo Simbana (https://github.com/marcelosr97)
  @date 02/02/2023
*/

#include "driverlib.h"
#include "Board.h"
#include "LcdIf.h"
#include "delay.h"
#include "typedef.h"

/******************************************************************************/
/*                 			GLOBAL FUNCTION DEFINITIONS      	              */
/******************************************************************************/

void CS_Init(void);
void GPIO_Init(void);
void SPI_Init(void);
void UART_Init(void);
void APP_Test_Features(void);

/******************************************************************************/
/*                 			GLOBAL FUNCTION DECLARATION      	              */
/******************************************************************************/

void CS_Init(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    //Set DCO FLL reference = REFO
    CS_initClockSignal(CS_FLLREF, CS_REFOCLK_SELECT,CS_CLOCK_DIVIDER_1);

    //Set ACLK = REFO
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    //For demonstration purpose, change DCO clock freq to 16MHz
    CS_initFLLSettle(16000, 488);

    //Set MCLK = DCO (16 Mhz)
    CS_initClockSignal(CS_MCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_1);

}

void GPIO_Init(void)
{
    //LCD
    //Set CS Pin as output
    GPIO_setAsOutputPin(LCD_CS_PORT, LCD_CS_PIN);
    // Set CS Pin as high
    GPIO_setOutputHighOnPin(LCD_CS_PORT, LCD_CS_PIN);

    //Set RST Pin as output
    GPIO_setAsOutputPin(LCD_RST_PORT, LCD_RST_PIN);
    //Set RST Pin as low
    GPIO_setOutputLowOnPin(LCD_RST_PORT, LCD_RST_PIN);

    //Set DC Pin as output
    GPIO_setAsOutputPin(LCD_DC_PORT, LCD_DC_PIN);
    //Set DC Pin as low
    GPIO_setOutputLowOnPin(LCD_DC_PORT, LCD_DC_PIN);

    // Configure SPI Pins for UCA1CLK, UCA1SIMO and UCA1SOMI
    GPIO_setAsPeripheralModuleFunctionInputPin(LCD_MOSI_PORT, LCD_MOSI_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(LCD_MISO_PORT, LCD_MISO_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(LCD_SCK_PORT, LCD_SCK_PIN, GPIO_PRIMARY_MODULE_FUNCTION);

    //TOUCH
    //Set CS pin as output
    GPIO_setAsOutputPin(TOUCH_CS_PORT, TOUCH_CS_PIN);
    //Set CS Pin as low
    GPIO_setOutputHighOnPin(TOUCH_CS_PORT, TOUCH_CS_PIN);

    //Enable IRQ internal resistance as pull-Up resistance
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_S1, GPIO_PIN_S1);

    //Configure UART pins
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_UCA0TXD,
        GPIO_PIN_UCA0TXD,
        GPIO_FUNCTION_UCA0TXD
    );
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_UCA0RXD,
        GPIO_PIN_UCA0RXD,
        GPIO_FUNCTION_UCA0RXD
    );

    //Disable the GPIO power-on default high-impedance mode to activate
    //previously configured port settings
    
    PMM_unlockLPM5();
}

void SPI_Init(void)
{
       //Initialize Master
       EUSCI_A_SPI_initMasterParam param = {0};
       param.selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK;
       param.clockSourceFrequency = CS_getSMCLK();
       param.desiredSpiClock = CS_getSMCLK()/8;
       param.msbFirst = EUSCI_A_SPI_MSB_FIRST;
       param.clockPhase = EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
       param.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
       param.spiMode = EUSCI_A_SPI_3PIN;
       EUSCI_A_SPI_initMaster(EUSCI_A1_BASE, &param);

       //Enable SPI module
       EUSCI_A_SPI_enable(EUSCI_A1_BASE);

       //Clear transmit interrupt flag
       EUSCI_A_SPI_clearInterrupt(EUSCI_A1_BASE,
             EUSCI_A_SPI_TRANSMIT_INTERRUPT
             );

       //Clear receive interrupt flag
       EUSCI_A_SPI_clearInterrupt(EUSCI_A1_BASE,
             EUSCI_A_SPI_TRANSMIT_INTERRUPT
             );

       __enable_interrupt();      // CPU off, enable interrupts
}

void UART_Init(void)
{
    //Configure UART
    //SMCLK = 16MHz, Baudrate = 115200
    //UCBRx = 8, UCBRFx = 10, UCBRSx = 0xF7, UCOS16 = 1
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 8;
    param.firstModReg = 10;
    param.secondModReg = 0xF7;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &param)) {
        return;
    }

    EUSCI_A_UART_enable(EUSCI_A0_BASE);
}

int main(void)
{
    // Driver Inicialization
    CS_Init();
    GPIO_Init();
    SPI_Init();
    UART_Init();
    // Test LcdIf
    // Lcd Interface test
    LcdIf_Init();
    LcdIf_FillScreen(BLACK);
    //APP_Test_Features();
    while(1)
    {
        uint16 x,y;
        static uint8 touchValue = 0U;
        delayMilliseconds(10);
        if(!GPIO_getInputPinValue(GPIO_PORT_S1, GPIO_PIN_S1) && touchValue)
        {
            LcdIf_ReadXY(&x,&y);
        }
        touchValue = GPIO_getInputPinValue(GPIO_PORT_S1, GPIO_PIN_S1);
    }
    return 0;
}

void APP_Test_Features(void)
{
    // Test rectangles
    LcdIf_DrawRectangle(20, 100, 200, 300, BLACK);
    LcdIf_DrawRectangle(40, 60, 90, 200, BLUE);
    LcdIf_DrawRectangle(70, 80, 200, 120, GREEN);
    LcdIf_DrawRectangle(150, 20, 230, 280, RED);
    // Test lines
    LcdIf_FillScreen(WHITE);
    LcdIf_DrawLine(80, 20, 80, 300, BLACK);
    LcdIf_DrawLine(200, 200, 200, 20, BLUE);
    LcdIf_DrawLine(20, 40, 200, 40, GREEN);
    LcdIf_DrawLine(150, 300, 150, 40, RED);
    LcdIf_FillScreen(GRAY);
    LcdIf_DrawLine(40, 300, 200, 20, RED);
    LcdIf_DrawLine(20, 20, 200, 300, WHITE);
    LcdIf_DrawLine(220, 50, 10, 300, BLUE);
    LcdIf_DrawLine(220, 250, 50, 10, GREEN);
    LcdIf_DrawLine(0, 160, 240, 160, BROWN);
    // Test text
    LcdIf_FillScreen(BLACK);
    LcdIf_DrawStr(20, 60, "Hello World,", GREEN, TRANSP);
    LcdIf_DrawStr(20, 70, "This is an example of ILI9341", GREEN, TRANSP);
    LcdIf_DrawStr(20, 80, "TFT-Display (240x320) for", GREEN, TRANSP);
    LcdIf_DrawStr(20, 90, "MSP-EXP430FR2433 Launchpad", GREEN, TRANSP);
    LcdIf_DrawStr(20, 100, "Development Kit using", GREEN, TRANSP);
    LcdIf_DrawStr(20, 110, "MSP Driver Library.", GREEN, TRANSP);
    LcdIf_DrawStr(20, 120, "https://github.com/marcelosr97", GREEN, TRANSP);
}
