#include <Keypad.h>
#include <LiquidCrystal.h>
#include "LED.hpp"
#include "Button.hpp"

int speakerPin = 3;

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

LED ledRed(A0, true);
LED ledYellow(A1, true);
LED ledGreen(A2, true);
Button button(2);

const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] =
{
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};

byte rowPins[rows] = { 10, 11, 12, 13 };
byte colPins[cols] = { 6, 7, 8, 9 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

/*const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);*/

void alarm(void)
{
    //beep2(speakerPin, 512, 1024, 10, 5);
    //beep3(speakerPin, 1024, 512, 10, 5);
    Beep(speakerPin, 512, 1024, 2500);
    Beep(speakerPin, 1024, 512, 2500);
}

void setup()
{
    button.SetPushCallback(alarm);

    pinMode(speakerPin, OUTPUT);
    Serial.begin(9600);

    /*lcd.begin(16, 2);
    lcd.print("Hello World!");*/
}

void loop()
{
    button.Poll();

    ledRed.Blink(5, 100);
    ledYellow.Blink(5, 100);
    ledGreen.Blink(5, 100);

    char key = keypad.getKey();
    if(key != '\0')
    {
        Serial.print("Key: ");
        Serial.println(key);
    }
}

float lerp(float v0, float v1, unsigned long t)
{
    return (1 - t) * v0 + t * v1;
}

float Lerp(float start, float end, float percent)
{
    return (start + percent * (end - start));
}

int Lerp(int start, int end, float percent)
{
    return (start + percent * (end - start));
}

void Beep(uint8_t pin, unsigned int a, unsigned int b, unsigned long int t)
{
    unsigned long int elapsedTime = 0UL;
    unsigned long prevTime = millis();
    while(elapsedTime < t)
    {
        unsigned int value = Lerp((int)a, (int)b, (float)elapsedTime / (float)t);
        tone(pin, value, 10);
        delay(5);

        unsigned long int currentTime = millis();
        elapsedTime += currentTime - prevTime;
        prevTime = currentTime;
    }
}
