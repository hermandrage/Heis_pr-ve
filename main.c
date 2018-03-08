#include "elev.h"
#include <stdio.h>
#include "lights.h"
#include "statemachine.h"
#include "io.h"
#include "buttons.h"
#include "quecontroller.h"
#include "timer.h"




int main(){
  if (!elev_init()) {
      printf("Unable to initialize elevator hardware!\n");
      return 1;
  }
  int next_order = read_next_order();
  printf("Starting whileloop");
  run_states();
}

