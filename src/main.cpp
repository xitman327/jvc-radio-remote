#include <Arduino.h>
#include <JVC-Stereo.h>
#include <EMA.h>  

EMA<2> buttons_filter;

unsigned int buttons_filtered, buttons_filtered_old, what_button;

#define buttons A0
#define radio_remote 5


/*
idle  778 - 781 780

plus  139 - 141   140
minus 84 - 86     85
circ  208 - 212   210
up    578 - 585   580
left  420 - 425   422
right 299 - 303   300


*/
#define BT_IDLE 780
#define BT_PLUS 140
#define BT_MINUS 85
#define BT_CIRC 210
#define BT_UP 580
#define BT_LEFT 422
#define BT_RIGHT 300

#define BT_ERR 10

JVCStereo JVC(radio_remote);

bool is_pressed(int val, int val_target, int error){
  return (val > (val_target-error) && val < (val_target+error));
}

void setup() {
  Serial.begin(115200);
  JVC.setup();
  pinMode(buttons, INPUT_PULLUP);

}
uint32_t analog_loop, serial_loop, buttons_loop;
#define analog_refresh 1
#define serial_refresh 100
#define buttons_refresh 100
void loop() {

  // if(millis() - analog_loop > analog_refresh){
  //   analog_loop = millis();
    buttons_filtered = buttons_filter(analogRead(buttons));
  // }

  // if(millis() - serial_loop > serial_refresh){
  //   serial_loop = millis();
  //   Serial.println(buttons_filtered);
  // }

  if(millis() - analog_loop > analog_refresh){
    analog_loop = millis();

    if(is_pressed(buttons_filtered, buttons_filtered_old, 1)){
      if(is_pressed(buttons_filtered, BT_PLUS, BT_ERR)){
        JVC.send(JVC_VOLUP);
      }else if(is_pressed(buttons_filtered, BT_MINUS, BT_ERR)){
        JVC.send(JVC_VOLDN);
      }else if(is_pressed(buttons_filtered, BT_CIRC, BT_ERR)){
        JVC.send(JVC_MUTE);
      }else if(is_pressed(buttons_filtered, BT_LEFT, BT_ERR)){
        JVC.send(JVC_SKIPBACK);
      }else if(is_pressed(buttons_filtered, BT_RIGHT, BT_ERR)){
        JVC.send(JVC_SKIPFWD);
      }else if(is_pressed(buttons_filtered, BT_UP, BT_ERR)){
        JVC.send(JVC_SOURCE);
      }
    }else{
      buttons_filtered_old = buttons_filtered;
    }

  }
}


