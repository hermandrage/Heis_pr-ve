typedef enum order_direction {
    NO_ORDER = 0,
    ORDER_UP = 1,
    ORDER_DOWN = 2,
    ORDER_WITHOUT_DIRECTION = 3
}order_dir;




void empty_que(void);
void print_que(void);
int read_next_order(void);
int check_for_order(int floor, order_dir dir);
int check_if_should_stop(int floor, order_dir direcion);
void add_order_to_que(int floor,order_dir direction);
void delete_order_from_que(int order_nr);
