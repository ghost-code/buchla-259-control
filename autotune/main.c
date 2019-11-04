// PORTA CLOCK OUPUTS // Interrupt timing
// 0: 112.640 Khz
// 1: 440 Hz
// 2: 220 Hz 1
// 3: 220 Hz 2
// 5: 27.5 Hz
//
// PORTB TUNE INPUT: 2

#include <avr/io.h>
#include <avr/interrupt.h>

#define INTERRUPT_FREQ 225280UL

ISR(TIM0_COMPA_vect, ISR_NAKED) {
  static uint16_t c = 0;
  static uint8_t output = 0;

  if (PINB & 0x4) {
    output = ((c & 0x1) | ((c & 0x1300) >> 7)); 
    output |= ((output ^ 0x4) & 0x4) << 1;
  }
  else {
    output = 0xC;
  }

  PORTA = output;
  c++;
  reti();
}

int main (void) {

  cli(); // Disable interrupts
 
  DDRA |= 0x3F; // Set output pins

  // Setup timer interrupt
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;  

  OCR0A = (F_CPU / (INTERRUPT_FREQ)); // Interrupt timing
  TCCR0A |= (1 << WGM01); // CTC - Clear Timer on Compare
  TCCR0B |= (1 << CS00); // No prescaler
  TIMSK0 |= (1 << OCIE0A); // Compare A Match Interrupt Enable
  sei(); // SREG |= (1 << 7);

  for(;;) { }
}


