//for this to work all proper like the fuses must be set to 0xD2 (0b11010010)
//This enables 8 mHz and reduces bootup time to 4ms
#include <avr/io.h>

int delaytime = 100;
int Voltcount = 0;
int ontime = 2000;
int Pins[] = {PA3, PB0, PA4, PB1, PA5, PB2, PA6, PA7};
bool Ports[] = {0, 1, 0, 1, 0, 1, 0, 0};
float Voltages[] = {12.5,14.0,14.5,15.0,15.5,16.0,16.5,17.0};
int VoltagesInt[] = {160,179,186,192,199,205,211,219};
float Vbat = 0.0;
int lednum = 0;
void setup(){//Bootsequency
  {//Clock Setup
    CLKPR = (1 << CLKPCE); //Enable changing of clock prescaler
    CLKPR = (0 << CLKPS0) | (0 << CLKPS1) | (0 << CLKPS2);
    }//Change clock prescaler to 1

  {//Pin Setups
    //Create Hold signal to power IC
    DDRA  |= (1<<PA2);   //set PA2 as Output,Holdsignal
    PORTA |= (1<<PA2);  //turn on PA2 Create Hold singal
    //LEDOrder: PA3 PB0 PA4 PB1 PA5 PB2 PA6 PA7
    // Pin Setup
    DDRA |= (1<<PA3);   //set Pin 3 as Output, LED1
    DDRA |= (1<<PA4);   //set Pin 4 as Output, LED3
    DDRA |= (1<<PA5);   //set Pin 5 as Output, LED5
    DDRA |= (1<<PA6);   //set Pin 6 as Output, LED7
    DDRA |= (1<<PA7);   //set Pin 7 as Output, LED8
    DDRB |= (1<<PB0);   //set PB1 as Output, LED2
    DDRB |= (1<<PB1);   //set PB1 as Output, LED4
    DDRB |= (1<<PB2);   //set PB1 as Output, LED6
  }
  
  {//Input Voltage Measurement
    
    ADMUX  = 0b00000001;  //set ADC to ADC1 and reference VCC
    ADCSRB = 0b00010000;  //left adjust the result
    ADCSRA |= (1<<ADPS2);
    ADCSRA |= (1<<ADPS1); //set ADC clock prescaler to 64 
                          //so that the frequency is in recomended range
    ADCSRA |= (1<<ADEN);  //enable ADC
    ADCSRA |= (1<<ADSC);  //start first ADC measurement, takes 25 cycles
    
    while(ADCSRA & (1<<ADSC)); //wait until conversion is complete 
    delay(20);
    ADCSRA |= (1<<ADSC);  //start used measurement
    
    while(ADCSRA & (1<<ADSC)); //wait until conversion is complete 
    
    //voltage divider: 130k/390k 5V reference
    Vbat = 20*ADCH/256;
    for (int i = 0; i < 8; i++) {
      if ( ADCH > VoltagesInt[i]){
        lednum++;
      }
    }
    
  }

  {//Makelighton
    //LEDOrder: PA3 PB0 PA4 PB1 PA5 PB2 PA6 PA7
    if(lednum>0){
      for (int i = 0; i < lednum; i++) {
        if(Ports[i]){
          PORTB |= (1<<Pins[i]);
        }
        else{
          PORTA |= (1<<Pins[i]);
        }
        delay(delaytime);
      }
      delay(ontime);
      for (int i = 0; i < lednum; i++) {
        if(Ports[i]){
          PORTB &= ~(1<<Pins[i]);
        }
        else{
          PORTA &= ~(1<<Pins[i]);
        }
        delay(delaytime);
      }
    }
    else{
      for (int i = 0; i < 5; i++) {
        PORTA |= (1<<Pins[0]);delay(200);
        PORTA &= ~(1<<Pins[0]);delay(200);
      }
    }
  }
  PORTA &= ~(1<<PA2);// turn off hold signal

}

void loop(){
  
}
