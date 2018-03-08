#include"lights.h"
#include"elev.h"
#include"io.h"
#include"statemachine.h"
#include"quecontroller.h"
#include<stdlib.h>
#include <stdio.h>
#include <time.h>


void update_all_lights(void){
  set_command_button_lights();
  actual_set_floor_lights();
  set_up_button_lights();
  set_down_button_lights();
}

void actual_set_floor_lights(void){  //turns on the light in the floor indicators, depending on which floor the elevator is in
  if (get_current_floor() >=0 && get_current_floor() <N_FLOORS && elev_get_floor_indicator_matrix(get_current_floor())==0){
    elev_set_floor_indicator(get_current_floor());
    set_floor_indicator_lights_matrix(get_current_floor());
  }
}


void set_command_button_lights(void){ //Sets lights in command buttons when pressed, turns them off when order's executed.
  for (int etg = 0 ; etg<4 ; ++etg){
    if (elev_get_button_signal(BUTTON_COMMAND,etg)){
      elev_set_button_lamp(BUTTON_COMMAND,etg,1);
    }
  }
  for (int btn=0; btn<N_FLOORS; btn++){
    if (check_for_order(btn, ORDER_WITHOUT_DIRECTION) == -1){
      elev_set_button_lamp(BUTTON_COMMAND,btn,0);
    }
  }
}


void set_up_button_lights(void){ //Sets lights in up buttons when pressed, turns them off when order's executed.
  for (int etg = 0 ; etg<(N_FLOORS-1) ; ++etg){
    if (elev_get_button_signal(BUTTON_CALL_UP,etg)){
      elev_set_button_lamp(BUTTON_CALL_UP,etg,1);
    }
  }
  for (int btn=0; btn<N_FLOORS-1; btn++){
    if (check_for_order(btn, ORDER_UP) == -1){
      elev_set_button_lamp(BUTTON_CALL_UP,btn,0);
    }
  }
}

void set_down_button_lights(void){ //Sets lights in down buttons when pressed, turns them off when order's executed. 
  for (int etg = 1 ; etg<(N_FLOORS) ; ++etg){
    if (elev_get_button_signal(BUTTON_CALL_DOWN,etg)){
      elev_set_button_lamp(BUTTON_CALL_DOWN,etg,1);
    }
  }
  for (int btn=1; btn<N_FLOORS; btn++){
    if (check_for_order(btn, ORDER_DOWN) == -1){
      elev_set_button_lamp(BUTTON_CALL_DOWN,btn,0);
    }
  }
}
