#include <cvzone.h>
#include <Stepper.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Password.h>

SerialData serialData(3, 1); //(numOfValsRec,digitsPerValRec)
int Scenarii[3];
int buzzer = 9;
int senzor_lumina;
int lifes = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int stepsPerRevolution = 2048;
byte a = 5;
bool value = true;
bool senz = false;
bool usa = false;
bool recognised = false;
byte maxPasswordLength = 6;
byte currentPasswordLength = 0;
const byte ROWS = 3;
const byte COLS = 4;


char keys[ROWS][COLS] = {
  {'#', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'},
};

byte rowPins[ROWS] = {2, 3, 4};
byte colPins[COLS] = {5, 6, 7, 8};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Stepper DOOREL = Stepper(stepsPerRevolution, 10, 12, 11, 13);

Password password = Password("0123"); //Enter your password


void setup() {
  pinMode(A3, OUTPUT); //rosu
  pinMode(A2, OUTPUT); // galben
  pinMode(A1, OUTPUT); // verde
  pinMode(A0, INPUT);  //senzor lumina
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  serialData.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("DOOR LOCK SYSTEM");
  delay(5000);
  lcd.clear();

  DOOREL.setSpeed(10);

}


void loop() {

  senzor_lumina = analogRead(A0);
  if (senz == true)
  {
    lcd.clear();
    senz = false;
    if(usa == true)
    {
    DOOREL.step(stepsPerRevolution / 4);
    DOOREL.step(0);
    delay(4000);
    usa = false;
    }
  }


  if(senzor_lumina > 700)
  {
    manual_control();
    senz=false;
  }
  else if(senzor_lumina < 0)
  {
    auto_control();
    senz=true;
  }
  
  Serial.println(senzor_lumina);
  delay(50);
}

void auto_control() {

  // Primim datele din Python
  serialData.Get(Scenarii);
  // Controlam LED-urile in functie de datele primite
  digitalWrite(A3, Scenarii[2]);  //red
  digitalWrite(A2, Scenarii[1]);  //yellow
  digitalWrite(A1, Scenarii[0]);  //green
  delay(10);//
  
  if (Scenarii[2] == 1)
  { 
    tone(buzzer, 1000); 
    delay(1000);        
    lcd.setCursor(5, 0);
    lcd.print("ALERT!");
    delay(3500);
    noTone(buzzer);     
    delay(1000);
  }
  else if (Scenarii[1] == 1 && !recognised) {
    lcd.setCursor(0, 0);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    lcd.print("FACE UNKNOWN");
    lcd.setCursor(1, 1);
    delay(1800);
    lcd.print("PLEASE GO AWAY");
    delay(2000);
    lcd.clear();
    recognised = true;
    if(usa == true)
    {
    DOOREL.step(stepsPerRevolution / 4);
    DOOREL.step(0);
    delay(4000);
    usa = false;
    }
  } else if (Scenarii[0] == 1 && recognised) {
    lcd.setCursor(0, 0);
    lcd.print("FACE RECOGNISED");
    lcd.setCursor(2, 1);
    delay(1800);
    lcd.print("DOOR OPENED");
    delay(2000);
    digitalWrite(buzzer, LOW);
    delay(120);
    digitalWrite(buzzer, HIGH);
    delay(120);
    digitalWrite(buzzer, LOW);
    delay(120);
    digitalWrite(buzzer, HIGH);
    delay(120);
    lcd.clear();
    recognised = false;
    if (usa == false)
    {
    DOOREL.step(-stepsPerRevolution / 4);
    DOOREL.step(0);
    delay(4000);
    usa = true;
    }
  }
  senzor_lumina = analogRead(A0);  
}

void manual_control() {

  if (lifes == 0)
      {
        digitalWrite(A3, 1);
        digitalWrite(A2, 1);
        digitalWrite(A1, 1);
      }

  lcd.setCursor(1, 0);
  lcd.print("ENTER PASSWORD");

  char key = keypad.getKey();
  if (key != NO_KEY) {
    delay(60);
    if (key == '*') {
      resetPassword();
    } else if (key == '#') {
    digitalWrite(buzzer, LOW);
    delay(130);
    digitalWrite(buzzer, HIGH);
    delay(130);
      if (value == true) {
        value = false;
      } else if (value == false) {
        lcd.clear();
        dooropen();
        value = true;
      }
    } else {
      processNumberKey(key);
    }
  }
  senzor_lumina = analogRead(A0);
}


void resetPassword() {
  password.reset();
  currentPasswordLength = 0;
  lcd.clear();
  a = 5;
}


void processNumberKey(char key) {
  lcd.setCursor(a, 1);
  lcd.print("*");
  a++;
  digitalWrite(buzzer, LOW);
  delay(20);
  digitalWrite(buzzer, HIGH);
  delay(20);
  if (a == 11) {
    a = 5;
  }
  currentPasswordLength++;
  password.append(key);

  if (currentPasswordLength == maxPasswordLength) {
    if(usa == true)
    {
    DOOREL.step(stepsPerRevolution / 4);
    DOOREL.step(0);
    delay(4000);
    usa = false;
    }
  }
}



void dooropen() {
  
  if (password.evaluate()) {
    lcd.setCursor(0, 0);
    lcd.print("CORRECT PASSWORD");
    lcd.setCursor(2, 1);
    delay(1800);
    lcd.print("DOOR OPENED");
    delay(2000);
    digitalWrite(buzzer, LOW);
    delay(120);
    digitalWrite(buzzer, HIGH);
    delay(120);
    digitalWrite(buzzer, LOW);
    delay(120);
    digitalWrite(buzzer, HIGH);
    delay(120);

    digitalWrite(A3, 0);
    digitalWrite(A2, 0);
    digitalWrite(A1, 0);

    lcd.clear();
    a = 5;
    DOOREL.step(-stepsPerRevolution / 4);
    DOOREL.step(0);
    delay(500);
    for(int i=7; i>0; --i)
    {
      lcd.setCursor(3, 0);
      lcd.print("LOCKS IN " + String(i));
      delay(1000);
      lcd.clear();
    }
    doorlocked();
    lcd.setCursor(2, 0);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    lcd.print("DOOR LOCKED");
    delay(2000);
    lcd.clear();
    lifes = 0;
  } else {
    tone(buzzer, 100); 
    delay(300);  
    noTone(buzzer);    
    delay(10); 
    digitalWrite(buzzer, HIGH); 
    lcd.setCursor(0, 0);
    lcd.print("WRONG PASSWORD!");
    lcd.setCursor(0, 1);
    lcd.print("PLEASE TRY AGAIN");
    delay(2000);
    lcd.clear();
    a = 5;

    lifes = lifes + 1;

   if (lifes == 1)
      {
        digitalWrite(A3, 1);
        digitalWrite(A2, 1);
        digitalWrite(A1, 0);
      }
    else if (lifes == 2)
      {
        digitalWrite(A3, 1);
        digitalWrite(A2, 0);
        digitalWrite(A1, 0);
      }
    else 
      {
        digitalWrite(A3, 0);
        digitalWrite(A2, 0);
        digitalWrite(A1, 0);
      }

    if (lifes > 2)
      ALERT();

  }
  resetPassword();
  
}

void doorlocked() {

  DOOREL.step(stepsPerRevolution / 4);
  DOOREL.step(0);

}

void ALERT()
{   lcd.setCursor(5, 0);
    lcd.print("ALERT!");
    delay(1200);
for(int i=4; i>0; --i)
{
    tone(buzzer, 600); 
    delay(1000);  
    noTone(buzzer);
    delay(180);    
}     
    digitalWrite(buzzer, HIGH); 
    lcd.setCursor(5, 0);
    lcd.print("ALERT!");
    delay(1500);
    lifes = 0;
}

