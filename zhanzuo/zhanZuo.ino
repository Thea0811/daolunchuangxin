#define INTERVAL_SENSOR   1000             //定义传感器采样时间间隔 
#define INTERVAL_NET      1000             //定义发送时间
#define USER_SEL_VERSION VERSION_18        //定义与wifi模块连接的串口，在.h文件中该波特率为9600

//传感器部分（光敏）================================   
#include <Wire.h>                                  //调用库  
#include "./ESP8266.h"
#define  sensorPin_1  A0

//WIFI部分(ESP8266)==================================
#define SSID           "iPhone"                   // cannot be longer than 32 characters!
#define PASSWORD       "123456789"

#define IDLE_TIMEOUT_MS  3000                     //定义未收到数据的等待时间   

//OneNet部分=======================================  
#define HOST_NAME   "api.heclouds.com"
#define DEVICEID   "23349080"
#define PROJECTID "113677"
#define HOST_PORT   (80)
String apiKey="hXmpGJwtjjNcM=ZzS2sWImPrc5s=";
char buf[10];

//数据名称定义部分=================================
#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

bool  youRenOrNot;                                     //用于是否有人判断

#define INTERVAL_OLED 1000

String mCottenData;
String jsonToSend;

#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2); /* RX:D3, TX:D2 */        //使用软串口
ESP8266 wifi(mySerial);                                     //定义一个ESP8266（wifi）的对象

unsigned long net_time1 = millis();                          //数据上传服务器时间
unsigned long sensor_time = millis();                        //传感器采样时间计时器

String postString;                                //用于存储发送数据的字符串

 //初始化函数===========================================
 void setup(void)    
{       
  //初始化串口波特率  
    Wire.begin();
    Serial.begin(115200);   
    while(!Serial);
    pinMode(sensorPin_1, INPUT);

   //ESP8266初始化
    Serial.print("setup begin\r\n");   
    Serial.print("FW Version:");
    Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }

   //加入无线网
   if (wifi.joinAP(SSID, PASSWORD)) {     
    Serial.print("Join AP success\r\n");  
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("\nJoin AP failure\r\n");
  }

  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }

  Serial.print("setup end\r\n");
    
  
}
//循环函数==========================================
void loop(void)     
{   
  if (sensor_time > millis())  sensor_time = millis();  
    
  if(millis() - sensor_time > INTERVAL_SENSOR)              //传感器采样时间间隔  
  {  
    getSensorData();                                        //读串口中的传感器数据
    sensor_time = millis();
  }  

    
  if (net_time1 > millis())  net_time1 = millis();
  
  if (millis() - net_time1 > INTERVAL_NET)                  //发送数据时间间隔
  {                
    updateSensorData();                                     //将数据上传到服务器的函数
    net_time1 = millis();
  }
  
}

//通过与设定的阈值相比较判断是否有人函数==========================================
void getSensorData(){  
    delay(1000);
    youRenOrNot = analogRead(sensorPin_1)>500?true:false;
}

//建立TCP连接及数据传递函数============================================
void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { 
    Serial.print("\ncreate tcp ok\r\n");

jsonToSend="{\"youRenOrNot\":";
jsonToSend+=youRenOrNot?"1":"0";                                //ture为1，false为0
    jsonToSend+="}";

 Serial.print("注意：有人为1，没人为0");
 Serial.println("\nSend:"+jsonToSend);

    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;

  const char *postArray = postString.c_str();                 //将str转化为char数组
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     
     //释放TCP连接
     if (wifi.releaseTCP()) {                                 
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("\ncreate tcp err\r\n");
  }
  
}
