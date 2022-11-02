#include "SD.h"
// #include "TMRpcm.h"
#include "SPI.h"
#include <Servo.h>

const uint8_t JAW_CLOSED = 0;
const uint8_t JAW_OPEN = 90;
const uint8_t JAW_CYCLE = 300;
const uint8_t HALF_CYCLE = JAW_CYCLE / 2;

uint8_t SDChipSelectPin = 10;
uint8_t motionSensor = 2;              // the pin that the motion sensor is atteched to
uint8_t leftEye = 3;
uint8_t rightEye = 4;                 // the pin that the LED is atteched to
uint8_t jawPin = 5;

uint8_t state = LOW;             // by default, no motion detected
uint8_t motionVal = 0;                 // variable to store the motion sensor status (value)
bool isReadSD = false;
bool isAudioPlaying = false;
uint32_t jawTimeStart = 0;

Servo jaw;
// TMRpcm audio;

void setup() {
  // Set pin modes
  pinMode(leftEye, OUTPUT);
  pinMode(rightEye, OUTPUT);
  pinMode(motionSensor, INPUT);    // initialize motion sensor as an input

  // Set up connection to SD card and record if connected properly
  isReadSD = SD.begin(SDChipSelectPin);

  // Set up speaker
  // audio.speakerPin = 9;
  // audio.setVolume(5);

  // Set initial jaw position
  jaw.attach(jawPin);
  jaw.write(JAW_CLOSED);

  // TODO: Remove
  // Serial.begin(9600);
}

void loop() {
  motionVal = readMotionSensor();
  updateAudioPlayback();
  updateJawPosition();
}

uint8_t readMotionSensor() {
  uint8_t val = digitalRead(motionSensor);
  if (val) {
    writeBothEyes(HIGH);   // turn LED ON
    delay(500);                // delay 100 milliseconds 
  }
  else {
    writeBothEyes(LOW);   // turn LED OFF
    delay(500);                // delay 200 milliseconds 
  }
  return val;
}

void writeBothEyes(uint8_t val) {
  digitalWrite(leftEye, val);
  digitalWrite(rightEye, val);
}

void updateAudioPlayback() {
  if (isAudioPlaying && motionVal == LOW) {
    // audio.disable();
    isAudioPlaying = false;
  }
  else if (!isAudioPlaying && motionVal == HIGH) {
    // audio.disable();
    // audio.play("laugh.wav");
    isAudioPlaying = true;
    jawTimeStart = millis();
    // Serial.print("Time start: ");
    // Serial.println(jawTimeStart);
  }
}

void updateJawPosition() {
  if (isAudioPlaying) {
    int pos = 0;
    for (int loop = 0; loop < 15; loop += 1) {
      for (pos = 0; pos <= 90; pos += 90) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        jaw.write(pos);              // tell servo to go to position in variable 'pos'
        delay(150);                       // waits 15 ms for the servo to reach the position
      }
      for (pos = 90; pos >= 0; pos -= 90) { // goes from 180 degrees to 0 degrees
        jaw.write(pos);              // tell servo to go to position in variable 'pos'
        delay(150);                       // waits 15 ms for the servo to reach the position
      }
    }
    // uint32_t elapsedTime = millis() - jawTimeStart;
    // // Serial.println("Elapsed Time: ");
    // // Serial.println(elapsedTime);
    // if (elapsedTime % JAW_CYCLE < HALF_CYCLE) {
    //   jaw.write(JAW_OPEN);
    //   // Serial.println("Jaw Open");
    // }
    // else{
    //   jaw.write(JAW_CLOSED);
    //   // Serial.println("Jaw Closed");
    // }
  }
  else {
    jaw.write(JAW_CLOSED);
  }
}