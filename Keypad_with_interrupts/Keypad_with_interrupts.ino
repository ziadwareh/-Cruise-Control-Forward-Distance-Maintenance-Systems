#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
//keys
char key[4][4]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}};
//needed variables
char row = 0;
char column = 0;
char* ipc = &column;
char* ipr = &row;
char n= 0; //the number of pin changes, since everytime the pin is changed, the interrupt will be started. This means that when you press and release, that is two interrupts. this constant is to metgate that.
int main (void){
  //defining the outputs to be PD4->PD7
  DDRD=0xF0;
  PORTD=0x00;
  //defining the inputs with pullups to be PC0->PC3
  DDRC=0x00;
  PORTC=0x0F;
  //handling the pinchange interrupt 
  sei();
  PCICR=0x02;
  PCMSK1=0x0F;
  Serial.begin(9600);
  while(1){
    PORTD=0x00;
    _delay_ms(1);
  }
}
void getcol(char* ipc){
  if(!(PINC&0x01)){
    *ipc=1;
  }
  else if(!(PINC&0x02)){
    *ipc=2;
  }
  else if(!(PINC&0x04)){
    *ipc=3;
  }
  else if(!(PINC&0x08)){
    *ipc=4;
  }
   Serial.print("colasaasdasdasdas: ");
         Serial.flush();
      Serial.println(column,10);
            Serial.flush();
}

ISR(PCINT1_vect){
  n++;
//  Serial.println("enter");
  if(n==2){
     //checking first row
       PORTD=0xE0;
       _delay_ms(1);
       if((PINC&0x0F)!=0x0F)
        row=1;
       //checking second row
       PORTD=0xD0;
       _delay_ms(1);
       if((PINC&0x0F)!=0x0F)
        row=2;
       //checking third row
       PORTD=0xB0;
       _delay_ms(1);
       if((PINC&0x0F)!=0x0F)
        row=3;
       //checking fourth row
       PORTD=0x70;
       _delay_ms(1);
       if((PINC&0x0F)!=0x0F)
        row =4;
      getcol(ipc);
      Serial.print("row: ");
            Serial.flush();
      Serial.println(row,10);
            Serial.flush();
      Serial.print("col: ");
            Serial.flush();
      Serial.println(column,10);
            Serial.flush();
      while((PINC&0x0F)!=0x0F)//waiting till button is released
      Serial.flush();
      Serial.println(key[row-1][*ipc-1]);
      n=0;
  }
}
