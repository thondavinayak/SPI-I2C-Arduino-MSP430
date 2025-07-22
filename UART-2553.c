//------------------------------------------------------------------------------
// uart_simple_2.c
//
// test de l'UART : echo des caractères reçus
//                  (réception et envoi par scrutation)
//
// Launchpad 1.5 / 2553 debug model
//------------------------------------------------------------------------------
#include <msp430g2553.h>
#include <string.h>

#define CMDLEN  12               // longueur maximum de la commande utilisateur
#define LF      0x0A
#define CR      0x0D


void Init_IO( void )
{
   P1DIR |= BIT0 | BIT6;  // port 1.0  en sortie
   P1OUT &= ~(BIT0 | BIT6);  // force etat bas P1.0 - LED1
   P1REN |= BIT6;
}

void InitUART(void)
{
    P1SEL |= (BIT1 + BIT2);                 // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= (BIT1 + BIT2);                // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;                   // SMCLK
    UCA0BR0 = 104;                          // 1MHz, 9600
    UCA0BR1 = 0;                            // 1MHz, 9600
    UCA0CTL0 &= ~UCPEN & ~UCPAR & ~UCMSB;
    UCA0CTL0 &= ~UC7BIT & ~UCSPB & ~UCMODE1;
    UCA0CTL0 &= ~UCMODE0 & ~UCSYNC;
    UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
}

void RXdata(unsigned char *c)
{
    while (!(IFG2&UCA0RXIFG));              // buffer Rx USCI_A0 plein ?
    *c = UCA0RXBUF;
}

void TXdata( unsigned char c )
{
    while (!(IFG2&UCA0TXIFG));              // buffer Tx USCI_A0 vide ?
    UCA0TXBUF = c;
}

void Send_STR_UART(const char *msg)
{
    int i = 0;
    for(i=0 ; msg[i] != 0x00 ; i++)
    {
        TXdata(msg[i]);
    }
}

void command( char *cmd )
{
  if(strcmp(cmd, "h") == 0)          // aide
  {
    P1OUT |= BIT6;
    Send_STR_UART("\r\nAide : commandes existantes :\n");
    Send_STR_UART("\r\n'ver' : version\n");
    Send_STR_UART("\r\n'l' : allumage led\n");
    Send_STR_UART("\r\n'e' : extinction led\n");
    Send_STR_UART("\r\n'h' : affichage de cette aide\n");
  }
  else if (strcmp(cmd, "ver") == 0)     // version
  {
    Send_STR_UART("\rDebug MSP430 v1.0\n");
  }
  else if(strcmp(cmd, "l") == 0)     // allumage led rouge
  {
    P1OUT |= BIT0;
    Send_STR_UART("\rAllumage led rouge\n");
  }
  else if(strcmp(cmd, "e") == 0)     // extinction led rouge
  {
    P1OUT &= ~BIT0;
    Send_STR_UART("\rExtinction led rouge\n");
  }
  else
  {
    Send_STR_UART("\rMauvaise commande ");
    Send_STR_UART(cmd);
    Send_STR_UART("\rEntrez 'h' pour l'aide\n");
  }
}

void main(void)
{
  unsigned char c;
  char  cmd[CMDLEN];      // tableau de caractere lie a la commande user
  int   nb_car;           // compteur nombre carateres saisis

    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT
    // clock calibration verification
    if(CALBC1_1MHZ==0xFF || CALDCO_1MHZ==0xFF)
      __low_power_mode_4();
    // factory calibration parameters
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    Init_IO();
    InitUART();

    nb_car = 0;
    Send_STR_UART("MSP430 Ready !");
    while(1)
    {
      if( nb_car<(CMDLEN-1) )
      {
        RXdata(&c);
        if( (cmd[nb_car]=c) != CR )
        {
        TXdata(c);
        nb_car++;
        }
        else
        {
          cmd[nb_car]=0x00;
          command(cmd);
          nb_car=0;
        }
      }
    }
}


