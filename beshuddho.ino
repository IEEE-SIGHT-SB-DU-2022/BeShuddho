#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensor_pin = 2; //flow meter
const int relay_pin1= A3; //relay
const int relay_pin2= A2;
volatile int Pulse_Count;
int Liter_per_minute;
unsigned long Current_Time, Loop_Time;


void setup()
{
  lcd.begin();
  lcd.backlight();
	Serial.begin(9600);
	pinMode(sensor_pin, INPUT);
	pinMode(relay_pin1, OUTPUT);
    pinMode(relay_pin2, OUTPUT);
	attachInterrupt(digitalPinToInterrupt(sensor_pin), Detect_Rising_Edge, RISING);
	Current_Time = millis();
	Loop_Time = Current_Time;
}
void loop ()
{
  Current_Time = millis();
  if(Current_Time >= (Loop_Time + 500))
  {
    Loop_Time = Current_Time;
    Liter_per_minute = (Pulse_Count *2 / 7.5);
    Pulse_Count = 0;
    Serial.print(Liter_per_minute);
    Serial.println(" Liter per minute");
    if(Liter_per_minute > 0){
    Serial.println("filter on");
    digitalWrite(relay_pin1, LOW); //change to high if working in reverse
    digitalWrite(relay_pin2, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Filter On");

    lcd.setCursor(0, 1);
    lcd.write("Flow: ");
    lcd.write(Liter_per_minute);
    lcd.write(" LPM.");

    delay(2000);

  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("BeShuddho:");
    lcd.setCursor(0, 1);
    lcd.write("System Standby.");

    Serial.println("filter off");
    digitalWrite(relay_pin1, HIGH); //change to low if working in reverse
    digitalWrite(relay_pin2, HIGH);
  }
  }

}
void Detect_Rising_Edge ()
{
  Pulse_Count++;
}
