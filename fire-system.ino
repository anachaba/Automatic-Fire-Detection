#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial mySerial(9, 10);
const int red = 3;
const int green = 4;

const int buzzer = 10;
const int flame = A0;
const int smoke = A1;
//const int Gas = A0;
//const int flasher1 = 11;
//const int flasher2 = 12;
// int flash_rate=100;
int thresh= 300;
int value= 200;
int status = true;
String alertMsg;
String mob1="+918789801436";           // Enter first mobile number with country code
String mob2="+917491981734";           // Enter second mobile number with country code

void setup()
{
  pinMode (relay,OUTPUT);
  pinMode (2,OUTPUT);
  pinMode(red, OUTPUT);
  //pinMode(flasher1,OUTPUT);
  //pinMode(flasher2,OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(smoke,INPUT);
//   pinMode(Gas,INPUT);
  pinMode(flame,INPUT);
  pinMode(buzzer, OUTPUT);
  
  lcd.init();                      // initialize the lcd 
  lcd.clear();  
  lcd.backlight();
  lcd.setCursor(1,0);
   lcd.print("Initializing....");
   delay(1000);
   

  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600); 
  Serial.println("Initializing...");// Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
}
void siren(int buzzer){
  digitalWrite(2,HIGH);
  delay(5000);
  digitalWrite(9,HIGH);
    delay(500);
    digitalWrite(9,LOW);
    delay(500);
    digitalWrite(9,HIGH);
    delay(500);
    digitalWrite(9,LOW);
    delay(500);
    digitalWrite(9,HIGH);
    delay(500);
    digitalWrite(9,LOW);
    delay(500);
    digitalWrite(9,HIGH);
    delay(500);
    digitalWrite(9,LOW);
    delay(500);
    digitalWrite(9,HIGH);
    delay(500);
    digitalWrite(9,LOW);
    delay(500);
     digitalWrite(9,HIGH);
    delay(500);
    digitalWrite(9,LOW);
    delay(500);
 
}

void loop()
{
  Serial.println("Smk Val: "+String(analogRead(smoke))+", Flame state: "+String(analogRead(flame)));
  if (analogRead(flame)>value || analogRead(smoke)>thresh)  //Flame or Smoke or Button detected
  {
    digitalWrite(red, HIGH);
    siren(buzzer);
    //digitalWrite(buzzer, HIGH);
    digitalWrite(green, LOW);
    
   // if(analogRead(flame)>value){
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.write(1);
      lcd.setCursor(3,1);
      lcd.print("GAS:"+String(analogRead(flame)));   
      lcd.setCursor(3,0);
      lcd.print("SMOKE:"+String(analogRead(smoke)));
      delay(5000);
      lcd.clear();
       lcd.setCursor(1,0); 
      alertMsg= "NO FIRE DETECTED";
      lcd.print(alertMsg);
     // lcd.setCursor(3,0);
   // }
    if(analogRead(flame)>value){
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.write(1);
      lcd.clear();
      lcd.setCursor(1,0); 
      alertMsg= "GAS DETECTED";
      lcd.print(alertMsg);
      delay(5000);
      lcd.setCursor(4,1);   
     lcd.print("FIRE:"+String(digitalRead(flame)==LOW?"HIGH":"LOW"));
  
    }
   
    if(analogRead(smoke)>thresh){
   
      digitalWrite(2,LOW);
      delay(1000);
       digitalWrite(relay,HIGH);
      delay(5000);
       digitalWrite(relay,LOW);
             delay(5000);
      lcd.setCursor(2, 0);
      lcd.write(1);
      lcd.clear();
      lcd.setCursor(1,0); 
      alertMsg= "SMOKE DETECTED";
      lcd.print(alertMsg);
      delay(5000);
      lcd.setCursor(4,1);   
     lcd.print("FIRE:"+String(digitalRead(flame)==LOW?"HIGH":"LOW"));
     delay(3000);
  
    }
    Serial.println(alertMsg);  //print on lcd
    if(status){   // run 1 time only when detects the fire after fire detection
     
      status = false;
      lcd.clear();
       lcd.setCursor(1,0); 
      alertMsg= "Calling Owner..";
      lcd.print(alertMsg);
      delay(10000);
      lcd.clear();
      lcd.setCursor(1,0);   
      lcd.print("Call Ended");
      delay(700);
      // 
      lcd.clear(); 
      lcd.setCursor(1,0);
      alertMsg= "Sending Sms....";
      lcd.print(alertMsg);
      delay(5000);
       lcd.clear();
      lcd.setCursor(1,0);   
      lcd.print("TEXT SENT!!");
      delay(700);
      //lcd.setCursor(1,0);
      String msg= "Alert Type: "+alertMsg;
      SendMessage(msg,mob1);  
      delay(8000); 
      SendMessage(msg,mob2);
    }
  }
  else{
    status = true;
    digitalWrite(2,HIGH);
    lcd.clear();
    lcd.setCursor(4,0);   
    lcd.print("SMOKE:"+String(analogRead(smoke)));
    lcd.setCursor(4,1);   
    lcd.print("FIRE:"+String(digitalRead(flame)==HIGH?"HIGH":"LOW"));
    delay(5000);
    lcd.clear();
    lcd.setCursor(4,0);   
    lcd.print("GAS:"+String(analogRead(flame)));
    lcd.setCursor(4,1);   
    lcd.print("FIRE:"+String(digitalRead(flame)==HIGH?"HIGH":"LOW"));
    delay(5000);
//    digitalWrite(flasher1, LOW);
//    digitalWrite(flasher2, LOW);
    digitalWrite(red, LOW);
    //digitalWrite(buzzer, LOW);
    noTone(buzzer);
    digitalWrite(green, HIGH);
  }
  delay(500);
  lcd.clear();
}

 void SendMessage(String msg, String mob)
{
  Serial.println(msg);  //Message sent to Mobile
  //digitalWrite(flasher1, HIGH);
  //digitalWrite(flasher2, HIGH);
  
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\""+mob+"\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(msg);// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);

}
 
