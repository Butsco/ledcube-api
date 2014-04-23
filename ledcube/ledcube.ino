//int LEDPin[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int LEDPin[] = {21,20,19,18,17,16,15,14,8,9,10,11,12,13,7,6};
int LayerPin[] = {A0, A1, A2, A3};
  
void setup(){
  
  // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);
  
  // Set up LED columns as output
  for(int pin = 0; pin < 16 ; pin++ ){
    pinMode(LEDPin[pin],OUTPUT);
    digitalWrite(LEDPin[pin],LOW);
  }
  // Set up LED Layers as output
  for(int layer = 0; layer < 4; layer++){
    pinMode(LayerPin[layer], OUTPUT);
    digitalWrite(LayerPin[layer], HIGH);
  }

}

boolean leds[4][16] = {
{
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false
},{
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false
},{
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false
},{
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false
}
};

void setLED(int led, boolean state) {
  leds[led/16][led%16] = state;
}

void setLEDbyte(int index, int data) {
  int layer = index/2;
  int start = (index%2) * 8;
  
  leds[layer][start + 0] = boolean(data & B00000001);
  leds[layer][start + 1] = boolean(data & B00000010);
  leds[layer][start + 2] = boolean(data & B00000100);
  leds[layer][start + 3] = boolean(data & B00001000);
  leds[layer][start + 4] = boolean(data & B00010000);
  leds[layer][start + 5] = boolean(data & B00100000);
  leds[layer][start + 6] = boolean(data & B01000000);
  leds[layer][start + 7] = boolean(data & B10000000);
}

byte incoming = 0;
byte buffer[8];
int index = 0;

void loop() {
  
  if (Serial.available() >= 8) {
    Serial.readBytes( (char*) buffer, 8);
    Serial.println("received 8 bytes");
    
    for(int i=0; i<8; i++) {
      setLEDbyte(i, buffer[i]); 
      if(i%2 == 0) {
        Serial.write(buffer[i]);
      } else {
        Serial.print(" ");
        Serial.println(buffer[i], BIN);
      }
    }    
  }
  
  
  /*
  if (Serial.available() > 0) {
    incoming = Serial.read();
        
    //Serial.println("I received: ");
    //Serial.println(incomingByte, DEC);
    
    setLEDbyte(index, incoming);     
    index++;
    index = index%8;  
  }*/
  
  showFrame();
}

void showFrame() {
  
   // display the leds data structure layer by layer
  for(int layer = 0; layer < 4; layer++) {
    
      for(int led = 0; led < 16; led++) {
          if(leds[layer][led]) {
              digitalWrite(LEDPin[led],HIGH);
          } else {
              digitalWrite(LEDPin[led],LOW);
          }
      }
   
      digitalWrite(LayerPin[layer],LOW);
      delay(4); 
      digitalWrite(LayerPin[layer],HIGH);
  }
  
}
