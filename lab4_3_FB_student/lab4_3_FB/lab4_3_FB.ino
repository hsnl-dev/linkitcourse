#include <LWiFi.h>
#include <LWiFiClient.h>

#define WIFI_AP "@"        //WiFi AP名稱
#define WIFI_PASSWORD "169251aa"  //WiFi密碼
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define SITE_URL "graph.facebook.com"  // note that "caveeducation" is part of the HTTP request rather than host name.
#define LIKE_SUBSTRING "\"category\":"
#define BUFFER_SIZE 65536


LWiFiClient c;
char buffer[BUFFER_SIZE];

void setup()
{
    //LTask.begin();
    LWiFi.begin();
    Serial.begin(115200);
  
    //while(!Serial)delay(100);

    // keep retrying until connected to AP
    Serial.println("Connecting to AP");
    while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
    {
        delay(1000);
    }

    // keep retrying until connected to website
    Serial.println("Connecting to WebSite");
    while (c.connect(SITE_URL, 80) == 0)
    {
        Serial.println("Re-Connecting to WebSite");
        delay(1000);
    }

    // send HTTP request, ends with 2 CR/LF
    Serial.println("send HTTP GET request");
    c.println("GET /145634995501895 HTTP/1.1");
    c.println("Host: " SITE_URL);
    c.println("Connection: close");
    c.println();

    // 等候伺服器回應
    Serial.println("waiting HTTP response:");
    while (!c.available())
    {
        delay(100);
    }
}

boolean disconnectedMsg = false;

void loop()
{
    /* 讀取http response */
    int n_bytes = c.read((uint8_t*)buffer, sizeof(buffer));
    if (0 == n_bytes)
    {
        Serial.println("disconnected by server");
        while (1);
    }
    //Serial.write(buffer);
    Serial.println();
    /* 尋找"\"category:\""的位置 */
    char *ptr_likes = strstr(buffer, LIKE_SUBSTRING);
    if (ptr_likes == NULL)
    {
        Serial.println("fail to parse response data");
        while (1);
    }
    ptr_likes += strlen(LIKE_SUBSTRING);

    /* 尋找"\"category:\""之後第一個逗號, */
    char *ptr_comma = strstr(ptr_likes, ",");
    if (ptr_comma == NULL)
    {
        Serial.println("fail to parse response data");
        while (1);
    }

    /* 印出結果 */
    Serial.print("category: ");
    Serial.write(ptr_likes, ptr_comma - ptr_likes);
    Serial.print("\n");
    
    while (1);
}


