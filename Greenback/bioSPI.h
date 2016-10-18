/* Arduino bioSPI
 * 2015 by Steven Lammers
 *
 * This file is part of the Arduino bioSPI Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino bioSPI Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "Greenback.h"
#include "bioSPI_pins.h"
#include <avr/io.h> //for loop_until_bit_is_set

#ifdef BIOSUPPORT // bio-printing enabled

#ifndef bioSPI_h
#define bioSPI_h

                                 /* Which pin selects BIOSPI as slave? */
#define SLAVE_SELECT    digitalWrite(CHIP_SELECT_PIN, LOW)
#define SLAVE_DESELECT  digitalWrite(CHIP_SELECT_PIN, HIGH)

// Instruction Set -- interface with Arduino  Not currently used
#define ACK_SPI          0b01111110
#define SPI_LED_ON       0b00000011                     /* read memory */
#define SPI_LED_OFF      0b00000010                 /* write to memory */

#define BIOSPI_RDSR      0b00000101            /* read status register */
#define BIOSPI_WRSR      0b00000001           /* write status register */
/*
//------------------------------------------------------------------------------
// SPI speed is F_CPU/2^(1 + index), 0 <= index <= 6
//  Set SCK to max rate of F_CPU/2. See Sd2Card::setSckRate(). 
uint8_t const SPI_FULL_SPEED = 0;
//  Set SCK rate to F_CPU/4. See Sd2Card::setSckRate().
uint8_t const SPI_HALF_SPEED = 1;
//  Set SCK rate to F_CPU/8. See Sd2Card::setSckRate(). 
uint8_t const SPI_QUARTER_SPEED = 2;
//  Set SCK rate to F_CPU/16. See Sd2Card::setSckRate(). 
uint8_t const SPI_EIGHTH_SPEED = 3;
//  Set SCK rate to F_CPU/32. See Sd2Card::setSckRate(). 
uint8_t const SPI_SIXTEENTH_SPEED = 4;
//------------------------------------------------------------------------------
*/

// Functions
void initBioPins(void);

                  /* Init SPI to run bioSPI with phase, polarity = 0,0 */
void initBioSPI(void);

                  /* Generic.  Just loads up HW SPI register and waits */
uint8_t bioSPI_tradeByte(uint8_t byte);

                           /* gets a byte from a given memory location */
uint8_t bioSPI_readByte();

                           /* writes a byte to a given memory location */
uint8_t bioSPI_writeByte(uint8_t byte);

                                                     /* turn on an LED */
void bioSPI_turnOnLED();

                                                    /* turn off an LED */
void bioSPI_turnOffLED();

                                                    /* latch on an LED */
void bioSPI_latchOnLED();

#endif //bioSPI
#endif //BIOSUPPORT

