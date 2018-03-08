#include "statemachine.h"
#include "elev.h"
#include "io.h"
#include "quecontroller.h"

#include <stdio.h>
#include <stdlib.h>


const int Number_of_floors=4;

const int nr_possible_orders=Number_of_floors+(2*(Number_of_floors-1));
//Køen vår:)
static int que[nr_possible_orders][2];

void empty_que(void){ // Sets all positions in que to: -1, NO_ORDER. Aka, Empty/no orders.Aka, delets all orders:)
  for (int ord_nr=0; ord_nr<nr_possible_orders; ord_nr++){
    que[ord_nr][0]=-1;
    que[ord_nr][1]=NO_ORDER;
  }
}
//reads from que what floor next order is in
int read_next_order(void){
  return que[0][0];

}

//both functions below returns the POTSITION of the order in que if the order is found. this is because is can be used in the delete-function when a order is finished.

//Checks if order is present (based on floor order and direction) returns the order position (in que) if present, -1 if not. DOES NOT return order_nr on ORDER_WITHOUT_DIRECTION
int check_for_order(int floor, order_dir dir){
  for (int order_nr = 0 ; order_nr < nr_possible_orders ; ++order_nr ){
    if (que[order_nr][0] == floor && que[order_nr][1] == dir){// || que[order_nr][1] == ORDER_WITHOUT_DIRECTION)
    return order_nr;
    }
  }
  return -1;
}
//checks if a order is in the direction the elev is mooving. returns the order_number if present, -1 if not. includes ORDER_WITHOUT_DIRECTION and direction sent in input
int check_if_should_stop(int floor, order_dir direction){
  if (read_next_order()==floor){
    return 0;
  }
  if (check_for_order(floor, direction)!=-1){
    return check_for_order(floor, direction);
  }
  if (check_for_order(floor, ORDER_WITHOUT_DIRECTION)!=-1){
    return check_for_order(floor, ORDER_WITHOUT_DIRECTION);
  }
  return -1;
}

//Checks if the new order already exists, if it doesn't: adds the new order last in the que. prints error-message if invalid input.
void add_order_to_que(int floor,order_dir direction){
  if (floor<0){
    printf("Error:floor<0");
    return;
  }
  if (direction==NO_ORDER) {
    printf ("ERROR:NO ORDER");
    return;
  }
  if (floor>=Number_of_floors){
    printf("ERROR:floor>=Number_of_floors" );
    return;
  }
  if (check_for_order(floor,direction)!=-1){
    //printf("Order allready exists");
    return;
  }
  for (int order_nr = 0 ; order_nr < nr_possible_orders ; ++ order_nr){
    if (que[order_nr][0] == -1 && que[order_nr][1]== NO_ORDER){
      que[order_nr][0] = floor;
      que[order_nr][1]=direction;
      printf("Order added to que\n");
      return;
    }
  }
}
//Deletes order from que, "leftshifts" the que and sets the last position in que to "orderless". lacks exception for unvalid order_number.
void delete_order_from_que(int order_nr){
  for(int temp_order_nr=order_nr; temp_order_nr<(nr_possible_orders-1); ++temp_order_nr){
    que[temp_order_nr][0]=que[temp_order_nr+1][0];
    que[temp_order_nr][1]=que[temp_order_nr+1][1];
  }
  que[-1][0]=-1;
  que[-1][1]=NO_ORDER;
}


void print_que(void){ // printer kø for testing
  for (int ord_nr=0; ord_nr<nr_possible_orders; ord_nr++){
    printf("ordernr: %d", ord_nr);
    printf("   floor %d", que[ord_nr][0]);
    printf("   direction: %d", que[ord_nr][1]);
    printf("\n");
  }
}
