# ifndef COMPLEX_H

#include <stddef.h>

#define MAX 15
#define FALSE 0
#define OK 1
#define FOREVER for (;;)
#define MAX_COMMANDS 9 /* Define this constant for the number of commands */
#define ASCII_A 65

/* Error codes */
#define ERROR_INVALID_NUMBER -1
#define ERROR_MISSING_COMMA -2
#define ERROR_EXTRANEOUS_TEXT -3
#define NO_PARAM -4
#define BUFFER_OVERFLOW -5

typedef struct complex {
    double real;
    double img;
} complex;




void print_comp(void);

void abs_comp(void);

void add_comp(void);

void sub_comp(void);

void mult_comp_comp(void);

void mult_comp_real(void);

void mult_comp_img(void);

void read_comp(void);

void stop(void);

void print_error_message(int error_code);

/* Structure to save a name for every command */
typedef struct {
    char *name;
    void (*func)(void);
} Cmd;

extern Cmd cmd[];

# endif