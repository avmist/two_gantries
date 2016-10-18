#include "bioSPI.h"


// Variables
bool LED_latch_FLAG = false; // 0-not latched, 1-latched


void initBioPins(void) {
  // set pin modes

  pinMode(CHIP_SELECT_PIN, OUTPUT);      // Set Chip Select to Output
  SLAVE_DESELECT;                 
  pinMode(SPI_MISO_PIN, INPUT); 
  pinMode(SPI_MOSI_PIN, OUTPUT);
  pinMode(SPI_SCK_PIN, OUTPUT);
  pinMode(SPI_SS_PIN, OUTPUT); // Why does this pin need to be set to output when it appears not to be used?

  /* Set MOSI and SCK output, all others input */
  //DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
  /* Enable SPI, Master, set clock rate fck/16 */
  //SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);
}

void initBioSPI(void) {
  /* Don't have to set phase, polarity b/c
   * default works with Arduino Uno */
  SPCR |= (1 << SPR1);                /* div 16, safer for breadboards */
 // SPCR |= (1 << SPR0);                /* div 16, safer for breadboards */
  SPCR |= (1 << MSTR);                                  /* clockmaster */
  SPCR |= (1 << SPE);                                        /* enable */
}


uint8_t bioSPI_tradeByte(uint8_t tData) {
  SPDR = tData;                       /* SPI starts sending immediately */
  //loop_until_bit_is_set(SPSR, SPIF);                /* wait until done */
  while(!(SPSR & (1<<SPIF)))
    ;
                                /* SPDR now contains the received byte */
  return(SPDR);
}

uint8_t bioSPI_readByte(void) {
  uint8_t tmp_rx = bioSPI_tradeByte(0);
  return (tmp_rx);
}

uint8_t bioSPI_writeByte(uint8_t wData) {
  uint8_t ack_rx = bioSPI_tradeByte(wData);
  return (ack_rx);
  //junk_rx = bioSPI_tradeByte(0);
  /* Start transmission */
  //SPDR = byte;
  //SERIAL_ECHOLN("In bioSPI_writeByte");
  /* Wait for transmission complete */ 
  //while(!(SPSR & (1<<SPIF)))
  //  ;
}



void bioSPI_turnOnLED() {
    SLAVE_SELECT;
    delay(30);

    uint8_t tmp_ack = 0;
    do {
      // Send serial byte to SPI slave
      tmp_ack = bioSPI_writeByte(SPI_LED_ON);
      //SERIAL_ECHO("ACK Recieved: ");
      //SERIAL_ECHOLN((int)tmp_ack);
    } while (!(tmp_ack == ACK_SPI));

    delay(30);
    SLAVE_DESELECT;

}

void bioSPI_turnOffLED() {
    SLAVE_SELECT;
    delay(30); // 50 works well Note: play with this delay to see how low it can be

    uint8_t tmp_ack = 0;
    do {
      // Send serial byte to SPI slave
      tmp_ack = bioSPI_writeByte(SPI_LED_OFF);
      //SERIAL_ECHO("ACK Recieved: ");
      //SERIAL_ECHOLN((int)tmp_ack);
    } while (!(tmp_ack == ACK_SPI));

    delay(30); // Delay to allow the transmissions to complete before deselecting the slave device 
    SLAVE_DESELECT;

    //SERIAL_ECHO("** SPI LED OFF ** ");

    //Unlatch LED_latch_FLAG
    LED_latch_FLAG = false;
    
}


void bioSPI_latchOnLED() {
    // If latch flag is not set, latch on
    if (!LED_latch_FLAG) {
      LED_latch_FLAG = true;

      SLAVE_SELECT;
      delay(30);

      uint8_t tmp_ack = 0;
      do {
        // Send serial byte to SPI slave
        tmp_ack = bioSPI_writeByte(SPI_LED_ON);
        //SERIAL_ECHO("ACK Recieved: ");
        //SERIAL_ECHOLN((int)tmp_ack);
      } while (!(tmp_ack == ACK_SPI));

      delay(30);
      SLAVE_DESELECT;

    }
    // else just leave it latched, unlatch with bioSPI_turnOffLED()

}
