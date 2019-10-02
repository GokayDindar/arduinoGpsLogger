#include <TinyGPS.h>
#include <String.h>

float oncekienlem=0.0;
float oncekiboylam=0.0;
float latitude,longitude;

bool mesaj = false;
bool git = false;
int dur;
int year;

int sayac1 = 0;
int sayac2 = 0;



byte month, day, hour, minute, second, hundredths;

TinyGPS shield;

void setup() 
{
  Serial.begin(115200);
  Serial2.begin(115200);///GPS
  Serial1.begin(19200);///GSM

}
void getgps(TinyGPS &gps)
{
 
  shield.f_get_position(&latitude, &longitude);
  shield.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
  Serial.println("--------------------------------------------------------------");
  Serial.print("ENLEM: "); 
  Serial.print(latitude,5); 
  Serial.print(" BOYLAM: "); 
  Serial.println(longitude,5);
  

    if((oncekienlem != latitude) ||(oncekiboylam != longitude))
     {
        
        Serial.println("1");
        sayac2++;
        oncekienlem = latitude;
        oncekiboylam = longitude;
        
         if((mesaj == false) && (sayac1 > 1000))
           {
               SendTextMessage();
               mesaj == true;
               sayac1 = 0;      
           }
         SendSQL(); 
          if(sayac2 == 32767)
           {
            sayac2 = 0;       
           }
       }     
    
   else
   {
     mesaj = false;
     sayac1++;
     sayac2 = 0;
     Serial.println(sayac1);
     if(sayac1 == 32767)
     {
         sayac1 = 0;
     }
     if(minute == 30){
         SendSQL();
         delay(60000);
     }
   }
}

void loop()
{
  byte a;
  if ( Serial2.available() > 0 ) 
  {
    a = Serial2.read(); 
    if(shield.encode(a)) 
    {
      getgps(shield); 
    }
  }
}

void SendTextMessage() // mesaj atar
{
  Serial.println("Sending Text...");
  Serial1.print("AT+CMGF=1\r"); 
  delay(100);
 
  Serial1.println("AT+CMGS = \"+905378200170\"");
  delay(100);
  Serial1.println("Arac Harekete Basladi:");
  delay(50);
  Serial1.println("http://46.197.249.53/harita.php");
  delay(100);
  Serial1.print((char)26);
  delay(100);
  Serial1.println();
  Serial.println("Text Sent.");
 
}
 
void SendSQL()
{
Serial1.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
delay(500);
Serial1.println("AT+SAPBR=3,1,\"APN\",\"CMNET\"");
delay(500);
Serial1.println("AT+SAPBR=1,1");
delay(500);
Serial1.println("AT+SAPBR=2,1");
delay(3000);
Serial1.println("AT+HTTPINIT");
delay(500);
Serial1.println("AT+HTTPPARA=\"CID\",1");
delay(500);
Serial1.print("AT+HTTPPARA=\"URL\",\"http://46.197.249.53/add_data.php?");
Serial1.print("gelenenlem=");
Serial1.print( latitude,5); 
Serial1.print("&gelenboylam=");
Serial1.print(longitude,5);
Serial1.println("\"");
delay(3000);
Serial1.println("AT+HTTPACTION=0");
delay(15000);
Serial1.println("AT+HTTPTERM");
delay(500);
Serial1.println("AT+SAPBR=0,1");
delay(500);
Serial.println("Databaseye Gonderildi");

}
