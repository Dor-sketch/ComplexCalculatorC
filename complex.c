#include "complex.h"
#include <math.h>
#include <stdio.h>

#define OK 1
#define FALSE 0
#define TRUE 1

/* Function declarations */
int is_over(void);
void prn_comp(complex *comp);
int get_num(int needs_comma, double *param);
int is_comma(char c);
complex *get_variable(void);
char get_nonspace_char(void);
int is_running;

/* Function to update the complex variable members */
void read_comp(void)
{
    complex *comp = get_variable();
    double real, img;

    if (!comp || !is_comma(get_nonspace_char()))
    {
        return;
    }

    if (get_num(TRUE, &real) != OK || get_num(FALSE, &img) != OK)
    {
        return;
    }

    comp->real = real;
    comp->img = img;
}

/* Function to add two complex variables and print the result */
void add_comp(void)
{
    complex *comp1 = get_variable();
    if (!comp1 || !is_comma(get_nonspace_char()))
    {
        return;
    }

    complex *comp2 = get_variable();
    if (comp2 && is_over() == OK)
    {
        complex result = {comp1->real + comp2->real, comp1->img + comp2->img};
        prn_comp(&result);
    }
}

/* Function to subtract two complex variables and print the result */
void sub_comp(void)
{
    complex *comp1 = get_variable();
    if (!comp1 || !is_comma(get_nonspace_char()))
    {
        return;
    }

    complex *comp2 = get_variable();
    if (comp2 && is_over() == OK)
    {
        complex result = {comp1->real - comp2->real, comp1->img - comp2->img};
        prn_comp(&result);
    }
}

/* Function to print a complex variable */
void print_comp(void)
{
    complex *comp = get_variable();
    if (comp && is_over() == OK)
    {
        prn_comp(comp);
    }
}

/* Function to calculate and print the absolute value of a complex variable */
void abs_comp(void)
{
    complex *comp = get_variable();
    if (comp && is_over() == OK)
    {
        double abs_value = sqrt(comp->real * comp->real + comp->img * comp->img);
        printf("Absolute value: %.2f\n", abs_value);
    }
}

/* Function to multiply a complex variable with a real number and print the result */
void mult_comp_real(void)
{
    complex *comp = get_variable();
    if (!comp || !is_comma(get_nonspace_char()))
    {
        return;
    }

    double real_num;
    if (get_num(FALSE, &real_num) == OK)
    {
        complex result = {comp->real * real_num, comp->img * real_num};
        prn_comp(&result);
    }
}

/* Function to multiply a complex variable with an imaginary number and print the result */
void mult_comp_img(void)
{
    complex *comp = get_variable();
    if (!comp || !is_comma(get_nonspace_char()))
    {
        return;
    }

    double img_num;
    if (get_num(FALSE, &img_num) == OK)
    {
        complex result = {-comp->img * img_num, comp->real * img_num};
        prn_comp(&result);
    }
}

/* Function to multiply two complex variables and print the result */
void mult_comp_comp(void)
{
    complex *comp1 = get_variable();
    if (!comp1 || !is_comma(get_nonspace_char()))
    {
        return;
    }

    complex *comp2 = get_variable();
    if (comp2 && is_over() == OK)
    {
        complex result = {
            comp1->real * comp2->real - comp1->img * comp2->img,
            comp1->real * comp2->img + comp1->img * comp2->real};
        prn_comp(&result);
    }
}

/* Function to stop the program */
void stop(void)
{
    printf("Program stopped\n");
    is_running = FALSE;
}
