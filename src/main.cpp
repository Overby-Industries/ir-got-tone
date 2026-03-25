#include <Arduino.h>
#include <IRremote.h>
#include <avr/wdt.h>       // Watchdog timer

const int IR_SEND_PIN     = 3;  // IR transmitter pin
const int RECV_PIN        = 11; // IR receiver pin
const int buzzerPin       = 8;  // Buzzer output pin
const int buttonPin       = 2;  // Button pin — must be 2 or 3 on Uno for interrupts
const int redLED          = 9;  // Red LED output pin
const int carrierFrequency = 38; // 38 kHz carrier

const uint16_t rawData[] = {
    5000, 2500,
    5000, 2500,
    5000, 2500,
    5000, 2500};
const int rawDataLength = sizeof(rawData) / sizeof(rawData[0]);

unsigned long buzzerStartTime = 0;
bool buzzerActive = false;

// volatile — shared between ISR and loop(), compiler must not cache this
volatile uint8_t buttonPressed = 0;

IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;

// ISR — called instantly by hardware when button pin falls LOW
// Rule: do as little as possible here — just set the flag
void buttonISR() {
  buttonPressed = 1;
}

void setup()
{
  wdt_enable(WDTO_8S); // Watchdog: reboot if loop() stalls for more than 8 seconds

  pinMode(buzzerPin, OUTPUT);
  pinMode(redLED,    OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Built-in pull-up: button pressed = LOW

  // Attach interrupt to buttonPin — fires buttonISR() on falling edge (HIGH -> LOW)
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);

  irrecv.enableIRIn();
  irsend.begin(IR_SEND_PIN);
  Serial.begin(115200);
  Serial.println("Ready — button on pin 2, watchdog active.");

  wdt_reset(); // Pet the dog after setup in case setup() took a while
}

void loop()
{
  // 1. Button check — flag set by ISR, handled here
  if (buttonPressed) {
    buttonPressed = 0; // Clear the flag first
    irsend.sendRaw(rawData, rawDataLength, carrierFrequency);
    Serial.println("IR signal sent.");
  }

  // 2. IR receive
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);

    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLED,    HIGH);
    buzzerStartTime = millis();
    buzzerActive = true;

    Serial.print("Raw timings: ");
    for (int i = 0; i < results.rawlen; i++) {
      Serial.print(results.rawbuf[i] * USECPERTICK);
      Serial.print(", ");
    }
    Serial.println();
    irrecv.resume();
  }

  // 3. Buzzer timeout — non-blocking, runs every loop
  if (buzzerActive && millis() - buzzerStartTime >= 1000)
  {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLED,    LOW);
    buzzerActive = false;
  }

  // 4. Pet the watchdog — must reach here within 8s or MCU reboots
  wdt_reset();
}
