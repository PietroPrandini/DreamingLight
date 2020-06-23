/*
 * Copyright 2020 Pietro Prandini
 * 
 * This file is part of DreamingLight.
 *
 * DreamingLight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DreamingLight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DreamingLight.  If not, see <https://www.gnu.org/licenses/>.
 */

const int buttonPin = 2;                  // the number of the button pin
const int lightPin = 13;                  // the number of the light pin
const int PUSHED = HIGH;                  // useful variable for understanding button
const int NOT_PUSHED = LOW;               // useful variable for understanging button
const int LIGHT_ON = HIGH;                // useful variable for understanging light states
const int LIGHT_OFF = LOW;                // useful variable for understanging light states
const unsigned long lightTime = 3600000;  // 1 hour of light time in milliseconds

int lightState = LIGHT_OFF;       // the current state of the light pin
int buttonState = NOT_PUSHED;     // the current reading from the button pin
int buttonLastState = NOT_PUSHED; // the last button state

unsigned long startLight = 0;     // variable for storing the time when the light starts
unsigned long lastPush = 0;       // variable for storing the last push of the button
unsigned long debounce = 200;     // the debounce time, increase if the output flickers (ms)

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LIGHT_OFF); // light off by default
}

void loop()
{
  // read the button signal
  buttonState = digitalRead(buttonPin);

  /* 
   * Checks if the button is pushed and before it was not pushed
   * with a margin of time (debounce) to avoid circuit flickering
   */
  
  if (
    buttonState == PUSHED && buttonLastState == NOT_PUSHED &&
    ((unsigned long) (millis() - lastPush)) > debounce
  ) {

    lastPush = millis(); // stores the last push time

    /*
     * If the light is ON and the button is pushed
     * the light will be turned off.
     * If the light is OFF and the button is pushed 
     * the light will be turned on.
     * In this last case the time is stored for a future use.
     */

    if      (lightState == LIGHT_ON)   { lightState = LIGHT_OFF; }
    else if (lightState == LIGHT_OFF)  { lightState = LIGHT_ON; startLight = millis(); }
    
  } else {

    /*
     * The button is not pushed, if the light is on,
     * checks if the working time is expired,
     * if yes, turns off the light.
     */
    
    if (
      lightState == LIGHT_ON &&
      ((unsigned long) (millis() - startLight)) > lightTime
    ) { lightState = LIGHT_OFF; }

  }

  digitalWrite(lightPin, lightState); // write the light state
  buttonLastState = buttonState; // stores the last button state
  
}
