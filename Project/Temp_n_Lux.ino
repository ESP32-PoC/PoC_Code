#include <driver/adc.h>
int cnt=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  adc1_config_width(ADC_WIDTH_BIT_12);
}

void Lux_calc(){
  //Read the voltage on ADC1, Channel 3 (GPIO 36)
  
  adc1_config_channel_atten(ADC1_CHANNEL_3,ADC_ATTEN_DB_11);
  int value = adc1_get_raw(ADC1_CHANNEL_3);
  float voltage = (float)value*3900./4095.;
  float res=((5000*10000)/(voltage))-10000;
  float lux=500000/res;
  Serial.println("Lux: "+ String(lux));
}

void Temp_calc(){
  
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
  int val = adc1_get_raw(ADC1_CHANNEL_0);
  float voltage2 = (float)val*1100./4095.;
  float temp = voltage2/10;
  Serial.println("Température: "+ String(temp));
}

void loop() {
  // put your main code here, to run repeatedly:
  Lux_calc();
  Temp_calc();
  delay(1000);
  if ( lux > 50)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else digitalWrite(LED_BUILTIN,LOW);
}
