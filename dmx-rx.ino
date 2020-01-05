/******************************************************************************
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License 
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.  If not, see 
<http://www.gnu.org/licenses/>.

This code is part of the Disco Death Star project. This is its DMX interface
 
References:
https://github.com/greiginsydney/DDS-DMX
https://github.com/greiginsydney/FastLED-DiscoDeathStar
*****************************************************************************/


#include <dmx.h>

int resetDDS = 32;
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
uint8_t lastPattern = 34;    // Save the last pattern and only write to screen if it changes
int currentSpeed = 0;
int lastSpeed = 0;
int currentBrightness = 0;
int lastBrightness = 0;

void setup()
{
  pinMode(resetDDS, OUTPUT);  // Break glass in an emergency: Go HIGH to force a hardware reset of the DDS.
  digitalWrite(resetDDS, LOW);

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

  DmxChannel = ReadDmxChannel(); //Read the DIP switches to determine our base (first) DMX channel
   
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
  //Retired: I ran out of IO pins!  
  //if (DMX::IsHealthy())
  //{
     //digitalWrite(DmxGoodLed, HIGH); 
  //}
  //else
  //{
    //digitalWrite(DmxGoodLed, LOW); 
  //}
  
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
    Serial.println("DMX Channel = " + String(DmxChannel + 1) + ", Speed = " + String(currentSpeed));
    lastSpeed = currentSpeed;
    (lastSpeed & 0b10000000) ? digitalWrite(speedB2, HIGH) : digitalWrite(speedB2, LOW) ;
    (lastSpeed & 0b01000000) ? digitalWrite(speedB1, HIGH) : digitalWrite(speedB1, LOW) ;
    (lastSpeed & 0b00100000) ? digitalWrite(speedB0, HIGH) : digitalWrite(speedB0, LOW) ;
  }

  currentBrightness = DMX::Read(DmxChannel + 2);
  if (lastBrightness != currentBrightness)
  {
    Serial.println("DMX Channel = " + String(DmxChannel + 2) + ", Brightness = " + String(currentBrightness));
    lastBrightness = currentBrightness;
    switch (currentBrightness)
    {
      case 255:  // 255 is banned! I don't want you to set the channel full on accidentally - or easily - and kill the DDS
        digitalWrite(resetDDS, LOW);
        digitalWrite(brightnessB2, LOW) ;
        digitalWrite(brightnessB1, LOW) ;
        digitalWrite(brightnessB0, LOW) ;
        break;
      case 251 ... 254:
        Serial.println("Have just reset the DDS. Brightness value = " + String(currentBrightness));
        digitalWrite(resetDDS, HIGH);
        digitalWrite(brightnessB2, LOW) ;
        digitalWrite(brightnessB1, LOW) ;
        digitalWrite(brightnessB0, LOW) ;
        break;
      default:
        digitalWrite(resetDDS, LOW);
        (lastBrightness & 0b10000000) ? digitalWrite(brightnessB2, HIGH) : digitalWrite(brightnessB2, LOW) ;
        (lastBrightness & 0b01000000) ? digitalWrite(brightnessB1, HIGH) : digitalWrite(brightnessB1, LOW) ;
        (lastBrightness & 0b00100000) ? digitalWrite(brightnessB0, HIGH) : digitalWrite(brightnessB0, LOW) ;
        break;
    }
  }   
}
