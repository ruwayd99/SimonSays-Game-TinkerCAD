/* Author: Mir Ruwayd Afeef
 * Student Number: 71924070
 * Email: mafeef@student.ubc.ca
 * Purpose: Interactive simon says game! User gets to imitiate pattern displayed through
 *          blinking of LEDs.
 */

#define LED_LOSE 13
#define LEDR 12
#define LEDY 11
#define LEDB 10
#define LEDG 9
#define LED_WIN 8

#define pb_red 7
#define pb_yellow 6
#define pb_blue 5
#define pb_green 4

void off(void)
{ // turns off the four lights
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDY, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, LOW);
}
void start_game(void)
{ // turning on LED for 2 secs when game starts
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDY, HIGH);
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDG, HIGH);
  delay(2000);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDY, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, LOW);
  delay(2000);
}

void display_LED(int num)
{ // displaying LED sequence to the user
  if (num == 0)
  {
    digitalWrite(LEDR, HIGH);
    delay(1000);
    digitalWrite(LEDR, LOW);
    delay(1000);
  }
  else if (num == 1)
  {
    digitalWrite(LEDY, HIGH);
    delay(1000);
    digitalWrite(LEDY, LOW);
    delay(1000);
  }
  else if (num == 2)
  {
    digitalWrite(LEDB, HIGH);
    delay(1000);
    digitalWrite(LEDB, LOW);
    delay(1000);
  }
  else
  {
    digitalWrite(LEDG, HIGH);
    delay(1000);
    digitalWrite(LEDG, LOW);
    delay(1000);
  }
}
int button_press(void)
{ // checking if buttons are pressed
  int button_check = 0;
  if (digitalRead(pb_red) == HIGH || digitalRead(pb_yellow) == HIGH || digitalRead(pb_blue) == HIGH || digitalRead(pb_green) == HIGH)
  {
    button_check = 1; // this will be one when any button pressed
  }
  return button_check;
}
int push_button(void)
{
  int user_button;  // number allocated based on the button user pressed
  int overtime = 0; // verifier that will be one to initiate timeout sequence
  if (button_press() == 0)
  {                                       // if user doesn't press button for 10 secs, user loses
    unsigned long startmillis = millis(); // record start time
    while (button_press() == 0 && overtime == 0)
    {
      if (millis() - startmillis > 10000)
      {                  // comparing time elapsed with the given condition
        user_button = 4; // allocating a number other than 0-3 inclusive so that user loses
        overtime = 1;
      }
    }
  }
  if (digitalRead(pb_red) == HIGH && overtime == 0)
  { // will run if overtime is 0 or user pressed button within 10 secs
    while (digitalRead(pb_red) == HIGH)
    {
      digitalWrite(LEDR, HIGH);
    }
    user_button = 0;
  }
  else if (digitalRead(pb_yellow) == HIGH && overtime == 0)
  {
    while (digitalRead(pb_yellow) == HIGH)
    {
      digitalWrite(LEDY, HIGH);
    }
    user_button = 1;
  }
  else if (digitalRead(pb_blue) == HIGH && overtime == 0)
  {
    while (digitalRead(pb_blue) == HIGH)
    {
      digitalWrite(LEDB, HIGH);
    }
    user_button = 2;
  }
  else if (digitalRead(pb_green) == HIGH && overtime == 0)
  {
    while (digitalRead(pb_green) == HIGH)
    {
      digitalWrite(LEDG, HIGH);
    }
    user_button = 3;
  }
  off();
  return user_button; // returns the number corresponding to the button user pressed (4 if pressed nothing for 10 secs)
}

void display_result(int verifier)
{ // LED sequence to display result
  if (verifier == 0)
  { // winning sequence
    for (int index1 = 0; index1 < 4; index1++)
    {
      digitalWrite(LED_WIN, HIGH);
      delay(500);
      digitalWrite(LED_WIN, LOW);
      delay(500);
    }
  }
  else
  { // losing sequence
    for (int index2 = 0; index2 < 4; index2++)
    {
      digitalWrite(LED_LOSE, HIGH);
      delay(500);
      digitalWrite(LED_LOSE, LOW);
      delay(500);
    }
  }
  delay(1500); // gives user time before next game starts
}
// initialization
void setup()
{
  pinMode(LED_LOSE, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LED_WIN, OUTPUT);
  pinMode(pb_red, INPUT);
  pinMode(pb_yellow, INPUT);
  pinMode(pb_blue, INPUT);
  pinMode(pb_green, INPUT);
  randomSeed(analogRead(0)); // to generate random numbers
}

// main program loop
void loop()
{
  int condition = 0;
  int start = 0;
  int check_array[5]; // 5 random numbers stored in this array

  for (int index1 = 0; index1 < 5; index1++)
  { // adding random num (0-3 inclusive) to check_array
    check_array[index1] = random(0, 4);
  }

  start_game(); // start_grame sequence

  while (start < 5 && condition == 0)
  { // loops runs for 5 levels or until user makes mistake

    int level = 0; // level will increment each time until 5 LEDS blinked in the last level
    while (level <= start && button_press() == 0)
    {                                  // second condition so that user can press button while LED sequence displaying, thus stopping the sequence
      display_LED(check_array[level]); // displayed LED according to number in the list that corresponds to the buttons
      level++;
    }

    int index2 = 0;
    while(index2 <= start && condition ==0)
    {
      int user_array[] = {5, 5, 5, 5, 5}; // array stores user input; new array created each time with dummy values (5 in this case) other than 0-3 to ensure user doesn't make mistake each level
      user_array[index2] = push_button(); // button number that user pressed is inserted to the array
      if (check_array[index2] != user_array[index2])
      {                // if user_array doesn't match check_array user made a mistake
        condition = 1; // condition 1 so user lost
      }
      index2++;
    }
    delay(1000); // delays 1 sec before while loop continues again
    start++;
  }
  display_result(condition); // displays result based on value of condition (win if 0 and lose if 1)
}
