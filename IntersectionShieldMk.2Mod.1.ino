//PROJECT   : Intersection Shield Mk.2 Mod.1
//PURPOSE   : To develop an interface with the DDB Intersection Shield
//COURSE    : ICS4U
//AUTHOR    : J. Shibley
//DATE      : 22/02/2021
//MCU       : ATtiny84
//STATUS    : Semi-Opperational
//REFERENCE : http://darcy.rsgc.on.ca/ACES/TEI4M/2021/DDPv6.html#intersection

#include <DDBv6JS.h>  //Include Shibley's personal DDB library

#define R1 1<<PA2     //Define the first RED light as Port A Pin 2
#define R2 1<<PA5     //Define the second RED light as Port A Pin 5

#define Y1 1<<PA1     //Define the first YELLOW light as Port A Pin 1
#define Y2 1<<PA4     //Define the second YELLOW light as Port A Pin 4

#define G1 1<<PA0     //Define the first GREEN light as Port A Pin 0
#define G2 1<<PA3     //Define the second GREEN light as Port A Pin 3

#define BASE1 1<<PA7  //Define the BASE1 transistor as Port A Pin 7
#define BASE0 1<<PA6  //Define the BASE0 transistor as Port A Pin 6

struct state {
  uint8_t lights;     //The active lights
  uint8_t duration;   //Record the WDT prescaler to be used to control the length of the state
  uint8_t base;       //The active PULL-DOWN transistor
};  //End state

// 6-Cell Constant Array of States
state states [] {
  {R1|R2, WDTps128K, 0},      //R1/R2, 1sec
  {G1|R2, WDTps128K, BASE0},  //G1/R2, 1sec * 7
  {Y1|R2, WDTps64K, 0},       //Y1/R2, 0.5sec
  {R1|R2, WDTps128K, 0},      //R1/R2, 1sec
  {R1|G2, WDTps128K, BASE1},  //R1/G2, 1sec * 7
  {R1|Y2, WDTps64K, 0},       //R1/Y2, 0.5sec
};

volatile boolean triggered = LOW;
uint8_t countdown = 7;
uint8_t currentState = 0;

void setupWDT(uint8_t ps) {
  WDTCSR = ps;             //Scale the WD oscillator
  WDTCSR &= ~(1 << WDIF);   //Clear the WD interrupt flag
  WDTCSR |= (1 << WDIE);    //Enable the WD interrupt
}//End setupWDT

ISR(WDT_vect) {
  triggered = HIGH;
}//End Watchdog ISR

void setup() {
  DDRA |= 0xFF;   //Set all the pins of Port A to OUTPUT
  DDRB |= 0b111;  //Set the DATA pins on the 4511 to OUTPUT
  cli();    //Disable global interrupts
  //WDTCSR |= (1 << WDIE);  //enable the WD interrupt
  //WDTCSR &= ~(1 << WDIF);  //clear the WD interrupt flag
  setupWDT(states[currentState].duration);
  sei();    //Endable global interrupts
}//End setup

void loop() {
  if (triggered) {
    if((currentState == 1) || (currentState == 4)){
      if(countdown == 0){
        currentState++;
        countdown = 7;
      }//End if
      else{
        DDRB |= countdown;//Send data to 4511
        countdown--; //Decrease countdown
      }//end Else
      
    }//End if (green state)
    
    else{
      if (currentState == 5) {
        currentState = 0;   //Set currentState to 0
      }//End if(currentState at MAX)
      else {
        currentState++;   //Increase currentState
      }//End else(currentState not at MAX)
    }//End else(currentState not GREEN)
    
    triggered = LOW;
    PORTA = states[currentState].lights | states[currentState].base;
    PORTB = countdown;

    setupWDT(states[currentState].duration);
  }//End triggered
}//End loop
