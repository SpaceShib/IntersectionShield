uint8_t segmentMap[] = {0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110,
                        0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111,
                        0b1110111, 0b1111100, 0b0111001, 0b1011110, 0b1111001,
                        0b1110001};
//Order: G F E D C B A

#define ONBOARDLED (1<<PB2)   //Define the On-Board LED as 

void onBoardLED (uint8_t state) {
  DDRB |= ONBOARDLED;
  if (state)
    PORTB |= ONBOARDLED;
  else
    PORTB &= ~ONBOARDLED;
}

void shiftOutSHiB(uint8_t dataPin, uint8_t clockPin, uint8_t val)
{
  uint8_t i;
  for (i = 0; i < 8; i++)  {
    if(!!(val & (1 << i)) == 1){
      PORTA |= dataPin;
    }else{
      PORTA &= ~dataPin;
    }//End else
    PORTA |= clockPin;
    PORTA &= ~clockPin;
  }//End for
}//End shiftOut

// Prescale constants for ATtiny84 Watchdog Timer Prescalers
// Comments assume System Clock Speed is 8 MHz or (approximately) 2^23, Vcc=5.0V
#define WDTps2K     0<<WDP3|0<<WDP2|0<<WDP1|0<<WDP0    // ~16ms
#define WDTps4K     0<<WDP3|0<<WDP2|0<<WDP1|1<<WDP0    // ~32ms
#define WDTps8K     0<<WDP3|0<<WDP2|1<<WDP1|0<<WDP0    // ~16ms
#define WDTps16K    0<<WDP3|0<<WDP2|1<<WDP1|1<<WDP0    // ~0.125s
#define WDTps32K    0<<WDP3|1<<WDP2|0<<WDP1|0<<WDP0    // ~0.25s
#define WDTps64K    0<<WDP3|1<<WDP2|0<<WDP1|1<<WDP0    // ~0.5s
#define WDTps128K   0<<WDP3|1<<WDP2|1<<WDP1|0<<WDP0    // ~1s
#define WDTps256K   0<<WDP3|1<<WDP2|1<<WDP1|1<<WDP0    // ~2s
#define WDTps512K   1<<WDP3|0<<WDP2|0<<WDP1|0<<WDP0    // ~4s
#define WDTps1024K  1<<WDP3|0<<WDP2|0<<WDP1|1<<WDP0    // ~8s

// Prescale constants for ATtiny84 Sleep Mode Prescalers
#define SMIdle      0<<SM1 | 0<<SM0                   // Idle
#define SMADCNR     0<<SM1 | 1<<SM0                   // ADC Noise Reduction
#define SMPD        1<<SM1 | 0<<SM0                   // Power-down
#define SMStandby   1<<SM1 | 1<<SM0                   // Standby
