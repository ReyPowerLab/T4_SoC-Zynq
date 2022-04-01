/**********************************header files************************************************/
#include <stdio.h>
#include <xparameters.h> /*  This file contains the processor’s address space and the device IDs.*/
#include <xscutimer.h>
#include <xscugic.h>	 /*This file holds the drivers for the configuration and use of the GIC. */
#include <xgpio.h>		 
#include <xil_exception.h> /*This file contains exception functions for the Cortex-A9.*/
#include "xeuchw.h"
#include <xil_printf.h>
/************************************************************************************************/

#define INTC_DEVICE_ID			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define EXTP_DEVICE_ID			XPAR_AXI_GPIO_0_DEVICE_ID
#define LEDS_DEVICE_ID 			XPAR_GPIO_LEDS_DEVICE_ID
#define BTNS_DEVICE_ID 			XPAR_GPIO_BTNS_DEVICE_ID
#define XHLS_DEVICE_ID			XPAR_EUCHW_0_DEVICE_ID
#define INTC_GPIO_INT_ID 		XPAR_FABRIC_GPIO_BTNS_IP2INTC_IRPT_INTR
#define INTC_ADDT_INT_ID		XPAR_FABRIC_EUCHW_0_INTERRUPT_INTR
#define TIMER_DEVICE_ID			XPAR_XSCUTIMER_0_DEVICE_ID
#define xil_printf 				printf
#define BTN_INT					XGPIO_IR_CH1_MASK

#define VECTOR_SIZE				768 /* 2*M -> Vectors A y B */
#define BUFFER_SIZE				96  /* VECTOR_SIZE/BRAMS*/
#define BRAMS					8
#define TIMER_LOAD_VALUE 	0xFFFFFFFF

enum errTypes
{
	ERR_HLS_INIT,
	ERR_GPIO_INIT,
	ERR_INTC_INIT,
	ERR_TIMER_INIT,
	ERR_DEFAULT
};
enum IP_ready
{
	IP_Ready,
	IP_Busy
};

int IntcInitFunction(u16 DeviceId, XGpio *GpioIns);
int errorHandler(enum errTypes err);
void BTN_InterruptHandler(void *InsPtr);
int TxDataSend(XEuchw *InstancePtr, int data[VECTOR_SIZE]);
void AdderTreeReceiveHandler(void *InstPtr);

XScuTimer timer;
XGpio leds, btns, extp; /* Gpio Device */
XScuGic intc;	 /*Interrupt Controller Driver*/
XEuchw hls_ip;
static int led_data;
static int btn_value;
volatile int ip_status;

void (*XHLSWriteFunc[])() = {	XEuchw_Write_AB_0_Words,
								XEuchw_Write_AB_1_Words,
								XEuchw_Write_AB_2_Words,
								XEuchw_Write_AB_3_Words,
								XEuchw_Write_AB_4_Words,
								XEuchw_Write_AB_5_Words,
								XEuchw_Write_AB_6_Words,
								XEuchw_Write_AB_7_Words};
u32 TxData[BUFFER_SIZE];
u32 RxData;



int main()
{
	/*************************Initialization******************************* */
	int status = XST_SUCCESS;   // Interruptions status

	/* INIT */
	/* HLS IP init */
	status += XEuchw_Initialize(&hls_ip, XHLS_DEVICE_ID);   // Init IP
	if (status != XST_SUCCESS) return errorHandler(ERR_HLS_INIT); // Verification

	/* gpio init */
	status += XGpio_Initialize(&leds, LEDS_DEVICE_ID);
	status += XGpio_Initialize(&btns, BTNS_DEVICE_ID);
	status += XGpio_Initialize(&extp, EXTP_DEVICE_ID); // ports dont's use in this proyect
	if (status != XST_SUCCESS) return errorHandler(ERR_GPIO_INIT);

    /* Directions */
	XGpio_SetDataDirection(&leds, 1, 0x00);
	XGpio_SetDataDirection(&extp, 1, 0x00);
	XGpio_SetDataDirection(&btns, 1, 0xff);

	/* interrupt controller init*/
	status = IntcInitFunction(INTC_DEVICE_ID, &btns);
	if (status != XST_SUCCESS) return errorHandler(ERR_INTC_INIT);

	/* Timer init */
    XScuTimer_Config *cfgPtr;
	cfgPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	status = XScuTimer_CfgInitialize(&timer, cfgPtr, cfgPtr->BaseAddr);
	if (status != XST_SUCCESS) return errorHandler(ERR_TIMER_INIT);

	ip_status = IP_Ready;
	XGpio_DiscreteWrite(&extp, 1, 0x00);
	int txbuffer[VECTOR_SIZE];
	/***************************** cycle ********************************/
	while(1){
		while (ip_status == IP_Busy) {};
		XGpio_DiscreteWrite(&leds, 1, 0xff); // Leds ON
		getVector(txbuffer); // Get data from PC
	//	XGpio_DiscreteWrite(&leds, 1, 0x00); // Leds Off
		XScuTimer_LoadTimer(&timer, TIMER_LOAD_VALUE);  // Timer
		if (XScuTimer_GetCounterValue(&timer) != TIMER_LOAD_VALUE) return errorHandler(ERR_DEFAULT); // Error timer
		XScuTimer_Start(&timer); // Start timer
		TxDataSend(&hls_ip, txbuffer); // send data to IP
		ip_status = IP_Busy;
		XEuchw_Start(&hls_ip);
		XGpio_DiscreteWrite(&extp, 1, 0x00);
	}
    return 0;
}
/*********************************************************************/


/************************* Functions******************************************/
int TxDataSend(XEuchw *InstancePtr, int data[VECTOR_SIZE])
{
	int status = XST_SUCCESS;
	for (int br = 0; br < BRAMS; br++)
	{
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			TxData[i] = (i*BRAMS +br)< VECTOR_SIZE ? *((u32*) &data[(i*BRAMS) + br]) : 0;
		}
		XHLSWriteFunc[br](InstancePtr, 0, TxData, BUFFER_SIZE);
	}
	return status;
}

void AdderTreeReceiveHandler(void *InstPtr)
{

	float results;
	u32 ticks;
	XEuchw_InterruptDisable(&hls_ip,1);

	XGpio_DiscreteWrite(&extp, 1, 0x00);
	RxData = XEuchw_Get_C(&hls_ip);
	results = *((float*) &(RxData));
	ticks = XScuTimer_GetCounterValue(&timer);
	XScuTimer_Stop(&timer);
	xil_printf("%lu:%f\n", TIMER_LOAD_VALUE - ticks,results);
	ip_status = IP_Ready;
	XEuchw_InterruptClear(&hls_ip,1);
	XEuchw_InterruptEnable(&hls_ip,1);
}

void getVector(int vec[VECTOR_SIZE])
{
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		scanf("%f", &vec[i]);
	}
}
void BTN_Intr_Handler(void *InsPtr)
{
	// Disable GPIO interrupts
	XGpio_InterruptDisable(&btns, BTN_INT);
	// Ignore additional button press
	if ((XGpio_InterruptGetStatus(&btns) & BTN_INT) != BTN_INT) return;

	btn_value = XGpio_DiscreteRead(&btns, 1);
	led_data = led_data + btn_value;

	XGpio_DiscreteWrite(&leds, 1, led_data);
	(void)XGpio_InterruptClear(&btns, BTN_INT);
	// Enable GPIO interrupts
	XGpio_InterruptEnable(&btns, BTN_INT);

}

int errorHandler(enum errTypes err)
{
	switch(err)
	{
		case(ERR_HLS_INIT):
		{
			xil_printf("Error inicializando bloque HLS\n");
			break;
		}
		case(ERR_GPIO_INIT):
		{
			xil_printf("Error inicializando GPIO\n");
			break;
		}
		case(ERR_INTC_INIT):
		{
			xil_printf("Error inicializando INTC\n");
			break;
		}
		case(ERR_TIMER_INIT):
				{
					xil_printf("Error inicializando Timer");
					break;
				}
		default:
		{
			xil_printf("Error en ejecucion\n");
		}
	}
	return XST_FAILURE;
}

int IntcInitFunction(u16 DeviceId, XGpio *GpioIns)
{
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialization
	//**********************Initialize the GIC***************************************/
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&intc, IntcConfig, IntcConfig->CpuBaseAddress);
	/********************************************************************************/
	if(status != XST_SUCCESS) return status;

	// Call to interrupt setup
	XGpio_InterruptEnable(GpioIns, BTN_INT);
	XGpio_InterruptGlobalEnable(&btns);
	//Connect to the hardware
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
								(Xil_ExceptionHandler) XScuGic_InterruptHandler,
								&intc);
	/*********************************************************************************/
	Xil_ExceptionEnable();

	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&intc,
							 INTC_GPIO_INT_ID,
							 (Xil_ExceptionHandler)BTN_Intr_Handler,
							 (void *)GpioIns);
	if(status != XST_SUCCESS) return status;

	status = XScuGic_Connect(&intc,
							 INTC_ADDT_INT_ID,
							 (Xil_ExceptionHandler)AdderTreeReceiveHandler,
							 (void *) (&hls_ip));

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioIns, 1);
	XGpio_InterruptGlobalEnable(GpioIns);
	XEuchw_InterruptEnable(&hls_ip, 1);
	XEuchw_InterruptGlobalEnable(&hls_ip);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&intc, INTC_GPIO_INT_ID);
	XScuGic_Enable(&intc, INTC_ADDT_INT_ID);

	return XST_SUCCESS;
	/***********************************************************************************/
}

