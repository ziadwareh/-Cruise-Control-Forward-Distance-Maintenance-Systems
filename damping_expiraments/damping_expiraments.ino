#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#define pi 3.14159
//#include <math.h>
float w=0;
float n=0;
float N=0;
int overflow_counter; //to get a delay of 1 second, we need to overflow around 61 times
int main (void){
  //setting the input pin
  DDRD=0b01101100;
  PORTD=0x00;
  //setting the needed variables
  float previous_reading=(PIND&0x10);
  //enabling interrupts
  sei();
  //setting up timer 2
  TCCR2A=0x00;
  TCCR2B=0x07;//prescaler 1024
  TIMSK2=0x01;//enabling timer overflow interrupt
    //Timer0 initialization for PWM
  TCCR0A = 0b10000001;//0b10100001; //phase correct non-inverting
  TCCR0B = 0b00000010; // prescaler is 8
  Serial.begin(9600);
  TCNT2=0; //resetting the counter so that we guarentee that timeOld=0 as a start;
  while(1){
    if((PIND&0x10)!=previous_reading){
      n++;
    previous_reading=(PIND&0x10);
    }
    OCR0A=150;
//    OCR0B=OCR0A*0.95;
    PORTD|=0x08;
  }
} 
ISR(TIMER2_OVF_vect){
  overflow_counter++;
  if(overflow_counter==61){
   N=n/40;
   w=(N*60);
   n=0;//reset the counter
   w=w*((2*pi)/60);
   Serial.print(w);
  Serial.println(" rad/s");
  overflow_counter=0;
  }
}
