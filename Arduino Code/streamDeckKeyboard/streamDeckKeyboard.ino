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

#define MAX_MACRO_SIZE 16

typedef struct {
    uint8_t pin;
    uint8_t keys[MAX_MACRO_SIZE];
    bool combineKeypresses; // allow key combos
    uint8_t pinStateLast;
} pinKey_t;

#define NUM_KEYS 8

//TBD: Keyboard.print() would be better at sending long strings

//Keys are sent sequentially when combineKeypresses = false, or all at once when combineKeypresses = true
pinKey_t pinKey1[NUM_KEYS] = {
    /* button r1.c2 */ { 2, {'u', 'i', KEY_RETURN, 'g', '0', '.', '0', '5', KEY_RETURN, 'g', 'd', '0', '0', '5', KEY_RETURN, 0}, false, HIGH },
    /*  PAGE SELECT */ { 3, {'0', 0}, false, HIGH },
    /* button r2.c1 */ { 4, {'z', ' ', '1', ' ', '6', ' ', '2', '6', ' ', '2', '9', KEY_RETURN, 0}, false, HIGH },
    /* button r2.c2 */ { 5, {'z', ' ', '2', '1', ' ', '2', '6', KEY_RETURN, 0}, false, HIGH },
    /* button r2.c3 */ { 6, {'B', KEY_RETURN, 0}, false, HIGH },
    /* knob 1       */ { 7, {'a', 'b', 0}, false, HIGH },
    /* knob 2       */ { 8, {'a', 'b', 0}, false, HIGH },
    /* button r1.c1 */ { 9, {'u', 'm', 'm', KEY_RETURN, 'g', '1', KEY_RETURN, 'g', 'd', '1', KEY_RETURN, 0},      false, HIGH },
};

pinKey_t pinKey2[NUM_KEYS] = {
    /* button r1.c2 */ { 2, {KEY_LEFT_CTRL, 'r', 0}, true, HIGH }, //
    /*  PAGE SELECT */ { 3, {'a', 'b', 'c', 0}, false, HIGH }, //
    /* button r2.c1 */ { 4, {KEY_F2, 0}, false, HIGH }, //
    /* button r2.c2 */ { 5, {'a', 'b', 'c', 0}, false, HIGH }, //
    /* button r2.c3 */ { 6, {'a', 'b', 'c', 0}, false, HIGH }, //
    /* knob 1       */ { 7, {'a', 'b', 'c', 0}, false, HIGH }, //
    /* knob 2       */ { 8, {'a', 'b', 'c', 0}, false, HIGH }, //
    /* button r1.c1 */ { 9, {KEY_LEFT_CTRL, 'e', 0}, true, HIGH }, //
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
  //Serial.begin(115200);
  //while (!Serial);

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
  uint8_t *current_key;
  bool combine_keypresses;

  //grab pointer to the macro list
  //press the keys sequentially
  if (pageFlag == 1) //Depending on pageFlag, send different keystroke
  {
    current_key = &pinKey1[i].keys[0];
    combine_keypresses = pinKey1[i].combineKeypresses;
  }
  else
  {
    current_key = &pinKey2[i].keys[0];
    combine_keypresses = pinKey2[i].combineKeypresses;
  }

  for (int j; j < MAX_MACRO_SIZE; j++)
  {
    Serial.println(*current_key, HEX);
    Keyboard.press(*current_key);

    if(combine_keypresses == false)
    {
      Keyboard.releaseAll();
      Serial.println(" Key Released");
    }

    current_key++;
    if(*current_key == 0)
    {
      break;
    }
  }
  pressed_flag = true;
  //Serial.println(" Key Pressed");
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
