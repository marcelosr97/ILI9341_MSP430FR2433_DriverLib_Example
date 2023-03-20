/**
  @file LcdIf_cfg.h
  @brief LCD Interface configuration file.
  @author Marcelo Simbana (https://github.com/marcelosr97)
  @date 02/02/2023
*/

#ifndef LCDIF_CFG_H_
#define LCDIF_CFG_H_

/******************************************************************************/
/*                             LCD SIZE                                       */
/******************************************************************************/

#define LCD_WIDTH             240U
#define LCD_HEIGHT            320U

/******************************************************************************/
/*                          CHAR PARAMETERS                                   */
/******************************************************************************/

#define CHAR_SPACE 		        2U

/******************************************************************************/
/*                       BOARD PIN CONFIGURATION                              */
/******************************************************************************/

/* LCD */
/* Chip Select (CS) to P2.1*/
#define LCD_CS_PORT           GPIO_PORT_P2 
#define LCD_CS_PIN            GPIO_PIN1
/* Reset (RST) to P2.2*/
#define LCD_RST_PORT          GPIO_PORT_P2
#define LCD_RST_PIN           GPIO_PIN2
/* Data/Command (DC) to P2.3 */
#define LCD_DC_PORT           GPIO_PORT_P3
#define LCD_DC_PIN            GPIO_PIN2
/* Master Output Slave Input (MOSI) to P2.6 */
#define LCD_MOSI_PORT         GPIO_PORT_P2
#define LCD_MOSI_PIN          GPIO_PIN6
/* Master Input Slave Ouput (MISO) to P2.5 */
#define LCD_MISO_PORT         GPIO_PORT_P2
#define LCD_MISO_PIN          GPIO_PIN5
/* Signal Clock (SCK) to P2.4*/
#define LCD_SCK_PORT          GPIO_PORT_P2
#define LCD_SCK_PIN           GPIO_PIN4

/* TOUCH */
/* Chip Select (CS) to P2.0*/
#define TOUCH_CS_PORT         GPIO_PORT_P2 
#define TOUCH_CS_PIN          GPIO_PIN0

/* Interrupt (IRQ) to P2.4 */
#define TOUCH_IRQ_PORT        GPIO_PORT_P1
#define TOUCH_IRQ_PIN         GPIO_PIN6

#endif /* LCDIF_CFG_H_ */
