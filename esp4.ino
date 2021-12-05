#include <SoftwareSerial.h>
#include <dht11.h>
#define RX 2
#define TX 3
#define dht_apin 11 // Analog Pin sensor is connected to
dht11 dhtObject;
#define VOLTAGE_MAX 5.0
#define VOLTAGE_MAXCOUNTS 1023.0
float sensor=A0;
float gas_value;
String AP = "your wifi name";       // AP NAME
String PASS = "your wifi password"; // AP PASSWORD
String API = "your thingspeak api key";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
//String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
const int buzzer = 9;
SoftwareSerial esp8266(RX,TX); 
 
  
void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  
}

void loop() {
 String temp = getTemperatureValue();
 String humidity=getHumidityValue();
 String gas_value=getAirValue();
 String sendData = "GET /update?api_key="+ API +"&field1="+temp+"&field2="+humidity+"&field3="+gas_value;
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(sendData.length()+4),4,">");
 esp8266.println(sendData);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 if ( (temp > "50") || (humidity > "80") || (gas_value > "500")) 
{
  digitalWrite(buzzer, HIGH);   // turn the LED on (HIGH is the voltage level)
 
 
 
}
}

String getTemperatureValue(){

   dhtObject.read(dht_apin);
   Serial.print(" Temperature(C)= ");
   int temp = dhtObject.temperature;
   Serial.println(temp); 
   delay(50);
   return String(temp); 
  
}


String getHumidityValue(){

   dhtObject.read(dht_apin);
   Serial.print(" Humidity in %= ");
   int humidity = dhtObject.humidity;
   Serial.println(humidity);
   delay(50);
   return String(humidity); 

} 
String getAirValue(){
  gas_value=analogRead(sensor);
   Serial.print(" air = ");
  Serial.println(gas_value);
  delay(50);
  return String(gas_value);
 
  
}


void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
