// BIOE 448 Lab 7
// Daniela Najmias, Alice Zhou


#include "thingProperties.h"
const int sensor_pin = 0;
int pulse_signal = 0;
float BPM = 0;
int pulse_period = 0;
int upper_threshold = 830; //or your threshold of choice
int lower_threshold = 810; //or your threshold of choice
int counter = 0;

//float BPM = 0.0;
bool ignore = false;

// Declaring storage data
bool any_peak_detected = false;
bool first_peak_detected = false;
unsigned long first_pulse_time = 0;
unsigned long second_pulse_time = 0;


void setup() {
  Serial.begin(9600);

  delay(1500);
  initProperties();
  //Connect to cloud and get info/errors
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  
  //Wait for cloud connection
  while (ArduinoCloud.connected() != 1) {
    ArduinoCloud.update();
    delay(500);
  }
}

void loop() {
  // Code from lab 6
  pulse_signal = analogRead(sensor_pin);
  Serial.println(pulse_signal);

  delay(75);
  //delay(50);

  if (pulse_signal > upper_threshold && any_peak_detected ==
  false) {
    any_peak_detected = true;

    if (first_peak_detected == false) {
      first_pulse_time = millis();
      first_peak_detected = true;
      } else {
      second_pulse_time = millis();
      pulse_period = second_pulse_time - first_pulse_time;
      //Serial.println("detected");
      //Serial.println("time: " + pulse_period);
      first_peak_detected = false;
      BPM = 60000 / pulse_period;
      //Serial.println(BPM);
      
      }
  

  } 
 

  if (pulse_signal < lower_threshold) {
    any_peak_detected = false;
   
  }

  // New IOT code
  counter++;
  if (counter > 200){
    ArduinoCloud.update();
    Serial.println(BPM);
    counter = 0;
  }

}