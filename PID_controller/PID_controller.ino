#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#define pi 3.14159
#include <math.h>
//gains
//int kp=2499;
//int ki=8989;
//int kd=154;
//take 1
//float ki=58.45;
//float kp=0.5;
//float kd=20;
//take 2
//float ki=28.577;
//float kp=0.800156;
//float kd=5.601092*pow(10,-3);
//take 3
//float ki=1.8394;//199.66;
//float kp=0.055371;//3.50694;
//float kd=17.890615*pow(10,-3);
float ki=221.8;
float kp=0.9981;
float kd=8.872*pow(10,-4);
//needed variables
float Ts=0.1;
float r=0.0335;// radius of wheels
float u_k;
float u_k_1;
float u_k_2;
float e_k;
float e_k_1;
float e_k_2;
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
  //TCCR2B=0x07;//prescaler 1024
  TCCR2B=0x01;
  TIMSK2=0x01;//enabling timer overflow interrupt
  //Timer0 initialization for PWM
  TCCR0A = 0b10100001; //phase correct non-inverting
  TCCR0B = 0b00000010; // prescaler is 8
  //initializing the variables needed;
  V_ref=90;
  V_ref=V_ref/100;
  u_k_1=0;
  u_k_2=0;
  e_k_1=0;
  e_k_2=0;
  V_actual=0;
  Serial.begin(9600);
  TCNT2=0; //resetting the counter so that we guarentee that timeOld=0 as a start;
  while(1){
    if((PIND&0x08)!=previous_reading){
      n++;
    previous_reading=(PIND&0x08);
    }
    //updating error
    e_k=V_ref-V_actual;
    //control law for difference equation
    u_k=(1/(2*Ts))*((2*Ts*u_k_2)+(ki*pow(Ts,2)*e_k)+(2*ki*pow(Ts,2)*e_k_1)+(ki*pow(Ts,2)*e_k_2)+(4*kd*e_k)-(8*kd*e_k_1)+(4*kd*e_k_2)+(2*Ts*kp*e_k)-(2*Ts*kp*e_k_1));
    //updating the variables
    u_k_2=u_k_1;
    u_k_1=u_k;
    e_k_2=e_k_1;
    e_k_1=e_k;
    //to prevent an overflow
    if(u_k>255)
      OCR0A=255;
    else
      OCR0A = (char)(u_k);
    OCR0B = 0.95 * OCR0A;
    PORTD|=0b0010000;
//    Serial.println(u_k);
//    Serial.print("OCR0A: ");
//    Serial.println(OCR0A);
//    Serial.print("E_Curr: ");
//    Serial.println(E_curr);
  }
}
ISR(TIMER2_OVF_vect){
  overflow_counter++;
  if(overflow_counter==6250){ //refresh every 0.25s
    n=n/40;
//   Serial.print(n);
//  Serial.println(" times");
  w=(n*60)*10;
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
