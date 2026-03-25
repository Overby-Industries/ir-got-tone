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
int rawDataLength = sizeof(rawData) / sizeof(rawData[0]);
int carrierFrequency = 38; // kHz
// put function declarations here:
IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results; // decode_results class is defined in IRremote.h

void setup()
{
  // put your setup code here, to run once:
  pinMode(redLED, OUTPUT);
  irrecv.enableIRIn();       // Start the receiver
  irsend.begin(IR_SEND_PIN); // Initialize IR sender
  // IrSender.begin(IR_SEND_PIN); // Initialize IR sender
  Serial.begin(115200);
  Serial.println("Ready to send raw IR signal...");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    // Buzzer and red LED on when IR signal is received
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLED, HIGH);
    delay(5000);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLED, LOW);

    for (int i = 0; i < 10; i++)
    {
      // Send the raw signal at 38 kHz carrier frequency
      // irsend.sendRaw(rawData, sizeof(rawData) / sizeof(rawData[0]), 38); // 38 kHz is a common IR frequency
      irsend.sendRaw(rawData, rawDataLength, carrierFrequency);
      // irsend.sendSony(0xa90, carrierFrequency); // code for Sony TV power command
                                                // Send the raw signal at 38 kHz carrier frequency
                                                // IrSender.sendRaw(rawData, sizeof(rawData) / sizeof(rawData[0]), 38); // 38 kHz is a common IR frequency
      delay(100);                               // small delay to prevent reading errors

      Serial.println("IR signal sent (raw IR Power ON).");
      irrecv.resume(); // Ensure the receiver is ready for the next signal
    }
  }
}

// put function definitions here:
