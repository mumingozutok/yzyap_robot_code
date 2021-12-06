/*
 * uart_comm.c
 *
 *  Created on: Sep 16, 2021
 *      Author: mg
 */

#include "main.h"
#include "uart_comm.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim6;
Uart_Ch uart_ch = {.fdata_ready=0,.rx_data_cntr=0, .new_byte_received_flag=0};

uint8_t f_send_uart_ack = 0;
uint8_t get_f_send_uart_ack(){return f_send_uart_ack;}
void set_f_send_uart_ack(){f_send_uart_ack = 1;}
void rst_f_send_uart_ack(){f_send_uart_ack = 0;}

Uart_Ch* get_uart_ch(){
	return &uart_ch;
}

uint8_t calc_chksum(){
	uint8_t sum = 0;
	for(uint8_t i = 0;i<uart_ch.rx_data_cntr;i++){
		sum = sum + uart_ch.rx_data_buf[i];
	}
	return sum;
}

uint8_t is_data_valid(uint8_t* data, uint8_t len){
	uint8_t incoming_checksum, package_len, i,total=0;

	if(len <2) return 0;//Each data pack must hold at least 4 byte
	if(data[0] != 0xFF | data[1] != 0xFE) return 0; //each data packet shall start with 0xFF-0xFE

	incoming_checksum  = data[len-1];//last byte is checksum
	package_len = data[2]; //data[2] is the length

	//calculate checksum for valid new data
	for(i=3;i<len-1;i++){
		total = total + data[i];
	}

	//disable checksum control
	if(total == incoming_checksum){
		return 1;
	}

	return 0;
}

void startTimer(TIM_HandleTypeDef* htim){
	htim->Instance->CR1 &= ~0x01; //Stop Timer
	htim->Instance->CNT = 0; //Reset Counter
	htim->Instance->CR1 |= 0x01; //Start Timer
}

void stopTimer(TIM_HandleTypeDef* htim){
	htim->Instance->CR1 &= ~0x01; //Stop Timer
	htim->Instance->CNT = 0; //Reset Counter
}

void UART_NewByteRcv_Callback(UART_HandleTypeDef *huart)
{
		HAL_UART_Receive_IT(huart, &(uart_ch.rxdata), 1);
		startTimer(&htim6);

		if((uart_ch.rx_data_cntr < 64) & (uart_ch.fdata_ready == 0)){ //parse edilmeyi bekleyen bir data varsa üzerine yazma.
			uart_ch.rx_data_buf[uart_ch.rx_data_cntr++] = uart_ch.rxdata;
		}
}

uint8_t uart_state = UART_STATE_FF;
void UART_Callback_StateMachine(UART_HandleTypeDef *huart){
	static uint8_t len;
	static uint8_t datacounter = 0;
	switch (uart_state){
		case UART_STATE_FF:
			uart_ch.rx_data_cntr = 0;
			uart_ch.rx_data_buf[uart_ch.rx_data_cntr++] = uart_ch.rxdata;
			if (uart_ch.rxdata == 0xFF) uart_state = UART_STATE_FE;
			break;
		case UART_STATE_FE:
			uart_ch.rx_data_buf[uart_ch.rx_data_cntr++] = uart_ch.rxdata;
			if (uart_ch.rxdata == 0xFE) uart_state = UART_STATE_LEN;
			else uart_state = UART_STATE_FF;
			break;
		case UART_STATE_LEN:
			uart_ch.rx_data_buf[uart_ch.rx_data_cntr++] = uart_ch.rxdata;
			datacounter = 0;
			len = uart_ch.rxdata;
			uart_state = UART_STATE_DATA;
			break;
		case UART_STATE_DATA:
			uart_ch.rx_data_buf[uart_ch.rx_data_cntr++] = uart_ch.rxdata;
			if(datacounter++ == len-1) uart_state = UART_STATE_ACK;
			break;
		case UART_STATE_ACK:
			uart_ch.rx_data_buf[uart_ch.rx_data_cntr++] = uart_ch.rxdata;
			if(is_data_valid(uart_ch.rx_data_buf, uart_ch.rx_data_cntr)){
				uart_ch.fdata_ready = 1;
			}

			else {
				uart_ch.data_error_counter++;
			}
			uart_state = UART_STATE_FF;
			break;
	}

	uart_ch.new_byte_received_flag = 1;
	initiate_Uart_Ch(huart);
	startTimer(&htim6);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		//uart_ch.new_byte_received_flag = 1;
	//UART_NewByteRcv_Callback(huart);
	UART_Callback_StateMachine(huart);
}

void uart_comm_timer_elapsed_callback(TIM_HandleTypeDef* htim)
{
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
	stopTimer(htim);
	uart_state = UART_STATE_FF;
	initiate_Uart_Ch(&huart2);

	/*if(is_data_valid(uart_ch.rx_data_buf, uart_ch.rx_data_cntr)){
		uart_ch.fdata_ready = 1;
	}

	else {
		uart_ch.data_error_counter++;
	}
	uart_ch.rx_data_cntr = 0;*/
}

void initiate_Uart_Ch(UART_HandleTypeDef *huart){
	HAL_UART_Receive_IT(huart, &(uart_ch.rxdata), 1);
}

void uart_send_data(UART_HandleTypeDef *huart, uint8_t* data, uint8_t len){
	HAL_UART_Transmit(huart, data, len, 100);
}
