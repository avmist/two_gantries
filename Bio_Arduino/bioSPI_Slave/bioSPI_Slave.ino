/* Bio SPI Slave Arduino 
 *
 * Controls print head for biological materials for RepRap printer.
 *
 * SPI pin numbers:
 * SCK   13  // Serial Clock.
 * MISO  12  // Master In Slave Out.
 * MOSI  11  // Master Out Slave In.
 * SS    10  // Slave Select
*/

//#include <avr.io>

// Pin Definitions
#define LED_PIN 8 
#define ACK 0x7E

// Useful functions
                                 /* Which pin selects BIOSPI as slave? */
//#define SLAVE_SELECT    {digitalWrite(SS, LOW); digitalWrite(MISO,OUTPUT)}
//#define SLAVE_DESELECT  digitalWrite(SS, HIGH)

// Instruction Set -- interface with Arduino  Not currently used
#define SPI_LED_ON    0b00000011                     /* turn on LED */
#define SPI_LED_OFF   0b00000010                    /* turn off LED */
#define SPI_TEST      0b00000111

#define SPI_RDSR      0b00000101            /* read status register */
#define SPI_WRSR      0b00000001           /* write status register */

int testVal = 10;

/* Define Pins */

void slaveInitSPI(void) {
  // Initialize SPI pins
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);
  // Enable SPI as slave
  //SPCR |= (1 << SPR1);      /* div 16, safer for breadboards */
  SPCR |= (1 << SPE);
}

/*
void slaveSelect() {
  //digitalWrite(SS, LOW); 
  digitalWrite(MISO, OUTPUT);
}

void slaveDeselect() {
  //digitalWrite(SS, HIGH); 
  digitalWrite(MISO, INPUT);
}
*/

uint8_t SPI_tradeByte(uint8_t data) {
  SPDR = data;                       /* SPI starts sending immediately */
  //loop_until_bit_is_set(SPSR, SPIF);                /* wait until done */
  while(!(SPSR & (1<<SPIF)))
    ;
  return (SPDR);
}
  
uint8_t SPI_readByte() {
  return SPI_tradeByte(ACK);
  //return(SPDR);
  //Serial.println("Begin bit trade, slave");
  //while(!(SPSR & (1<<SPIF)));
  //Serial.println("End bit trade, slave");
  //delay(10);
  //while(!(SPSR & (1<<SPIF)))
  //  ;
  //return (SPDR);
  
}

uint8_t SPI_writeByte(uint8_t data) {
  //SPDR = byte;
  return SPI_tradeByte(data);
}




void setup() {
  // Initialize serial for troubleshooting.
  Serial.begin(9600);
  // Initialize slave LED pin.
  pinMode(LED_PIN, OUTPUT);
  // Initialize SPI Slave.
  slaveInitSPI();
  Serial.println("Slave Initialized");
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
}


void loop() {
  
  // Poll SS and if it goes Low there is an incomming message to deal with

  // If SS goes low, there is an incomming transmission
    //SPDR = 0;
  if (!digitalRead(SS)) {

    //Serial.println("Slave Enabled");
    Serial.println("SPI SS triggered: ");


    
    // Get byte from SPI 
    //Serial.print("SPI_TEST: ");
    //Serial.println(SPI_TEST);
    //SPI_writeByte(SPI_TEST);
    uint8_t msg_byte = SPI_readByte();  
    Serial.println(msg_byte);
    //Serial.print("SPDR: ");
    //Serial.println(SPDR);
    
    msg_byte = SPI_readByte(); 
    Serial.println(msg_byte);
    msg_byte = SPI_readByte(); 
    Serial.println(msg_byte);


    msg_byte = SPI_writeByte(5);
    
    Serial.println("");
    
    // Note: play with this delay to determine how long it needs to be
    // delay(500); //works but is probably too slow
    // delay(50); //works
    delay(10); // This delay allows for the master to return SS to High before next loop
    
    // Acknowledge byte was recieved
    //SPI_writeByte(msg_byte);
    
    /*
    
    // Light LED if msg_byte valid command
    if (msg_byte == SPI_LED_ON){
      digitalWrite(LED_PIN, HIGH);
      Serial.println("On CMD");
      Serial.println("");
      //SPI_writeByte(msg_byte);
      SPI_writeByte(SPI_TEST);
    }
    else if (msg_byte == SPI_LED_OFF) {
      digitalWrite(LED_PIN, LOW);
      Serial.println("Off CMD");
      Serial.println("");
      //SPI_writeByte(msg_byte);
      SPI_writeByte(SPI_TEST);
    }
    else {
      Serial.println("No CMD");
      Serial.println("");
      //SPI_writeByte(255);
      SPI_writeByte(SPI_TEST);
    }
    
    */
  } // if(!digitalRead(SS))

  //delay(1500);

} // loop()
