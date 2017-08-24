#include <SoftwareSerial.h>                                                                                                                                                                                                                                                                                           

#define echo 2
#define trig 3

#define IN_1 4
#define IN_2 5
#define IN_3 6
#define IN_4 7

#define BT_TX 8 
#define BT_RX 9

#define EN_A 10
#define EN_B 11

int speed;
int safe_flag;

SoftwareSerial BT(BT_TX, BT_RX);


void car_control()
{
    if (Serial.available() || BT.available()) 
    {
        char val;
        if(Serial.available())
        {
          val = Serial.read();
          BT.print(val);
        }
        else 
        {
          val = BT.read();
          Serial.print(val);
        }
      
          switch(val)
          {
            case '0': //forward
              forward();
              break;
            case '1': //back
              backward();
              break;
            case '2': //right
              right();
              break;
            case '3': //left
              left();
              break;          
            case '4': //speedpup
              speedup();
              break;      
            case '5': //spedddown
              speeddown();
              break;      
            case '6': //stop
              stop();
              break;      
          }
    }
 }



            
int check_distance()
{
    float duration, distance;
    digitalWrite(trig, HIGH);
    delayMicroseconds(1000);
    digitalWrite(trig, LOW);
    duration = pulseIn (echo, HIGH);
    distance = (duration/2)/29;
    Serial.print("distance = ");
    Serial.print(distance);
    Serial.println(" cm");  
    if(distance < 20)
        return 1;
    else
        return 0;
}

void forward()
{    
      analogWrite(EN_A, speed);    //輸出 PWM 脈衝到 ENA
      analogWrite(EN_B, speed);    //輸出 PWM 脈衝到 ENB   
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
}

void backward()
{
    analogWrite(EN_A, 64);    //輸出 PWM 脈衝到 ENA
    analogWrite(EN_B, 64);    //輸出 PWM 脈衝到 ENB   
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
}

void left()
{
    analogWrite(EN_A, speed);    //輸出 PWM 脈衝到 ENA
    analogWrite(EN_B, speed);    //輸出 PWM 脈衝到 ENB   
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
}


void right()
{
    analogWrite(EN_A, speed);    //輸出 PWM 脈衝到 ENA
    analogWrite(EN_B, speed);    //輸出 PWM 脈衝到 ENB   
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
}

void stop()
{
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, LOW);
    speed = 128;
}

void speedup()
{
    speed*=2;
    if(speed>=255)
      speed = 255;
}

void speeddown()
{
    speed/=2;
    if(speed <= 64)
      speed = 64;
}

void setup() 
{    
    Serial.begin(9600);
    Serial.println(" ");
    
    pinMode (trig, OUTPUT);
    pinMode (echo, INPUT);
    Serial.println("PING is ready!");
    
    pinMode(IN_1 , OUTPUT);
    pinMode(IN_2 , OUTPUT);
    pinMode(IN_3 , OUTPUT);
    pinMode(IN_4 , OUTPUT);
    pinMode(EN_A , OUTPUT);
    pinMode(EN_B , OUTPUT);
    safe_flag = 1;
    stop();
    Serial.println("CAR is ready!");

    BT.begin(9600);
    Serial.println("Bluetooth is ready!");
    
}

void loop() 
{
    int check = check_distance();
    if(check == 1)
    {
      safe_flag = 0;
      backward();
    }
    else
    {   
      if(safe_flag == 0)
      {
         stop();
         safe_flag = 1;
      }
       car_control();
    }
}
