#include <dmx.h>

int DmxGoodLed = 32;
int enablePin = 21;    //default on ESP32

// DIP Switch definitions:
int DipB0 = 34; // Inputs.
int DipB1 = 39;
int DipB2 = 36;

int DipLow = 25; // Outputs. I take these high in turn to read 3 switches
int DipMed = 26;
int DipHigh = 4;

// 5 I/O pins output the chosen pattern:
int patternB4 = 15; // MSB
int patternB3 = 33;
int patternB2 = 27;
int patternB1 = 12;
int patternB0 = 13; // LSB

// 3 I/O pins output the chosen speed:
int speedB2 = 23; // MSB
int speedB1 = 22;
int speedB0 = 14; // LSB

// 3 I/O pins output the chosen brightness:
int brightnessB2 = 19; // MSB
int brightnessB1 = 18;
int brightnessB0 =  5; // LSB

int DmxChannel = 0;
uint8_t currentPattern = 33; // Default to an invalid pattern
uint8_t lastPattern = 999;   // Save the last pattern and only write to screen if it changes
int currentSpeed = 0;
int lastSpeed = 0;
int currentBrightness = 0;
int lastBrightness = 0;

void setup()
{
  pinMode(DmxGoodLed, OUTPUT);
  digitalWrite(DmxGoodLed, LOW);

  // Set the ESP32 DMX board permanently to Input.
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);

   // DIP switches:
  pinMode(DipB0, INPUT);
  pinMode(DipB1, INPUT);
  pinMode(DipB2, INPUT);
  pinMode(DipLow, OUTPUT);
  pinMode(DipMed, OUTPUT);
  pinMode(DipHigh, OUTPUT);
  digitalWrite(DipLow, LOW); //Initialise off
  digitalWrite(DipMed, LOW); //Initialise off
  digitalWrite(DipHigh, LOW); //Initialise off

  // Pattern pins:
  pinMode(patternB4, OUTPUT);
  pinMode(patternB3, OUTPUT);
  pinMode(patternB2, OUTPUT);
  pinMode(patternB1, OUTPUT);
  pinMode(patternB0, OUTPUT);
  digitalWrite(patternB4, LOW); //Initialise off
  digitalWrite(patternB3, LOW); //Initialise off
  digitalWrite(patternB2, LOW); //Initialise off
  digitalWrite(patternB1, LOW); //Initialise off
  digitalWrite(patternB0, LOW); //Initialise off

  // Speed pins:
  pinMode(speedB2, OUTPUT);
  pinMode(speedB1, OUTPUT);
  pinMode(speedB0, OUTPUT);
  digitalWrite(speedB2, LOW); //Initialise off
  digitalWrite(speedB1, LOW); //Initialise off
  digitalWrite(speedB0, LOW); //Initialise off
  
  // Brightness pins:
  pinMode(brightnessB2, OUTPUT);
  pinMode(brightnessB1, OUTPUT);
  pinMode(brightnessB0, OUTPUT);
  digitalWrite(brightnessB2, LOW); //Initialise off
  digitalWrite(brightnessB1, LOW); //Initialise off
  digitalWrite(brightnessB0, LOW); //Initialise off

  Serial.begin(115200);

  DmxChannel = ReadDmxChannel();
   
  DMX::Initialize();
}

int ReadDmxChannel()
{
  int dmxCh = 0;
  digitalWrite(DipLow, HIGH); 
  dmxCh = Read3Bits();
  digitalWrite(DipLow, LOW); 
  digitalWrite(DipMed, HIGH); 
  dmxCh += 8 * Read3Bits();  
  digitalWrite(DipMed, LOW); 
  digitalWrite(DipHigh, HIGH); 
  dmxCh += 64 * Read3Bits();  
  digitalWrite(DipHigh, LOW);
  return dmxCh;
}

int Read3Bits()
{
  int value = 0;
  if (digitalRead(DipB0) == HIGH) { value += 1; }
  if (digitalRead(DipB1) == HIGH) { value += 2; }
  if (digitalRead(DipB2) == HIGH) { value += 4; }
  return value;
}


void loop()
{
  if (DMX::IsHealthy())
  {
     digitalWrite(DmxGoodLed, HIGH);
  }
  else
  {
    digitalWrite(DmxGoodLed, LOW); 
  }

  
  currentPattern = DMX::Read(DmxChannel);
  if (lastPattern != currentPattern)
  {
    Serial.println("DMX Channel = " + String(DmxChannel) + ", Pattern = " + String(currentPattern));
    lastPattern = currentPattern;
    (lastPattern & 0b10000000) ? digitalWrite(patternB4, HIGH) : digitalWrite(patternB4, LOW) ;
    (lastPattern & 0b01000000) ? digitalWrite(patternB3, HIGH) : digitalWrite(patternB3, LOW) ;
    (lastPattern & 0b00100000) ? digitalWrite(patternB2, HIGH) : digitalWrite(patternB2, LOW) ;
    (lastPattern & 0b00010000) ? digitalWrite(patternB1, HIGH) : digitalWrite(patternB1, LOW) ;
    (lastPattern & 0b00001000) ? digitalWrite(patternB0, HIGH) : digitalWrite(patternB0, LOW) ;
  }
  
  currentSpeed = DMX::Read(DmxChannel + 1);
  if (lastSpeed != currentSpeed)
  {
    Serial.println("DMX Channel = " + String(DmxChannel + 1) + ", Pattern = " + String(currentSpeed));
    lastSpeed = currentSpeed;
    (lastSpeed & 0b10000000) ? digitalWrite(speedB2, HIGH) : digitalWrite(speedB2, LOW) ;
    (lastSpeed & 0b01000000) ? digitalWrite(speedB1, HIGH) : digitalWrite(speedB1, LOW) ;
    (lastSpeed & 0b00100000) ? digitalWrite(speedB0, HIGH) : digitalWrite(speedB0, LOW) ;
  }

  currentBrightness = DMX::Read(DmxChannel + 2);
  if (lastBrightness != currentBrightness)
  {
    Serial.println("DMX Channel = " + String(DmxChannel + 2) + ", Pattern = " + String(currentBrightness));
    lastBrightness = currentBrightness;
    (lastBrightness & 0b10000000) ? digitalWrite(brightnessB2, HIGH) : digitalWrite(brightnessB2, LOW) ;
    (lastBrightness & 0b01000000) ? digitalWrite(brightnessB1, HIGH) : digitalWrite(brightnessB1, LOW) ;
    (lastBrightness & 0b00100000) ? digitalWrite(brightnessB0, HIGH) : digitalWrite(brightnessB0, LOW) ;
  }   
}
