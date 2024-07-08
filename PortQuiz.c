#include <msp430.h>
#include "msp430g2553.h"
int sw2=0;
int count_sema=1;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   //stop watchdog timer
    P1DIR  = 0x00;                              //port 1 all inputs
    P1DIR |= (BIT0 | BIT6);                     //set P1.0 and P1.6 as outputs (LED1, LED2)
    P1REN |= BIT3;                              //activate resister on P1.3
    P1OUT |= BIT3;                              //make it pull up because SW2 is active low

    for (;;)
    {
      sw2 = P1IN;                               //read values from P1
      sw2 &= BIT3;                              //mask out only BIT3 where SW2 is connected
      if (sw2 == BIT3)                          //Switch open, value high
         {  
	    if (count_sema == 1) {
		P1OUT &= ~BIT6;                     //turn LED2 off
		for (;;) {			    //loop over 0.25 seconds
                P1OUT ^= BIT0;                      //toggle LED1
		__delay_cycles(125000)
		P1OUT &= ~BIT0;                      //turn LED1 off
		__delay_cycles(125000)
		}
	    } 
	    if (count_sema == 2) {
		P1OUT &= ~BIT0;                     //turn LED1 off
		for (;;) {			    //loop over 0.25 seconds
                P1OUT ^= BIT6;                      //toggle LED2
		__delay_cycles(125000)
		P1OUT &= ~BIT6;                      //turn LED2 off
		__delay_cycles(125000)
		}

	    }
	    if (count_sema == 3) {
		for (;;) {			    //loop over 0.25 seconds
                P1OUT ^= BIT0;                      //toggle LED1
                P1OUT ^= BIT6;                      //toggle LED2
		__delay_cycles(125000)
		P1OUT &= ~BIT0;                      //turn LED1 off
		P1OUT &= ~BIT6;                     //turn LED2 off
		__delay_cycles(125000)
		}
	    }
	    if (count_sema == 4) {
		for (;;) {			    //loop over 0.25 seconds
 		P1OUT &= ~BIT0;                      //turn LED1 off
                P1OUT ^= BIT6;                      //toggle LED2
		__delay_cycles(125000)
                P1OUT ^= BIT0;                      //toggle LED1
		P1OUT &= ~BIT6;                     //turn LED2 off
		__delay_cycles(125000)
		}
	    }
            __delay_cycles(50000);              //delay 50,000 micro seconds
         }
      else                                      //Switch closed, value low
         {  
	   count_sema++;			//increment semaphore
	   if (count_sema > 4) {count_sema = 1}  //cycles sema over 4 values
           __delay_cycles(50000);                //delay 200,000 micro seconds
        }
    }  // end of for infinite loop
} //end of main