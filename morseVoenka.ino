//**********************************************************
//Voenka edition
//
//
//
//**********************************************************

//MEM: russian letters are coded as 2 symbols in String. Bad coded in char. watch the 130 string of this code.
//MEM2: the function pow() in GOVNO! (int)pow(3, i) works uncorrect in "for" for example
//MEM3: filename's length is restricted by 12 symbols including ".txt".It won't see file "12345678.txt"

#include<SPI.h>
#include<SD.h>

const uint8_t letterCode[33] = {0, 1, 2, 4, 5, 7, 8, 13, 14, 16, 17, 22, 23, 25, 26, 40, 41,
43, 44, 49, 50, 52, 79, 71, 67, 68, 77, 76, 130, 233, 53, 80, 70};

const char engLet[33] = {'0', 'e', 't', 'i', 'n', 'a', 'm', 's', 'd', 'r', 'g', 'u',
'k', 'w', 'o', 'h', 'b', 'l', 'z', 'f', 'c', 'p', 'j', 'q', 'v', 'x', 'y', 
'1', '2', '3', '4', '5', '6'};

const byte rusLetersInASCII[33] = {32, 229, 242, 232, 237, 224, 236, 241, 228, 240, 227, 243,
234, 226, 238, 245, 225, 235, 231, 244, 246, 239, 233, 249, 230, 252, 251, 254,
253, 250, 247, 248, 255};

//-----changable parametrs-----
//the parametrs of output signal are in function signalMorse() at the end
//you can change output speed by changing them
const int LETTER_TIME = 700; //the delay after which the input if new letter starts
const int OFF_TIME = 2000; //the time of pressing bitton to off/on the system
const int CHANGE_DELAY = 20; //the delay for changing the position of the button (to ignore chatter)
const int PRINT_TIME = 2000; //the delay after which the input will be stopped and the word wil be printed
const uint8_t LENGHT = 1000; //the lenght of buffers (both of dots/lines and symbol ones)
const uint8_t I_LENGHT = 80; //LENGHT for inputBuffer
//10 pin is not allowed!
const uint8_t but = 3; //button pin. here interrupts are included. Don't change
const uint8_t led = 2; //led/pisk/motor ect pin
const uint8_t sd = 4; //sd pin
//-----changable parametrs-----

uint32_t timer = 0; //time of the last event
uint32_t clicked_time = 0; //how long was the button pressed

boolean change; //wheather the position of the button is changing now or not
boolean pushed; //wheather is pushed now or not
boolean word_printed; //wheather the word has been alredy printed or not
boolean ON; //wheather it is ON or OFF
volatile boolean printNow; //Wheathen it outputs the file now or not. To interrupt the system

uint8_t inputBuffer[I_LENGHT] = {0}; //buffer of morse code for one word (dots - 1, lines - 2, spaces - 3)
int inputPos = 0; //the position of the recent symbol in morse code

uint8_t outputBuffer[LENGHT] = {0}; //buffer of morse code for one word, to transmit
int outputPos = 0; //the position of the recent symbol in morse code. to transmit

byte translateBuffer[LENGHT] = {0};

void setup() 
{
   pinMode(but, INPUT); //if button (not sensor) INPUT_PULLUP
   attachInterrupt(1, interrupt, RISING);
   pinMode(led, OUTPUT);
   pinMode(10, OUTPUT);
   pinMode(7, OUTPUT); //5v
   digitalWrite(7, HIGH);
   SD.begin(sd);
   
   //Serial.begin(9600);
   /*
   Serial.print("Initializing SD card...");
   if (!SD.begin(sd)) 
   {
     Serial.println("Card failed, or not present");
     return;
   }
   Serial.println("card initialized.");
   */
}

void loop() 
{
  if (ON) // if the system works
  {
    if(!change && !pushed && digitalRead(but)) //press
    {
      pushed = !pushed;
      change = !change;
      word_printed = false;
      clicked_time = 0;
      int wait_time = millis() - timer;
      timer = millis();
      if (wait_time >= LETTER_TIME && wait_time < PRINT_TIME && inputPos != 0) //to differ letters
        add_to_inputBuffer(-1); //spase
    }
    else if (!change && pushed && digitalRead(but) == 0) //unpress
    {
      pushed = !pushed;
      change = !change;
      clicked_time = millis() - timer;
      timer = millis();
      if (clicked_time > OFF_TIME) // if off
        turn();
      add_to_inputBuffer(clicked_time); //send the length of the signal 
    }
    if (millis() - timer > CHANGE_DELAY)
      change = false;
    if ((millis() - timer > PRINT_TIME) && !word_printed && inputPos != 0) //print the word
    {
      word_printed = !word_printed;
      printNow = true;
      buff_to_string(); 
      inputPos = 0;
      for (int i = 0; i < I_LENGHT; i++) //clean buffer
        inputBuffer[i] = 0;
    }
  }
  else // if the system sleeps
  {
    if(!change && !pushed && digitalRead(but)) //press
    {
      pushed = !pushed;
      change = !change;
      timer = millis();
    }
    else if (!change && pushed && digitalRead(but) == 0) //unpress
    {
      pushed = !pushed;
      change = !change;
      if (millis() - timer > OFF_TIME) // if on
        turn();
    }
    if (millis() - timer > CHANGE_DELAY)
      change = false;
  }
}

void interrupt()
{
  printNow = false;
}

void turn() // on/off the system
{
  pushed = false;
  change = false;
  timer = 0;
  ON = !ON;
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
}

void add_to_inputBuffer(int ct) //ct = clicked_time 
{
  if (ct > 20 && ct < 201)
  {
    inputBuffer[inputPos] = 1; //dot
    inputPos++;
  }
  else if (ct > 200 && ct < LETTER_TIME)
  {
    inputBuffer[inputPos] = 2; //line
    inputPos++;
  }
  else if (ct == -1)
  {
    inputBuffer[inputPos] = 3; //letter differenciator
    inputPos++;
  }
}

void buff_to_string() //makes string from the word in inputBuffer
{
  int i = 0;
  String str = "";
  for(int j = 0; j < 5; j++)
  {
    int s = 0;
    int p = i;
    while (inputBuffer[i] == 2 || inputBuffer[i] == 1) //one letter
    {
      s += power(3, i - p) * inputBuffer[i]; //letter in number
      i++;
    }
    if (s != 0)
    {
      for (int z = 0; z < 33; z++)
      {
        if (letterCode[z] == s)
        {
          str += engLet[z];
          break;
        }
      }
    }
    else
      str += "0";
    i++;
  }
  //Serial.println(str);
  translateString(str); //string that we want to translate
}

void translateString(String str) //gets eng string and fills translateBuffer by translated bytes
{
  String s = str + "/";
  File dir = SD.open(s, FILE_READ);
  if (dir)
  {
    while (true)
    {
      File f = dir.openNextFile();
      f.read(translateBuffer, LENGHT);
      divideLettersFromBuffer();
      if (!f)
        break;
      f.close();
    }
    printNow = false;
  }
  else
    signalError();
  dir.close();
}

void divideLettersFromBuffer()
{
  for (int i = 0; i < LENGHT; i++)
  {
    byte b = translateBuffer[i];
    for (int j = 0; j < 33; j++)
    {
      if (rusLetersInASCII[j] == b)
      {
        if (j != 0)
        {
          letter_to_outputBuffer(letterCode[j]);
          letter_to_outputBuffer(-1); //letter diffirenciator
        }
        else
          letter_to_outputBuffer(-2); //if space
        break;
      }
    }
    
    if (b == 0) //if letters in translateBuffer end
    {
      letter_to_outputBuffer(-3); //the end
      break;
    }
  }
  for (int i = 0; i < LENGHT; i++) //clean translateBuffer
    translateBuffer[i] = 0;
}

void letter_to_outputBuffer(int8_t code) //codes the number of letter in morse code to dots and lines  
{                         //(-1 and -2 for letter and word differenciators, -3 for end)     
  int a = code;
  if (a != -1 && a != -2 && a != -3) //if letter
  {
    for (int i = 1; i < 7; i++)
    {
      int s = a % 3;
      a = (a - s) / 3; 
      if (s != 0) //if symbol
      {
        outputBuffer[outputPos] = s;
        outputPos++;
      }
    }
  }
  else if (a == -1) //if differer between letters
  {
    outputBuffer[outputPos] = 3;
    outputPos++;
  }
  else if (a == -2) //if space
  {
    outputBuffer[outputPos] = 4;
    outputPos++;
  }
  else if (a == -3) //the end
  {
    signalMorse();
    outputPos = 0;
    for (int i = 0; i < LENGHT; i++)
      outputBuffer[i] = 0;
  }
}

void signalMorse() //makes singal on led/pisk/ect a = 1 or 2 or 3 or 4 or 5
{
  for (int i = 0; outputBuffer[i] != 0 && printNow; i++)
  {
    if (outputBuffer[i] == 1) 
    {
      digitalWrite(led, HIGH);
      delay(180); //short signal
      digitalWrite(led, LOW);
      delay(60); //delay between signals
    }
    else if (outputBuffer[i] == 2) 
    {
      digitalWrite(led, HIGH);
      delay(450); //long signal
      digitalWrite(led, LOW);
      delay(60); //delay between signals
    }
    else if (outputBuffer[i] == 3) //between letters
    {
      delay(1200);
    }
    else if (outputBuffer[i] == 4) //between words
    {
      delay(2000);
    }
  }
}

void signalError()
{
  digitalWrite(led, HIGH);
  delay(700);
  digitalWrite(led, LOW);
}

int power(int x, int a)
{
  int ans = 1;
  for (int i = 0; i < a; i++)
    ans *= x;
   return ans;
}

