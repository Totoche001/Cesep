//Libraries
#include <Keypad.h>//https://github.com/Chris--A/Keypad
#include <Key.h>
//Constants
#define ROWS 4
#define COLS 4
#define DLENGTH 5
#define MAX_ATTEMPTS 5
//Parameters
const char kp4x4Keys[ROWS][COLS] 	= {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
byte rowKp4x4Pin [4] = {11, 10, 9, 8};
byte colKp4x4Pin [4] = {7, 6, 5, 4};
char digits[DLENGTH];
char code[]="1234A";
int attemptCount = 0;
int index=0;
bool lockedOut = false;
bool maxAttemptsMessageShown = false;
//Variables
Keypad kp4x4 	= Keypad(makeKeymap(kp4x4Keys), rowKp4x4Pin, colKp4x4Pin, ROWS, COLS);

void resetSystem() {
  // Reset the system state
  attemptCount = 0;
  index = 0;
  lockedOut = false;
  maxAttemptsMessageShown = false;
  Serial.println(F("System reset! You can try again."));
}

void setup() {
 	//Init Serial USB
 	Serial.begin(9600);
 	Serial.println(F("Initialize System"));
}
void loop() {
 if (!lockedOut) {
    readKp4x4();
  } else {
    if (!maxAttemptsMessageShown) {
      Serial.println(F("Max attempts reached! System locked."));
      maxAttemptsMessageShown = true; // Ensure the message is shown only once
    }
    char resetKey = kp4x4.getKey();
    if (resetKey == '#') {
      resetSystem(); // Call the reset function when # is pressed
    }
  }

}
void readKp4x4() { /* function readKp4x4 */
 	//// Read button states from keypad
 	char customKey = kp4x4.getKey();
 	if (customKey) {
 			//Serial.println(customKey);
      if (customKey == '#') {
      resetSystem(); // Reset the system if # is pressed
      return; // Exit the function after reset
    }
 			digits[index]=customKey;
 			index=index+1;
 			if(index==DLENGTH){
 					Serial.println(digits);
 					index=0;
          attemptCount++;
 					if (strcmp(digits, code) == 0) {
        Serial.println(F("Code correct!"));
        attemptCount = 0;  // Reset attempts after success
        lockedOut = false;
        maxAttemptsMessageShown = false;
      } else {
        Serial.println(F("Incorrect code!"));
        if (attemptCount >= MAX_ATTEMPTS) {
          
          lockedOut = true;
        }
      }
    }
  }
}
