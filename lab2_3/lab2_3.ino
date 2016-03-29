#include <LGPS.h>
#include <LDateTime.h>
#include <Grove_LED_Bar.h>
gpsSentenceInfoStruct info;
datetimeInfo t;
char buff[256];
double latitude,longitude;
char localNS,localWE;
Grove_LED_Bar bar(9, 8, 0);  // Clock pin, Data pin, Orientation

static unsigned char getComma(unsigned char num,const char *str)
{
  unsigned char i,j = 0;
  int len=strlen(str);
  for(i = 0;i < len;i ++)
  {
     if(str[i] == ',')
      j++;
     if(j == num)
      return i + 1; 
  }
  return 0; 
}

static double getDoubleNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atof(buf);
  return rev; 
}
/*
static char getChar(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atof(buf);
  return rev; 
}
*/
void parseGPGGA(const char* GPGGAstr)
{
  /* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
   * Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
   * Where:
   *  GGA          Global Positioning System Fix Data
   *  123519       Fix taken at 12:35:19 UTC
   *  4807.038,N   Latitude 48 deg 07.038' N
   *  01131.000,E  Longitude 11 deg 31.000' E
   *  1            Fix quality: 0 = invalid
   *                            1 = GPS fix (SPS)
   *                            2 = DGPS fix
   *                            3 = PPS fix
   *                            4 = Real Time Kinematic
   *                            5 = Float RTK
   *                            6 = estimated (dead reckoning) (2.3 feature)
   *                            7 = Manual input mode
   *                            8 = Simulation mode
   *  08           Number of satellites being tracked
   *  0.9          Horizontal dilution of position
   *  545.4,M      Altitude, Meters, above mean sea level
   *  46.9,M       Height of geoid (mean sea level) above WGS84
   *                   ellipsoid
   *  (empty field) time in seconds since last DGPS update
   *  (empty field) DGPS station ID number
   *  *47          the checksum data, always begins with *
   */
  int tmp1,tmp2;
  if(GPGGAstr[0] == '$')
  {
    tmp1 = getComma(2, GPGGAstr);
    tmp2 = getComma(3, GPGGAstr);
    latitude = getDoubleNumber(&GPGGAstr[tmp1]);
    localNS = getDoubleNumber(&GPGGAstr[tmp2]);
    sprintf(buff, "Latitude = %10.4f , %c", latitude,localNS);//////////////////////////////////
    Serial.println(buff);   
      
    tmp1 = getComma(4, GPGGAstr);
    tmp2 = getComma(5, GPGGAstr);
    longitude = getDoubleNumber(&GPGGAstr[tmp1]);
    localWE = getDoubleNumber(&GPGGAstr[tmp2]);
    sprintf(buff, "Longitude = %10.4f , %c", longitude,localWE);///////////////////////////////////////
    Serial.println(buff);   
  }
  else
  {
    Serial.println("Not get data"); 
  }
}

void setup() {
  Serial.begin(115200);
  bar.begin();
  LGPS.powerOn();
  Serial.println("LGPS Power on, and waiting ..."); 
  delay(3000);
  //LGPS.getData(&info);
  //Serial.println((char*)info.GPGGA); 
}


void loop() {
  int i;
  LGPS.powerOn();
  delay(3000);
  LGPS.getData(&info);
  Serial.println((char*)info.GPGGA);
  parseGPGGA((const char*)info.GPGGA);  
  double error=abs(latitude-2447.6868)+abs(longitude-12059.6039);
  if (error>0.1)
      i=0;
  else if(error<=0.1 & error>0.08) 
      i=1;
  else if(error<=0.08 & error>0.06) 
      i=3;
  else if(error<=0.06 & error>0.04) 
      i=5;
  else if(error<=0.04 & error>0.02) 
      i=7;
  else if(error<=0.02 & error>0) 
      i=9;
  else if(error==0) 
      i=10;
  bar.setLevel(i);
  Serial.println(error);
  delay(1000);
}
