#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#define pi 3.14159
#include <math.h>
//gains
float Ki=62.307;//28.355;//48.45;//4493;  //6.69*pow(10,4);
float Kp=0.055371;//3.613809;///0;  //1609; //500;
//needed variables
float Ts=0.25;
float r=0.0335;// radius of wheels
float U_prev;
float U_curr;
float E_prev;
float E_curr;
float w=0;
float V_actual;
float V_ref; //in cm/s
float n=0;
int overflow_counter; //to get a delay of 1 second, we need to overflow around 61 times
int main (void){
  //setting the input pin
   DDRD=0b11110000;
  PORTD=0x00;
  //setting the needed variables
  float previous_reading=(PIND&0x08);
  //enabling interrupts
  sei();
  //setting up timer 2
  TCCR2A=0x00;
  TCCR2B=0x07;//prescaler 1024
  TIMSK2=0x01;//enabling timer overflow interrupt
  //Timer0 initialization for PWM
  TCCR0A = 0b10100001; //phase correct non-inverting
  TCCR0B = 0b00000010; // prescaler is 8
  //initializing the variables needed;
  V_ref=60;
  V_ref=V_ref/100;
  U_prev=0;
  E_prev=0;
  V_actual=0;
  Serial.begin(9600);
  TCNT2=0; //resetting the counter so that we guarentee that timeOld=0 as a start;
  while(1){
    if((PIND&0x08)!=previous_reading){
      n++;
    previous_reading=(PIND&0x08);
    }
    //updating error
    E_curr=V_ref-V_actual;
    //control law for difference equation
//    U_curr = U_prev + (Kp*E_curr) + (Kp*E_prev) + (Ki*Ts*E_prev);
    U_curr = U_prev + (Kp*E_curr) + (Kp*E_prev) + (0.45985*E_prev);
    U_prev=U_curr;
    E_prev=E_curr;
    if(U_curr>255)
      OCR0A=255;
    else
      OCR0A =(char)(U_curr);
    OCR0B = 0.95 * OCR0A;
    PORTD|=0b0010000;
//    Serial.print("OCR0A: ");
//    Serial.println(OCR0A);
//    Serial.print("E_Curr: ");
//    Serial.println(E_curr);
  }
}
ISR(TIMER2_OVF_vect){
  overflow_counter++;
  if(overflow_counter==15){ //refresh every 0.25s
    n=n/40;
//   Serial.print(n);
//  Serial.println(" times");
  w=(n*60)*4;
  n=0;//reset the counter
//  Serial.print(w);
//  Serial.println(" rpm");
   w=w*((2*pi)/60);
//   Serial.print("w_actual: ");
//  Serial.print(w);
//  Serial.println(" rad/s");
   V_actual = w * r;
  Serial.print("V_actual: ");
  Serial.print(V_actual);
  Serial.println(" m/s");
  overflow_counter=0;
  }
}
