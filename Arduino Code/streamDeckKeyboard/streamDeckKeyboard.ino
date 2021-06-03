/* DIY Stream Deck Keyboard Code (DIY Stream Deck - Super Make Something Episode 22)
 * by: Alex - Super Make Something
 * date: November 8th, 2020
 * license: Creative Commons - Attribution - Non-Commercial.  More information available at: http://creativecommons.org/licenses/by-nc/3.0/
 */

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Keyboard.h>
#include <Encoder.h>

Encoder upDown(10,15);
Encoder leftRight(14,16);

typedef struct {
    uint8_t pin;
    uint8_t key;
    uint8_t pinStateLast;
} pinKey_t;

#define NUM_KEYS 8

pinKey_t pinKey1[NUM_KEYS] = {
    { 2, 'a', HIGH }, //button r1.c2    
    { 3, 'b', HIGH }, // PAGE SELECT    
    { 4, 'c', HIGH }, //button r2.c1    
    { 5, 'd', HIGH }, //button r2.c2    
    { 6, 'e', HIGH }, //button r2.c3    
    { 7, 'f', HIGH }, //knob 1          
    { 8, 'g', HIGH }, //knob 2   
    { 9, 'h', HIGH }, //button r1.c1           
}; 

pinKey_t pinKey2[NUM_KEYS] = {  
    { 2, 'j', HIGH }, //button r1.c2    
    { 3, 'k', HIGH }, // PAGE SELECT    
    { 4, 'l', HIGH }, //button r2.c1    
    { 5, 'm', HIGH }, //button r2.c2    
    { 6, 'n', HIGH }, //button r2.c3    
    { 7, 'o', HIGH }, //knob 1          
    { 8, 'p', HIGH }, //knob 2   
    { 9, 'q', HIGH }, //button r1.c1         
}; 

int pageFlag = 1; // Flag for button pages

long posUpDown        = 0; //Position counter for up/down encoder
long posLeftRight     = 0; //Position counter for left/right encoder
long posUpDownPrev    = 0; //Previous position of up/down encoder
long posLeftRightPrev = 0; //Previous position of left/right encoder

int counterUpDown = 0;
int counterLeftRight = 0;

bool pressed_flag = false;

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.print("Initializing Pins...");

  // Initialize pins
  for (uint8_t i = 0; i < NUM_KEYS; i++) 
  {
      // Set pin to input
      pinMode(pinKey1[i].pin, INPUT_PULLUP);

      // Set current pin state
      pinKey1[i].pinStateLast = digitalRead(pinKey1[i].pin);
  }
  
  Serial.println(" Done!");

  Serial.print("Initializing Keyboard...");
  //Keyboard.begin();
  Serial.println(" Done!");

  delay(50);

  digitalWrite(A0, LOW);
  
  Serial.println("Setup Complete!");
}

void loop() 
{
  process_buttons();
  process_encoders();

  if(pressed_flag)
  {
    //delay(50);
    Keyboard.releaseAll(); // Release all pressed keys
  }
}

/* ************************************ */
/*               BUTTONS                */
/* ************************************ */

void process_buttons()
{
    uint8_t pinState;
    uint8_t pinsState = 0;
    
    // BUTTONS
    for (uint8_t i = 0; i < NUM_KEYS; i++) 
    {
        pinState = digitalRead(pinKey1[i].pin);
        pinsState |= pinState << i;
        
        // Check if pin changed since last read
        if (pinKey1[i].pinStateLast != pinState) 
        {
            Serial.print("Button Changed: ");
            Serial.println(i);
  
            if(pinState == LOW)
            {
              Serial.print("Button Pressed: ");
              Serial.println(i);
              //button was "just" pressed
              switch (i)
              {
                case 0: // pin 2
                case 2: // pin 4
                case 3: // pin 5
                case 4: // pin 6
                case 5: // pin 7
                case 6: // pin 8
                case 7: // pin 9
                  press_key_from_list(i);
                  break;
                case 1: // pin 3
                  change_page();
                  break;
                default:
                  break;
              }
              
            } //end button just pressed
        } //end key change
        pinKey1[i].pinStateLast = pinState;
    } //end loop

    //Serial.print("Button Readings: ");
    //Serial.println(pinsState, BIN);
}

void press_key_from_list(byte i)
{
    switch (pageFlag) //Depending on pageFlag, send different keystroke
    {
      case 1:
        Keyboard.press(pinKey1[i].key);
        break;
      default:
        Keyboard.press(pinKey2[i].key);
        break;
    }
    pressed_flag = true;
    Serial.print("Key Pressed");
}

void change_page()
{
    if (pageFlag == 1)
    {
      pageFlag = 2;
      digitalWrite(A0, HIGH);
    }
    else //if (pageFlag == 2)
    {
      pageFlag = 1;
      digitalWrite(A0, LOW);
    }
    Serial.print("Page Flag: ");
    Serial.println(pageFlag);
}

/* ************************************ */
/*              ENCODERS                */
/* ************************************ */

void process_encoders()
{
    // ENCODERS
    posUpDown = upDown.read(); //Up/down encoder
    if (posUpDown!=posUpDownPrev) //If new position doesn't equal previous position
    {
      if ((posUpDown-posUpDownPrev)>0)
      {
        counterUpDown = counterUpDown+1;
        if (counterUpDown>3) // 3 counts per physical encoder "click"
        {
          Keyboard.press(KEY_UP_ARROW);
          pressed_flag = true;
          counterUpDown = 0;
        }
      }
      else if ((posUpDown-posUpDownPrev)<0)
      {
        counterUpDown = counterUpDown-1;
        if (counterUpDown<-3)
        {
          Keyboard.press(KEY_DOWN_ARROW);
          counterUpDown = 0;
          pressed_flag = true;
        }
      }
    }
    if(pressed_flag)
    {
      //delay(50);
      Keyboard.releaseAll();
    }
    posUpDownPrev = posUpDown;
    //Serial.println(counterUpDown);
  
    posLeftRight = leftRight.read(); // Left/right encoder
    if (posLeftRight!=posLeftRightPrev) //If new position doesn't equal previous position
    {
      if ((posLeftRight-posLeftRightPrev)>0)
      {
        counterLeftRight = counterLeftRight+1;
        if (counterLeftRight>3) // 3 counts per physical encoder "click"
        {
          Keyboard.press(KEY_RIGHT_ARROW);
          counterLeftRight = 0;
          pressed_flag = true;
        }
      }
      else if ((posLeftRight-posLeftRightPrev)<0)
      {
        counterLeftRight = counterLeftRight-1;
        if (counterLeftRight<-3)
        {
          Keyboard.press(KEY_LEFT_ARROW);
          counterLeftRight = 0;
          pressed_flag = true;
        }
      }
    }
    posLeftRightPrev = posLeftRight;
    //Serial.println(counterLeftRight);
}
