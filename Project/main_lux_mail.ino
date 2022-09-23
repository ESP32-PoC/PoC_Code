/*
  Rui Santos
  Complete project details at:
   - ESP32: https://RandomNerdTutorials.com/esp32-send-email-smtp-server-arduino-ide/
   - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-send-email-smtp-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Example adapted from: https://github.com/mobizt/ESP-Mail-Client
*/

// To send Emails using Gmail on port 465 (SSL), you need to create an app password: https://support.google.com/accounts/answer/185833

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>
#include <driver/adc.h>

#define LUX_CHANNEL ADC1_CHANNEL_3
#define LUX_ATT ADC_ATTEN_DB_11
#define TEMP_CHANNEL ADC1_CHANNEL_0
#define TEMP_ATT ADC_ATTEN_DB_0

#define WIFI_SSID "Test"
#define WIFI_PASSWORD "comores9977"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "alanabdou46@gmail.com"
#define AUTHOR_PASSWORD "bctlotowuuibsstb"

/* Recipient's email*/
#define RECIPIENT_EMAIL "alanabdou46@gmail.com"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;
int test_value = 0;
int cnt=0;
int verif = 0;

void send_email(int test_value);
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  adc1_config_width(ADC_WIDTH_BIT_12);

//  delay(1000);
//  esp_sleep_enable_ext0_wakeup(GPIO_NUM_X, level)
  gpio_wakeup_enable( GPIO_NUM_39, GPIO_INTR_HIGH_LEVEL); 
  esp_sleep_enable_gpio_wakeup();
    esp_err_t e = esp_sleep_enable_timer_wakeup(500000);
  Serial.println("ENABLE TIMER = "+e);
  // put to sleep
//  if (esp_err_t == ESP_ERR_INVALID_STATE) Serial.println(" Maybe Wifi is UP");

}

float Lux_calc(){
  //Read the voltage on ADC1, Channel 3 (GPIO 36)
  adc1_config_channel_atten(LUX_CHANNEL,ADC_ATTEN_DB_11);
  int value = adc1_get_raw(ADC1_CHANNEL_3);
  float voltage = (float)value*3900./4095.;
  float res=((5000*10000)/(voltage))-10000;
  float lux=500000/res;
  Serial.println("Lux: "+ String(lux));
  return lux;
}

float Temp_calc(){
  
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
  int val = adc1_get_raw(ADC1_CHANNEL_0);
  float voltage2 = (float)val*1100./4095.;
  float temp = voltage2/10;
  Serial.println("Température: "+ String(temp));
  return temp;
}

void setup_Wifi()
{
  Serial.println();
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

  float lux = Lux_calc();
  float temp = Temp_calc();
  delay(500);
  if ( lux > 50)
    {
      if( cnt >= 6 && verif == 0)
      {
        digitalWrite(LED_BUILTIN, HIGH);
        test_value = 1;
        setup_Wifi();
        send_email(1);
        verif = 1;
      }
      cnt++;
    }
    else 
    {
      digitalWrite(LED_BUILTIN,LOW);  
      esp_light_sleep_start();
//      if (esp_err_t == ESP_ERR_INVALID_STATE) Serial.println(" Maybe Wifi is UP");
      cnt = 0;
      verif = 0;
    }
//       esp_light_sleep_start();

}

void send_email(int test_value){
  if(test_value == 1){
    
    /** Enable the debug via Serial port
     * none debug or 0
     * basic debug or 1
    */
    smtp.debug(1);
  
    /* Set the callback function to get the sending results */
    smtp.callback(smtpCallback);
  
    /* Declare the session config data */
    ESP_Mail_Session session;
  
    /* Set the session config */
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "";
  
    /* Declare the message class */
    SMTP_Message message;
  
    /* Set the message headers */
    message.sender.name = "ESP";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "ESP Test Email";
    message.addRecipient("Alan", RECIPIENT_EMAIL);
  
    /*Send HTML message*/
    String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Le frigo est ouvert depuis plus de 30s!</h1><p>- Sent from ESP board</p></div>";
    message.html.content = htmlMsg.c_str();
    message.html.content = htmlMsg.c_str();
    message.text.charSet = "us-ascii";
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
    /*
    //Send raw text message
    String textMsg = "Hello World! - Sent from ESP board";
    message.text.content = textMsg.c_str();
    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    
    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;*/
  
    /* Set the custom message header */
    //message.addHeader("Message-ID: <test>");
  
    /* Connect to server with the session config */
    if (!smtp.connect(&session))
      return;
  
    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message))
      Serial.println("Error sending Email, " + smtp.errorReason());
  }
  if(test_value == 2){
      
    /** Enable the debug via Serial port
     * none debug or 0
     * basic debug or 1
    */
    smtp.debug(1);
  
    /* Set the callback function to get the sending results */
    smtp.callback(smtpCallback);
  
    /* Declare the session config data */
    ESP_Mail_Session session;
  
    /* Set the session config */
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "";
  
    /* Declare the message class */
    SMTP_Message message;
  
    /* Set the message headers */
    message.sender.name = "ESP";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "ESP Test Email";
    message.addRecipient("Alan", RECIPIENT_EMAIL);
  
    /*Send HTML message*/
    String htmlMsg = "<div style=\"color:#2f4468;\"><h1>La température interne est trop élevée !</h1><p>- Sent from ESP board</p></div>";
    message.html.content = htmlMsg.c_str();
    message.html.content = htmlMsg.c_str();
    message.text.charSet = "us-ascii";
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
    /*
    //Send raw text message
    String textMsg = "Hello World! - Sent from ESP board";
    message.text.content = textMsg.c_str();
    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    
    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;*/
  
    /* Set the custom message header */
    //message.addHeader("Message-ID: <test>");
  
    /* Connect to server with the session config */
    if (!smtp.connect(&session))
      return;
  
    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message))
      Serial.println("Error sending Email, " + smtp.errorReason());
  }
}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}
