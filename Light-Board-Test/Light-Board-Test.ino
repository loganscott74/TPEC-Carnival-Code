#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial14.h>

// DMD controls the entire display
SoftDMD dmd(1,1);
// "box" provides a text box to automatically write to/scroll the display
DMD_TextBox box(dmd, 0, 2);

void setup() {
  Serial.begin(9600);
  dmd.setBrightness(150);
  dmd.selectFont(Arial14);
  dmd.begin();
}

int score = 123;

void loop() {
  Serial.print("Score: ");
  Serial.print(score);

  int widthLocation = (32 - dmd.stringWidth(String(score))) / 2;

  dmd.drawString(widthLocation,2,String(score));
  //dmd.stringWidth_P(const char *flashStr)

  Serial.println("Width: ");
  Serial.println(dmd.charWidth(char(score)));
  Serial.println("Length: ");
  Serial.println(dmd.stringWidth(String(score)));
  
  //box.print(' ');
  //box.println(score);
  //box.println(' ');
  //box.println(F("..."));
  delay(100);
}