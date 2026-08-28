#include "SR04.h"
#include "pitches.h"

int trig_pin = 12;
int echo_pin = 11;

int buzzer_pin = 3;

int volume_button_pin = 8;

int record_button_pin = 6;

// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_B0};

int all_notes[] = {
  NOTE_B0, NOTE_C1, NOTE_CS1, NOTE_D1, NOTE_DS1, NOTE_E1, NOTE_F1, NOTE_FS1, NOTE_G1, NOTE_GS1, NOTE_A1, NOTE_AS1, NOTE_B1,
  NOTE_C2, NOTE_CS2, NOTE_D2, NOTE_DS2, NOTE_E2, NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2, NOTE_B2,
  NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
  NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
  NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
  NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
  NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
  NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8
};

int duration = 500;  // 500 miliseconds

SR04 sensor = SR04(echo_pin, trig_pin);

long distance;

// volume button
bool lastVolumeButtonRead = HIGH;
bool volumeButtonPressed = false;

// note tracker
int currentNote = -1; // -1 = "no note yet"
unsigned long noteStartTime = 0;
const int NOTE_TOLERANCE = 5;  // Hz buffer to see if the note is the same

// record button
bool lastRecordButtonRead = HIGH;
bool recordButtonPressed = false;

enum State {
  THEREMIN_STATE,
  RECORD_STATE,
  PLAY_STATE
};

State currentState = THEREMIN_STATE;

enum ButtonType {
  VOLUME,
  RECORD,
  PLAY,
};


void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer_pin, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  pinMode(volume_button_pin, INPUT_PULLUP);
  pinMode(record_button_pin, INPUT_PULLUP);


}

void noteDurationLogger(int currentNote, int heldFor) {
  Serial.print("note: ");
  Serial.print(currentNote);
  Serial.print(" Hz, duration: ");
  Serial.print(heldFor);
  Serial.println(" ms");

}

bool buttonToggle(ButtonType button, bool buttonReading, bool lastButtonRead, bool isButtonPressed) {
  if (buttonReading == LOW && lastButtonRead == HIGH) {
    Serial.print(button);
    Serial.println(" button pressed");
    isButtonPressed = !isButtonPressed;
  }
  return isButtonPressed;
}

void loop() {

  // loop setup

  bool volumeReading = digitalRead(volume_button_pin);

  // ------ on / off volume button control ------
  // low means the button was just pressed
  if (volumeReading == LOW && lastVolumeButtonRead == HIGH) {
    Serial.println("Volume button pressed");
    volumeButtonPressed = !volumeButtonPressed;
  }
  lastVolumeButtonRead = volumeReading;
  // ----------------------------------------------

  bool recordButtonReading = digitalRead(record_button_pin);
  if (recordButtonReading == LOW && lastRecordButtonRead == HIGH) {
    Serial.println("Record button pressed");
    recordButtonPressed = !recordButtonPressed;
  }
  lastRecordButtonRead = recordButtonReading;

  distance = sensor.Distance();
  //Serial.println(distance);

  // NOTE_B5 = higher pitch
  //NOTE_F1 = lower pitch
  int frequency = map(distance, 0, 100, NOTE_B5, NOTE_F1);


  // states 
  switch(currentState) {
    case THEREMIN_STATE:
      if (volumeButtonPressed) {
        noTone(buzzer_pin);
      } else {
        tone(buzzer_pin, frequency);
      }
      break;
    case RECORD_STATE:
      break;
    case PLAY_STATE:
      break;
  }

  // if (volumeState) {
  //   noTone(buzzer_pin);
  // } else {
  //   tone(buzzer_pin, frequency);
    
  //   if (currentNote == -1) {
  //     // first reading ever
  //     currentNote = frequency;
  //     noteStartTime = millis();
  //   } else if (abs(frequency - currentNote) > NOTE_TOLERANCE) {
  //     // how long the PREVIOUS note lasted
  //     unsigned long heldFor = millis() - noteStartTime;
  //     noteDurationLogger(currentNote, heldFor);

  //     // start timing the new note
  //     currentNote = frequency;
  //     noteStartTime = millis();
  //   }
  // }
}
