#include<ESP8266WiFi.h>

const char* host="152.163.0.69";           //ip adress of aol server
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.begin("Yo!","12345678a");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print(WiFi.SSID());
  Serial.println("connected");
  

}

void loop() {
  
  if(client.connect(host,587)){
    Serial.println("connected");
    Serial.println("sending helo");
    client.println("HELO 192.168.43.243");
   void  response();
    Serial.println("sending auth login");
    client.println("AUTH LOGIN");
    void response();
    client.println("dHRscHJvamVjdDIwMTg=");  //aol username
   void response();
    client.println("YW9sQDEyMw==");                //aol password
   void response();
    client.println("MAIL FROM:<ttlproject2018@aol.com>");
   void response();
    client.println("RCPT TO:<ttlproject2018@gmail.com>");
    void response();
    Serial.print("sending data");
    client.println("DATA");
    void response();
    client.println("From: ttlproject2018@aol.com <ttlproject2018@aol.com>");
    void response();
    client.println("To: ttlproject2018@gmail.com <ttlproject2018@gmail.com>");
    void response();
    client.println("Subject: analog");
    void response();
    client.printf("analog input:%d\n",analogRead(2));
    client.println(".");
    void response();
    client.println("QUIT");
    void response();
    client.stop();
    Serial.println("disconnected");
  }
  delay(6000);
}
void response(){
 
    while(client.available()){
      String line=client.readStringUntil('\n');
      Serial.println(line);
    }
}

    
