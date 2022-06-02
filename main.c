//Parqueo proyecto
//Katherine Rac

#include <stdint.h>//
#include <stdbool.h>//
#include "inc/tm4c123gh6pm.h"//
#include "inc/hw_memmap.h"//
#include "inc/hw_types.h"//
#include "inc/hw_ints.h"//
#include "inc/hw_gpio.h"//
#include "driverlib/uart.h"//
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"//
#include "driverlib/interrupt.h"//
#include "driverlib/gpio.h"//
#include "driverlib/timer.h"//
#include "driverlib/pin_map.h"//


uint8_t PSH1;
uint8_t PSH2;
uint8_t PSH3;
uint8_t PSH4;


void setup(void);
void Timer0IntHandler(void);

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    UARTCharPutNonBlocking(UART1_BASE, PSH1 + 48);
    UARTCharPutNonBlocking(UART1_BASE, PSH2 + 48);
    UARTCharPutNonBlocking(UART1_BASE, PSH3 + 48);
    UARTCharPutNonBlocking(UART1_BASE, PSH4 + 48);
    UARTCharPutNonBlocking(UART1_BASE, 10);

}

/**
 * main.c
 */
int main(void)
{
    setup();
    while(1){
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2) == 0){
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 32);
            PSH1 = 1;
        }
        else{
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);
            PSH1 = 0;
        }
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3) == 0){
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 2);
            PSH2 = 1;
        }
        else{
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 1);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
            PSH2 = 0;
        }
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1) == 0){
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 8);
            PSH3 = 1;
        }
        else{
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 4);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
            PSH3 = 0;
        }
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0){
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 4);
            PSH4 = 1;
        }
        else{
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 2);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
            PSH4 = 0;
        }
    }
}


void setup(void){

    //Clock 40MHz
    SysCtlClockSet(SYSCTL_OSC_MAIN| SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_5);
    //PUSH-LEDS
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);        //ACTIVAR PUERTO A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);        //ACTIVAR PUERTO B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);        //ACTIVAR PUERTO C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);        //ACTIVAR PUERTO D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);        //ACTIVAR PUERTO E
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        //ACTIVAR PUERTO F

    //Salidas leds
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1| GPIO_PIN_2  |GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_2 | GPIO_PIN_3);
    //Salidas display
    //GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6);                                       //display7seg pines de salida
    //GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7 | GPIO_PIN_6);                          //display7seg pines de salida
    //GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 |GPIO_PIN_3 | GPIO_PIN_4);              //display7seg pines de salida
    //GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_7 | GPIO_PIN_6);                          //display7seg pines de salida

    //Entrada
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_STRENGTH_2MA);

    //CONFIG TMR0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ((SysCtlClockGet())/200) - 1);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    IntEnable(INT_TIMER0A);
    TimerEnable(TIMER0_BASE, TIMER_A);

    //CONFIG UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)){}
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1)){}
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTEnable(UART1_BASE);

    IntMasterEnable();//GENERAL INTERRUPTS
}
