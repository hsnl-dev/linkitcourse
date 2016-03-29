#include <HttpClient.h>
#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LDateTime.h>
#include <LDHT.h>

#define WIFI_AP "@" //my wifi ap
#define WIFI_PASSWORD "169251aa" //my wifi ap password
#define WIFI_AUTH LWIFI_WPA //the kind of authority of my wifi which is WPA2
#define per 50
#define per1 3
#define DEVICEID "DOVIYPVz" //the device id given by cloud sandbox
#define DEVICEKEY "QGcsNdWVGSMqoJCb" //the device key given by cloud sandbox
#define SITE_URL "api.mediatek.com" //the site of the API

#define DHTPIN 2          // what pin we're connected to
#define DHTTYPE DHT22     // using DHT22 sensor
LDHT dht(DHTPIN,DHTTYPE);

unsigned int rtc;  //real-time clock
unsigned int lrtc;
unsigned int rtc1;
unsigned int lrtc1;
char port[4]={0};
char connection_info[21]={0};
char ip[21]={0};             
int portnum;
int val = 0;
String tcpdata = String(DEVICEID) + "," + String(DEVICEKEY) + ",0";

int pin=8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;//sampe 30s&nbsp;;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;

LWiFiClient c2;
HttpClient http(c2);//http client


#include <Wire.h> //include wire library to enable Grove
#include <Barometer.h> //include library of Barometer sensor
Barometer myBarometer;

float humidity;
float temperature;
float dust;

void setup() {
  //initialize
  dht.begin();
  LTask.begin();
  LWiFi.begin();
  Serial.begin(115200);
  myBarometer.init();
  pinMode(8,INPUT);
  starttime = millis();//get the current time;
  
  while(!Serial) delay(1000); /* comment out this line when Serial is not present, ie. run this demo without connect to PC */
  
  Serial.println("Connecting to AP");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    delay(1000);
  }
  
  Serial.println("calling connection");

  while (!c2.connect(SITE_URL, 80))
  {
    Serial.println("Re-Connecting to WebSite");
    delay(1000);
  }
  delay(100);

  getconnectInfo();
  connectTCP();
}

void getconnectInfo(){
  //calling RESTful API to get TCP socket connection
  c2.print("GET /mcs/v2/devices/");
  c2.print(DEVICEID);
  c2.println("/connections.csv HTTP/1.1");
  c2.print("Host: ");
  c2.println(SITE_URL);
  c2.print("deviceKey: ");
  c2.println(DEVICEKEY);
  c2.println("Connection: close");
  c2.println();
  
  delay(500);

  int errorcount = 0;
  while (!c2.available())
  {
    Serial.println("waiting HTTP response: ");
    Serial.println(errorcount);
    errorcount += 1;
    if (errorcount > 10) {
      c2.stop();
      return;
    }
    delay(100);
  }
  int err = http.skipResponseHeaders();

  int bodyLen = http.contentLength();
  Serial.print("Content length is: ");
  Serial.println(bodyLen);
  Serial.println();
  char c;
  int ipcount = 0;
  int count = 0;
  int separater = 0;
  while (c2)
  {
    int v = c2.read();
    if (v != -1)
    {
      c = v;
      Serial.print(c);
      connection_info[ipcount]=c;
      if(c==',')
      separater=ipcount;
      ipcount++;    
    }
    else
    {
      Serial.println("no more content, disconnect");
      c2.stop();

    }
    
  }
  Serial.print("The connection info: ");
  Serial.println(connection_info);
  int i;
  for(i=0;i<separater;i++)
  {  ip[i]=connection_info[i];
  }
  int j=0;
  separater++;
  for(i=separater;i<21 && j<5;i++)
  {  port[j]=connection_info[i];
     j++;
  }
  Serial.println("The TCP Socket connection instructions:");
  Serial.print("IP: ");
  Serial.println(ip);
  Serial.print("Port: ");
  Serial.println(port);
  portnum = atoi (port);
  Serial.println(portnum);

} //getconnectInfo

void uploadstatus(){
  //calling RESTful API to upload datapoint to MCS(Meditek Cloud Sandbox)
  Serial.println("calling connection");
  LWiFiClient c2;  
 
  char bufferH[5];
  char bufferT[5];
  char bufferD[6];
  
  sprintf(bufferH, "%.2f", humidity);
  sprintf(bufferT, "%.2f", temperature);
  sprintf(bufferD, "%.2f", dust);
  
  String uploadHumidity = "Hum_Display,," + String(bufferH);
  String uploadTemperature = "Temp_Display,," + String(bufferT);
  String uploadDust = "Dust_Display,," + String(bufferD);
  
  while (!c2.connect(SITE_URL, 80))
  {
    Serial.println("Re-Connecting to WebSite");
    delay(1000);
  }
  delay(100);
  
  String uploadData =  uploadHumidity + "\n" +
                       uploadTemperature + "\n"+
                       uploadDust + "\n";

  HttpClient http(c2);
  c2.print("POST /mcs/v2/devices/");
  c2.print(DEVICEID);
  c2.println("/datapoints.csv HTTP/1.1");
  c2.print("Host: ");
  c2.println(SITE_URL);
  c2.print("deviceKey: ");
  c2.println(DEVICEKEY);
  c2.print("Content-Length: ");
  c2.println(uploadData.length());
  c2.println("Content-Type: text/csv");
  c2.println("Connection: close");
  c2.println();
  c2.println(uploadData);
  //upload the data
  
  delay(500);

  int errorcount = 0;
  while (!c2.available())
  {
    Serial.print("waiting HTTP response: ");
    Serial.println(errorcount);
    errorcount += 1;
    if (errorcount > 10) {
      c2.stop();
      Serial.println("uploadStatus failed");
      return;
    }
    delay(100);
  }
  int err = http.skipResponseHeaders();

  int bodyLen = http.contentLength();
  Serial.print("Content length is: ");
  Serial.println(bodyLen);
  Serial.println();
  while (c2)
  {
    int v = c2.read();
    if (v != -1)
      Serial.print(char(v));
    else
    {
      Serial.println("no more content, disconnect");
      c2.stop();
    }
  }
}

void connectTCP(){
  //establish TCP connection with TCP Server with designate IP and Port
  c2.stop();
  Serial.println("Connecting to TCP");
  Serial.println(ip);
  Serial.println(portnum);
  while (0 == c2.connect(ip, portnum))
  {
    Serial.println("Re-Connecting to TCP");    
    delay(1000);
  }  
  Serial.println("send TCP connect");
  c2.println(tcpdata);
  c2.println();
  Serial.println("waiting TCP response:");
} //connectTCP

void heartBeat(){
  Serial.println("send TCP heartBeat");
  c2.println(tcpdata);
  c2.println();
} //heartBeat


void loop() {     
    duration = pulseIn(pin, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;
    
    if ((millis()-starttime) >= sampletime_ms)//if the sampel time = = 30s
    {
      ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=&gt;100
      dust = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
      Serial.println("===================================");
      Serial.print("concentration = ");
      Serial.print(dust);
      Serial.println(" pcs/0.01cf");
      Serial.println("\n");
      lowpulseoccupancy = 0;
      starttime = millis();

      if(dht.read())
      {
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();

        Serial.print("Temperature Celcius = ");
        Serial.print(dht.readTemperature());
        Serial.println("C");
    
        Serial.print("Humidity = ");
        Serial.print(dht.readHumidity());
        Serial.println("%");
    
        Serial.println("===================================");
      }
  
      LDateTime.getRtc(&rtc);
      if ((rtc - lrtc) >= per) {
        heartBeat();
        lrtc = rtc;
      }
      //Check for report datapoint status interval
      LDateTime.getRtc(&rtc1);
      if ((rtc1 - lrtc1) >= per1) {
        uploadstatus();
        lrtc1 = rtc1;
      }
  }
}




