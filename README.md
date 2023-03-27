# ILI9341_MSP430FR2433_DriverLib_Example
 Example of example of ILI9341 TFT-Display (240x320) for MSP-EXP430FR2433 Launchpad Development Kit using MSP Driver Library. 

Pinout configuration:

| MSP430FR2433  | ILI9341       |
| ------------- | ------------- |
| P2.5          | SDO(MISO)     |
| 3.3V          | LED           |
| P2.4          | SCK           |
| P2.6          | SDI(MOSI)     |
| P3.2          | DC            |
| P2.2          | RESET         |
| P2.1          | CS            |
| GND           | GND           |
| 5V            | VCC           |

| MSP430FR2433  | XPT2046       |
| ------------- | ------------- |
| P2.7          | T_IRQ         |
| P2.5          | T_DO          |
| P2.6          | T_DIN         |
| P2.0          | T_CS          |
| P2.4          | T_CLK         |

For debugging:
    1. Open CCS
    2. Import project
    3. General -> Existing Projects into Workspace
    4. Select this root directory
    5. Finish
    6. Debug -> Debug configurations -> Select ILI9341_MSP430FR2433_DriverLib_Example.launch
    
Reference material used for this project:

ILI9341 TFT-Display mit MSP430
https://xdec.de/ili9341-tft-display-mit-msp430/

2.8inch SPI Module ILI9341 SKU:MSP2807
http://www.lcdwiki.com/2.8inch_SPI_Module_ILI9341_SKU:MSP2807

Implemented routine calibration
https://www.embedded.com/how-to-calibrate-touch-screens/