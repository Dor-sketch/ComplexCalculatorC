/* This file contains the code for the calculator program */
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Expression {
    struct Expression *left;  // Pointer to left operand (sub-expression)
    struct Expression *right; // Pointer to right operand (sub-expression)
    uint8_t op;               // Operation
    int value;                // Value (used if it's a leaf node with a number)
} Expression;

inline void skipSpaces(char **s) {
    if (!s || !*s) {
        return;
    }
    while (**s) {
        // TODO: Implement order of operations. multiplication currently runs as
        // + and -. For now put parentheses around the multiplication.
        if (**s == '(' || **s == ')' || **s == '+' || **s == '-' ||
            **s == '*' || isdigit(**s)) {
            return;
        }
        (*s)++;
    }
}

inline int getDigit(char c) { return c - '0'; }

inline int getMinusSign(char c) { return c == '-' ? -1 : 1; }

inline int getIntFromString(char **s) {
    int sign = 1;
    int result = 0;

    // Handle sign
    if (**s == '-') {
        sign = -1;
        (*s)++;
    }

    skipSpaces(s);

    // Parse digits
    while (**s && isdigit(**s)) {
        result = result * 10 + getDigit(**s);
        (*s)++;
    }
    return result * sign;
}

inline char getOp(char **s) {
    char op = 0;
    while (**s) {
        if ('+' == (**s) || (**s) == '-' || (**s) == '*') {
            op = **s;
            break;
        }
        (*s)++;
    }
    (*s)++;
    return op;
}

inline bool checkUnary(char **s) {
    char *t = *s; // Copy the pointer to avoid changing the original
    if (**s != '-')
        return false;
    t++;
    skipSpaces(&t); // Pass the address of the copy

    // Check for unary operator and that it's not followed by a digit
    if (!isdigit(*t)) {
        return true;
    }
    return false;
}

void freeExpression(Expression *exp) {
    if (!exp) {
        return;
    }

    if (exp->left) {
        freeExpression(exp->left);
    }
    if (exp->right) {
        freeExpression(exp->right);
    }
    free(exp);
    exp = NULL;
}

Expression *parseExpression(char **s);

// Parses a simple expression (number or sub-expression in parentheses)
inline Expression *parseSimpleExpression(char **s) {
    if (!s || !*s) {
        return NULL;
    }

    skipSpaces(s);

    if (checkUnary(s)) {
        (*s)++; // Skip the unary operator

        Expression *exp = (Expression *)malloc(sizeof(Expression));
        if (!exp) {
            return NULL; // Handle memory allocation failure
        }

        exp->op = '-';
        exp->left = NULL;
        exp->right = parseSimpleExpression(s);
        if (exp->right == NULL) {
            free(exp);
            return NULL;
        }
        return exp;
    } else if (*s && **s == '(') {
        (*s)++; // Skip the '('
        Expression *exp = parseExpression(s);
        if (exp == NULL) {
            return NULL;
        }
        skipSpaces(s);
        if (!s || !*s || **s != ')') {
            freeExpression(exp);
            return NULL;
        }
        (*s)++; // Skip the ')'
        return exp;
    } else {
        Expression *leaf = (Expression *)malloc(sizeof(Expression));
        if (!leaf) {
            return NULL; // Handle memory allocation failure
        }

        leaf->op = 0;
        leaf->left = leaf->right = NULL;
        leaf->value = getIntFromString(s);
        return leaf;
    }
}

/* parseExpression: build the expression tree recursively */
inline Expression *parseExpression(char **s) {
    skipSpaces(s);
    uint8_t op = 0;

    // Start with the leftmost expression (number or sub-expression)
    Expression *currentExp = parseSimpleExpression(s);

    while (*s && **s != '\0' && **s != ')') {
        skipSpaces(s);

        if (!**s || **s == ')' || **s == '\0') {
            // End of the string or the current sub-expression
            break;
        }

        // Get the next operator
        op = getOp(s);

        // Create a new expression for this operator
        Expression *newExp = (Expression *)malloc(sizeof(Expression));
        if (!newExp) {
            // Handle memory allocation failure
            freeExpression(currentExp);
            return NULL;
        }

        newExp->op = op;
        newExp->left =
            currentExp; // The current expression becomes the left child
        newExp->right = parseSimpleExpression(s); // Parse the right operand

        currentExp = newExp; // Update the current expression
    }

    return currentExp;
}

int evaluateExpression(Expression *exp) {
    if (exp == NULL) {
        return 0;
    }
    if (exp->op == '-' && exp->left == NULL) {
        // Handle unary negation
        return -evaluateExpression(exp->right);
    }

    if (exp->left == NULL && exp->right == NULL) {
        int val = exp->value;
        // freeExpression(exp);
        // exp = NULL;
        return val;
    }

    int leftVal = evaluateExpression(exp->left);
    int rightVal = evaluateExpression(exp->right);
    char op = exp->op;

    switch (op) {
    case '+':
        return leftVal + rightVal;
    case '-':
        return leftVal - rightVal;
    case '*':
        return leftVal * rightVal;
    }

    return 0; // Default return
}

void printExpressionTree(Expression *exp) {
    if (exp == NULL) {
        return;
    }

    // Check if it's a leaf node
    if (exp->left == NULL && exp->right == NULL) {
        printf("%d", exp->value);
        return;
    }

    // Print left subtree
    if (exp->left != NULL) {
        printf("("); // Add parenthesis for clarity
        printExpressionTree(exp->left);
    }

    // Print operator
    printf(" %c ", exp->op);

    // Print right subtree
    if (exp->right != NULL) {
        printExpressionTree(exp->right);
        printf(")"); // Add parenthesis for clarity
    }
}

void printExpressionTreeInternal(Expression *exp, int level, bool isLeft) {
    if (exp == NULL) {
        return;
    }

    // Increase the level for the child nodes
    int nextLevel = level + 1;

    // Print right subtree first with increased level
    printExpressionTreeInternal(exp->right, nextLevel, false);

    // Print the current node
    printf("\n");
    for (int i = 0; i < level * 4; i++) { // Indent based on the level
        printf(" ");
    }

    if (level > 0) {
        // Print branch
        printf(isLeft ? "└── " : "┌── ");
    } else {
        printf("root:");
    }

    if (exp->left == NULL && exp->right == NULL) {
        printf("%d", exp->value);
    } else {
        printf("%c", exp->op);
    }

    // Print left subtree with increased level
    printExpressionTreeInternal(exp->left, nextLevel, true);
}

void printVisualExpressionTree(Expression *exp) {
    printExpressionTreeInternal(exp, 0, false);
    printf("\n"); // New line after printing the whole tree
}

int calculate(char *s) {
    Expression *exp = parseExpression(&s); // Parse the entire expression
    if (exp == NULL) {
        return 0;
    }
    printVisualExpressionTree(exp); // Print the expression tree
    int ret = evaluateExpression(exp); // Evaluate the expression tree
    freeExpression(exp);              // Free the expression tree
    return ret;
}

int main(void) {
    char s[100];
    printf("Enter an expression: ");
    while (scanf("%s", s) != EOF) {
        if (s[0] == 'q') {
            break;
        }
        printf("%s = %d\n", s, calculate(s));
        printf("================================================\n");
        printf("Enter an expression: ");
    }
    return 0;
}