// configuration options
int ledSpeed    = 50;

// first shift register (A)
int aLatchPin   = 3;
int aClockPin   = 4;
int aDataPin    = 2;
byte aLeds      = 0;

// second shift register (B)
int bLatchPin   = 6;
int bClockPin   = 7;
int bDataPin    = 5;
byte bLeds      = 0;


void setup() 
{
  // establish registers as outputs 
  pinMode(aLatchPin, OUTPUT);
  pinMode(aDataPin,  OUTPUT);  
  pinMode(aClockPin, OUTPUT);
  pinMode(bLatchPin, OUTPUT);
  pinMode(bDataPin,  OUTPUT);  
  pinMode(bClockPin, OUTPUT);
}


void loop() 
{
  // test mode
  for(int i=0; i<=16; i++){
    lightLEDs(i);
    delay(1000);
  }
  delay(2000);
}


void lightLEDs(int ledCount){
  // reset byte sets and shift registers to zero
  aLeds = 0;
  bLeds = 0;
  updateShiftRegisterA();
  updateShiftRegisterB();

  // count up and update the first register
  int countA = ledCount;
  if(ledCount > 8) countA = 8;
  for (int i = 0; i < countA; i++)
  {
    bitSet(aLeds, i);
    updateShiftRegisterA();
    delay(ledSpeed);
  }

  // count up and update the second register  
  int countB = 0;
  if(ledCount > 8) countB = ledCount - 8;
  for (int i = 0; i < countB; i++)
  {
    bitSet(bLeds, i);
    updateShiftRegisterA();
    delay(ledSpeed);
  }
}


void updateShiftRegisterA(){
  digitalWrite(aLatchPin, LOW);
  shiftOut(aDataPin, aClockPin, LSBFIRST, aLeds);
  digitalWrite(aLatchPin, HIGH); 
}


void updateShiftRegisterB(){
  digitalWrite(bLatchPin, LOW);
  shiftOut(bDataPin, bClockPin, LSBFIRST, bLeds);
  digitalWrite(bLatchPin, HIGH); 
}
