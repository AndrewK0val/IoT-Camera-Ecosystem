void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
}

void loop() {
  int sensorValue = analogRead(A0); // Read analog input from pin A0
  Serial.write(sensorValue & 0xFF); // Send the least significant 8 bits
  Serial.write((sensorValue >> 8) & 0xFF); // Send the most significant 8 bits
  delay(10); // Adjust the delay based on the required sampling rate
}