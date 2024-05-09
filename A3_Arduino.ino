const int X_ANALOG_INPUT_PIN = A0;
const int Y_ANALOG_INPUT_PIN = A1;
const int WIDTH_INPUT_PIN = A2;
const int JOYSTICK_BUTTON_PIN = 2;

const int COLOR_BUTTON_PIN = 3;

const int MAX_ANALOG_VAL = 1023; 

const long BAUD_RATE = 115200;

float _x;
float _y;

void setup() {
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  pinMode(COLOR_BUTTON_PIN, INPUT_PULLUP);
  pinMode(WIDTH_INPUT_PIN, INPUT);
  digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);
  Serial.begin(BAUD_RATE);
}

void loop() {
  // Read the analog values
  int xAnalogVal = analogRead(X_ANALOG_INPUT_PIN);
  int yAnalogVal = analogRead(Y_ANALOG_INPUT_PIN);
  float widthVal = analogRead(WIDTH_INPUT_PIN);

  // Calculate normalized x,y location
  _x = xAnalogVal / (float)MAX_ANALOG_VAL;
  _y = yAnalogVal / (float)MAX_ANALOG_VAL;

  // Clear screen when button pressed
  if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
    Serial.println("clear");
  }

  // Change color mode if button pressed
  if (digitalRead(COLOR_BUTTON_PIN) == LOW) {
    Serial.println("color");
  }

  // Transmit over serial as comma-separated string
  Serial.print(_x, 4);
  Serial.print(", ");
  Serial.print(_y, 4);
  Serial.print(", ");
  Serial.println(widthVal, 4);
  
  delay(10);
}