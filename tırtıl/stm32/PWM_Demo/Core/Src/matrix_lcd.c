/*
 * matrix_lcd.c
 *
 *  Created on: Sep 14, 2021
 *      Author: rk
 */
#include "main.h"
#include "matrix_lcd.h"

GPIOType LedMatrixRows[MATRIX_LED_ROW];
GPIOType LedMatrixCols[MATRIX_LED_COL];
uint8_t MatrixLCD[MATRIX_LED_ROW][MATRIX_LED_COL];

void resetLEDIOs()
{
	for(uint8_t i = 0;i<MATRIX_LED_COL;i++){
		HAL_GPIO_WritePin(LedMatrixCols[i].GPIO_PORT, LedMatrixCols[i].GPIO_PIN, GPIO_PIN_SET);
	}

	for(uint8_t i = 0;i<MATRIX_LED_ROW;i++){
		HAL_GPIO_WritePin(LedMatrixRows[i].GPIO_PORT, LedMatrixRows[i].GPIO_PIN, GPIO_PIN_RESET);
	}
}

void initLedMatrixGPIO(){
	LedMatrixRows[0].GPIO_PORT = GPIO7_GPIO_Port;
	LedMatrixRows[0].GPIO_PIN = GPIO7_Pin;

	LedMatrixRows[1].GPIO_PORT = GPIO6_GPIO_Port;
	LedMatrixRows[1].GPIO_PIN = GPIO6_Pin;

	LedMatrixRows[2].GPIO_PORT = GPIO5_GPIO_Port;
	LedMatrixRows[2].GPIO_PIN = GPIO5_Pin;

	LedMatrixRows[3].GPIO_PORT = GPIO4_GPIO_Port;
	LedMatrixRows[3].GPIO_PIN = GPIO4_Pin;

	LedMatrixRows[4].GPIO_PORT = GPIO3_GPIO_Port;
	LedMatrixRows[4].GPIO_PIN = GPIO3_Pin;

	LedMatrixCols[0].GPIO_PORT = GPIO8_GPIO_Port;
	LedMatrixCols[0].GPIO_PIN = GPIO8_Pin;

	LedMatrixCols[1].GPIO_PORT = GPIO9_GPIO_Port;
	LedMatrixCols[1].GPIO_PIN = GPIO9_Pin;

	LedMatrixCols[2].GPIO_PORT = GPIO15_GPIO_Port;
	LedMatrixCols[2].GPIO_PIN = GPIO15_Pin;

	LedMatrixCols[3].GPIO_PORT = GPIO17_GPIO_Port;
	LedMatrixCols[3].GPIO_PIN = GPIO17_Pin;

	LedMatrixCols[4].GPIO_PORT = GPIO18_GPIO_Port;
	LedMatrixCols[4].GPIO_PIN = GPIO18_Pin;

	resetLEDIOs();
}

void MatrixLCD_SetPixel(uint8_t row, uint8_t col){
	MatrixLCD[row][col] = 1;
}

void MatrixLCD_ResetPixels(){
	for(uint8_t i = 0;i<MATRIX_LED_ROW;i++)
		for (uint8_t j = 0;j<MATRIX_LED_COL;j++){
			MatrixLCD[i][j] = 0; //row,col
		}

	//the best place to send uart ack
	set_f_send_uart_ack();
}

void MatrixLCD_SetPixels2Buf(uint8_t* buf){
	uint8_t x = 0;
	for(uint8_t i = 0;i<MATRIX_LED_ROW;i++)
		for (uint8_t j = 0;j<MATRIX_LED_COL;j++){
			MatrixLCD[i][j] = buf[x++]; //row,col
		}
}

void Matrix2LCD(uint8_t ledIndex){
	for(uint8_t i = 0;i<MATRIX_LED_ROW;i++){
		HAL_GPIO_WritePin(LedMatrixRows[i].GPIO_PORT,
				LedMatrixRows[i].GPIO_PIN, MatrixLCD[i][ledIndex]);
	}
}

uint8_t MatrixLED_JohnsonCounter(){
	static uint8_t ledIndex = 0;

	if(ledIndex == MATRIX_LED_COL) ledIndex = 0;

	//reset all rows
	for(uint8_t i= 0;i<MATRIX_LED_ROW;i++){
		HAL_GPIO_WritePin(LedMatrixRows[i].GPIO_PORT, LedMatrixRows[i].GPIO_PIN, GPIO_PIN_RESET);
	}

	//set all cols
	for(uint8_t i= 0;i<MATRIX_LED_COL;i++){
		HAL_GPIO_WritePin(LedMatrixCols[i].GPIO_PORT, LedMatrixCols[i].GPIO_PIN, GPIO_PIN_SET);
	}

	//only reset the dedicated col
	HAL_GPIO_WritePin(LedMatrixCols[ledIndex].GPIO_PORT, LedMatrixCols[ledIndex].GPIO_PIN, GPIO_PIN_RESET);

	return ledIndex++;
}

void MatrixLED_Timer_Callback()
{
	uint8_t ledIndex;
	static uint16_t matrixLed_RefreshCounter = 0;

	if(matrixLed_RefreshCounter < 65000)
		matrixLed_RefreshCounter++;
	else matrixLed_RefreshCounter = 0;

	if(matrixLed_RefreshCounter%10 == 0){
		ledIndex = MatrixLED_JohnsonCounter();
		Matrix2LCD(ledIndex);
	}
}

//test drawing
void draw_cross2MatrixLED(){
	MatrixLCD_SetPixel(0, 0);
	MatrixLCD_SetPixel(1, 1);
	MatrixLCD_SetPixel(2, 2);
	MatrixLCD_SetPixel(3, 3);
	MatrixLCD_SetPixel(4, 4);

	MatrixLCD_SetPixel(0, 4);
	MatrixLCD_SetPixel(1, 3);
	MatrixLCD_SetPixel(2, 2);
	MatrixLCD_SetPixel(3, 1);
	MatrixLCD_SetPixel(4, 0);
}

void draw_HappyMatrixLED(){
	MatrixLCD_SetPixel(0, 2);

	MatrixLCD_SetPixel(1, 1);
	MatrixLCD_SetPixel(1, 4);

	MatrixLCD_SetPixel(2, 1);
	MatrixLCD_SetPixel(2, 3);

	MatrixLCD_SetPixel(3, 1);
	MatrixLCD_SetPixel(3, 4);

	MatrixLCD_SetPixel(4, 2);
}

void draw_SadMatrixLED(){
	MatrixLCD_SetPixel(0, 1);

	MatrixLCD_SetPixel(1, 2);
	MatrixLCD_SetPixel(1, 4);

	MatrixLCD_SetPixel(2, 2);

	MatrixLCD_SetPixel(3, 2);
	MatrixLCD_SetPixel(3, 4);

	MatrixLCD_SetPixel(4, 1);
}

void draw_AngryMatrixLED(){
	MatrixLCD_SetPixel(0, 0);
	MatrixLCD_SetPixel(0, 4);

	MatrixLCD_SetPixel(1, 1);
	MatrixLCD_SetPixel(1, 3);

	MatrixLCD_SetPixel(2, 1);

	MatrixLCD_SetPixel(3, 1);
	MatrixLCD_SetPixel(3, 3);

	MatrixLCD_SetPixel(4, 0);
	MatrixLCD_SetPixel(4, 4);
}

void draw_OnMatrixLED(){
	MatrixLCD_SetPixel(2, 2);
	MatrixLCD_SetPixel(2, 3);
	MatrixLCD_SetPixel(3, 2);
	MatrixLCD_SetPixel(3, 3);
}

void draw_heart(){
	MatrixLCD_SetPixel(0, 2);
	MatrixLCD_SetPixel(0, 3);
	MatrixLCD_SetPixel(0, 4);

	MatrixLCD_SetPixel(1, 1);
	MatrixLCD_SetPixel(1, 2);
	MatrixLCD_SetPixel(1, 3);
	MatrixLCD_SetPixel(1, 4);

	MatrixLCD_SetPixel(2, 0);
	MatrixLCD_SetPixel(2, 1);
	MatrixLCD_SetPixel(2, 2);
	MatrixLCD_SetPixel(2, 3);

	MatrixLCD_SetPixel(3, 1);
	MatrixLCD_SetPixel(3, 2);
	MatrixLCD_SetPixel(3, 3);
	MatrixLCD_SetPixel(3, 4);

	MatrixLCD_SetPixel(4, 2);
	MatrixLCD_SetPixel(4, 3);
	MatrixLCD_SetPixel(4, 4);

}

void draw_OffMatrixLED(){
}
