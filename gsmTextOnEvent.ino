#include <SoftwareSerial.h>


SoftwareSerial SIM900(10, 11);
//SoftwareSerial cr800(19,18);


char c = 0;
char c2 = 0;
char i = 0;
boolean stringComplete = false; 
const int DELAY_1 = 10;  //aumentar si hay poca señal
const String API_KEY = "UJ3N8LXLLV5C6X5D";
int data;

void setup()
{
  
  SIM900.begin(19200);
  Serial.begin(19200);
  //cr800.begin(19200);

  SIM900.println("ATE0\n\r");
  SIM900.println("AT+HTTPTERM");
  SIM900.println("AT+SAPBR=0,1");
  delay(2000);

}
void loop(){
  
  //getHttp("10");
  delay(1000);
}


void serialEvent() {
  String inputString = "";
  String str;
  int i = 0;
  char inChar = Serial.read();
  
  if(inChar =='*' )
  {
    while (Serial.available()) {
      // get the new byte:
      inChar = (char)Serial.read();
      // add it to the inputString:
      inputString += inChar;
      // if the incoming character is a newline, set a flag so the main loop can
      // do something about it:
      if (inChar == '\n') {
        stringComplete = true;
      }
      if ((inChar >= 44) && (inChar< 58))
        str+=inChar;
      if (inChar == ':')
      {
        i = (int)((char)Serial.read() - '0');
        break;
      }
    }
    Serial.println("----------------Interrupcion: " + inputString+ " " + str + " " + String(i) );
    getHttp(str, (int)i);
  }
}


void getHttp(int data){
  SIM900.println("AT+HTTPTERM");
  SIM900.println("AT+SAPBR=0,1");
  delay(1000);
  Serial.println(sendData("AT+CPIN?"));
  Serial.println(sendData("AT+SAPBR=3,1,\"Contype\",\"GPRS\""));
  Serial.println(sendData("AT+SAPBR=3,1,\"APN\",\"www\""));
  Serial.println(sendData("AT+SAPBR=1,1"));
  delay(8000);
  Serial.println(sendData("AT+SAPBR=2,1"));
  Serial.println(sendData("AT+HTTPINIT"));
  Serial.println(sendData("AT+HTTPPARA=\"CID\",1"));
  Serial.println(sendData("AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/update?api_key="+API_KEY+"&field1="+String(data)+"\""));
  Serial.println(sendData("AT+HTTPACTION=0 "));
  delay(5000);
  Serial.println(sendData("AT+HTTPREAD"));
  Serial.println(sendData("AT+HTTPTERM"));
  SIM900.println("AT+SAPBR=0,1 ");
}

void getHttp(String data, int field){
  SIM900.println("AT+HTTPTERM");
  SIM900.println("AT+SAPBR=0,1");
  delay(1000);
  Serial.println(sendData("AT+CPIN?"));
  Serial.println(sendData("AT+SAPBR=3,1,\"Contype\",\"GPRS\""));
  Serial.println(sendData("AT+SAPBR=3,1,\"APN\",\"www\""));
  Serial.println(sendData("AT+SAPBR=1,1"));
  delay(8000);
  Serial.println(sendData("AT+SAPBR=2,1"));
  Serial.println(sendData("AT+HTTPINIT"));
  Serial.println(sendData("AT+HTTPPARA=\"CID\",1"));
  Serial.println(sendData("AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/update?api_key="+API_KEY+"&field"+String(field)+'='+data+"\""));
  Serial.println(sendData("AT+HTTPACTION=0 "));
  delay(5000);
  Serial.println(sendData("AT+HTTPREAD"));
  Serial.println(sendData("AT+HTTPTERM"));
  SIM900.println("AT+SAPBR=0,1 ");
}



String sendData(String data) //int field )
{
  String str = "";
  char i  = 0; 
  char t;
  while(SIM900.available() > 0) {
     t = SIM900.read();
  }

  SIM900.println(data);
  delay(500);
  while(true)
  {
      delay(DELAY_1);
      while(SIM900.available()>0)
      {  
        str+= (char)SIM900.read();
        delay(DELAY_1);
      }
      return str;
  }
}
