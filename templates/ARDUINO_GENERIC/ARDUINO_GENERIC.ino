#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "KD_ardu_button.h"

/* Hardware configuration */

// HW
#define BUTTON_DEBOUNCE 20                          // button debounce (ms)

// ADC
#define AUX_VREF 2.56
#define ADC_CHANNELS 1024

// I2C
#define I2C_ADDR 0x10
#define REG_HIGH_BYTE 0x00
#define REG_LOW_BYTE 0xA3

/* Define debugging level */

#define DEBUG_LEVEL 5
#define POST_INTERVAL 5000
#define SERIAL_PLOTTER 0
#define POLL_DELAY 50                               // Serial plotter poll delay

/* Serial port */

#define SERIAL_BAUD_RATE 57600                      // 57600 max with internal 8 MHz oscillator
#define READ_BUFFER_SIZE 32

/***********************************************************************************************************************/
/*                                                  pin definitions                                                    */
/***********************************************************************************************************************/

/* Arduino digital */
const int digi1 = 1;                                //
const int debugLED = 13;                            //

/* Arduino analog */
const int analog0 = 14;                             //

/* MCP23017 */

// GPIO A:
const unsigned char mcp_A0 = 0;
const unsigned char mcp_A1 = 1;
const unsigned char mcp_A2 = 2;
const unsigned char mcp_A3 = 3;
const unsigned char mcp_A4 = 4;
const unsigned char mcp_A5 = 5;
const unsigned char mcp_A6 = 6;
const unsigned char mcp_A7 = 7;

// GPIO B:
const unsigned char mcp_B0 = 8;
const unsigned char mcp_B1 = 9;
const unsigned char mcp_B2 = 10;
const unsigned char mcp_B3 = 11;
const unsigned char mcp_B4 = 12;
const unsigned char mcp_B5 = 13;
const unsigned char mcp_B6 = 14;
const unsigned char mcp_B7 = 15;

/***********************************************************************************************************************/
/*                                                   VARIABLES                                                         */
/***********************************************************************************************************************/

/* Other variables */

Adafruit_MCP23017 mcp;

// pin, pullup, inverted
Button button1(digi1, LOW, HIGH);

// Debugging
unsigned long _last_post_time = 0;
byte cycle_count = 0;

// enum type
enum machine_state_list {
  state1,
  state2,
  fault,
  unknown
};

machine_state_list machine_state = unknown;

/***********************************************************************************************************************/
/*                                                   FUNCTIONS                                                         */
/***********************************************************************************************************************/

void machine_state1();

/* Analog in */
float read_voltage();

/* Functions */
byte I2C_command(byte _command);

/***********************************************************************************************************************/
/*                                                   SETUP LOOP                                                        */
/***********************************************************************************************************************/

void setup() {

  // ADC
  analogReference(EXTERNAL);

  /* Pin settings on Arduino */

  // LEDs
  pinMode(debugLED, OUTPUT);
  digitalWrite(debugLED, LOW);

  /* Pin settings on MCP23017 */

  // I2C
  Wire.begin();
  Wire.setClock(100000);

  
  /* Pin settings on MCP23017 */

  // I2C
  Wire.begin();
  Wire.setClock(100000);

  // MCP23017
  mcp.begin();

  // GPIO A:
  mcp.pinMode(mcp_A0, OUTPUT);
  mcp.digitalWrite(mcp_A0, LOW);
  mcp.pullUp(mcp_A0, LOW);

  // GPIO B:
  mcp.pinMode(mcp_B0, OUTPUT);
  mcp.digitalWrite(mcp_B0, LOW);

  /* Serial communications setup */
  
  Serial.begin(SERIAL_BAUD_RATE);
  while(!Serial){
    ;                                               // Needed for Atmega32U4
  }  

#if DEBUG_LEVEL > 1
  Serial.println(F("Serial port opened"));
  Serial.print(F("Serial baud rate: "));
  Serial.print(SERIAL_BAUD_RATE);
  Serial.println(F(" bit/s"));
#endif

  /* Setup complete */

  // Delay with blinking
  for(int i = 0; i < 5; i++) {
    digitalWrite(debugLED, HIGH);
    delay(100);
    digitalWrite(debugLED, LOW);
    delay(100);
  }

#if DEBUG_LEVEL > 1
  Serial.println(F("Setup complete"));
#endif
  
}

/***********************************************************************************************************************/
/*                                                   MAIN LOOP                                                         */
/***********************************************************************************************************************/

void loop() { 

  // State machine
  machine_state = state1;
  
  switch(machine_state) {
    case state1:
      machine_state1();
      break;

    case state2:
      break;

    case fault:
      break;

    case unknown:
      break;
    
    default:
      break;
  }

// Debugging
#if DEBUG_LEVEL > 4
    if((_last_post_time + POST_INTERVAL) < millis())
    {
      // Debug info
      Serial.println(F("debugging info:"));
      Serial.print(F("Cycle: "));
      Serial.println(cycle_count);

      _last_post_time = millis();
      cycle_count++;
      Serial.println("------");
  }
#endif
} // void loop()

/***********************************************************************************************************************/
/*                                             STATE MACHINE 1:                                                        */
/***********************************************************************************************************************/

void machine_state1() {
  if(button1.pressed()){
    return;
  } else {
    return;
  }
}

/***********************************************************************************************************************/
/*                                             STATE MACHINE 2:                                                        */
/***********************************************************************************************************************/

/***********************************************************************************************************************/
/*                                             STATE MACHINE fault                                                     */
/***********************************************************************************************************************/

/***********************************************************************************************************************/
/*                                             STATE MACHINE unknown                                                   */
/***********************************************************************************************************************/

/***********************************************************************************************************************/
/*                                                   FUNCTIONS                                                         */
/***********************************************************************************************************************/

/* Analog in */
float read_voltage() {
  float _result = 0;
  _result = AUX_VREF * analogRead(A1) / ADC_CHANNELS;
  return _result;
}

/* I2C */

byte I2C_command(byte _command) {
  byte _reading = 0x00;
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(REG_HIGH_BYTE));
  Wire.write(byte(REG_LOW_BYTE));
  Wire.write(_command);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDR, 1);
  if (Wire.available()) {
    _reading = Wire.read();
  }
  return _reading;
}
