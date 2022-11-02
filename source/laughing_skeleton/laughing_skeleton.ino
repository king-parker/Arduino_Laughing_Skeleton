#include <Servo.h>

const uint8_t JAW_CLOSED = 0;
const uint8_t JAW_OPEN = 90;

uint8_t motionSensor = 2;           // Pin that receives the motion sensor trigger input
uint8_t leftEye = 6;                // Pin the left eye LED is attached to
uint8_t rightEye = 7;               // Pin the right eye LED is attached to
uint8_t jawPin = 5;                 // Pin for the servo jaw Servo signal
uint8_t audio = 9;                  // Pin for the audio board trigger for the laugh
uint8_t checkPlaying = 8;           // Pin to monitor if audio is currently playing

uint8_t motionVal = 0;              // Variable to store the motion sensor status

Servo jaw;                          // Object to write to the jaw servo

void setup() {
  // Set pin modes
  pinMode(leftEye, OUTPUT);
  pinMode(rightEye, OUTPUT);
  pinMode(motionSensor, INPUT);
  pinMode(audio, OUTPUT);
  pinMode(checkPlaying, INPUT);

  // Set initial jaw position
  jaw.attach(jawPin);
  jaw.write(JAW_CLOSED);
}

void loop() {
  motionVal = readMotionSensor();
  updateAudioPlayback();
  updateJawPosition();
  updateEyeLighting();
}

uint8_t readMotionSensor() {
  return digitalRead(motionSensor);
}

bool isAudioPlaying() {
  bool isPlaying = false;
  uint8_t val = digitalRead(checkPlaying);
  // Sound Board Act pin outputs low when audio is playing
  if (val == HIGH) {
    isPlaying = false;
  }
  else {
    isPlaying = true;
  }
  return isPlaying;
}

void updateAudioPlayback() {
  // Audio is not playing and the motion sensor was triggered
  // Trigger audio to start
  if (!isAudioPlaying() && motionVal == HIGH) {
    digitalWrite(audio, LOW);
  }
  // If audio is playing, do not send a trigger to start it
  else if (isAudioPlaying()) {
    digitalWrite(audio, HIGH);
  }
}

void updateJawPosition() {
  if (isAudioPlaying()) {
    jaw.write(JAW_OPEN);
  }
  else {
    jaw.write(JAW_CLOSED);
  }
}

void updateEyeLighting() {
  if (isAudioPlaying()) {
    writeBothEyes(HIGH);
  }
  else {
    writeBothEyes(LOW);
  }
}

void writeBothEyes(uint8_t val) {
  digitalWrite(leftEye, val);
  digitalWrite(rightEye, val);
}