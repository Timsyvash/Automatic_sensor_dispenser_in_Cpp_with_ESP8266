#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define ECHO_PIN D6
#define TRIG_PIN D7
#define SDA_PIN D2
#define SCL_PIN D1
#define SERVO_PIN D5

unsigned long last_time = 0;

int current_angle = -1;

Servo servo;
LiquidCrystal_I2C dis(0x27, 16, 2);

void setup()
{
  servo.attach(SERVO_PIN);

  Wire.begin(SDA_PIN, SCL_PIN);

  dis.init();
  dis.backlight();

  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
}

void loop()
{
  unsigned long cur_time = millis();
  if (cur_time - last_time >= 500)
  {
    last_time = cur_time;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG_PIN, LOW);

    long dur = pulseIn(ECHO_PIN, HIGH, 40000);
    short distance = dur * 0.0343 / 2;

    if (distance <= 0 || distance > 400)
    {
      dis.setCursor(0, 0);
      dis.print("Error sensor   ");
    }
    else
    {
      dis.setCursor(0, 0);
      dis.print("Distance: ");
      dis.print(distance);
      dis.print(" cm  ");
    }

    if (distance < 15)
    {
      if (current_angle != 90)
      {
        servo.attach(SERVO_PIN); // 1. Вмикаємо керування серво
        servo.write(180);        // 2. Повертаємо на 180 градусів
        delay(250);              // 3. Даємо трохи часу (250мс для SG90 зазвичай достатньо)
        servo.detach();          // 4. ЗУПИНЯЄМО (знеструмлюємо) мотор
        current_angle = 90;      // Запам'ятовуємо поточний кут
      }
    }
    else
    {
      if (current_angle != 180)
      {
        servo.attach(SERVO_PIN); // 1. Вмикаємо керування серво
        servo.write(0);          // 2. Повертаємо на 0 градусів
        delay(250);              // 3. Даємо час на поворот
        servo.detach();          // 4. ЗУПИНЯЄМО мотор
        current_angle = 180;     // Запам'ятовуємо поточний кут
      }
    }
  }
}
