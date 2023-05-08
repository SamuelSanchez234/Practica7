#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <RTClib.h>

#define SERVO_PIN 10
Servo servo1;
#define ROWS 4
#define COLS 4
#define RTC_SDA A4
#define RTC_SCL A5

int angulo = 0;
char password[5] = {'1', '2', '3', '4'};
char input_password[5];
byte fila_pins[ROWS] = {9,8,7,6};
byte columna_pins[COLS] = {5,4,3,2};
const byte KEY_ROWS = 4;
const byte KEY_COLS = 4;
char keymap[KEY_ROWS][KEY_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
Keypad teclado = Keypad(makeKeymap(keymap), fila_pins, columna_pins, ROWS, COLS);
RTC_DS1307 rtc;
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  
  lcd.init();
  lcd.backlight();
  rtc.begin();
  Wire.begin();
  servo1.attach(SERVO_PIN);
  Serial.begin(9600);
}

void loop() {
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);

  char key = teclado.getKey();

  if (key != NO_KEY) {
    if (strlen(input_password) < 4) {
      input_password[strlen(input_password)] = key;
      lcd.setCursor(strlen(input_password) - 1, 3);
      lcd.print("*");
    }
  }

  if (strlen(input_password) == 4) {
    if (strncmp(password, input_password, 4) == 0) {
      lcd.clear();
      lcd.print("Bienvenido!");
      delay(1000);
      abrirPuerta();
      input_password[0] = '\0'; // Reiniciar la variable de la contraseña
    }
    else {
      lcd.clear();
      lcd.print("Contraseña incorrecta");
      delay(1000);
      input_password[0] = '\0'; // Reiniciar la variable de la contraseña
    }
    lcd.clear();
    lcd.setCursor(0, 3);
    lcd.print("Ingrese contraseña:");
  }
}

void abrirPuerta() {
  for (int i = 180; i <= 90; i++) { 
    angulo = i;
    servo1.write(180);
    delayMicroseconds(500 + (angulo * 11)); 
    delay(1500); 
  }

  delay(3000); 

  for (int i = 180; i >= 90; i--) { 
    angulo = i;
    servo1.write(180);
    delayMicroseconds(500 + (angulo * 11)); 
    delay(1500); 
  }

  lcd.clear();
  lcd.print("Puerta cerrada"); // Mostrar mensaje de puerta cerrada en pantalla LCD
}
