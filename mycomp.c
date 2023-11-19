#include "complex.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 15
#define FALSE 0
#define OK 1
#define FOREVER for (;;)
#define MAX_COMMANDS 9 // Define this constant for the number of commands

complex a, b, c, d, e, f; // the 6 complex variables
int is_running =
    OK; // a global variable that changes only when closing the program

// Structure to keep the name for each variable address
struct {
  char name;
  complex *var;
} vars[] = {{'A', &a}, {'B', &b}, {'C', &c},  {'D', &d},
            {'E', &e}, {'F', &f}, {'#', NULL}};

// Structure to save a name for every command
struct {
  char *name;
  void (*func)(void);
} cmd[] = {{"print_comp", print_comp},
           {"abs_comp", abs_comp},
           {"mult_comp_real", mult_comp_real},
           {"mult_comp_img", mult_comp_img},
           {"read_comp", read_comp},
           {"add_comp", add_comp},
           {"sub_comp", sub_comp},
           {"mult_comp_comp", mult_comp_comp},
           {"stop", stop},
           {"not_valid", NULL}};

// Function to get a char from the user and echo it
char get_and_echo_char(void) {
  char c = getchar();
  putchar(c);
  return c;
}

// Function to skip backtab and spaces with getchar, echoing the input
char get_nonspace_char(void) {
  char c;
  do {
    c = get_and_echo_char();
  } while (c == ' ' || c == '\t');
  return c;
}

// Function that checks if an input character is a comma while avoiding spaces
int is_comma(char c) {
  if (c != ',') {
    fprintf(stderr, "Missing comma\n");
    while (c != '\n' && c != EOF) {
      c = get_and_echo_char();
    }
    return FALSE;
  }
  return OK;
}

// Function to check if the user command ends correctly
int is_over(void) {
  char c = get_and_echo_char();
  if (c == '\n') {
    return OK;
  } else {
    fprintf(stderr, "Extraneous text after end of command\n");
    while (c != '\n') {
      c = get_and_echo_char();
    }
    return FALSE;
  }
}

// Function to convert stream of character digits to a double
int get_num(int needs_comma, double *param) {
  double num = 0.0, power = 1.0;
  int sign = 1;
  char c = get_nonspace_char();

  if (c == '-') {
    sign = -1;
    c = get_and_echo_char();
  }

  if (!isdigit(c)) {
    printf("Invalid parameter - not a number\n");
    while (c != '\n' && c != EOF)
      c = get_and_echo_char();
    return FALSE;
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

  *param = sign * num / power; // Calculate final value

  if (needs_comma && c != ',') {
    printf("Missing comma after number\n");
    while (c != '\n' && c != EOF)
      c = get_and_echo_char();
    return FALSE;
  }

  if (!needs_comma) {
    while (c == ' ' || c == '\t')
      c = get_and_echo_char();
    if (c != '\n') {
      printf("Extraneous text after end of command\n");
      while (c != '\n' && c != EOF)
        c = get_and_echo_char();
      return FALSE;
    }
  }

  return OK;
}

// Function to print the complex variable passed in the args
void prn_comp(complex *comp) {
  printf("%.2f + (%.2f)i\n", comp->real, comp->img);
}

// Function to convert the user input into the complex pointer
complex *get_variable(void) {
  char varName = get_nonspace_char();

  if (varName < 'A' || varName > 'F') {
    fprintf(stderr, "Undefined or missing complex variable\n");
    return NULL;
  }

  return vars[varName - 'A'].var;
}

/* a function to print user instructions */
void user_instructions(void) {
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
  printf("   - Subtracts the second complex variable from the first and prints "
         "the result.\n");
  printf("   - Usage: sub_comp A, B\n\n");

  printf("5. mult_comp_real <variable>, <real_number>\n");
  printf("   - Multiplies the specified complex variable with a real number "
         "and prints the result.\n");
  printf("   - Usage: mult_comp_real A, 3\n\n");

  printf("6. mult_comp_img <variable>, <imaginary_number>\n");
  printf("   - Multiplies the specified complex variable with an imaginary "
         "number and prints the result.\n");
  printf("   - Note: an imaginary number is a real number multiplied by - no "
         "need to enter the 'i' suffix.\n");
  printf("   - Usage: mult_comp_img A, 2 // A*2i = (a+bi)*(0+2i) = -2a+bi\n\n");

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

/* Function to find and execute a command */
int execute_command(const char *command, char c) {
  for (int i = 0; i < MAX_COMMANDS; i++) {
    if (strcmp(command, cmd[i].name) == 0) {
      if (strcmp(command, "stop") == 0 && (c == '\n' || c == EOF)) {
        is_running = FALSE;
        printf("Program stopped\n");
        return 0; /* Indicate that the program should stop */
      }

      if (cmd[i].func == NULL) {
        while (c != '\n') {
          c = get_and_echo_char();
        }
        printf("Undefined command name\n");
        return 1; /* Indicate an error or undefined command */
      } else {
        (*(cmd[i].func))(); /* Execute the found command */
        return 0;           /* Indicate successful execution */
      }
    }
  }
  return 1; /* Indicate command not found */
}

/* Function to get the user commands and call it */
void next_command(void) {
  char command[MAX];
  char c;

  while (is_running) {
    memset(command, 0, MAX);
    c = get_nonspace_char();
    int i = 0;

    while (!isspace(c) && i < MAX - 1) {
      command[i++] = c;
      c = get_and_echo_char();
    }
    command[i] = '\0';
    putchar(c);

    if (execute_command(command, c) == 0 && !is_running) {
      return;
    }
  }
}

int main(void) {
  user_instructions();
  while (is_running) {
    next_command();
  }
  return 0;
}