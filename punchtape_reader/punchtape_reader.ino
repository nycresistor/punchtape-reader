#include <stdint.h>

const int SPROCKET_PIN = 21;
const int CHANNEL_COUNT = 8;
const int channels[] = { 26, 25, 24, 23, 22, 20, 19, 18 };

void setup()
{
  Serial.begin(115200);
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    pinMode(channels[i],INPUT);
    digitalWrite(channels[i],LOW);
  }
  pinMode(SPROCKET_PIN,INPUT);
  digitalWrite(SPROCKET_PIN,LOW);
}

const uint8_t CONSENSUS_PASSES = 2;

inline bool checkSprocket() {
  bool val = digitalRead(SPROCKET_PIN) != LOW;
  uint8_t matches = 0;
  while (matches <= CONSENSUS_PASSES) {
    bool r = digitalRead(SPROCKET_PIN) != LOW;
    if (val == r) {
      matches++; 
    } else {
      matches = 0; val = r;
    }
  }
  return val;
}

void waitForSprocket(bool val) {
  // wait for sprocket pin to go to given value.
  while (checkSprocket() != val);
}

uint8_t readHoles() {
  // read all eight bits.
  uint8_t c = 0;  
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    c <<= 1;
    c |= (digitalRead(channels[i]) == LOW)?0:1;
  }
  return c;
}

uint8_t readHolesConsensus() {
  uint8_t val = readHoles();
  uint8_t matches = 0;
  while (matches < CONSENSUS_PASSES) {
    uint8_t r = readHoles();
    if (val == r) {
      matches++; 
    } else {
      matches = 0; val = r;
    }
  }
  return val;
}

char buffer[5];

void loop()
{
  waitForSprocket(true);
  for (int i = 4; i > 0; i--) {
    buffer[i] = buffer[i-1];
  }
  buffer[0] = readHolesConsensus();
  char out = (buffer[0] & 0x40) |
    (buffer[1] & 0x12) |
    (buffer[3] & 0x24) |
    (buffer[4] & 0x89);
#ifdef DEBUG    
  Serial.print('|');
  for (int i = 0; i < 8; i++) {
    Serial.print((out&(1<<i))==0?' ':'#');
  }
  Serial.print("|  ");
  char c = out & 0x7F;
  if (c > 32) Serial.print(c); else Serial.print(" ");
  Serial.print(" ");
  Serial.println(((int)out&0xff),HEX);
#else
  Serial.print(out);
  Serial.flush();
#endif
  waitForSprocket(false);
}

