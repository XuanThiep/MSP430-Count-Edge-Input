#include "driverlib.h"
#include "mymsp430.h"

volatile uint16_t count_value=0;

/* In this example Timer A0 count edge from P1.0 (TA0CLK)
 * If count value = 9 + one pulse => Timer A0 generate interrupt. In
 * Timer 0 Interrupt Service Rountie blink led */

void Init_Timer_Count_Edge(void);

void main( void )
{
    /* Stop watchdog timer */
    WDT_A_hold(WDT_A_BASE);
    
    Clk_Using_DCO_Init(16000,8000,SMCLK_CLOCK_DIVIDER_2);

    /* Init gpio for led */
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN4);

    Init_Timer_Count_Edge();

    /* Enable global interrupt */
    __enable_interrupt();

    while(1)
    {
    	count_value = Timer_A_getCounterValue(TIMER_A0_BASE);
    	__no_operation();
    }
}

void Init_Timer_Count_Edge(void)
{
	Timer_A_initUpModeParam para;

	/* Init P1.0 as TA0 clock signal TA0CLK input */
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,GPIO_PIN0);

	/* Clear interrupt source */
	Timer_A_clearTimerInterrupt(TIMER_A0_BASE);

	para.clockSource = TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK; //Rising edge capture
	para.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
	para.timerPeriod = 9;
	para.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
	para.timerClear = TIMER_A_DO_CLEAR;
	para.startTimer = true;

	Timer_A_initUpMode(TIMER_A0_BASE,&para);
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_ISR(void)
{
	GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN4);
	Timer_B_clearTimerInterrupt(TIMER_A0_BASE);
}
