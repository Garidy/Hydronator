#include <msp430.h> 

int i;
char data;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer

     // Setup I2C ports
    UCB0CTLW0 |= UCSWRST;               // put into software reset

    UCB0CTLW0 |= UCMODE_3;              // put into I2C mode
    UCB0CTLW0 |= UCMST;                 // put into Master mode
    UCB0CTLW0 |= UCTR;                  // put into Tx/WRITE mode

    /**
     * UCB0I2COA0 = 0x01;     //SLAVE ADDRESS
     * UCB0CTLW0 &= ~UCMST;   //slave MODE
     * UCB0CTLW0 &= ~UCTR;      //RECEIVE MODE
     * UCB0CTLW1 &= ~UCSWACK;  // auto acknowledge
     * UCB0TBCNT = 0x01;      // Length of Receiving data
     */

    //-- setup ports
    P1SEL1 &= ~BIT3;   //P1.3 SCL (48)
    P1SEL0 |= BIT3;

    P1SEL1 &= ~BIT2;   //P1.2 SDA (1)
    P1SEL0 |= BIT2;


    UCB0CTLW0 &= ~UCSWRST;               // put out of software reset
    PM5CTL0 &= ~LOCKLPM5;                // Turn on I/O

    UCB0IE |= UCTXIE0;                   // Enable I2C TX0 IRQ (transmit reg is ready)
    UCB0IE |= UCRXIE0;                   // Enable I2C RX0 IRQ (receive reg is ready)
    __enable_interrupt();                // global enable

    while(1){

    }
	return 0;
}


// ISRs -------------------------------------------------------------
/**
 * I2C Interrupt Service Routine
 */
#pragma vector=EUSCI_B0_VECTOR
__interrupt void EUSCI_B0_I2C_ISR(void) {

    switch(UCB0IV) {
        case 0x16:                                              // ID16 = RXIFG0 -> Receive
            data = UCB0RXBUF;
            break;

        case 0x18:                                              // ID18 = TXIFG0 -> Transmit
            UCB0TXBUF = sendChar;                           // send current sendData char
            break;

        default:
            break;
      }

}

/** Slave I2C ISR
 * __interrupt void EUSCI_B0_I2C_ISR(void)
{
    UCB0IE &= ~UCRXIE0;
    if(UCB0RXBUF != '\0') {
        data = UCB0RXBUF;
    }
    UCB0IE |= UCRXIE0;
}
 **/
