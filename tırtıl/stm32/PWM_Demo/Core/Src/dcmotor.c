/*
 * motor.c
 *
 *  Created on: Sep 27, 2021
 *      Author: mg
 */
#include "main.h"
#include "dcmotor.h"

MotorType wheelMotors[WHEEL_MOTOR_NUMBER];

uint8_t f_motor_new_action = 0;

uint8_t get_f_motor_new_action(){return f_motor_new_action;}
void set_f_motor_new_action(){f_motor_new_action = 1;}
void rst_f_motor_new_action(){f_motor_new_action = 0;}

void initWheelMotors(){
	//MOTOR1
	wheelMotors[1].MOTOR_PORT = GPIOA;
	wheelMotors[1].MOTOR_F_PIN = GPIO_PIN_9;
	wheelMotors[1].MOTOR_B_PIN = GPIO_PIN_8;

	//MOTOR2
	wheelMotors[0].MOTOR_PORT = GPIOA;
	wheelMotors[0].MOTOR_F_PIN = GPIO_PIN_10;
	wheelMotors[0].MOTOR_B_PIN = GPIO_PIN_11;

	//MOTOR3
	wheelMotors[2].MOTOR_PORT = GPIOC;
	wheelMotors[2].MOTOR_F_PIN = GPIO_PIN_6;
	wheelMotors[2].MOTOR_B_PIN = GPIO_PIN_7;

	//MOTOR4
	wheelMotors[3].MOTOR_PORT = GPIOC;
	wheelMotors[3].MOTOR_F_PIN = GPIO_PIN_9;
	wheelMotors[3].MOTOR_B_PIN = GPIO_PIN_8;


	for(uint8_t i = 0;i<WHEEL_MOTOR_NUMBER;i++){
		wheelMotors[i].velocity = 0;
		wheelMotors[i].initial_velocity = 0;
		wheelMotors[i].step_number = 0;
		wheelMotors[i].action = ACTION_STOP;
		wheelMotors[i].target_velocity = 100;
		wheelMotors[i].state_motion = 0;
		HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_B_PIN,0);
		HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_F_PIN,0);

	}

}
//motor_number: 1,2,3,4
//action: ileri, geri, dur
//hiz: 0-100
void motor_action(uint8_t motor_number, uint8_t action, uint8_t velocity, uint8_t step_number){
	if (action <= ACTION_STEP_LEFT){
		wheelMotors[motor_number].action = action;
	}
	else {
		wheelMotors[motor_number].action = ACTION_STOP;
	}

	if(velocity > 100) {
		wheelMotors[motor_number].velocity = 0;
		wheelMotors[motor_number].initial_velocity = 0;
	}
	else{
		wheelMotors[motor_number].velocity = 100;
		wheelMotors[motor_number].initial_velocity = 100;
		wheelMotors[motor_number].target_velocity = 100-velocity;
	}
	wheelMotors[motor_number].step_number = step_number;
	wheelMotors[motor_number].state_motion = 0;

	wheelMotors[motor_number].no_move = (velocity == 0) ? 1:0;

	set_f_motor_new_action();
}

void motor_all_action(uint8_t action, uint8_t velocity, uint8_t step){
	if(action == ACTION_STEP_RIGHT){
		motor_action(0,action,velocity,step);
		motor_action(1,action,0,step);
		motor_action(2,action,velocity,step);
		motor_action(3,action,0,step);
	}
	else if(action == ACTION_STEP_LEFT){
		motor_action(0,action,0,step);
		motor_action(1,action,velocity,step);
		motor_action(2,action,0,step);
		motor_action(3,action,velocity,step);
	}
	else{
		motor_action(0,action,velocity,step);
		motor_action(1,action,velocity,step);
		motor_action(2,action,velocity,step);
		motor_action(3,action,velocity,step);
	}
}

void motor_pwm_timer_callback()
{
	static uint16_t motor_pwm_counter = 0;
	static uint16_t velocity_change_counter = 0;
	static uint8_t state_motion = 0;
	if(motor_pwm_counter < 100){
		for(uint8_t i = 0;i<WHEEL_MOTOR_NUMBER;i++){

			if(wheelMotors[i].velocity == motor_pwm_counter){
				if(wheelMotors[i].action == ACTION_STEP_FORWARD){
					HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_F_PIN,1);
					HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_B_PIN,0);
				}
				else if(wheelMotors[i].action == ACTION_STEP_BACKWARD){
					HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_F_PIN,0);
					HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_B_PIN,1);
				}
				else if(wheelMotors[i].action == ACTION_STEP_LEFT |
							wheelMotors[i].action == ACTION_STEP_RIGHT ){
					HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_F_PIN,1);
					HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_B_PIN,0);
				}
			}
		}
		motor_pwm_counter++;

	}
	else{
		motor_pwm_counter = 0;
		for(uint8_t i = 0;i<WHEEL_MOTOR_NUMBER;i++){
			HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_B_PIN,0);
			HAL_GPIO_WritePin(wheelMotors[i].MOTOR_PORT,wheelMotors[i].MOTOR_F_PIN,0);
			if(wheelMotors[i].no_move == 0){
				switch(wheelMotors[i].state_motion){
					case 0:
						if(wheelMotors[i].target_velocity < wheelMotors[i].velocity) {
							wheelMotors[i].velocity--;
						}
						else{
							wheelMotors[i].state_motion = 1;
						}
						break;
					case 1:
						if(wheelMotors[i].velocity < 100) {
							wheelMotors[i].velocity++;
						}
						else{
							wheelMotors[i].state_motion = 2;
						}
						break;
					case 2:
						if((wheelMotors[i].step_number-1) > 0){
							wheelMotors[i].step_number = wheelMotors[i].step_number -1;
						}
						else{
							wheelMotors[i].target_velocity = 100;
							rst_f_motor_new_action();
							set_f_send_uart_ack();
						}

						wheelMotors[i].state_motion = 0;
						break;
				}
			}
		}
	}
}

void test_motor()
{
	for(uint8_t i = 0;i<WHEEL_MOTOR_NUMBER;i++){
	  motor_action(i, ACTION_FORWARD, 30,5);
	  HAL_Delay(5000);
	  motor_action(i, ACTION_FORWARD, 70,5);
	  HAL_Delay(5000);
	  motor_action(i, ACTION_BACKWARD, 30,5);
	  HAL_Delay(5000);
	  motor_action(i, ACTION_BACKWARD, 70,5);
	  HAL_Delay(5000);
	  motor_action(i, ACTION_STOP, 0,0);
	  HAL_Delay(5000);

	}
}
