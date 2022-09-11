#include<SoftwareSerial.h>
SoftwareSerial Serial1(2,3);     //make RX arduino line is pin 2, make TX arduino line is pin 3.
SoftwareSerial gps(10,11);

#include<LiquidCrystal.h>
LiquidCrystal lcd(14,15,16,17,18,19);

boolean No_IP=false;
String IP="";

String webpage="";
int i=0,k=0;
int  gps_status=0;
String name="<p>1. Name: Your Name </p>";   //22
String dob="<p>2. DOB: 12 feb 1993</p>";     //21
String number="<p>4. Vehicle No.: RJ05 XY 4201</p>";//29
String cordinat="<p>Coordinates:</p>";          //17
String latitude=""; 
String logitude="";                       

String gpsString="";
char *test="$GPGGA";

void check4IP(int t1)
{
  int t2=millis();
  while(t2+t1>millis())
  {
    while(Serial1.available()>0)
    {
      if(Serial1.find("WIFI GOT IP"))
      {
        No_IP=true;
      }
    }
  }
}

void get_ip()
{
  IP="";
  char ch=0;
  while(1)
  {
    Serial1.println("AT+CIFSR");
    while(Serial1.available()>0)
    {
      if(Serial1.find("STAIP,"))
      {
        delay(1000);
        Serial.print("IP Address:");
        while(Serial1.available()>0)
        {
          ch=Serial1.read();
          if(ch=='+')
          break;
          IP+=ch;
        }
      }
      if(ch=='+')
      break;
    }
    if(ch=='+')
    break;
    delay(1000);
  }
  lcd.clear();
  lcd.print(IP);
  lcd.setCursor(0,1);
  lcd.print("Port: 80");
  Serial.print(IP);
  Serial.print("Port:");
  Serial.println(80);
  delay(1000);
}

void connect_wifi(String cmd, int t)
{
  int temp=0,i=0;
  while(1)
  {
    Serial.println(cmd);
    Serial1.println(cmd); 
    while(Serial1.available()>0)
    {
      if(Serial1.find("OK"))
      {

      i=8;
      }
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8)
  {
    Serial.println("OK");
  }
  else
  {
  Serial.println("Error");
  }
  delay(1000);
}

void setup() 
{
  Serial1.begin(9600);
  Serial.begin(9600);
  lcd.begin(16,2);  
  lcd.print("Vehicle Tracking");
  lcd.setCursor(0,1);
  lcd.print("     System     ");
  delay(2000);
  lcd.clear();
  lcd.print("WIFI Connecting..");
//  lcd.setCursor(0,1);
//  lcd.print("Please Wait...");
  delay(1000);
  connect_wifi("AT",1000);
  connect_wifi("AT+CWMODE=3",1000);
  connect_wifi("AT+CWQAP",1000);  
  connect_wifi("AT+RST",5000);
  check4IP(5000);
  if(!No_IP)
      {
        Serial.println("Connecting Wifi....");
        connect_wifi("AT+CWJAP=\"1st floor\",\"muda1884\"",7000);   //AT+CWJAP=”wifi_username”,”wifi_password”
      }
      else
        {
        }
      Serial.println("Wifi Connected");
      lcd.clear();
      lcd.print("WIFI Connected");
      delay(2000);
      lcd.clear();
      lcd.print("Getting IP"); 
      get_ip();
      delay(2000);
      connect_wifi("AT+CIPMUX=1",100);
      connect_wifi("AT+CIPSERVER=1,80",100);
      Serial1.end();
      lcd.clear();
      lcd.print("Waiting For GPS");
      lcd.setCursor(0,1);
      lcd.print("     Signal    ");
      delay(2000);
      gps.begin(9600);
      get_gps();
      show_coordinate();
      gps.end();
      Serial1.begin(9600);
      delay(2000);
      lcd.clear();
      lcd.print("GPS is Ready");
      delay(1000);
      lcd.clear();
      lcd.print("System Ready");
      Serial.println("System Ready..");
}

void loop() 
{
  k=0;
  Serial.println("Please Refresh Ur Page");
  lcd.setCursor(0,0);
  lcd.print("Please Refresh  ");
  lcd.setCursor(0,1);
  lcd.print("Your Web Page..  ");
  while(k<1000)
  {
    k++;

   while(Serial1.available())
   {
    if(Serial1.find("0,CONNECT"))
    {
      Serial1.end();
  gps.begin(9600);
  get_gps();
  gps.end();
  Serial1.begin(9600);
  Serial1.flush();
   /* lcd.clear();
      lcd.print("Sending Data to ");
      lcd.setCursor(0,1);
      lcd.print("    Web Page     ");*/
      Serial.println("Start Printing");
      Send();
      show_coordinate();
      Serial.println("Done Printing");
      delay(5000);
      lcd.clear();
      lcd.print("System Ready");
      delay(1000);
      k=1200;
      break;
    }
  }
  delay(1);
 }
}

void gpsEvent()
{
  gpsString="";
  while(1)
  {
   while (gps.available()>0)                //Serial incoming data from GPS
   {
    char inChar = (char)gps.read();
     gpsString+= inChar;                    //store incoming data from GPS to temporary string str[]
     i++;
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])       //check for right string
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>65)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}

void get_gps()
{
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    int str_lenth=i;
    latitude="";
    logitude="";
    coordinate2dec();
        i=0;x=0;
    str_lenth=0;
   }
}

void show_coordinate()
{
    lcd.clear();
    lcd.print("Latitide:");
    lcd.print(latitude);
    lcd.setCursor(0,1);
    lcd.print("Longitude:");
    lcd.print(logitude);
    Serial.print("Latitude:");
    Serial.println(latitude);
    Serial.print("Longitude:");
    Serial.println(logitude);
}

void coordinate2dec()
{
    
    //j=0;
    String lat_degree="";
    for(i=18;i<20;i++)          //extract latitude from string
      lat_degree+=gpsString[i];
    
    String lat_minut="";
    for(i=20;i<28;i++)
      lat_minut+=gpsString[i];
    
    String long_degree="";
    for(i=30;i<33;i++)          //extract longitude from string
      long_degree+=gpsString[i];
      
    String long_minut="";
    for(i=33;i<41;i++)
      long_minut+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= long_minut.toFloat();
     minut=minut/60;
     degree=long_degree.toFloat();
     logitude=degree+minut;
}

void Send()
{
     
      webpage = "<h1>Welcome to Saddam Khan's Page</h1><body bgcolor=f0f0f0>";
      webpage+=name;
      webpage+=dob;
      webpage+=number;
      webpage+=cordinat;
      webpage+="<p>Latitude:";
      webpage+=latitude;
      webpage+="</p>";
      webpage+="<p>Longitude:";
      webpage+=logitude;
      webpage+="</p>";
     webpage+= "<a href=\"http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
     webpage+=latitude;
     webpage+='+';              //28.612953, 77.231545   //28.612953,77.2293563
     webpage+=logitude;
     webpage+="\">Click Here for google map</a>";
     sendwebdata();
       webpage="";

     while(1)
     {
      Serial.println("AT+CIPCLOSE=0");
      Serial1.println("AT+CIPCLOSE=0");
      while(Serial1.available())
      {
        //Serial.print(Serial1.read());
        if(Serial1.find("0,CLOSE"))
        {
          return;
        }
      }
      delay(500);
      i++;
      if(i>5)
      {
        i=0;
      }
      if(i==0)
      break;
     }
}

void sendwebdata()
{
     i=0;
     while(1)
     {
      unsigned int l=webpage.length();
      Serial1.print("AT+CIPSEND=0,");
      Serial1.println(l+2);
      Serial.println(l+2);
      Serial.println(webpage);

      Serial1.println(webpage);
      while(Serial1.available())
      {
        if(Serial1.find("OK"))
        {
         return;
        }
      }
            i++;
      if(i>5)
        i=0;
        if(i==0)
        break;
      delay(200);
     }
}
