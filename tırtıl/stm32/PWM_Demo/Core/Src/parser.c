/*
 * parser.c
 *
 *  Created on: Sep 16, 2021
 *      Author: mg
 */

#include "main.h"
#include "uart_comm.h"
#include "dcmotor.h"
#include "parser.h"

extern UART_HandleTypeDef huart2;

#define UART_CMD_DC_MOTOR 			0
#define UART_CMD_LED_MATRIX 		1

#define UART_CMD_DC_STEP_FORWARD 	41
#define UART_CMD_DC_STEP_BACKWARD 	42
#define UART_CMD_DC_STEP_RIGHT 		43
#define UART_CMD_DC_STEP_LEFT 		44

#define UART_CMD_LED_MATRIX_ON 		50
#define UART_CMD_LED_MATRIX_OFF 	51
#define UART_CMD_LED_MATRIX_SAD 	52
#define UART_CMD_LED_MATRIX_HAPPY 	53
#define UART_CMD_LED_MATRIX_CROSS 	54
#define UART_CMD_LED_MATRIX_ANGRY 	55
#define UART_CMD_LED_MATRIX_HEART 	56

void process_uart_data(uint8_t* buf){
	uint8_t len = buf[0];
	uint8_t cmd = buf[1];
	switch (cmd){

	case UART_CMD_DC_MOTOR:
		motor_action(buf[2], buf[3], buf[4], buf[5]);
		break;

	case UART_CMD_DC_STEP_FORWARD:
		motor_all_action(ACTION_STEP_FORWARD,buf[2], buf[3]);
		break;

	case UART_CMD_DC_STEP_BACKWARD:
		motor_all_action(ACTION_STEP_BACKWARD,buf[2], buf[3]);
		break;

	case UART_CMD_DC_STEP_LEFT:
		motor_all_action(ACTION_STEP_LEFT,buf[2], buf[3]);
		break;

	case UART_CMD_DC_STEP_RIGHT:
		motor_all_action(ACTION_STEP_RIGHT,buf[2], buf[3]);
		break;

	case UART_CMD_LED_MATRIX_HAPPY:
		MatrixLCD_ResetPixels();
		draw_HappyMatrixLED();
		break;

	case UART_CMD_LED_MATRIX_SAD:
		MatrixLCD_ResetPixels();
		draw_SadMatrixLED();
		break;

	case UART_CMD_LED_MATRIX_ANGRY:
		MatrixLCD_ResetPixels();
		draw_AngryMatrixLED();
		break;

	case UART_CMD_LED_MATRIX:
		MatrixLCD_ResetPixels();
		MatrixLCD_SetPixels2Buf(&buf[2]);
		break;

	case UART_CMD_LED_MATRIX_ON:
		MatrixLCD_ResetPixels();
		draw_OnMatrixLED();
		break;

	case UART_CMD_LED_MATRIX_OFF:
		MatrixLCD_ResetPixels();
		draw_OffMatrixLED();
		break;

	case UART_CMD_LED_MATRIX_CROSS:
		MatrixLCD_ResetPixels();
		draw_cross2MatrixLED();
		break;

	case UART_CMD_LED_MATRIX_HEART:
		MatrixLCD_ResetPixels();
		draw_heart();
		break;
	}
}

void parse_uart_data(){
	Uart_Ch* uch = get_uart_ch();

	if(uch->fdata_ready){
		//uart_send_data(&huart1, uch->rx_data_buf, uch->rx_data_cntr);
		process_uart_data(&(uch->rx_data_buf[2]));
		uch->fdata_ready = 0;
	}
}

