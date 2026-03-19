#define PWM_PIN 6

void setup() {
  analogWriteFrequency(PWM_PIN, 107000); // 108 kHz
  analogWriteResolution(PWM_PIN, 8);     // 8 bits
  analogWrite(PWM_PIN, 190);             // 50% duty 255 - 75% duty 190
}

void loop() {}