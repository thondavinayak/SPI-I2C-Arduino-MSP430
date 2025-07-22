#include <msp430.h>
#include "Afficheur.h"

#define SS BIT4   // Slave Select (SS) on P1.4
#define SCK BIT5  // Clock (SCK) on P1.5
#define MISO BIT6 // MISO on P1.6
#define MOSI BIT7 // MOSI on P1.7

volatile unsigned char c = 'A'; // Character to send

volatile unsigned char byteReceived = 0x00;


//Init_USI_SPI();
//Send_char_SPI();

void Init_USI_SPI1(void) {
    P1DIR |= SCK + MOSI + SS;    // Set SCK, MOSI, and SS as outputs
    P1DIR &= ~MISO;               // Set MISO as input
    P1SEL |= SCK + MOSI + MISO;   // Set SCK, MOSI, and MISO to USI functionality

    UCB0CTL1 = UCSWRST;
    P1DIR |= BIT3; // CS of arduino
    P1OUT |= BIT3;

    //P1DIR |= BIT4; // CS of display
    //P1OUT &= ~BIT4;
    //P1OUT |= BIT4;

    P1SEL |= BIT7 + BIT5;  // Enable SIMO + SCLK
    P1SEL2 |= BIT7 + BIT5; // Enable SIMO + SCLK
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2; // SMCLK
    UCB0CTL1 &= ~UCSWRST;
}

void Send_char_SPI1(unsigned char c) {
    P1OUT &= ~BIT3;  // CS high for arduino
    UCB0TXBUF = c;   // Put character in transmit buffer
    while (!(IFG2 & UCB0TXIFG)); // USCI_A0 TX buffer ready?
    P1OUT |= BIT3;   // CS high for display
}


void Receive_char_SPI1(unsigned char c) {
    if (IFG2 & UCB0RXIFG) {  // Check if data is available in RX buffer
        byteReceived = UCB0RXBUF;  // Read received byte from RX buffer
    }
}


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;    // Stop the watchdog timer

    P1DIR |= BIT0;               // Set P1.0 as output for debugging (optional)

    Init_USI_SPI1();              // Initialize SPI

    Aff_Init();

    __delay_cycles(1000000);      // Wait for any initial setup delay (optional)

    while (1) {
        P1OUT ^= BIT0;            // Toggle P1.0 for debugging

        Send_char_SPI1(c);         // Send the character via SPI

        __delay_cycles(100000);    // Small delay

        //c = c + 0x01;

        Receive_char_SPI1(byteReceived);

        if(byteReceived != 'A') {
            Aff_valeur(convert_Hex_Dec((int)byteReceived));
        }
    }
}
