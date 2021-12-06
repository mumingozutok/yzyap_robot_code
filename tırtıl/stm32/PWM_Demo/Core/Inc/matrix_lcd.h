/*
 * matrix_lcd.h
 *
 *  Created on: Sep 14, 2021
 *      Author: rk
 */

#ifndef INC_MATRIX_LCD_H_
#define INC_MATRIX_LCD_H_

#define MATRIX_LED_COL 5
#define MATRIX_LED_ROW 5

typedef struct S_GPIO{
	uint32_t GPIO_PORT;
	uint32_t GPIO_PIN;
}GPIOType;

void resetLEDIOs();
void initLedMatrixGPIO();
void MatrixLCD_SetPixel(uint8_t row, uint8_t col);
void MatrixLCD_ResetPixels();
void Matrix2LCD(uint8_t ledIndex);
uint8_t MatrixLED_JohnsonCounter();
void MatrixLCD_SetPixels2Buf(uint8_t* buf);

void draw_cross2MatrixLED();
void draw_Smile2MatrixLED();
void draw_Anger2MatrixLED();


#endif /* INC_MATRIX_LCD_H_ */
