#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>  
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL

#define PWM_PERIOD  (800)  // x 50 usec

/* freq = 20kHz, period = 50 usec
 * 50 usec / 800 = 0.0625 ns -> 16MHz tick frequency
 */

#define PWM_PIN_OUT()     (DDRB |= (1 << DDB1))   // use PB1 (D9 pin) for PPM output
#define LED_PIN_OUT()     (DDRB |= (1 << DDB5))   // use PB5 (D13 pin) for LED output
#define LED_OUT_TOGGLE()  (PINB |=  (1 << PINB5))

volatile uint16_t ovf_count = 0;

ISR(TIMER1_OVF_vect) { // overflow 
   ovf_count++;
}

void timer1_init() {
   uint8_t SREG_tmp = SREG;  // save the status register
   cli();                    // disable interrupts

   TCCR1B = (0 << ICNC1) | (0 << ICES1) 
          | (0 << WGM13) | (0 << WGM12) 
          | (0 << CS12)  | (0 << CS11) | (0 << CS10);

   TCCR1A = (1 << COM1A1) | (0 << COM1A0)
          | (0 << COM1B1) | (0 << COM1B0)
          | (1 << WGM11)  | (0 << WGM10); 

   TCNT1 = 0; 
   ICR1  = PWM_PERIOD-1;
   OCR1A = 0;
   OCR1B = 0;

   TIMSK1 = (0 << ICIE1) | (0 << OCIE1B) | (0 << OCIE1A) | (1 << TOIE1); 
   TCCR1B = (0 << ICNC1) | (0 << ICES1) 
          | (1 << WGM13) | (1 << WGM12)
          | (0 << CS12)  | (0 << CS11) | (1 << CS10);
   ovf_count = 0;            // reset overflow counter
   SREG = SREG_tmp;          // restore the status register
}

void setup() {
  PWM_PIN_OUT(); 
  LED_PIN_OUT();
  timer1_init();
  sei();
}

uint16_t dc = 0; 

void loop() {
  _delay_ms(100);
  dc = (dc+10) % 800;
  OCR1A = dc;
  LED_OUT_TOGGLE();
}
////////////////////////////////////////////////////////////////////////////////////

