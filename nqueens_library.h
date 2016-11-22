#define TRUE 1
#define FALSE 0

void get_array_string(int *list, int n, char *s);
int generate_n_queens_serial_one(int n, int print_flag, int first);
int generate_n_queens_serial(int n, int print_flag);
int check_diagonal(int *list, int n);
void *queens_thread(void *infopointer);


typedef struct ti {
    int n; 
    int first;
    int out_flag;
    int result;
} thread_info_t;
