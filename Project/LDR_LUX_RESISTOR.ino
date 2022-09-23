#include <driver/adc.h>
void setup() {
  Serial.begin(9600);
}
void loop() {
    // Configure ADC1 on 12bits (0-4095)
    adc1_config_width(ADC_WIDTH_BIT_12);
    // Configure attenuation of ADC1, Channel 0 to 11dB (full-scale voltage of 3.9V)
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);
    // Read the voltage on ADC1, Channel 0 (GPIO 36)
    int value = adc1_get_raw(ADC1_CHANNEL_0);
    // Print the read value (0-4095)
    Serial.println("Value: " + String(value));
    // Convert the read value to millivolt, using the full-scale configured with the attenuation (3.9V) and the number of bits (4095)
    float voltage = (float)value*3900./4095.;
    // Print the vaoltage in millivolts (0-3.9V)
    float res=((5000*10000)/(voltage))-10000;
    float lux=500000/res;
    Serial.println("Millivolts: " + String(voltage));
    Serial.println("Resistance: " + String(res));
    Serial.println("Lux: "+ String(lux));
    delay(1000);
}
