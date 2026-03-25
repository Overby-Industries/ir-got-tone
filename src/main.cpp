#include <Arduino.h>
#include <IRremote.h>

const int IR_SEND_PIN = 3; // Define your IR transmitter pin
const int RECV_PIN = 11;   // define input pin on Arduino to which the IR receiver is connected
const int buzzerPin = 8;   // define output pin on Arduino to which the buzzer is connected
const int redLED = 9;      // define output pin on Arduino to which the red LED is connected
// Raw data for an IR signal
const uint16_t rawData[] = {
    5000, 2500, // Mark: 5ms, Space: 2.5ms (example pattern)
    5000, 2500,
    5000, 2500,
    5000, 2500};
const int rawDataLength = sizeof(rawData) / sizeof(rawData[0]);
const int carrierFrequency = 38; // 38 kHz
// put function declarations here:
IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results; // decode_results class is defined in IRremote.h

void setup()
{
  // put your setup code here, to run once:
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  pinMode(redLED, OUTPUT);    // Set red LED pin as output
  irrecv.enableIRIn();        // Start the receiver
  irsend.begin(IR_SEND_PIN);  // Initialize IR sender
  Serial.begin(115200);
  Serial.println("Ready to send raw IR signal...");
}

void loop()
{
  // put your main code here, to run repeatedly:

  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    
    // Buzzer and LED on for 1 second (blocking is fine for now)
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLED, HIGH);
    delay(1000); // Block for 1 second
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLED, LOW);

    // Print captured raw timings to Serial (so you can copy real codes)
    Serial.print("Raw timings: ");
    for (int i = 0; i < results.rawlen; i++) {
      Serial.print(results.rawbuf[i] * USECPERTICK);
      Serial.print(", ");
    }
    Serial.println();

    // Retransmit once (replace rawData[] later with real captured timings)
    irsend.sendRaw(rawData, rawDataLength, carrierFrequency);
    Serial.println("IR signal sent.");

    irrecv.resume();
  }
}

// put function definitions here:
