#include "DHT.h"                    //온습도
#include <LiquidCrystal_I2C.h>      //lcd
#include <Adafruit_NeoPixel.h>      //led
#include <SoftwareSerial.h>         //블루투스
#define PIN            7                              // LED 제어 신호핀
#define NUMPIXELS      12                             // 제어하고 싶은 LED 개수
#define DHTPIN         4                              //온습도 센서핀
#define DHTTYPE        DHT11                          //온습도센서 타입 설정

// 0x27 I2C 주소를 가지고 있는 16x2 LCD객체를 생성.
LiquidCrystal_I2C lcd(0x27, 16, 2);
//led 스트립 객체를 생성.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//블루투스 객체를 생성.
SoftwareSerial mySerial(2, 3); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정
//온습도센서 객체를 생성
DHT dht(DHTPIN, DHTTYPE);

int AA = 10;                  // AA를 연결한 디지털 핀(정방향 모터)
int AB = 6;                   // AB를 연결한 디지털 핀(역방향 모터)
String myString = "";         //블루투스 수신 데이터 변수
String text = "none";         //식물 이름 저장 변수
int led = 0;                  //led 제어 변수
int motor = 0;                //모터 제어 변수

void setup() {
  Serial.begin(9600);         //시리얼 모니터 시작
  mySerial.begin(9600);       //블루투스 수신,송신 시작
  pixels.begin();             // led 시작
  dht.begin();                //온습도센서 시작
  lcd.init();                 // I2C LCD를 초기화 합니다..
  lcd.backlight();            // I2C LCD의 백라이트를 켜줍니다.
  pinMode(AA, OUTPUT);        // AA를 출력 핀으로 설정
  pinMode(AB, OUTPUT);        // AB를 출력 핀으로 설정
}

void loop() {
  //앱에서 데이터 수신
  myString="";
  while(mySerial.available())  //mySerial 값이 있으면
  {
    char myChar = (char)mySerial.read();  //mySerial int형식의 값을 char형식으로 변환
    myString+=myChar;                     //수신되는 문자열을 myString에 모두 붙임 (1바이트씩 전송되는 것을 모두 붙임)
    delay(5);                             //수신 문자열 끊김 방지
  }   
  Serial.println(myString);
  
  lcd.clear();      //lcd 초기화
 
  int humidity = dht.readHumidity();            //습도 변수 선언
  int temperature = dht.readTemperature();      //온도 변수 선언
  
  // LCD에 출력할 습도 메세지를 만듭니다.
  String humi = "Hum ";
  humi += (String)humidity;
  humi += "%";
 
  // LCD에 출력할 온도 메세지를 만듭니다.
  String temp = "Temp ";
  temp += (String)temperature;
  temp += "C";
  
  //Serial.print(humidity);
  //Serial.println(temperature);
  
  //온습도 앱으로 송신
  mySerial.print(humidity);
  mySerial.print(temperature);
  
  //lcd 출력
  lcd.setCursor(0,0);
  lcd.print("Plant:");
  lcd.setCursor(0,1);           
  lcd.print(temp);  
  lcd.print(" ");         
  lcd.print(humi);

   //받은 데이터가 on이면 led 변수를 1로 설정/off면 0으로 설정
   if(myString == "on") {
    led = 1;
   } else if(myString == "off") {
    led = 0;
    
   }
    Serial.println(led);
   //led가 1이면 led 켜기/0이면 끄기
   if(led == 1) {
    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i,95,0,255); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
     }
   }
   
   if(led == 0) {
     for(int i=0;i<NUMPIXELS;i++){  
       pixels.setPixelColor(i,0,0,0);
       pixels.show();
     }
   }

  //led와 동일
  if(myString == "mon") {
    motor = 1;
   } else if(myString == "moff") {
    motor = 0;
   }
   
   if(motor == 1) {
    digitalWrite(AA, HIGH);  // 정방향으로 모터 회전
    digitalWrite(AB, LOW);
   }
   
   if(motor == 0) {
    digitalWrite(AA, LOW);  // 모터 정지
    digitalWrite(AB, LOW);
   }

   //수신 받은 식물이름을 text 전역변수에 저장 후 상시출력
   if(myString != "on" && myString != "off" && myString != "mon" && myString != "moff" && myString != "") {
    text = myString;
   }
   lcd.setCursor(7,0); 
   lcd.print(text);
   delay(2000);  
}
