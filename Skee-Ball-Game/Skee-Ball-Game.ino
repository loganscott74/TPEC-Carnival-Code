#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial14.h>


//A sensor pin (NOTE: was 9 but that is used by the sceen now)
int sensorTrigPin0 = 5;
//int sensorTrigPin1 = ?;
//int sensorTrigPin2 = ?;
//int sensorTrigPin3 = ?;
//int sensorTrigPin4 = ?;
//TODO: Maybe we should renames theses based on what pin they are??

//The button pin
int buttonTrigPin = 3;

//The current score
int score = 0;
//Was the senser covered last check?
boolean isFlagged = false;

//MARK: Items for the display
//DMD controls the entire display
SoftDMD dmd(1,1);
//"box" provides a text box to automatically write to/scroll the display
DMD_TextBox box(dmd, 0, 2);

//Setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Senser Pins Setup
  pinMode(sensorTrigPin0 , INPUT);
  //pinMode(trigPin2, INPUT);
  //pinMode(trigPin3, INPUT);
  //pinMode(trigPin4, INPUT);
  //pinMode(trigPin5, INPUT);

  //Button Pin Setup
  pinMode(buttonTrigPin , INPUT);

  //Display Setup
  dmd.setBrightness(255);
  dmd.selectFont(Arial14);
  dmd.begin();
}


//Updates the score board to show the current score
void updateScoreBoard() {
  //Calculates the width needed to center it on the screen
  int widthLocation = (32 - dmd.stringWidth(String(score))) / 2;
  dmd.drawString(widthLocation,2,String(score));
}

//Updates the score by adding the given value to the score total
void updateScore(int addScore) {
  //Checks if isFlagged is false before running, prevents the score from constantly added to itself
  if (isFlagged == false) {
    isFlagged = true;
    score += addScore;
    Serial.println(score);
    dmd.clearScreen(); //Clears the screen to prevent weird numbers from showing on update
    updateScoreBoard();
    delay(100);
  }
}

//Resets the score to 0
void resetScore() {
  //Prevents the screen from refreshing too fast
  if (score != 0) {
    score = 0;
    Serial.println(score);
    dmd.clearScreen(); //Clears the screen. Without this, the screen would not update to 0
    updateScoreBoard();
    delay(100);
  }
}

//Loop
void loop() {
  // put your main code here, to run repeatedly:
  updateScoreBoard();

  //Not sure if these are still needed or what they are for
  //digitalWrite(trigPin, INPUT);
  //digialWrite(echoPin, OUTPUT);
  
  //Sensor status
  int sensor0Status = digitalRead(sensorTrigPin0);
  //int sensorStatus2 = digitalRead(sensorTrigPin1);
  //int sensorStatus3 = digitalRead(sensorTrigPin2);
  //int sensorStatus4 = digitalRead(sensorTrigPin3);
  //int sensorStatus5 = digitalRead(sensorTrigPin4);

  //Button status
  int button0Status = digitalRead(buttonTrigPin);
  
  //Updates the score based on the status of the sensor(s)
  if (sensor0Status == 0) {
    updateScore(100);
  } /*else if (sensor1Status == 0) {
    updateScore(200);
  } else if (sensor2Status == 0) {
    updateScore(300);
  } else if (sensor3Status == 0) {
    updateScore(400);
  } else if (sensor4Status == 0) {
    updateScore(500);
  }*/ else if (isFlagged == true) {
    /*
    This code will only run if all the sensors have a status of 1 and isFlagged is true
    Prevents the sensor from adding to the score multiple times when it is only covered once
    */
    isFlagged = false;
    delay(100);
  }

  //Resets the score when the button is pressed
  if (button0Status == 1) {
    resetScore();
  }
}
