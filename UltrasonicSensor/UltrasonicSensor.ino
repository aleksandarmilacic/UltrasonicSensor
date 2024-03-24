void setup() {
    Serial.begin(9600); // Start serial communication with the computer
    Serial1.begin(19200); // Start Serial1 for the sensor, matching the sensor's default baud rate
    Serial.println("URM06 Sensor Reader Initialized");
}

void loop() {
    // Command to measure distance: 0x55 0xAA 0x11 0x00 0x02 0x12
    byte measureCommand[] = { 0x55, 0xAA, 0x11, 0x00, 0x02, 0x12 };
    Serial1.write(measureCommand, sizeof(measureCommand));
    Serial.println("Distance measure command sent");

    delay(100); // Delay to allow the sensor time to respond

    if (Serial1.available() >= 8) { // Ensure there are enough bytes for a complete response
        byte response[8];
        for (int i = 0; i < 8; i++) {
            response[i] = Serial1.read();
        }

        // Validate response header and command echo
        if (response[0] == 0x55 && response[1] == 0xAA && response[2] == 0x11 && response[3] == 0x02 && response[4] == 0x02) {
            // Calculate distance from response
            int distance = (response[5] << 8) | response[6]; // Combine high and low bytes
            Serial.print("Distance: ");
            Serial.print(distance);
            Serial.println(" mm");
        }
        else {
            Serial.println("Invalid response");
        }
    }
    else {
        Serial.println("No response or incomplete response received");
        // Optional: Print any received data for debugging
        while (Serial1.available() > 0) {
            Serial.print(Serial1.read(), HEX);
            Serial.print(" ");
        }
        Serial.println(); // New line for readability
    }

    delay(1000); // Wait a bit before sending the next command
}
