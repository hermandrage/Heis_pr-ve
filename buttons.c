#include "buttons.h"
#include "statemachine.h"//
#include "elev.h"
#include "io.h"
#include "quecontroller.h"
#include "lights.h"
#include <stdlib.h>
#include <stdio.h>

//checks if the the comand buttons are pressed, and adds the order to que if it doesn't already exsist
void read_command_button(void) {
  for (int etg = 0 ; etg<4 ; ++etg){
    if (elev_get_button_signal(BUTTON_COMMAND,etg)){
      add_order_to_que(etg, ORDER_WITHOUT_DIRECTION);
    }
  }
}

//checks if the the UP and Down buttons are pressed, and adds the order to que if it doesn't already exsist
void read_up_and_down_buttons(void) {
  for (int etg = 0 ; etg<N_FLOORS ; ++etg){
    if (etg<N_FLOORS-1){
      if (elev_get_button_signal(BUTTON_CALL_UP,etg)){
        add_order_to_que(etg, ORDER_UP);
      }
    }
    if(etg>0){
      if (elev_get_button_signal(BUTTON_CALL_DOWN,etg)){
        add_order_to_que(etg, ORDER_DOWN);
      }
    }
  }
}

//Checks if stop button is pressed, if pressed: Empty que and stops the elevator
void read_stop_button(void){
  if (elev_get_stop_signal()){
    elev_set_stop_lamp(1);
    empty_que();
    elev_set_motor_direction(DIRN_STOP);
    set_current_direction(DIRN_STOP);
    update_all_lights();
    while (elev_get_stop_signal()){
    }

    if (elev_get_floor_sensor_signal()!=-1){
      set_current_state(DOOR_OPEN);

    }
    else{
      set_current_state(IDLE);
    }
    elev_set_stop_lamp(0);
  }
}

void read_all_buttons(void){
  read_up_and_down_buttons();
  read_command_button();
  read_stop_button();

}
