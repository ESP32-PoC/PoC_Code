# PoC_Code

#Parental Lock Fridge

You can find more detailled informations in the report (diagram, etc...)

You can find the actual Code in the "Project" directory, you only need to run the file *main_lux_mail.ino*.
It has all the configuration inside.

## Prior to run you need the following :
- the library to map all adc pin : driver/adc.h
- the library ESP Mail Client (find more informations here : https://github.com/mobizt/ESP-Mail-Client) (accept all the side librairies that come along)
- change a few of the setup data according to your own environemnt.
 
## Setup Informations 
You have to change in the head of the file the following :

![image](https://user-images.githubusercontent.com/74834074/192166466-f56ee485-a949-4d3f-ade5-049624c75f9f.png)
              
remplace by your own
              
![image](https://user-images.githubusercontent.com/74834074/192166486-e7854db4-569f-4156-be59-3f92b8c37206.png)
              
remplace by your own
however you have to configure first your gmail account to get the key you can see below ↑
this tutorial can come in handy → https://randomnerdtutorials.com/esp32-send-email-smtp-server-arduino-ide/ ☺

![image](https://user-images.githubusercontent.com/74834074/192166607-464e4a0a-b47c-47cb-aa1b-aea94541f372.png)
              
change this to the recipient email
(you don't have to configure anything here just change it to the good one)
