/*
2013 yebizo exihibition
the ponp controler
written by Koichiro Mori
 */
 
 #include <MsTimer2.h>
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int led_1 = 0;
int led_2 = 1;

int currentLevel;
int nextLevel;

boolean busy;
boolean toggleBlink;

int checkByte;
int inByte;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode( led, OUTPUT );     
  pinMode( led_1, OUTPUT );  
  pinMode( led_2, OUTPUT );  
  
  busy = false;
  currentLevel = 0;
  nextLevel = 0;
  toggleBlink = false;
  
  Serial.begin( 9600 );
  
  MsTimer2::set( 125, blinkLED ); // 500ms period
  MsTimer2::start();
}

// the loop routine runs over and over again forever:
void loop() {
  
  if ( Serial.available() > 0 ) {
    digitalWrite(led, HIGH);
    checkByte = Serial.read();
    blinkLED();
    
    if ( checkByte == 'X' ) {
      
      inByte = Serial.read();
      blinkLED();
      
      if(inByte == 'F') { //inByte F -> fowerd    user1 -> back
          digitalWrite(led_1, HIGH);
          digitalWrite(led_2, LOW);
          //counter = 0;
          busy = true;
          blinkLED();
      } else if ( inByte == 'B' ) {
          digitalWrite(led_1, LOW);
          digitalWrite(led_2, HIGH);
          //counter = 0;
          busy = true;
          blinkLED();
      
      } else if ( inByte == 'R' ) {
          digitalWrite(led_1, LOW);
          digitalWrite(led_2, LOW);
          //counter = 0;
          busy = false;
        
      }
      
    } else {
      Serial.read();
      
    }
    digitalWrite(led, LOW);
    delay( 10 );
    blinkLED();
  }
  
}

void reset() {
  
}

void blinkLED() {
  if( !toggleBlink ) {
      digitalWrite(led, HIGH);
      toggleBlink = true;
  } else {
      digitalWrite(led, LOW);
      toggleBlink = false;
  }

}
