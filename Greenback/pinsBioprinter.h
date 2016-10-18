#ifndef pinsBioprinter_h
#define pinsBioprinter_h

#ifdef BIOEXTRUDERS
extern bool g_bioprint_flag;  // 1-enables, 0-disables bioprinting

#define BIOPRINT_ON  = 1; // Turn bioprint on
#define BIOPRINT_OFF = 0; // Turn bioprint off

#define B0_PIN         32 // Digital Pin 32 on AUX-4 pin 3
                          // pinMode set in Greenback_main.cpp
//#define B1_PIN       47 // Digital pin 47 on AUX-4 pin 4
#define WRITE_E_BIO(v) WRITE(B0_PIN, v)

// This definition is used to turn off the physical checks on thermister
// readings, typically set in pins.h. T0 and T1 thermistors provided temperature data for
// the extruder and heated bed respectively. These thermistors are disconnected
// for the bioprinter. So we can ignore the associated errors using this definition
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE
// Temperature errors persist, set TEMP_SENSOR_0 = 0 AND TEMP_SENSOR_BED = 0 in Configuration.h

#endif //BIOEXTRUDERS
#endif
