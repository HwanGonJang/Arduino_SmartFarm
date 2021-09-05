# 아두이노 스마트팜(앱인벤터)_숭실대학교 적정기술 공모전
숭실대학교 적정기술 공모전에 몽골을 위한 적정기술을 주제로 참여하여 진행한 스마트팜 프로젝트.

## 개요
한동대학교의 적정기술 공모전(2등), 숭실대학교의 적정기술 공모전(장려상)에 참여하여 몽골을 주제로 적정기술 프로젝트를 여름방학 동안 진행했습니다. 몽골은 문화적 특성으로 육류 소비량이 채소 소비량보다 압도적으로 많아 최근 채소를 소비하기 위해 노력하고 있다고합니다. 따라서 자연적으로 가정에서 쉽게 재배할 수 있는 스마트팜에 대한 관심이 늘고있고 특히 물이 부족한 나라의 특성으로 수경재배 방식을 선호한다고 합니다.  이에 저희는 몽골 특성에 맞춘 스마트 팜 기술을 고안했고 이 글에서는 전체적인 틀과 소프트웨어 적인 부분을 살펴보겠습니다.

## 개발과정
몽골의 아파트는 대한민국과 거의 비슷하지만 대부분 20평 안팎의 좁은 공간입니다. 따라서 스마트 팜을 개발하더라도 공간적 특성을 고려하여 설계해야했고 저희는 작은 크기의 스마트팜을 모듈화 하여 공간에 맞게 여러개의 팜을 이어 붙일 수 있는 설계를 고안했습니다.

#### 기술
* Arduino
  * 개발을 위한 보드입니다. 아두이노 코딩은 기본적으로 C++을 기반으로 합니다.

### 1. 스마트 팜 모형 모델링
Fusion360을 이용하여 구현할 스마트팜을 모델링 합니다. 하나의 모습과 이어붙인 모습을 모델링했습니다.
<img src='https://github.com/HwanGonJang/HwanGonJang.github.io/blob/master/Pictures/af_3.png?raw=true'>
<img src='https://github.com/HwanGonJang/HwanGonJang.github.io/blob/master/Pictures/af_4.png?raw=true'>

### 2. 재료 구입 및 형태 구현
PVC파이프 등 여러가지 재료를 구입하고 아두이노가 들어갈 모형을 구현합니다.

<img src='https://github.com/HwanGonJang/HwanGonJang.github.io/blob/master/Pictures/af_1.jpg?raw=true'>
<img src='https://github.com/HwanGonJang/HwanGonJang.github.io/blob/master/Pictures/af_2.jpg?raw=true'>

### 3. 아두이노 코딩
~~~c
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
~~~

스마트팜의 led, 습도, 온도, lcd, 펌프 등을 제어하고 앱인벤터로 제작한 스마트팜 제어 어플로 정보를 전달합니다.


### 4. 스마트 팜 제어 어플(앱인벤터)
앱 인벤터는 MIT에서 개발한 간단한 어플을 만들기 위해 제공하는 서비스로 코딩 블럭을 옮기는 것으로 기능을 구현하고 버튼 등 여러가지 UI를 편하게 옮겨 화면을 구성할 수 있습니다. 다음은 앱인벤터로 만든 스마트 팜 제어 어플의 코딩 부분입니다.



## 결과
이렇게 숭실대학교 적정기술 공모전의 스마트 팜을 완성했습니다. 몽골이 아니여도 여러 가정에서 자신의 상황에 맞게 모듈을 붙여서 효율적으로 사용할 수 있을 것이라고 생각합니다.
<img src='https://github.com/HwanGonJang/HwanGonJang.github.io/blob/master/Pictures/af_5.jpg?raw=true'>

아래는 시연영상입니다.
https://youtu.be/_V6L2CyljtU
