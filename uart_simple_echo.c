//------------------------------------------------------------------------------
// uart_simple.c
//
// test de l'UART : écho des caractères reçus
//------------------------------------------------------------------------------
#include <msp430g2553.h>

void InitUART(void)
{
    P1SEL |= (BIT1 | BIT2);                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= (BIT1 | BIT2);                    // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 = UCSWRST;                         // SOFTWARE RESET
    UCA0CTL1 |= UCSSEL_3;                       // SMCLK (2 - 3)

    UCA0CTL0 &= ~(UCPEN | UCMSB | UCDORM);
    UCA0CTL0 &= ~(UC7BIT | UCSPB | UCMODE_3 | UCSYNC); // dta:8 stop:1 usci_mode3uartmode
    UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**

    UCA0BR0 = 104;                     			// 1MHz, OSC16, 9600 (8Mhz : 52) : 8/115k
    UCA0BR1 = 0;                                // 1MHz, OSC16, 9600
    UCA0MCTL = 10;

    /* Enable USCI_A0 RX interrupt */
    IE2 |= UCA0RXIE;
}

void TXdata( unsigned char c )
{
    while (!(IFG2 & UCA0TXIFG));  // USCI_A0 TX buffer ready?
    UCA0TXBUF = c;              // TX -> RXed character
}

void main(void)
{
	// Stop watchdog timer to prevent time out reset
	WDTCTL = WDTPW | WDTHOLD;

	if(CALBC1_1MHZ==0xFF || CALDCO_1MHZ==0xFF)
	{
		__bis_SR_register(LPM4_bits); // Low Power Mode #trap on Error
	}
	else
	{
		// Factory parameters
		BCSCTL1 = CALBC1_1MHZ;
		DCOCTL = CALDCO_1MHZ;
	}
	
    InitUART();

    __bis_SR_register(GIE); // interrupts enabled

    TXdata('>');
    while(1);
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    unsigned char c;

    c = UCA0RXBUF;
    TXdata(c);
}

