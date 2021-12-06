/*
 * uart_comm.h
 *
 *  Created on: Sep 16, 2021
 *      Author: mg
 */

#ifndef INC_UART_COMM_H_
#define INC_UART_COMM_H_

#define DC_RX_DATA_BUF_LEN 32

#define UART_STATE_FF 0
#define UART_STATE_FE 1
#define UART_STATE_LEN 2
#define UART_STATE_DATA 3
#define UART_STATE_ACK 4

typedef struct S_Uart_Ch{
	uint8_t rxdata;
	uint8_t rx_data_cntr;
	uint8_t rx_data_buf[DC_RX_DATA_BUF_LEN];
	uint8_t fdata_ready;
	uint8_t data_error_counter;
	uint8_t new_byte_received_flag;
} Uart_Ch;

Uart_Ch* get_uart_ch();
void initiate_Uart_Ch(UART_HandleTypeDef *huart);
void uart_send_data(UART_HandleTypeDef *huart, uint8_t* data, uint8_t len);
void UART_NewByteRcv_Callback();

#endif /* INC_UART_COMM_H_ */
