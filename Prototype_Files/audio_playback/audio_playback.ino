#include "SD.h"
#define SD_ChipSelectPin 10
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm audio;

void setup()
{
  audio.speakerPin=9;
  Serial.begin(9600);
  if(!SD.begin(SD_ChipSelectPin))
  {
      Serial.println("SD fail");
      return;
  }
  audio.setVolume(5);
  audio.play("laugh.wav");
}

void loop() {
  // put your main code here, to run repeatedly:

}
