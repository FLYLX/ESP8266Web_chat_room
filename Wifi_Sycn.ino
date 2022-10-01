#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h> 
#include <DNSServer.h>


DNSServer dnsServer;
IPAddress apIP(192,168,4,1);
const byte DNS_PORT = 53;


const char *ssid = "网页聊天室";
const char *password = "";

String context[10];
String tmp;

int count = 0;
AsyncWebServer server(80); 


void initDNS(void){
  if(dnsServer.start(DNS_PORT,"*",apIP)){
    Serial.println("dns success");
  }
  else{
    Serial.println("failed");
  }
}


String processor(const String& var){

  return String();
}

void addmessage(AsyncWebServerRequest *request){
  Serial.println("adding");
  if(request->params()){
  Serial.println("in first");
    if(count<10){
      Serial.println("in second");
      context[count]=request->getParam(0)->value().c_str();
      Serial.println(context[count]);
      Serial.println(request->getParam(0)->value().c_str());
      request->send(200,"text/plain",context[count]);
      tmp = context[count];
      count++;
    }else{
      Serial.println("in third");
      for(int i = 9;i>=0;i--){
        delay(200);
        context[i] = context[i-1];
        if(i==0){
          context[i] = request->getParam(0)->value().c_str();
          request->send(200,"text/plain",context[i]);
          Serial.println(context[i]);
          tmp = context[i];
        }
        }
      }
    
  }else{
    Serial.println("NULL message");
    
    }

  
}

void fresh(AsyncWebServerRequest *request){
  /***
  int i =atoi( request->getParam(0)->value().c_str());
  Serial.println(i);
  Serial.println(context[i]);
  ***/
  request->send(200,"text/plain",tmp);
    
}

void handleRoot(AsyncWebServerRequest *request) //回调函数
{
  Serial.println("Root opened");
  request->send(SPIFFS,"/test_1.html",String(),false,processor);
  

  
}

void setup()
{
  Serial.begin(115200);
  Serial.println();


  WiFi.softAP(ssid,password);
 
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot); 
  server.onNotFound(handleRoot);
  server.on("/fresh",HTTP_GET,fresh);
  server.on("/addmessage",HTTP_GET,addmessage);

  server.begin(); //启动服务器
   if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
   }
   initDNS();

  Serial.println("Web server started");
}

void loop(){
  dnsServer.processNextRequest();
  }
