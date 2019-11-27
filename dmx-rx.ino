int ledPin = 13;

int DipB0 = 34;
int DipB1 = 39;
int DipB2 = 36;

int DipLow = 25;
int DipMed = 26;
int DipHigh = 4;

int DmxChannel = 0;

void setup()
{
  pinMode(ledPin, OUTPUT);

  pinMode(DipLow, OUTPUT);
  pinMode(DipMed, OUTPUT);
  pinMode(DipHigh, OUTPUT);
  digitalWrite(DipLow, LOW); //Initialise off
  digitalWrite(DipMed, LOW); //Initialise off
  digitalWrite(DipHigh, LOW); //Initialise off

  pinMode(DipB0, INPUT);
  pinMode(DipB1, INPUT);
  pinMode(DipB2, INPUT);

  Serial.begin(115200);
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
    DmxChannel = ReadDmxChannel();
    
    //Serial.println("Hello, world!");
    Serial.println("DMX Channel = " + String(DmxChannel));
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
}

