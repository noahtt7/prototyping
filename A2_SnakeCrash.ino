#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins
const int BUTTON1_INPUT_PIN = 7;
const int BUTTON2_INPUT_PIN = 4;

const int BUTTON3_INPUT_PIN = 8;
const int BUTTON4_INPUT_PIN = 9;

const int PLAYER1_LED_PIN = 10;
const int PLAYER2_LED_PIN = 12;

const int DELAY_LOOP_MS = 5; 

bool isMenu = true;
bool isReady = false;

const char strStart[] = "Press any to start!";
int16_t x, y;
uint16_t txtWidth, txtHeight;

// Player1 variables
int xPlayerPos = 10;
int yPlayerPos = 10;
int playerRadius = 1;
int xSpeed = 0;
int ySpeed = 0;
int player1Score = 0;

// Player2 variables
int player2Radius = 1;
int xPlayer2Pos = 100;
int yPlayer2Pos = 10;
int x2Speed = 0;
int y2Speed = 0;
int player2Score = 0;


// Button values
int button1Value = 0;
int button2Value = 0;
int button3Value = 0;
int button4Value = 0;

void setup() {
  pinMode(BUTTON1_INPUT_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_INPUT_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_INPUT_PIN, INPUT_PULLUP);
  pinMode(BUTTON4_INPUT_PIN, INPUT_PULLUP);

  pinMode(PLAYER1_LED_PIN, OUTPUT);
  pinMode(PLAYER2_LED_PIN, OUTPUT);

  Serial.begin(9600);

  initializeOledAndShowStartupScreen();

  showMenu();

  xSpeed = 1;
  x2Speed = -1;

  player1Score = 0;
  player2Score = 0;

}

void loop() {
  button1Value = digitalRead(BUTTON1_INPUT_PIN);
  button2Value = digitalRead(BUTTON2_INPUT_PIN);
  button3Value = digitalRead(BUTTON3_INPUT_PIN);
  button4Value = digitalRead(BUTTON4_INPUT_PIN);

  collisionDetect();

  // First player reaching 5 points wins
  if (player1Score == 5) {
    winMenu(true);
  } else if (player2Score == 5) {
    winMenu(false);
  }

  moveDirection();

  // Update player pos
  xPlayerPos += xSpeed;
  yPlayerPos += ySpeed;

  xPlayer2Pos += x2Speed;
  yPlayer2Pos += y2Speed;

  drawPlayers();

  reverseDirection();
  
  // Render buffer to screen
  _display.display();

  if(DELAY_LOOP_MS > 0){
    delay(DELAY_LOOP_MS);
  }
}

// Function for handling player collisions
// Starts a new round when a player hits another player or wall.
void collisionDetect() {
  // Collision Detect
  // Player 1
  if (xSpeed != 0) {
    if ((xSpeed < 0 && _display.getPixel(xPlayerPos - 2, yPlayerPos) == 1) || (xSpeed > 0 && _display.getPixel(xPlayerPos + 2, yPlayerPos) == 1)) {
      player2Score += 1;
      digitalWrite(PLAYER2_LED_PIN, HIGH);
      resetLevel();
    }
  }  else if (ySpeed != 0) {
    if ((ySpeed < 0 && _display.getPixel(xPlayerPos, yPlayerPos - 2) == 1) || (ySpeed > 0 && _display.getPixel(xPlayerPos, yPlayerPos + 2) == 1)) {
      player2Score += 1;
      digitalWrite(PLAYER2_LED_PIN, HIGH);
      resetLevel();
    }
  }
  // Player 2
  if (x2Speed != 0) {
    if ((x2Speed < 0 && _display.getPixel(xPlayer2Pos - 2, yPlayer2Pos) == 1) || (x2Speed > 0 && _display.getPixel(xPlayer2Pos + 2, yPlayer2Pos) == 1)) {
      player1Score += 1;
      digitalWrite(PLAYER1_LED_PIN, HIGH);
      resetLevel();
    }
  }  else if (y2Speed != 0) {
    if ((y2Speed < 0 && _display.getPixel(xPlayer2Pos, yPlayer2Pos - 2) == 1) || (y2Speed > 0 && _display.getPixel(xPlayer2Pos, yPlayer2Pos + 2) == 1)) {
      player1Score += 1;
      digitalWrite(PLAYER1_LED_PIN, HIGH);
      resetLevel();
    }
  }
}

// Function for changing players' direction on button press.
void moveDirection() {
  // Button pressed (for player1 going LEFT)
  if (button1Value == LOW) {
    // Player going DOWN
    if (ySpeed > 0) {
      ySpeed = 0;
      xSpeed = 1;
    } 
    // Player going RIGHT
    else if (xSpeed > 0) {
      xSpeed = 0;
      ySpeed = -1;
    }
    // Player going UP
    else if (ySpeed < 0) {
      ySpeed = 0;
      xSpeed = -1;
    }
    // Player going LEFT
    else if (xSpeed < 0) {
      xSpeed = 0;
      ySpeed = 1;
    }
    tone(6, 500);
    delay(50);
    noTone(6);
    delay(100);
  }

  // Button pressed (for player1 going RIGHT)
  if (button2Value == LOW) {
    // Player going DOWN
    if (ySpeed > 0) {
      ySpeed = 0;
      xSpeed = -1;
    }
    // Player going RIGHT
    else if (xSpeed > 0) {
      xSpeed = 0;
      ySpeed = 1;
    }
    // Player going UP
    else if (ySpeed < 0) {
      ySpeed = 0;
      xSpeed = 1;
    }
    // Player going LEFT
    else if (xSpeed < 0) {
      xSpeed = 0;
      ySpeed = -1;
    }

    tone(6, 500);
    delay(50);
    noTone(6);
    delay(100);
  }

  // Button pressed (for player2 going RIGHT)
  if (button3Value == LOW) {
    // Player going DOWN
    if (y2Speed > 0) {
      y2Speed = 0;
      x2Speed = -1;
    }
    // Player going RIGHT
    else if (x2Speed > 0) {
      x2Speed = 0;
      y2Speed = 1;
    }
    // Player going UP
    else if (y2Speed < 0) {
      y2Speed = 0;
      x2Speed = 1;
    }
    // Player going LEFT
    else if (x2Speed < 0) {
      x2Speed = 0;
      y2Speed = -1;
    }

    tone(6, 500);
    delay(50);
    noTone(6);
    delay(100);
  }

  // Button pressed (for player2 going LEFT)
  if (button4Value == LOW) {
    // Player going DOWN
    if (y2Speed > 0) {
      y2Speed = 0;
      x2Speed = 1;
    } 
    // Player going RIGHT
    else if (x2Speed > 0) {
      x2Speed = 0;
      y2Speed = -1;
    }
    // Player going UP
    else if (y2Speed < 0) {
      y2Speed = 0;
      x2Speed = -1;
    }
    // Player going LEFT
    else if (x2Speed < 0) {
      x2Speed = 0;
      y2Speed = 1;
    }

    tone(6, 500);
    delay(50);
    noTone(6);
    delay(100);
  }
}

// Draws both players on the display.
void drawPlayers() {
  // Draw player 1
  _display.writePixel(xPlayerPos, yPlayerPos, SSD1306_WHITE);
  _display.writePixel(xPlayerPos-1, yPlayerPos, SSD1306_WHITE);
  _display.writePixel(xPlayerPos+1, yPlayerPos, SSD1306_WHITE);
  _display.writePixel(xPlayerPos, yPlayerPos-1, SSD1306_WHITE);
  _display.writePixel(xPlayerPos, yPlayerPos+1, SSD1306_WHITE);

  _display.writePixel(xPlayerPos+1, yPlayerPos+1, SSD1306_WHITE);
  _display.writePixel(xPlayerPos-1, yPlayerPos+1, SSD1306_WHITE);
  _display.writePixel(xPlayerPos+1, yPlayerPos-1, SSD1306_WHITE);
  _display.writePixel(xPlayerPos-1, yPlayerPos-1, SSD1306_WHITE);

  // Draw player 2
  _display.writePixel(xPlayer2Pos, yPlayer2Pos, SSD1306_WHITE);
  _display.writePixel(xPlayer2Pos-1, yPlayer2Pos, SSD1306_WHITE);
  _display.writePixel(xPlayer2Pos+1, yPlayer2Pos, SSD1306_WHITE);
  _display.writePixel(xPlayer2Pos, yPlayer2Pos-1, SSD1306_WHITE);
  _display.writePixel(xPlayer2Pos, yPlayer2Pos+1, SSD1306_WHITE);

  _display.writePixel(xPlayer2Pos+1, yPlayer2Pos+1, SSD1306_WHITE);
  _display.writePixel(xPlayer2Pos-1, yPlayer2Pos+1, SSD1306_WHITE);
  _display.writePixel(xPlayer2Pos+1, yPlayer2Pos-1, SSD1306_WHITE);
  _display.writePixel(xPlayer2Pos-1, yPlayer2Pos-1, SSD1306_WHITE);

}

// Reverses the direction of a player if they hit a wall,
// effectively making the player lose
void reverseDirection() {
  // Check for ball bounce
  if(xPlayerPos - playerRadius <= 0 || xPlayerPos + playerRadius >= _display.width()){
    xSpeed = xSpeed * -1; // reverse x direction
  }
  
  if(yPlayerPos - playerRadius <= 0 || yPlayerPos + playerRadius >= _display.height()){
    ySpeed = ySpeed * -1; // reverse y direction
  }

  if(xPlayer2Pos - player2Radius <= 0 || xPlayer2Pos + player2Radius >= _display.width()){
    x2Speed = x2Speed * -1; // reverse x direction
  }
  
  if(yPlayer2Pos - player2Radius <= 0 || yPlayer2Pos + player2Radius >= _display.height()){
    y2Speed = y2Speed * -1; // reverse y direction
  }
}

// Shows the start menu.
void showMenu() {

  _display.setTextSize(2);
  _display.setTextColor(WHITE, BLACK);
  _display.setCursor(5, 0);
  _display.println("SnakeCrash");
  _display.display();

  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);
  _display.getTextBounds(strStart, 0, 0, &x, &y, &txtWidth, &txtHeight);
  _display.setCursor(_display.width() / 2 - txtWidth / 2, _display.height() / 2 - txtHeight / 2);
  _display.println(strStart);
  _display.display();

  while (isMenu) {
    button1Value = digitalRead(BUTTON1_INPUT_PIN);
    button2Value = digitalRead(BUTTON2_INPUT_PIN);
    button3Value = digitalRead(BUTTON3_INPUT_PIN);
    button4Value = digitalRead(BUTTON4_INPUT_PIN);

    // Breaks out of menu on button press
    if (button1Value == LOW || button2Value == LOW || button3Value == LOW || button4Value == LOW) {
      isMenu = false;
      tone(6, 500);
      delay(50);
      noTone(6);
      break;
    }

  }

  _display.clearDisplay();
}

// Resets the game.
void resetLevel() {
  _display.clearDisplay();

  _display.setTextSize(0.9);
  _display.setTextColor(WHITE, BLACK);
  _display.setCursor(0, 0);
  _display.println("Player 1 " + String(player1Score) +"-" + player2Score + " Player 2");
  _display.display();
  delay(1000);
  _display.clearDisplay();

  // Reset LEDs
  digitalWrite(PLAYER1_LED_PIN, LOW);
  digitalWrite(PLAYER2_LED_PIN, LOW);

  // Reset player positions
  xPlayerPos = 10;
  yPlayerPos = 10;
  xPlayer2Pos = 100;
  yPlayer2Pos = 10;
}

// Shows a menu on a player win, gives option to restart
void winMenu(bool player1Wins) {

  digitalWrite(5, HIGH);
  delay(500);
  digitalWrite(5, LOW);

  // Reset scores
  player1Score = 0;
  player2Score = 0;

  _display.clearDisplay();

  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);
  _display.setCursor(25, 0);

  if (player1Wins) {
    _display.println("Player 1 wins!");
  } else {
    _display.println("Player 2 wins!");
  }

  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);
  _display.setCursor(45, 30);
  _display.println("Reset?");

  _display.drawRect(42, 24, 40, 20, SSD1306_WHITE);

  _display.display();

  bool showResetButton = true;

  while (!isReady) {
    button1Value = digitalRead(BUTTON1_INPUT_PIN);
    button2Value = digitalRead(BUTTON2_INPUT_PIN);
    button3Value = digitalRead(BUTTON3_INPUT_PIN);
    button4Value = digitalRead(BUTTON4_INPUT_PIN);

    // Restarts game on button press
    if (button1Value == LOW || button2Value == LOW || button3Value == LOW || button4Value == LOW) {
      isReady = true;
      tone(6, 500);
      delay(50);
      noTone(6);
      break;
    }

  }

  digitalWrite(PLAYER1_LED_PIN, LOW);
  digitalWrite(PLAYER2_LED_PIN, LOW);

  isReady = false;
  _display.clearDisplay();
}

/**
 * Call this from setup() to initialize the OLED screen
 */
void initializeOledAndShowStartupScreen(){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    //Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  _display.clearDisplay();

  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);
  _display.setCursor(0, 0);
  _display.println("Screen initialized!");
  _display.display();
  delay(500);
  _display.clearDisplay();
}