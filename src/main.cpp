#include <Arduino.h>
#include <IRremote.h>

const int IR_SEND_PIN = 3; // Define your IR transmitter pin
const int RECV_PIN = 11;   // define input pin on Arduino to which the IR receiver is connected
const int buzzerPin = 8;   // define output pin on Arduino to which the buzzer is connected
const int redLED = 9;      // define output pin on Arduino to which the red LED is connected
unsigned long buzzerStartTime = 0; // Track when the buzzer was activated
bool buzzerActive = false; // Track if the buzzer is currently active
// Raw data for an IR signal
const uint16_t rawData[] = {
    5000, 2500, // Mark: 5ms, Space: 2.5ms (example pattern)
    5000, 2500,
    5000, 2500,
    5000, 2500};
const int rawDataLength = sizeof(rawData) / sizeof(rawData[0]); // Length of the raw data array
const int carrierFrequency = 38; // Carrier frequency in kHz (common for many IR devices, adjust as needed)
// put function declarations here:
IRrecv irrecv(RECV_PIN); // Create IR receiver object
IRsend irsend; // Create IR receiver and sender objects
decode_results results; // decode_results class is defined in IRremote.h

void setup()
{
  // put your setup code here, to run once:
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  pinMode(redLED, OUTPUT);    // Set red LED pin as output
  irrecv.enableIRIn();        // Start the receiver
  irsend.begin(IR_SEND_PIN);  // Initialize IR sender
  Serial.begin(115200); //  Start serial communication at 115200 baud rate for debugging
  Serial.println("Ready to send raw IR signal..."); // Print initial message to Serial
}

void loop()
{
  // put your main code here, to run repeatedly:

  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX); // Print the decoded value in hexadecimal format (for reference)
    
    // Activate buzzer and red LED for 1 second
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLED, HIGH);
    buzzerStartTime = millis();  // Record the time when the buzzer was activated
    buzzerActive = true; // Set flag to indicate buzzer is active
    
    // Separately, check if 1 second has passed
  if (buzzerActive && millis() - buzzerStartTime >= 1000)
  {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLED, LOW);
    buzzerActive = false; // Reset flag after deactivating buzzer and LED
  }

    // Print captured raw timings to Serial (so you can copy real codes)
    Serial.print("Raw timings: "); // Print label for raw timings
    for (int i = 0; i < results.rawlen; i++) {
      Serial.print(results.rawbuf[i] * USECPERTICK); // Convert ticks to microseconds
      Serial.print(", "); // Print comma-separated values
    }
    Serial.println(); // New line after printing all timings

    // Retransmit once (replace rawData[] later with real captured timings)
    irsend.sendRaw(rawData, rawDataLength, carrierFrequency); // Send the raw data with specified carrier frequency
    Serial.println("IR signal sent."); // Print confirmation of IR signal transmission

    irrecv.resume(); // Receive the next value
  }
}

// put function definitions here:
