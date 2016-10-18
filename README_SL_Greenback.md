Changes and Notes for Bioprinter Greenback (Barlin)
=======================================

By Steve Lammers

Notes and Stuff To Do
=====================
*  **Fix printer bed size, currently too small**
*  **ADVANCE in stepper.cpp, use this to define a new method for BIOADVANCE**

Printer Details
===============
 *  AO-100 (not AO-101)
 *  RAMPS 1.4 board
 *  Arduino MEGA 2560 micro
 *  See link at LulzBot for [details](http://download.lulzbot.com/AO-100) 

Configure Greenback for AO-100 printer
===================================
AP-100 Printer is based on the RAMPS1.4 board with an extruder, bed and fan power. Change the following code in Configuration.h 

`BASIC SETTINGS: select your board type, temperature sensor type, axis scaling, and endstop configuration`

    // The following define selects which electronics board you have.
    // Please choose the name from boards.h that matches your setup
    #ifndef MOTHERBOARD
    //#define MOTHERBOARD BOARD_ULTIMAKER
    #define MOTHERBOARD BOARD_RAMPS_13_EFB
    #endif
    
The name 'BOARD_RAMPS_13_EFB' found in the boards.h file.

Baud Rate
---------
    // This determines the communication speed of the printer
    #define BAUDRATE 115200 //250000

Thermal Settings
----------------
    #define TEMP_SENSOR_0 7
    #define TEMP_SENSOR_1 0
    #define TEMP_SENSOR_2 0
    #define TEMP_SENSOR_BED 7

Mechanical Settings
-------------------
Set the direction that the stepper motors move the gantry

    #define INVERT_X_DIR false    // for Mendel set to false, for Orca set to true
    #define INVERT_Y_DIR true    // for Mendel set to true, for Orca set to false
    #define INVERT_Z_DIR false     // for Mendel set to false, for Orca set to true
    #define INVERT_E0_DIR true   // for direct drive extruder v9 set to true, for geared extruder set to false
    #define INVERT_E1_DIR false    // for direct drive extruder v9 set to true, for geared extruder set to false
    #define INVERT_E2_DIR false   // for direct drive extruder v9 set to true, for geared extruder set to false

Define endstop inverting

    // The pullups are needed if you directly connect a mechanical endswitch between the signal and ground pins.
    const bool X_MIN_ENDSTOP_INVERTING = false; // set to true to invert the logic of the endstop.
    const bool Y_MIN_ENDSTOP_INVERTING = false; // set to true to invert the logic of the endstop.
    const bool Z_MIN_ENDSTOP_INVERTING = false; // set to true to invert the logic of the endstop.
    const bool X_MAX_ENDSTOP_INVERTING = false; // set to true to invert the logic of the endstop.
    const bool Y_MAX_ENDSTOP_INVERTING = false; // set to true to invert the logic of the endstop.
    const bool Z_MAX_ENDSTOP_INVERTING = false; // set to true to invert the logic of the endstop.

Define manual homing position

    // The position of the homing switches
    #define MANUAL_HOME_POSITIONS  // If defined, MANUAL_*_HOME_POS below will be used
    //#define BED_CENTER_AT_0_0  // If defined, the center of the bed is at (X=0, Y=0)


Endstop Config
--------------
    #define min_software_endstops true // If true, axis won't move to coordinates less than HOME_POS.
    #define max_software_endstops true  // If true, axis won't move to coordinates greater than the defined lengths below.
    
    // Travel limits after homing
    #define X_MAX_POS 200
    #define X_MIN_POS 0
    #define Y_MAX_POS 190
    #define Y_MIN_POS 0
    #define Z_MAX_POS 100
    #define Z_MIN_POS 0

Axis Scaling
------------
    #define DEFAULT_AXIS_STEPS_PER_UNIT   {44.7,44.7,1512,800}  // default steps per unit for Ultimaker
    #define DEFAULT_MAX_FEEDRATE          {800, 800, 10, 500}    // (mm/sec)
    #define DEFAULT_MAX_ACCELERATION      {9000,9000,100,10000}    // X, Y, Z, E maximum start speed for accelerated 
    

Flashing the Firmware
======================
 *  Use Arduino 1.0.1 or 1.0.5
 *  Use Arduino MEGA 2560 board
 *  Use correct Greenback version for the AO-100 printer
 *  Electronics board in the Configuration.h set to 33 (RAMPS 1.3 = 33 (Power outputs: Extruder, Bed, Fan))
 *  In Configuration.h: #define DEFAULT_AXIS_STEPS_PER_UNIT   {50.25,50.25,756.00,408.00}  // default steps per unit for ultimaker 
 *  May need to set X_MAX_LENGTH, etc for the size of the bed????
 *  Need to use current version of Greenback if we are going to use multiple extruders!!!!!!!!!!!!!!!!!
 
 
Pin Definitions for Test LED
============================
###In pinsBioprinter.h

E0 LED Pin is on Aux-4 pin 3: `#DEFINE E0_LED_PIN 32 // Digital Pin 32 on AUX-4` 

###In Greenback_main.cpp

Initialize Pins: `pinMode(E0_LED_PIN, OUTPUT);`

###In Stepper.cpp and Stepper.h

Define WRITE_E_LED() to toggle LED during extrusion

Configure for BioExtruders
=============================
###In Configuration.h
* define BIOEXTRUDERS 2 // Defines the number of bioextruders

Copy and alter Stepper.h and Stepper.cpp to BioValve.h and BioValve.cpp

In pinsBioprinter.h
`define BIOPRINT 1 // 1-enables bioprint, 0-enables filament`
Set BIOPRINT to 1 when a bioprint tool is selected, set it to 0 when a filament tool is selected

In Greenback_main.cpp: Define tools T0-T9 as filament extruders and T10-T19 as bio-print heads. When T0-T9 is selected set BIOPRINT = 0, when T10-T19 is selected set BIOPRINT to 1.

Use BIOPRINT to govern how extrusion is done in stepper.cpp and stepper.h. Since this contains the ISR which is used in all of the printing, keep using G1 E codes for printing. Change to bioprint heads using T codes, and flag what material is being printed (bio or plastic) using BIOPRINT.


plan_buffer_line()
===================
`void plan_buffer_line(const float &x, const float &y, const float &z, const float &e, float feed_rate, const uint8_t &extruder)`

target[]: rounded total steps in x,y,z,e-direction
`target[X_AXIS] = lround(x*axis_steps_per_unit[X_AXIS]);`

Prevent dangerous extrude code follows

define total absolute steps to move stepper motors for each axis i.e.:
`block->steps_x = labs(target[X_AXIS]-position[X_AXIS]);`

`block->steps_e` has some additional code, basically: `steps_e/(pi R^2)*100/100` where R is filament diameter, basically normalizes thin and thick filament feedrates. **Will need to add to this for bioprinter**

```
  block->steps_e = labs(target[E_AXIS]-position[E_AXIS]);
  block->steps_e *= volumetric_multiplier[active_extruder];
  block->steps_e *= extrudemultiply;
  block->steps_e /= 100;
```

step_event_count is the max step size for x,y,z,e directions 
`block->step_event_count = max(block->steps_x, max(block->steps_y, max(block->steps_z, block->steps_e)));`

Fan Speed `block->fan_speed = fanSpeed;`

Stuff for BAEICUDA

```
  #ifdef BARICUDA
  block->valve_pressure = ValvePressure;
  block->e_to_p_pressure = EtoPPressure;
  #endif
```
Compute direction bits for this block `block->direction_bits = 0;`, then shift in bits in the LSB for X,Y,Z,E direction 0 or 1

Set active extruder
`block->activer_extruder = extruder;`

Ennable active axes

```
  if(block->steps_x != 0) enable_x();
  if(block->steps_y != 0) enable_y();
```

Enable extruder(s)
This code ensures that only the active extruder is enabled if DISABLE_INACTIVE_EXTRUDER is defined (which it is by default). **Will need to add to this for bioprinters?**


Calculate the total length of the movement. For cartesian bots, the `X_AXIS` is the real X movement and same for `Y_AXIS`.

```
  float delta_mm[4];
  delta_mm[X_AXIS] = (target[X_AXIS]-position[X_AXIS])/axis_steps_per_unit[X_AXIS];
  delta_mm[Y_AXIS] = (target[Y_AXIS]-position[Y_AXIS])/axis_steps_per_unit[Y_AXIS];
  delta_mm[Z_AXIS] = (target[Z_AXIS]-position[Z_AXIS])/axis_steps_per_unit[Z_AXIS];
  delta_mm[E_AXIS] = ((target[E_AXIS]-position[E_AXIS])/axis_steps_per_unit[E_AXIS])*volumetric_multiplier[active_extruder]*extrudemultiply/100.0;
  if ( block->steps_x <=dropsegments && block->steps_y <=dropsegments && block->steps_z <=dropsegments )
  {
    block->millimeters = fabs(delta_mm[E_AXIS]);
  } 
  else
  {
    block->millimeters = sqrt(square(delta_mm[X_AXIS]) + square(delta_mm[Y_AXIS]) + square(delta_mm[Z_AXIS]));
  {
  float inverse_millimeters = 1.0/block->millimeters;  // Inverse millimeters to remove multiple divides 

    // Calculate speed in mm/second for each axis. No divide by zero due to previous checks.
  float inverse_second = feed_rate * inverse_millimeters;

  int moves_queued=(block_buffer_head-block_buffer_tail + BLOCK_BUFFER_SIZE) & (BLOCK_BUFFER_SIZE - 1);
```

In configuration.h, SLOWDOWN defined as default. inverse_second is redeined to slow down when the look ahead
buffer is only half full

Calculate the speedin mm/sec and the step-rate in step/sec

```
block->nominal_speed = block->millimeters * inverse_second; // (mm/sec) Always > 0
block->nominal_rate = ceil(block->step_event_count * inverse_second); // (step/sec) Always > 0
```

Calculate `current_speed` in mm/sec for each axis and `speed_factor <=1` for the minimum of either 1 or max_feedrate/current_speed

XY_FREQUENCY_LIMIT not defined by default

Speed is corrected using speed_factor

```
  // Correct the speed  
  if( speed_factor < 1.0)
  {
    for(unsigned char i=0; i < 4; i++)
    {
      current_speed[i] *= speed_factor;
    }
    block->nominal_speed *= speed_factor;
    block->nominal_rate *= speed_factor;
  }
```

Calculate acceleration and save to block

```
block->acceleration_st
block->acceleration
block->acceleration_rate
```

Correct for jerking due to high acceleration

```
block->max_entry_speed
block->entry_speed
block->normal_length_flag
block->recalculate_flag
```

Calculate trapezoid for the block, move buffer head, update position and wake up the stepper motors

```
calculate_trapezoid_for_block(block, block->entry_speed/block->nominal_speed,
safe_speed/block->nominal_speed);

// Move buffer head
block_buffer_head = next_buffer_head;

// Update position
memcpy(position, target, sizeof(target)); // position[] = target[]

planner_recalculate();

st_wake_up();
```
st_wake_up called in stepper.cpp
***END PLAN BUFFER LINE***


Timers
======
Timers all have something in Greenback_main.cpp
* Timer0 ????? Pins 1 and 26
* Timer1 - stepper.cpp
* Timer2 ????? Pins 16 and 23 are open on AUX-4
* Timer3 - servo.h
* Timer4 - servo.h
* Timer5 - servo.h

Extruder Offsets
================
Need to figure out how to have the firmware apply offsets for each of the bio-extruders.

```
// Offset of the extruders (uncomment if using more than one and relying on firmware to position when changing).
// The offset has to be X=0, Y=0 for the extruder 0 hotend (default extruder).
// For the other hotends it is their distance from the extruder 0 hotend.
#define EXTRUDER_OFFSET_X {0.0, 10.00} // (in mm) for each extruder, offset of the hotend on the X axis
#define EXTRUDER_OFFSET_Y {0.0, 5.00}  // (in mm) for each extruder, offset of the hotend on the Y axis
```

In Greenback_main.cpp

```
// Extruder offset
#if EXTRUDERS > 1 || defined(BIOEXTRUDERS) //SL Note: Added BIOEXTRUDERS to account for offset
#ifndef DUAL_X_CARRIAGE
  #define NUM_EXTRUDER_OFFSETS 2 // only in XY plane
#else
  #define NUM_EXTRUDER_OFFSETS 3 // supports offsets in XYZ plane
#endif
//float extruder_offset[NUM_EXTRUDER_OFFSETS][EXTRUDERS] = {
float extruder_offset[NUM_EXTRUDER_OFFSETS][10+BIOEXTRUDERS] = { //SL Note: Added 10 to account for Filament extruders 0-9
#if defined(EXTRUDER_OFFSET_X) && defined(EXTRUDER_OFFSET_Y)
  EXTRUDER_OFFSET_X, EXTRUDER_OFFSET_Y 
#endif
};
```

So `extruder_offset` is a 3xn array where n = 10+BIOEXTRUDERS. BIOEXTRUDERS is the number of bioextruders, 10+ is the number of possible filament printheads. Of course this is much higher than the actual number the RepRap could support, but it was chosen such the T10 would be the first bioprint head. So to make this work, fill in the `EXTRUDER_OFFSET_X` and `EXTRUDER_OFFSET_Y` arrays in Configuration.h

```
#define EXTRUDER_OFFSET_X {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 10.0} // (in mm) for each extruder, offset of the hotend on the X axis
#define EXTRUDER_OFFSET_Y {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.00}  // (in mm) for each extruder, offset of the hotend on the Y axis
```

Now when the tool is changed, say to bioprinthead[0] using T10 command, the `current_position` array is updated to reflect the new printhead. So if `EXTRUDER_OFFSET_X[10]=10` and `current_position[0]=100` before the tool was changed, after issuing T10 command `current_position=110`, because `EXTRUDER_OFFSET_X[10]=10`. However, the printer stage does not move, the position will be reflected at the next move command. If you want to change printheads and begin printing at the same point with the new printhead, you will need to resend the position command using G1.

Removal of Existing Extruder
=============================
The existing extruder and part heating pad needs to be removed. When the extruder heater is disconnected, the printer throws an error "Printer stopper due to errors. Fix the error and use M999 to reset. (Temperature is reset. Set it after restatring)". This happened because the Stop() function was called. To correct this, use an existing override by adding the following to pinsBioprinter.h

'''
// This definition is used to turn off the physical checks on thermister
// readings, typically set in pins.h. T0 and T1 thermistors provided temperature data for
// the extruder and heated bed respectively. These thermistors are disconnected
// for the bioprinter. So we can ignore the associated errors using this definition
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE
// Temperature errors persist, set TEMP_SENSOR_0 = 0 AND TEMP_SENSOR_BED = 0 in Configuration.h
'''

Temperature errors persist. Set in Configuration.h

'''
#define TEMP_SENSOR_0 0 // 7 Setting for Lulzbot AO-100
#define TEMP_SENSOR_1 0
#define TEMP_SENSOR_2 0
#define TEMP_SENSOR_BED 0 // 7 Setting for Lulzbot AO-100
'''


