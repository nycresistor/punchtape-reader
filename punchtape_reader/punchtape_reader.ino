#include <stdint.h>

#define CHANNELS 8
int n_sides[CHANNELS] = { 27,  0,  1,  3,  4,  5,  7,  8 };
int p_sides[CHANNELS] = { 26, 25, 24, 22, 21, 20, 19, 18 };

const int SPKT_N = 2;
const int SPKT_P = 23;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialized, starting scans");
}

bool checkSprocket() {
  // check the sprocket pin.
  pinMode(SPKT_N,OUTPUT);
  pinMode(SPKT_P,OUTPUT);
  digitalWrite(SPKT_N,HIGH);
  digitalWrite(SPKT_P,LOW);
  
  // small delay for charge
  delayMicroseconds(50);

  // Isolate the pin N end of the diode
  pinMode(SPKT_N,INPUT);
  digitalWrite(SPKT_N,LOW);  // turn off internal pull-up resistor

  // Count how long it takes the diode to bleed back down to a logic zero
  for ( int8_t j = 0; j < 100; j++) {
    delayMicroseconds(7*9);
    if (digitalRead(SPKT_N)==0) return true;
  }
  return false;
}

void waitForSprocket(bool val) {
  // wait for sprocket pin to go high.
  while (checkSprocket() != val);
}

uint8_t readHoles() {
  // read all eight bits.
  for (int i = 0; i < CHANNELS; i++) {
    pinMode(n_sides[i],OUTPUT);
    pinMode(p_sides[i],OUTPUT);
    digitalWrite(n_sides[i],HIGH);
    digitalWrite(p_sides[i],LOW);
  }
  
  uint8_t c = 0;

  // small delay for charge
  delayMicroseconds(50);

  // Isolate the pin 2 end of the diode
  for (int i = 0; i < CHANNELS; i++) {
    pinMode(n_sides[i],INPUT);
    digitalWrite(n_sides[i],LOW);  // turn off internal pull-up resistor
  }

  int j;
  // Count how long it takes the diode to bleed back down to a logic zero
  for ( j = 0; j < 100; j++) {
    delayMicroseconds(5);
    for (int i = 0; i < CHANNELS; i++) {
      if (digitalRead(n_sides[i])==0) c |= 1<<i;
    }
  }

  // You could use 'j' for something useful, but here we are just using the
  // delay of the counting.  In the dark it counts higher and takes longer, 
  // increasing the portion of the loop where the LED is off compared to 
  // the 1000 microseconds where we turn it on.
  return c;
}

uint8_t n1 = 0;
void loop()
{
  waitForSprocket(true);
  uint8_t n0 = readHoles();
  uint8_t next = (n0 & 0xAD) | (n1 & 0x52);
  n1 = n0;
  Serial.print('|');
  for (int i = 0; i < 8; i++) {
    Serial.print((next&(1<<i))==0?' ':'#');
  }
  Serial.print("|  ");
  char c = next & 0x7F;
  if (c > 32) Serial.print(c); else Serial.print(" ");
  Serial.print(" ");
  Serial.println(next,HEX);
  waitForSprocket(false);
}

