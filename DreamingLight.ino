/*
 * Copyright 2020 Pietro Prandini.
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

/**
 * @brief The number of the button pin.
 */
const int BUTTON_PIN = 2;

/**
 * @brief The number of the light pin.
 */
const int LIGHT_PIN = 13;

/**
 * @brief Useful variable for understanding button pushed state.
 */
const int PUSHED = HIGH;

/**
 * @brief Useful variable for understanding button NOT pushed state.
 */
const int NOT_PUSHED = LOW;

/**
 * @brief Useful variable for understanding light ON state.
 */
const int LIGHT_ON = HIGH;

/**
 * @brief Useful variable for understanding light OFF state.
 */
const int LIGHT_OFF = LOW;

/**
 * @brief Light ON default time in milliseconds.
 * 
 * Note: 1 hour is composed by 60 minutes,
 * 1 minute is composed by 60 seconds,
 * 1 seconds is 1000 milliseconds. 
 */
const unsigned long LIGHT_TIME_ms = 3600000;

/**
 * @brief The debounce time in milliseconds. 
 * 
 * Note: increase if the output flickers.
 */
const unsigned long DEBOUNCE_ms = 200;

/**
 * @brief Variable for the light state.
 */
int light_state = LIGHT_OFF;

/**
 * @brief Variable for the button state.
 */
int button_state = NOT_PUSHED;

/**
 * @brief Variable for the button last state.
 */
int button_last_state = NOT_PUSHED;

/**
 * @brief Variable for storing the time when the light start in milliseconds.
 */
unsigned long start_light_ms = 0;

/**
 * @brief Variable for storing the last push of the button.
 */
unsigned long last_push_ms = 0;

/**
 * @brief Sets up the used pins and the default variables settings.
 */
void setup()
{
    // Used pins
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LIGHT_PIN, OUTPUT);

    // Defaults
    digitalWrite(LIGHT_PIN, LIGHT_OFF);
    light_state = LIGHT_OFF;
    button_state = NOT_PUSHED;
    button_last_state = NOT_PUSHED;
    start_light_ms = 0;
    last_push_ms = 0;
}

/**
 * @brief Controls a button and a light to realize a dreaming light
 * 
 * Note: see the description of the project for more details.
 */
void loop()
{
    // reads the button signal
    button_state = digitalRead(BUTTON_PIN);

    /* 
     * Checks if the button is pushed and before it was not pushed
     * with a margin of time from the last operation
     * to avoid circuit flickering.
     */
  
    if (
        button_state == PUSHED && button_last_state == NOT_PUSHED &&
        ((unsigned long) (millis() - last_push_ms)) > DEBOUNCE_ms
    ) {
  
        last_push_ms = millis(); // stores the last pushed button time
    
        /*
         * If the light is ON and the button is pushed
         * the light will be turned off.
         * If the light is OFF and the button is pushed 
         * the light will be turned on.
         * In this last case the time is stored for a future use.
         */
    
        if      (light_state == LIGHT_ON)  { light_state = LIGHT_OFF; }
        else if (light_state == LIGHT_OFF) {
            light_state = LIGHT_ON;
            start_light_ms = millis();
        }
      
    } else if (button_state == NOT_PUSHED && button_last_state == NOT_PUSHED) {
  
        /*
         * The button is not pushed.
         * If the light is on, checks if the working time is expired,
         * if yes, turns off the light.
         */
        
        if (
            light_state == LIGHT_ON &&
            ((unsigned long) (millis() - start_light_ms)) > LIGHT_TIME_ms
        ) { light_state = LIGHT_OFF; }
  
    }
  
    digitalWrite(LIGHT_PIN, light_state); // writes the light state
    button_last_state = button_state; // stores the last button state
  
}
