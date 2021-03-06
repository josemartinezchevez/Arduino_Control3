//RFID
#include <RFID.h>
#include <SPI.h>
//----------------------------

//Variables
char val;
int ledPin = 38; 
int ledPin2 = 39; 
int ledPin3 = 40; 
int speaker = 41; 
int panico = 0;
int block_panico = 0;
char OPC = '1';
int s[4];
int x[3];
int leds[3]= {0,0,0};
int tarjeta_maestra[5] = {96,160,202,26,16}; 
char pass_maestra[5] = {'A','B','C','1','2'};
char pass_temp[5];
int pos_array = 0;
bool acceso[2] = {false, false};
bool flag = false;
bool lectura_tarjetas = false;
//----------------------------

//RFDID DEFINICIONES
#define SDA_DIO 9
#define RESET_DIO 8
#define BTN_1 16724175
#define BTN_2 16718055
#define BTN_3 16743045
#define BTN_0 16738455
//---------------------------- 

//INSTANCIA DE LA LIBRERIA
RFID RC522(SDA_DIO, RESET_DIO);
//---------------------------- 


//IR CONTROL REMOTO
#include <IRremote.h>
int RECV_PIN = 13;
int led = 39;
IRrecv irrecv(RECV_PIN);
decode_results results;
//---------------------------- 

//TECLADO 4x4
#include <Keypad.h>
const byte rowsCount = 4;
const byte columsCount = 4; 
char keys[rowsCount][columsCount] = {
   { '1','2','3', 'A' },
   { '4','5','6', 'B' },
   { '7','8','9', 'C' },
   { '*','0','#', 'D' }
};
const byte rowPins[rowsCount] = { 24, 26, 28, 30};
const byte columnPins[columsCount] = { 25, 27, 29, 31 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);
//---------------------------- 

//DIMER

#include <TimerOne.h>

#define channel_1 4 
#define channel_2 5
#define channel_3 6
#define channel_4 7

#define SPEED 20

#define GATE_IMPULSE 5

#define FREQ 84
  
unsigned int  CH1, CH2, CH3, CH4;
unsigned int  buf_CH1, buf_CH2, buf_CH3, buf_CH4;
unsigned char clock_cn;    
unsigned int  clock_tick;   
unsigned char i;  

//----------------------------

//Push-Bottons

const int inputPin = 12;
 
int value = 0;

//---------------------------- 

//Intesidad Variables

unsigned char i_temp;  
int selected = 0;
//------------------------------------
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(speaker, OUTPUT);
  pinMode(inputPin, INPUT);
  Serial.begin(9600); 
  irrecv.enableIRIn();
  SPI.begin();
  RC522.init();

  pinMode(channel_1, OUTPUT);
  pinMode(channel_2, OUTPUT);
  pinMode(channel_3, OUTPUT);
  pinMode(channel_4, OUTPUT);
  attachInterrupt(1, zero_crosss_int, RISING);
  Timer1.initialize(10); 
  Timer1.attachInterrupt( timerIsr );
}

void timerIsr()
{    
    clock_tick++;


    if (clock_cn) 
     {
      clock_cn++;
      
       if (clock_cn==GATE_IMPULSE)
       {
        digitalWrite(channel_1, LOW); 
        digitalWrite(channel_2, LOW); 
        digitalWrite(channel_3, LOW); 
        digitalWrite(channel_4, LOW);
        clock_cn=0;
       }
     }
   
        if (CH1==clock_tick)
         {
          digitalWrite(channel_1, HIGH);
          clock_cn=1;
         }  
    
           if (CH2==clock_tick)
            {
             digitalWrite(channel_2, HIGH);
             clock_cn=1;
            }  
        
              if (CH3==clock_tick)
               {
                digitalWrite(channel_3, HIGH);
                clock_cn=1;
               }  
    
                 if (CH4==clock_tick)
                  {
                   digitalWrite(channel_4, HIGH);
                   clock_cn=1;
                  }   
                  
}

void zero_crosss_int()
{
  CH1=buf_CH1;
   CH2=buf_CH2;
    CH3=buf_CH3;
     CH4=buf_CH4;
  
  clock_tick=0;        
}

unsigned int DIMM_VALUE (unsigned char level)
{
 unsigned int buf_level;

 if (level < 26)  {level=26;}
 if (level > 229) {level=229;}
  
 return ((level*(FREQ))/256)*10;  
}


void loop() {
  
 switch(OPC)
 {
   case '1':
   if (Serial.available()) 
   { 
     val = Serial.read();
   }

    Leer_Teclado();
    Control_Remoto();
    if(lectura_tarjetas  == true)
    {
        Tarjetas();
    }
    Boton_Panico();
    Lectura_Push();

   if(panico == 1 && block_panico != 1)
   {
    Brillitos();
    Panico();
   }
   else
   {
    if(flag == true)
    {
        i = 255;
        buf_CH2=DIMM_VALUE(i);
        buf_CH3=DIMM_VALUE(i);
        buf_CH4=DIMM_VALUE(i);
        digitalWrite(ledPin, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        flag = false;
    }
   }

              switch(val)
              {
               case 'A':
               if(panico != 1)
               {
                leds[0] = leds[0] == 0 ? 1: 0;
                if(leds[0] == 1)
                {
                  i = 25;
                  buf_CH2=DIMM_VALUE(i);delay(SPEED);
                  digitalWrite(ledPin, HIGH); 
                }
                else
                {
                  i = 255;
                  buf_CH2=DIMM_VALUE(i);
                  digitalWrite(ledPin, LOW);
                }
                val = 0;
               }
               break;
               
               case 'B':
               if(panico != 1)
               {
                leds[1] = leds[1] == 0 ? 1: 0;
                if(leds[1] == 1)
                {
                  i = 25;
                  buf_CH3=DIMM_VALUE(i);delay(SPEED);
                  digitalWrite(ledPin2, HIGH); 
                }
                else
                {
                  i = 255;
                  buf_CH3=DIMM_VALUE(i);
                  digitalWrite(ledPin2, LOW);
                }
                val = 0;
               }
               break;
          
               case 'C':
               if(panico != 1)
               {
                leds[2] = leds[2] == 0 ? 1: 0;
                if(leds[2] == 1)
                {
                  i = 25;
                  buf_CH4=DIMM_VALUE(i);delay(SPEED);
                  digitalWrite(ledPin3, HIGH); 
                }
                else
                {
                  i = 255;
                  buf_CH4=DIMM_VALUE(i);
                  digitalWrite(ledPin3, LOW);
                }
                val = 0;
               }
               break;
               
               case 'D':
               if(block_panico != 1)
               {
                panico = panico == 1 ? 0:1;
                flag = panico == 0 ? true:false;
                val = 0;
               }
               break; 

              //Seleccion
               case 'E':
               if(panico != 1)
               {
                selected = selected != 1 ? 1:0;
                  if(selected == 1)
                  {
                    i_temp = 255;
                    Serial.println("Selecciono Led 1");
                  }
                  else
                  {
                    Serial.println("Des-selecciono Led 1");
                    i_temp = 255;
                    buf_CH2=DIMM_VALUE(i_temp);delay(SPEED);
                  }
                  val = 0;
                 }
               break;

               case 'F':
               if(panico != 1)
               {
                  selected = selected != 2 ? 2:0;
                  if(selected == 2)
                  {
                    i_temp = 255;
                    Serial.println("Selecciono Led 2");
                  }
                  else
                  {
                    i_temp = 255;
                    buf_CH3=DIMM_VALUE(i_temp);delay(SPEED);
                    Serial.println("Des-selecciono Led 2");
                  }
                  val = 0;
               }
               break;

               case 'G':
               if(panico != 1)
               {
                  selected = selected != 3 ? 3:0;
                  if(selected == 3)
                  {
                    i_temp = 255;
                    Serial.println("Selecciono Led 3");
                  }
                  else
                  {
                    Serial.println("Des-selecciono Led 3");
                    i_temp = 255;
                    buf_CH4=DIMM_VALUE(i_temp);delay(SPEED);
                  }
                  val = 0;
               }
               break;

              //Intensidad
               case 'H':
               if(panico != 1)
               {
                if(selected != 0){
                  Serial.println("Aumentar Intensidad");
                  i_temp = (i_temp - 25) < 0 ? 0:(i_temp - 25);
                  Serial.println(i_temp);
                    switch(selected){
                      case 1:
                      digitalWrite(ledPin, HIGH);delay(50);digitalWrite(ledPin, LOW);
                      buf_CH2=DIMM_VALUE(i_temp);delay(SPEED);
                      break;
                      case 2:
                      digitalWrite(ledPin2, HIGH);delay(50);digitalWrite(ledPin2, LOW);
                      buf_CH3=DIMM_VALUE(i_temp);delay(SPEED);
                      break;
                      case 3:
                      digitalWrite(ledPin3, HIGH);delay(50);digitalWrite(ledPin3, LOW);
                      buf_CH4=DIMM_VALUE(i_temp);delay(SPEED);
                      break;
                    }
                }
                val = 0;
               }
               break;

               case 'I':
               if(panico != 1)
               {
                if(selected != 0){
                  Serial.println("Reducir Intensidad");
                  i_temp = (i_temp + 25) > 255 ? 255:(i_temp + 25);
                  Serial.println(i_temp);
                    switch(selected){
                      case 1:
                      digitalWrite(ledPin, HIGH);delay(50);digitalWrite(ledPin, LOW);
                      buf_CH2=DIMM_VALUE(i_temp);delay(SPEED);
                      break;
                      case 2:
                      digitalWrite(ledPin2, HIGH);delay(50);digitalWrite(ledPin2, LOW);
                      buf_CH3=DIMM_VALUE(i_temp);delay(SPEED);
                      break;
                      case 3:
                      digitalWrite(ledPin3, HIGH);delay(50);digitalWrite(ledPin3, LOW);
                      buf_CH4=DIMM_VALUE(i_temp);delay(SPEED);
                      break;
                    }
                  }
                }
                val = 0;
                break;

               case 'J':
               Serial.println("Ya se pueden ver las tarjetas");
               lectura_tarjetas = true;
               val = 0;
               break;

             break;
  
             }


          
               /*case 'E':

               break; 
          
               case 'F':
               break; 
          
               case 'G':
               if(block_panico != 1)
               {
                 panico = 1;
               }
               break;
          
               case 'H':
               if(block_panico != 1)
               {
                  i = 255;
                  buf_CH2=DIMM_VALUE(i);
                  buf_CH3=DIMM_VALUE(i);
                  buf_CH4=DIMM_VALUE(i);
                  panico = 0;
               }
               break;

               case 'I':
               break;*/
               delay(100);
               break; 
              }
        //Serial.println(x);
        delay(50);
 }



void Panico()
{
  if(panico == 1)
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(speaker, HIGH);
    delay(30);
    digitalWrite(speaker, LOW);
    digitalWrite(ledPin, LOW);
    delay(30);
    digitalWrite(ledPin2, HIGH);
    delay(30);
    digitalWrite(ledPin2, LOW);
    delay(30);
    digitalWrite(ledPin3, HIGH);
    delay(30);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin3, HIGH);
    delay(30);
    digitalWrite(ledPin3, LOW);
    delay(30);
    digitalWrite(ledPin2, HIGH);
    delay(30);
    digitalWrite(ledPin2, LOW);
    delay(30);
    digitalWrite(ledPin, HIGH);
    delay(30);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(30);
    digitalWrite(ledPin2, LOW);
    delay(30);
    digitalWrite(ledPin3, HIGH);
    delay(30);
    digitalWrite(ledPin3, LOW);
    delay(30);
    digitalWrite(ledPin, HIGH);
    delay(30);
    digitalWrite(ledPin, LOW);
    digitalWrite(speaker, LOW);
    delay(30);
  }
}

void Control_Remoto(){
if (irrecv.decode(&results))
    {
     switch(results.value)
     {
       case BTN_1:
        val = 'A';
       break;

       case BTN_2:
        val = 'B';
       break;

       case BTN_3:
       val = 'C';
       break;

       case BTN_0:
       if(block_panico != 1)
       {
        panico = panico == 1 ? 0:1;
        if(panico == 0)
        {
          val = 'H';
        }
       }
       break;
     }
     //Serial.println(results.value);
     irrecv.resume(); // Receive the next value
    }
}

void Leer_Teclado()
{
   char key = keypad.getKey();
   if (key) {
      Serial.println(key);
      digitalWrite(ledPin, HIGH);delay(50);digitalWrite(ledPin, LOW);
      pass_temp[pos_array] = key;
      pos_array++;

      if(pos_array == 5)
      {
        bool temp = true;
        for(int i = 0;i<5;i++)
        {
          if(pass_temp[i] != pass_maestra[i])
            temp = false;
        }
        if(temp == true)
        {
          Serial.println("Contraseña Correcta");
          acceso[1] = true;
          pos_array = 0;
          digitalWrite(ledPin3, HIGH);
          delay(100);
          digitalWrite(ledPin3, LOW);
        }
        else
        {
          Serial.println("Contraseña Equivocada");
          acceso[0] = acceso[1] = false;
          pos_array = 0;
          digitalWrite(ledPin2, HIGH);
          delay(100);
          digitalWrite(ledPin2, LOW);
        }
      }
   }
}

void Tarjetas()
{
  if (RC522.isCard())
  {
    /* If so then get its serial number */
    RC522.readCardSerial();
    Serial.println("Card detected:");
    bool temp = true;
    for(int i=0;i<5;i++)
    {
    //Serial.println(RC522.serNum[i],DEC);
    if(RC522.serNum[i] != tarjeta_maestra[i])
      temp = false;
    }
    if(temp == true)
    {
      acceso[0] = true;
      digitalWrite(ledPin3, HIGH);
      delay(500);
      digitalWrite(ledPin3, LOW);
    }
    Serial.println(acceso[0]);
    delay(10);
    lectura_tarjetas = false;
    Serial.println("Ya no se pueden ver las tarjetas");
  }
}

void Boton_Panico()
{
  if(acceso[0] == true && acceso[1] == true)
  {
    Serial.println("Boton de Panico A/D");
    block_panico = block_panico == 0 ? 1:0;
    acceso[0] = false;
    acceso[1] = false;
   digitalWrite(ledPin, HIGH);digitalWrite(ledPin2, HIGH);digitalWrite(ledPin3, HIGH);
   delay(100);
   digitalWrite(ledPin, LOW);digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);
  }
}

void Brillitos()
{
for (i=255;i>1;i--) {buf_CH2=DIMM_VALUE(i);buf_CH3=DIMM_VALUE(i);buf_CH4=DIMM_VALUE(i); delay(0);}
}

void Lectura_Push()
{
   value = digitalRead(inputPin);  //lectura digital de pin
   if (value == HIGH) {
      //Serial.println("Encendido");
      value = 0;
   }
}