/*
	----------------------------------------------------------------------
	University at Buffalo, EE379, Spring 2025	
	----------------------------------------------------------------------
	File Name: lab5.cc
	Description: Starting code for Lab 5, basic interrupt setup

	Revision History:
	Date      By          Change
	----------------------------------------------------------------------
	20200413 cvfritz     Original
	20250403 cvfritz     Modified to work with new BSP - Interrupt Clear

 */

#include "xscugic.h"
#include "xtmrctr.h"
#include "xparameters.h"
#include <XGpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <MyDisp.h>

//Defines for interrupt IDs
#define INTC_DEVICE_ID XPAR_PS7_SCUGIC_0_DEVICE_ID
#define GPIO_INT_ID XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define TIMER_INT_ID XPAR_FABRIC_AXI_TIMER_0_INTERRUPT_INTR

//Global variables accessible from any function
XTmrCtr timer;
XGpio input;
MYDISP display;

//Global Interrupt Controller
static XScuGic GIC;

int dir = 0; //0 = left, 1 = right, 2 = up, 3 = down

uint32_t color = clrBlue;

//This function initalizes the GIC, including vector table setup and CPSR IRQ enable
void initIntrSystem(XScuGic * IntcInstancePtr) {

	XScuGic_Config *IntcConfig;
	IntcConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, (void *)IntcInstancePtr);
	Xil_ExceptionEnable();

}

void timerInterruptHandler(void *userParam, u8 TmrCtrNumber) {

}

//Weekly exercise - complete this function!
void buttonInterruptHandler(void *instancePointer) {
	//Read the button state using XGpio_DiscreteRead
	//Set dir based on which bit is a '1'
	
	XGpio_InterruptClear(&input, 0xF); //Leave this line at the end of this function
}


int main() {

	initIntrSystem(&GIC);

	//Prelab Assignment 1
	//Configure GPIO input and output and set direction as usual
	//Note that the input variable is already declared as a global variable
	XGpio_Initialize(&input, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&input, 1, 0xF); //1 = input, 0 = output
	

	//Prelab Assignment 2
	//Configure Timer and timer interrupt as done in class, and comment every line
	XTmrCtr_Initialize (&timer, XPAR_AXI_TIMER_0_DEVICE_ID);
	XTmrCtr_SetHandler(&timer, ( XTmrCtr_Handler ) timerInterruptHandler, ( void*) 0x12345678 );
	XScuGic_Connect (&GIC, TIMER_INT_ID, ( Xil_InterruptHandler ) XTmrCtr_InterruptHandler, &timer);
	XScuGic_Enable (&GIC, TIMER_INT_ID );
	XScuGic_SetPriorityTriggerType (&GIC, TIMER_INT_ID, 0x0, 0x3 );
	XTmrCtr_SetOptions (&timer, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	XTmrCtr_SetResetValue (&timer, 0, 0xFFFFFFFF - 33333333); // 1 Hz
	XTmrCtr_Start(&timer, 0);


	//Prelab Assignment 3
	//Configure GPIO interrupt as done in class, and comment every line
	XScuGic_Connect(&GIC, GPIO_INT_ID, (Xil_ExceptionHandler) buttonInterruptHandler, &input);
	XGpio_InterruptEnable (&input, XGPIO_IR_CH1_MASK);
	XGpio_InterruptGlobalEnable (&input);
	XScuGic_Enable (&GIC, GPIO_INT_ID);
	XScuGic_SetPriorityTriggerType (&GIC, GPIO_INT_ID, 0x8, 0x3);


	display.begin();
	display.clearDisplay(clrWhite);

	//In the main loop move the square based on the global direction variable
	//Set the square's color based on the global color variable
	//You can use delay in the main loop to set the speed that the square moves
	int cx = 120;
	int cy = 160;
    int w = 20; 
    
	while (true) {
        
	}

}

