#include "complex.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


complex a, b, c, d, e, f; /* the 6 complex variables */

 int is_running = 1;

/* Structure to keep the name for each variable address */
struct {
    char name;
    complex* var;
} vars[] = {{'A', &a}, {'B', &b}, {'C', &c},  {'D', &d},
            {'E', &e}, {'F', &f}, {'#', NULL}};

/* Structure to save a name for every command */
Cmd cmd[] = {{"print_comp", print_comp},
             {"abs_comp", abs_comp},
             {"mult_comp_real", mult_comp_real},
             {"mult_comp_img", mult_comp_img},
             {"read_comp", read_comp},
             {"add_comp", add_comp},
             {"sub_comp", sub_comp},
             {"mult_comp_comp", mult_comp_comp},
             {"stop", stop},
             {"not_valid", NULL}};

/* Function to get a char from the user and echo it */
char get_and_echo_char(void)
{
    char c = getchar();
    if (c != EOF)
        putchar(c);
    return c;
}

/* Function to skip backtab and spaces with getchar, echoing the input */
char get_nonspace_char(void)
{
    char c;
    do {
        c = get_and_echo_char();
    } while (c == ' ' || c == '\t');
    return c;
}

/* Function to flash the input buffer */
void flush_input_buffer(void)
{
    char c = get_and_echo_char();
    while (c != '\n' && c != EOF)
        c = get_and_echo_char();
}

/* Function that checks if an input character is a comma while avoiding spaces
 */
int is_comma(char c)
{
    if (c != ',') {
        flush_input_buffer();
        fprintf(stderr, "Missing comma\n");
        return FALSE;
    }
    return OK;
}

/* Function to print relevant error messages based on error codes */
void print_error_message(int error_code)
{
    switch (error_code) {
    case ERROR_INVALID_NUMBER:
        printf("Error: Invalid parameter - not a number.\n");
        break;
    case ERROR_MISSING_COMMA:
        printf("Error: Missing comma after number.\n");
        break;
    case ERROR_EXTRANEOUS_TEXT:
        printf("Error: Extraneous text after end of command.\n");
        break;
    case NO_PARAM:
        printf("Error: Missing parameters in command diffrent from stop.\n");
		break;
	case BUFFER_OVERFLOW:
		printf("Error: Buffer overflow detected - command too long.\n");
    default:
        printf("Unknown error occurred.\n");
    }
}

int is_over_util(void)
{
    char c = get_and_echo_char();
    while (c == ' ' || c == '\t')
        c = get_and_echo_char();
    if (c == '\n' || c == EOF)
        return OK;
    /* complete echo of the input before printing the error message */
    flush_input_buffer();
    return ERROR_EXTRANEOUS_TEXT;
}

/* Function to check if the user command ends correctly */
int is_over(void)
{
    int result = is_over_util();
    if (result != OK) {
        print_error_message(result);
        result = FALSE;
    }
    return result;
}

/* Utility function for get_num to handle the processing */
int get_num_util(int needs_comma, double* param)
{
    double num = 0.0, power = 1.0;
    int sign = 1;
    char c = get_nonspace_char();

    if (c == '-') {
        sign = -1;
        c = get_and_echo_char();
    }

    if (!isdigit(c)) {
        /* complete echo of the input before printing the error message */
        flush_input_buffer();
        return ERROR_INVALID_NUMBER;
    }

    while (isdigit(c)) {
        num = num * 10.0 + (c - '0');
        c = get_and_echo_char();
    }

    if (c == '.') {
        c = get_and_echo_char();
        while (isdigit(c)) {
            num = num * 10.0 + (c - '0');
            power *= 10.0;
            c = get_and_echo_char();
        }
    }

    *param = sign * num / power;

    if (needs_comma && c != ',') {
        /* complete echo of the input before printing the error message */
        flush_input_buffer();
        return ERROR_MISSING_COMMA;
    }

    if (!needs_comma && c != '\n') {
        /* complete echo of the input before printing the error message */
        flush_input_buffer();
        return ERROR_EXTRANEOUS_TEXT;
    }

    return OK;
}

/* Function to convert stream of character digits to a double */
int get_num(int needs_comma, double* param)
{
    int result = get_num_util(needs_comma, param);
    if (result != OK) {
        print_error_message(result);
        result = FALSE;
    }
    return result;
}

/* Function to print the complex variable passed in the args */
void prn_comp(complex* comp)
{
    printf("%.2f + (%.2f)i\n", comp->real, comp->img);
}

/* Function to comvert the user input into the compex pointer.
    it compare the input name with the saved name in the variables arrey */
complex* get_variable(void)
{
    char c = get_nonspace_char();

    if ('A' <= c && c <= 'F')
        return (
            (vars[c - ASCII_A].var)); /* it substract tha ASCII value to get the
                                    variable location
                                    ('A' = 65 in ASCII and placed 1st in the
                                    variables array 'B' = 66 in ASCII and places
                                    2nd in the variables array and so on...) */
    if (c == '\n')
        printf("Missing parameter\n");
    else { /*true if the user typed an Undifined complex variable */
        while (c != '\n') {
            c = get_and_echo_char();
        }
        printf("Undefined complex variable\n");
    }

    return (vars[6].var); /* NULL */
}

/* Function to print user instructions */
void user_instructions(void)
{
    printf("Complex Number Operations Program\n");
    printf("------------------------------------------------\n");
    printf("This program allows manipulation of complex numbers using the "
           "following commands:\n\n");

    printf("1. read_comp <variable>, <real_part>, <imaginary_part>\n");
    printf("   - Sets the real and imaginary parts of the specified complex "
           "variable.\n");
    printf("   - Usage: read_comp A, 3.5, 2.1 // sets A to 3.5 + (2.1)i\n\n");

    printf("2. print_comp <variable>\n");
    printf("   - Prints the specified complex variable.\n");
    printf("   - Usage: print_comp A // for the example above, prints 3.5 + "
           "(2.1)i\n\n");

    printf("3. add_comp <variable1>, <variable2>\n");
    printf("   - Adds two complex variables and prints the result.\n");
    printf("   - Usage: add_comp A + B\n\n");

    printf("4. sub_comp <variable1>, <variable2>\n");
    printf("   - Subtracts the second complex variable from the first and "
           "prints "
           "the result.\n");
    printf("   - Usage: sub_comp A, B\n\n");

    printf("5. mult_comp_real <variable>, <real_number>\n");
    printf("   - Multiplies the specified complex variable with a real number "
           "and prints the result.\n");
    printf("   - Usage: mult_comp_real A, 3\n\n");

    printf("6. mult_comp_img <variable>, <imaginary_number>\n");
    printf("   - Multiplies the specified complex variable with an imaginary "
           "number and prints the result.\n");
    printf("   - Note: an imaginary number is a real number multiplied by "
           "- no "
           "need to enter the 'i' suffix.\n");
    printf("   - Usage: mult_comp_img A, 2 // A*2i = (a+bi)*(0+2i) = "
           "-2a+bi\n\n");

    printf("7. mult_comp_comp <variable1>, <variable2>\n");
    printf("   - Multiplies two complex variables and prints the result.\n");
    printf("   - Usage: mult_comp_comp A, B // (a+bi)*(c+di) = "
           "(ac-bd)+(ad+bc)i\n\n");

    printf("8. abs_comp <variable>\n");
    printf("   - Calculates and prints the absolute value of the specified "
           "complex variable.\n");
    printf("   - Usage: abs_comp A // print |A|\n\n");

    printf("To stop the program, type 'stop'.\n");
    printf("------------------------------------------------\n");
}



/* Function to get the user commands and call it */
void run_commands(void)
{
    char command[MAX];
    int i = 0;
    char c;

    FOREVER
    {
        for (i = 0; i < MAX; i++) {
            c = get_and_echo_char();
            if (isspace(c))
                break;
            command[i] = c;
        }

		if (i == MAX){
			flush_input_buffer();
			print_error_message(BUFFER_OVERFLOW);
			return;
		}

        command[i] = '\0';

        if (c == EOF || c == '\n') {
            if (strcmp(command, "stop") != 0){
                flush_input_buffer();
                print_error_message(NO_PARAM);
            	return;
			}
        }

        for (i = 0; i < MAX_COMMANDS; i++) { /*compare the string to the saved
                                     functions name in the vars array */
            if (strcmp(command, cmd[i].name) == 0)
                break;
        }

        if (cmd[i].func ==
            NULL) { /* true if the user typed an undefined command */
            flush_input_buffer();
            printf("Undefined command name: %s\n", command);
        } else
            (*(cmd[i].func))();

        memset(command, 0, MAX);
        i = 0;
        if (is_running == FALSE)
            break;
    }
    return;
}

void run_commands_from_gui(const char *input_command) {
    char command[MAX];
    int i;
    strncpy(command, input_command, MAX - 1);
    command[MAX - 1] = '\0';

    for (i = 0; i < MAX_COMMANDS; i++) {
        if (strcmp(command, cmd[i].name) == 0)
            break;
    }

    if (cmd[i].func == NULL) {
        printf("Undefined command name: %s\n", command);
    } else {
        (*(cmd[i].func))(); /* Execute the command */
    }
}
