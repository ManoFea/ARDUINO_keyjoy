#include <Keyboard.h>
#include <Mouse.h>

int pins[11] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
//- SET YOUR LIST OF KEYBOARD KEYS YOU WANT TO TRIGGER 
int keys[11] = {'w', 'a', 's', 'd', ' ', 'e', 'f', 'g', 'h', 'i', 'j'}; 
//- HOW MANY KEYS DO YOU WANT TO ENABLE, ONLY use what you need
int totalPins = 4; 

int startKeyboardEnabled = 1;  //- SET "0" for KEYBOARD debug mode
int startJoystickEnabled = 0;  //- SET "0" for MOUSE debug mode

/*-------------------------------------------------------------

IN THE Serial Monitor you can Enable or Disable the Keyboard 
or the Mouse control by typing:
1 or 0 : Enable/Disable Mouse
k or l : Enable/Disable Keyboard

Left-Mouse-Click is always enabled on and dedicated to pin 2

/ ------------------------------------------------------------- */

const int xPin = A0;         
const int yPin = A1;

const int xMin = 300;
const int xMax = 680;
const int yMin = 260;
const int yMax = 730;

const float smoothingFactor = 0.4;

float smoothX = 0;
float smoothY = 0;

int keyboardEnabled = 0;
int joystickEnabled = 1;

void setup() {
  Keyboard.begin();
  Mouse.begin();
  Serial.begin(115200);

  for (int i = 0; i < totalPins; i++) {
    pinMode(pins[i], INPUT);
  }

  pinMode(2, INPUT); // LEFT CLICK PIN

  keyboardEnabled = startKeyboardEnabled;
  joystickEnabled = startJoystickEnabled;

  delay(2000);
}

void loop() {

  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '1') joystickEnabled = 1;
    if (c == '0') joystickEnabled = 0;
    if (c == 'k') keyboardEnabled = 1;
    if (c == 'l') keyboardEnabled = 0;
  }

  // keyboard
  for (int i = 0; i < totalPins; i++) {
    if (keyboardEnabled == 1) {
      if (digitalRead(pins[i]) == HIGH) {
        Keyboard.press(keys[i]);
      } else {
        Keyboard.release(keys[i]);
      }
    } else {
      Keyboard.release(keys[i]);
    }
  }

  // mouse movement
  int x = analogRead(xPin);
  int y = analogRead(yPin);

  x = constrain(x, xMin, xMax);
  y = constrain(y, yMin, yMax);

  x = x - ((xMin + xMax) / 2);
  y = y - ((yMin + yMax) / 2);

  x = map(x, -(xMax - xMin) / 2, (xMax - xMin) / 2, 50, -50);
  y = map(y, -(yMax - yMin) / 2, (yMax - yMin) / 2, -50, 50);

  smoothX += (x - smoothX) * smoothingFactor;
  smoothY += (y - smoothY) * smoothingFactor;

  // LEFT CLICK (pin 2)
  if (digitalRead(2) == HIGH) {
    Mouse.press(MOUSE_LEFT);
  } else {
    Mouse.release(MOUSE_LEFT);
  }

  if (joystickEnabled == 1) {
    Mouse.move((int)smoothX, (int)smoothY, 0);
  } else {
    Mouse.release(MOUSE_LEFT);
  }

  Serial.print("K=");
  Serial.print(keyboardEnabled);
  Serial.print(" J=");
  Serial.print(joystickEnabled);
  Serial.print(" M=");
  Serial.print((int)smoothX);
  Serial.print(",");
  Serial.println((int)smoothY);

  delay(10);
}
