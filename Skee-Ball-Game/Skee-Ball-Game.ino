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
int singleButtonPin = 3;
//Pin for multiplay button
int multiButtonPin = 10;

//The current score of the first player
int p1Score = 0;
//The current score of the second player
int p2Score = 0;
//Is a game currently being played
boolean isPlaying = false;
//Is the game in multiplayer mode?
boolean isMultiPlayer = false;
//The current player player
int currentPlayer = 1;

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
  pinMode(singleButtonPin , INPUT);
  pinMode(multiButtonPin , INPUT);

  //Display Setup
  dmd.setBrightness(255);
  dmd.selectFont(Arial14);
  dmd.begin();
}


//Updates the p1Score board to show the current p1Score
void updateScoreBoard() {
  //Calculates the width needed to center it on the screen
  if (currentPlayer == 1 || isMultiPlayer == false) {
    int widthLocation = (32 - dmd.stringWidth(String(p1Score))) / 2;
    dmd.drawString(widthLocation,2,String(p1Score));
  } else if (currentPlayer == 2) {
    int widthLocation = (32 - dmd.stringWidth(String(p2Score))) / 2;
    dmd.drawString(widthLocation,2,String(p2Score));
  }
}

//Updates the p1Score by adding the given value to the p1Score total
void updateScore(int addScore) {
  //Checks if isFlagged is false before running, prevents the p1Score from constantly added to itself
  if (isFlagged == false) {
    if (currentPlayer == 1 || isMultiPlayer == false) {
      isFlagged = true;
      p1Score += addScore;
      Serial.println(p1Score);
      dmd.clearScreen(); //Clears the screen to prevent weird numbers from showing on update
      updateScoreBoard();
      delay(100);
    } else if (currentPlayer == 2) {
      isFlagged = true;
      p2Score += addScore;
      Serial.println(p2Score);
      dmd.clearScreen();
      updateScoreBoard();
      delay(100);
    }
  }
}

//Resets the p1Score to 0
void resetScore() {
  //Prevents the screen from refreshing too fast
  //Maybe use isPlaying here instead?
  if (p1Score != 0 || p2Score != 0) {
    p1Score = 0;
    p2Score = 0;
    isMultiPlayer = false;
    currentPlayer = 1;
    Serial.println(p1Score);
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
  int button0Status = digitalRead(singleButtonPin);
  int multiButtonStatus = digitalRead(multiButtonPin);
  
  //Updates the p1Score based on the status of the sensor(s)
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
    Prevents the sensor from adding to the p1Score multiple times when it is only covered once
    */
    isFlagged = false;
    delay(100);
  }
  
  //Resets the p1Score when the button is pressed
  if (button0Status == 1 && !isPlaying) {
    resetScore();
    isPlaying = true;
    //Starts single player game
    //Servo moter to open
  }

  if (multiButtonStatus == 1 && !isPlaying) {
    resetScore();
    isPlaying = true;
    isMultiPlayer = true;
    //Start multiplay game
    //Servo moter to open
  }
}
