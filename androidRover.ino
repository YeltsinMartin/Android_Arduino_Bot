// android controlled rover


#define SONAR_TRIGGER_PIN     2
#define SONAR_ECHO_PIN        4
#define motorRpinF 10
#define motorRpinR 9
#define motorLpinF 6
#define motorLpinR 5
#define LedPin 13
#define LedPin1 3

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(11, 12); // RX | TX
char btChar[5];
unsigned int range = 0 , interval = 250, interval1 = 500 ;
unsigned long currentMillis = 0, previousMillis = 0;
int temp = 0 , i = 0 ,  Sint[6];
boolean Heading = HIGH , ledState = LOW, BtledState = LOW;
//int motor_func(int motorRf , int motorRr, int motorLf, int motorLr);

void setup() {
  

  Serial.begin(9600);
  Serial.println("Wwlcome To Android Controlled Rover");
  BTSerial.begin(115200);  // HC-05 default speed in AT command more
  pinMode(motorRpinF, OUTPUT);
  pinMode(motorRpinR, OUTPUT);
  pinMode(motorLpinF, OUTPUT);
  pinMode(motorLpinR, OUTPUT);
  pinMode(LedPin, OUTPUT);
  pinMode(LedPin1, OUTPUT);
  pinMode(SONAR_TRIGGER_PIN, OUTPUT);
  pinMode(SONAR_ECHO_PIN, INPUT);
}

void loop() {


           currentMillis = millis();
         
         if(currentMillis - previousMillis >= interval) 
        { 
            range = measure_distance();
          //  Serial.println(range);
        }
        
        
          
            while (BTSerial.available())
              {
            
                  for( i= 0 ; i < 6; i++)
                        {  
                          btChar[i] = BTSerial.read();
                          Serial.println(btChar[i]);
                          if (btChar[i] == '\n')
                          break;                
                        }
              }
        
              
                for (int j = 0 ; j <6 ; j ++)
                {
                  Sint[j] =  btChar[j] - 48;
                 // Serial.println(Sint[j]);
                }
        
            
            if (btChar[0] == '1')
            {
               BtledState = HIGH;
            }
            if (btChar[0] == '0')
            {
               BtledState = LOW;
            }
            
             if (btChar[0] == 'p'){
            //Serial.println(i);
            {
              if (i == 3) temp = Sint[1] ;
              
              if (i == 4) temp = (Sint[1] *10)  + (Sint[2]);
        
              if (i == 5) temp = (Sint[1] *100)  + (Sint[2]*10) + (Sint[3]);
            Serial.println(temp);
            analogWrite(LedPin1, temp);
             }
            
            }
    

     
           if (btChar[0] == 'F')                                 // Check if forward
                    {
                      if (btChar[1] == 'f')
                      {
                        Heading = HIGH;
                        Serial.println("Rover Forward");
                      }
                    }

            if (btChar[0] == 'R')                                 // check if backward
                    {
                      if (btChar[1] == 'r')
                      {
                        Heading = LOW;
                        Serial.println("Rover Reverse");
                      }
                    }
            
            
            
            if ( range > 10)
            {     

              
                    if (btChar[0] == 'x')                                 // Spin clockwise
                    {
                         motor_func( 0 , 255,  255,  0);
                    }
                   if (btChar[0] == 'y')                                 // Spin clockwise
                    {
                         motor_func( 255 , 0,  0,  255);
                    } 
                      if (Heading == HIGH)
                      {
                            if( btChar[0] == 'g' )
                          {
                                 motor_func( 255 , 0,  255,  0);
                                 Serial.println("Rover Forward");
                                 Serial.println(range);
                          }
                          if( btChar[0] == 's')
                          {
                                 motor_func( 0 , 0,  0,  0);
                                 Serial.println("Rover Stop");
                            
                          }
                          if( btChar[0] == 'l')
                          {
                                 motor_func( 255 , 0,  75,  0);
                                 Serial.println("Rover Left");
                            
                          }
                          if( btChar[0] == 'r')
                          {
                                 motor_func( 75 , 0,  255,  0);
                                 Serial.println("Rover Right");
                          }
                      }

                   
                 }


           if (Heading == LOW)
                      {
                            if( btChar[0] == 'g' )
                          {
                                 motor_func( 0 , 0,  0,  0);
                                 Serial.println("Rover Stop");
                                // Serial.println(range);
                          }
                          if( btChar[0] == 's')
                          {
                                 motor_func( 0 , 240,  0,  255);
                                 Serial.println("Rover Reverse");
                            
                          }
                          if( btChar[0] == 'l')
                          {
                                 motor_func( 0 , 255,  0,  75);
                                 Serial.println("Rover Rev Left");
                            
                          }
                          if( btChar[0] == 'r')
                          {
                                 motor_func( 0 , 75,  0,  255);
                                 Serial.println("Rover Rev Right");
                          }
                    }

    
          else if (range < 20)
          {
            motor_func( 0 , 0,  0,  0);
            Serial.println("Stopped");
          }
      
          for(int i= 0 ; i < 5; i++)
                  {  
                    btChar[i] = '\0';
                  }


      if(BtledState == HIGH )
      {
          if(currentMillis - previousMillis >= interval1) 
          {
            
            previousMillis = currentMillis;
             if (ledState == LOW)
                ledState = HIGH;
             else
                ledState = LOW;
            
            digitalWrite(LedPin, ledState);
          }
      }
      if (BtledState == LOW)
      {
        digitalWrite(13, LOW);
      }
  }

    
    
  



unsigned int measure_distance()
{
   // Trigger the SRF05:
   digitalWrite(SONAR_TRIGGER_PIN, HIGH);
   delayMicroseconds(10);
   digitalWrite(SONAR_TRIGGER_PIN, LOW);

   // Wait for Echo Pulse
   unsigned long pulse_length = pulseIn(SONAR_ECHO_PIN, HIGH);

   // Ensure the ultrasonic "beep" has faded away
   delay(50);

   // Convert Pulse to Distance (inches) 
   // pulse_length/58 = cm or pulse_length/148 = inches
   return( (unsigned int) (pulse_length / 58) );
}

int motor_func(int motorRf , int motorRr, int motorLf, int motorLr)
{
   analogWrite(motorRpinF,motorRf );
   analogWrite(motorRpinR,motorRr );
   analogWrite(motorLpinF,motorLf );
   analogWrite(motorLpinR,motorLr );
   
}

