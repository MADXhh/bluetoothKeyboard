#include <Keyboard.h>


void AskForDebugMode();
void SetKeyboardLayout();

bool isDebugEnabled;
unsigned long timeForQuestionInMs;
const unsigned int TimeBetweenKeypressesInMs = 35;
unsigned long NextKeypressTimeinMs;
char LastChar;

void setup()
{
  delay(500);
  
  timeForQuestionInMs = millis() + 1500;

  Serial1.begin(9600); // Bluetooth data mode baud
  
  AskForDebugMode();

  if (isDebugEnabled)
  {
    Serial.begin(115200);
  }

  SetKeyboardLayout();
  
  LastChar = '\n';


  NextKeypressTimeinMs = millis();
}

void loop()
{
  if(Serial.available())
  {
    Serial1.write(Serial.read());
  }
  
  if (Serial1.available() > 0 && NextKeypressTimeinMs < millis())
  {
    char input = Serial1.read();
    
    if ((input == '\n') || (input == '\r'))
    {
      if(input != '\n' || LastChar != '\r')
      {        
        Keyboard.write(KEY_RETURN);
        if (isDebugEnabled)
        {
          Serial.println("");
        }
      }
    }
    else
    {     
      Keyboard.write(input);
      if (isDebugEnabled)
      {
        Serial.print(input);
      }
    }

    NextKeypressTimeinMs = millis() + TimeBetweenKeypressesInMs;
    LastChar = input;
  }
}

void AskForDebugMode()
{
  String input;
  Serial1.println("");

  while (Serial1.available() < 1)
  {
    if (timeForQuestionInMs < millis())
    {
      Serial1.println("Enable Debug mode? (Y/n)");
      timeForQuestionInMs += 5500;
    }
  }

  input = Serial1.readString();
  Serial1.println(input);
  input.remove(1); // remove everthing after last char
  Serial1.println(input);
  if (input == "N" || input == "n")
  {
    isDebugEnabled = false;
    Serial1.println("Debug mode off!");
  }
  else
  {
    isDebugEnabled = true;
    Serial1.println("Debug mode on!");
  }
}

void SetKeyboardLayout()
{
  String input;
  Serial1.println("");
  
  while (Serial1.available() < 2)
  {
    if (timeForQuestionInMs < millis())
    {
      Serial1.println("Which Keybord layout? DE or US");
      timeForQuestionInMs += 5500;
    }
  }

  input = Serial1.readString();
  Serial1.println(input);
  input.remove(2); // remove everthing after last char
  Serial1.println(input);
  if (input == "US" || input == "us" || input == "EN" || input == "en")
  {
    Keyboard.begin(KeyboardLayout_en_US);
    Serial1.println("US selected");
  }
  else
  {
    Keyboard.begin(KeyboardLayout_de_DE);
    Serial1.println("DE selected");
  }
}

/* ## EOF ## */