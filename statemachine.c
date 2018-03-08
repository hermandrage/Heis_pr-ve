#include "statemachine.h"
#include "elev.h"
#include "io.h"
#include "quecontroller.h"
#include "buttons.h"
#include "lights.h"
#include "timer.h"
//
#include <stdio.h>
#include <stdlib.h>

int current_floor=-1;


int get_current_floor(void) {
  return current_floor;
}


// setter variabelene cuttent_floor og previous_floor til riktig etasje.
void set_floor_variables(void){
  int temp_get_floor = elev_get_floor_sensor_signal();
  if (temp_get_floor > -1 && temp_get_floor < N_FLOORS){
    current_floor = temp_get_floor;
  }
}


void print_status(void){
    printf("------------------------------------------------------------ \n");
    printf("QUE:\n");
    print_que();
    printf("\n");
    printf("VARIABLES:\n");
    printf("Current state %d",current_state);
    printf("\nCURRENT FLOOR: %d\n", get_current_floor());
    printf("CURRENT DIRECTION    %d\n", get_current_direction() );
    printf("------------------------------------------------------------ \n");
}

void set_current_state(state_t state){
  current_state=state;
}
void print_current_state(void){
  printf("%d",current_state);
}


void run_states(void){
  int next_order = read_next_order();
  while (1){

  	next_order = read_next_order();
      switch (current_state){
        case IDLE: // HER STARTER STATEN
        if (current_direction!=DIRN_STOP){//Stopper heisen
          current_direction=DIRN_STOP;
          elev_set_motor_direction(DIRN_STOP);
        }
        next_order = read_next_order();
        if (next_order== -1){
        }
        else if (next_order - get_current_floor() <0){
          current_state=DRIVE_DOWN;
          print_status();
        }
        else if (next_order - get_current_floor() >0){
          current_state=DRIVE_UP;
          print_status();
        }
        else if (next_order - get_current_floor() ==0 && elev_get_floor_sensor_signal()!=-1){
          current_state=DOOR_OPEN;
          print_status();
        }
        set_dir_before_stopped(DIRN_STOP);
        read_all_buttons();
        update_all_lights();
        break;
        //////////------------------------------------------------------------------------------------

        case DOOR_OPEN:// HER STARTER STATEN

        if (current_direction!=DIRN_STOP){//Stopper heisen
          current_direction=DIRN_STOP;
          elev_set_motor_direction(DIRN_STOP);
        }
        set_floor_variables();
        if (timer_is_timeout() == -1){//Starter timer hvis den ikke allerede er startet.
          start_timer();
          elev_set_door_open_lamp(1);
        }
        next_order = read_next_order();
        if (timer_is_timeout()){ //reads next order and sets current_state
        	if(get_current_floor()==read_next_order()){
        		delete_order_from_que(0);
        	}
        	elev_set_door_open_lamp(0);

        	if (next_order== -1){
        		current_state=IDLE;
            print_status();
            }
            else if (next_order - get_current_floor() <0){
            	current_state=DRIVE_DOWN;
              print_status();
            }
            else if (next_order - get_current_floor() >0){
            	current_state=DRIVE_UP;
              print_status();
            }



        }
        set_dir_before_stopped(DIRN_STOP);
        read_all_buttons();
        update_all_lights();
        break;
        //////////------------------------------------------------------------------------------------

        case DRIVE_UP: // HER STARTER STATEN
        if (current_direction!=DIRN_UP){//Setter riktig retning
          current_direction=DIRN_UP;
          elev_set_motor_direction(DIRN_UP);
        }
        int temp_current_floor=elev_get_floor_sensor_signal();//mellomlagrer etsje/-1 dersom mellom etasjer.

        if ( temp_current_floor != -1){//Slår ut om vi er i en etasje.
          set_floor_variables();//oppdaterer curren og previous_floor
          int temp_order_number= check_if_should_stop(get_current_floor(), ORDER_UP);//mellomlagrer ordrenummeret til orderen vi eventuelt skal stoppe i, tar også hensyn til første order.

          while (temp_order_number != -1){// Bruker while i tillfelle det er flere bestillinger som blir utført og dermed skal slettes.
            current_state = DOOR_OPEN;
            delete_order_from_que(temp_order_number);
            temp_order_number= check_if_should_stop(get_current_floor(), ORDER_UP);//Sjekker på nytt om det er flere ordre som blir utført.
          }
          print_status();
        }
        set_dir_before_stopped(DIRN_UP);
        read_all_buttons();
        update_all_lights();
        break;
        //////////------------------------------------------------------------------------------------

        case DRIVE_DOWN:// HER STARTER STATEN
        if (current_direction!=DIRN_DOWN){
          current_direction=DIRN_DOWN;
          elev_set_motor_direction(DIRN_DOWN);
      }

        int temp_current_floor2=elev_get_floor_sensor_signal();//mellomlagrer etsje/-1 dersom mellom etasjer.

        if ( temp_current_floor2 != -1){//Slår ut om vi er i en etasje.
          set_floor_variables();//oppdaterer curren og previous_floor
          int temp_order_number= check_if_should_stop(get_current_floor(), ORDER_DOWN);//mellomlagrer ordrenummeret til orderen vi eventuelt skal stoppe i, tar også hensyn til første order.

          while (temp_order_number != -1){// Bruker while i tillfelle det er flere bestillinger som blir utført og dermed skal slettes.
            current_state = DOOR_OPEN;
            delete_order_from_que(temp_order_number);
            temp_order_number= check_if_should_stop(get_current_floor(), ORDER_DOWN);//Sjekker på nytt om det er flere ordre som blir utført.
          }
          print_status();
        }
        set_dir_before_stopped(DIRN_DOWN);
        read_all_buttons();
        update_all_lights();
        break;


        case STOPPED:
        elev_set_stop_lamp(0);

        if (elev_get_floor_sensor_signal()!=-1){
          set_current_state(DOOR_OPEN);

        }
        else if(read_next_order()!=-1){
            if( get_dir_before_stopped()==DIRN_UP){
              set_current_state(DRIVE_DOWN);
              print_status();
            }
            else if( get_dir_before_stopped()==DIRN_DOWN){
              set_current_state(DRIVE_UP);
              print_status();
            }
        }
        read_all_buttons();
        update_all_lights();


        break;
  }
}
}
