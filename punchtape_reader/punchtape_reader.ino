//
// This example shows one way of using an LED as a light sensor.
// You will need to wire up your components as such:
//
//           + digital2
//           |
//           <
//           > 100 ohm resistor
//           <
//           |
//           |
//         -----
//          / \  LED, maybe a 5mm, clear plastic is good
//         -----
//           |
//           |
//           + digital3
//
// What we are going to do is apply a positive voltage at digital2 and
// a low voltage at digital3. This is backwards for the LED, current will
// not flow and light will not come out, but we will charge up the 
// capacitance of the LED junction and the Arduino pin.
//
// Then we are going to disconnect the output drivers from digital2 and
// count how long it takes the stored charge to bleed off through the 
// the LED. The brighter the light, the faster it will bleed away to 
// digital3.
//
// Then just to be perverse we will display the brightness back on the 
// same LED by turning it on for a millisecond. This happens more often
// with brighter lighting, so the LED is dim in a dim room and brighter 
// in a bright room. Quite nice.
//
// (Though a nice idea, this implementation is flawed because the refresh
// rate gets too long in the dark and it flickers disturbingly.)
//

#define CHANNELS 9
int n_sides[CHANNELS] = { 27,  0,  1,  2,  3,  4,  5,  7,  8 };
int p_sides[CHANNELS] = { 26, 25, 24, 23, 22, 21, 20, 19, 18 };

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialized, starting scans");
}

void loop()
{
  for (int i = 0; i < CHANNELS; i++) {
    pinMode(n_sides[i],OUTPUT);
    pinMode(p_sides[i],OUTPUT);
    digitalWrite(n_sides[i],HIGH);
    digitalWrite(p_sides[i],LOW);
  }
  
  int v[CHANNELS];
  for (int i = 0; i < CHANNELS; i++) {
    v[i] = -1;
  }

  // small delay for charge
  delayMicroseconds(100);

  // Isolate the pin 2 end of the diode
  for (int i = 0; i < CHANNELS; i++) {
    pinMode(n_sides[i],INPUT);
    digitalWrite(n_sides[i],LOW);  // turn off internal pull-up resistor
  }

  int j;
  // Count how long it takes the diode to bleed back down to a logic zero
  for ( j = 0; j < 200; j++) {
    delayMicroseconds(7);
    for (int i = 0; i < CHANNELS; i++) {
      if ( v[i] == -1 && digitalRead(n_sides[i])==0) v[i] = j;
    }
  }

    for (int i = 0; i < CHANNELS; i++) {
      if ( v[i] == -1 ) v[i] = 200;
    }

  // You could use 'j' for something useful, but here we are just using the
  // delay of the counting.  In the dark it counts higher and takes longer, 
  // increasing the portion of the loop where the LED is off compared to 
  // the 1000 microseconds where we turn it on.
  for (int i = 0; i < CHANNELS; i++) {
    Serial.print(v[i] > 100?'X':'O');
  }
  Serial.println();
  // Turn the light on for 1000 microseconds
  //digitalWrite(p_sides[i],HIGH);
  //digitalWrite(n_sides[i],LOW);
  //pinMode(p_sides[i],OUTPUT);
  //pinMode(n_sides[i],OUTPUT);
  //delayMicroseconds(1000);
  // we could turn it off, but we know that is about to happen at the loop() start
}

