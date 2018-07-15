// Timer1 is used for 4 s TimerInterrupt to query DHT11
// See my ebook: 
// Arduino Interrupts: Speed up your Arduino to be responsive to events
// https://www.amazon.com/Arduino-Interrupts-Speed-responsive-events-ebook/dp/B071NW54K9

unsigned int reload = 0xF424;    // OCR1A Reload for 4 seconds@16 MHz clock & prescaler 1024

void initTimer1()
{  
  cli();
  TCCR1A = 0;
  TCCR1B = 0;                        // Stop Timer1
  OCR1A = reload;
  TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);   // CTC Mode Prescaler = 256
  TIMSK1 = (1<<OCIE1A);              // Timer1 CompareA Interrupt enable
  sei();                             // Global Interrupt enable
};

