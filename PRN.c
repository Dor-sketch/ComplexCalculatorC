#include <stdio.h>
#include <stdlib.h> // for atoi and malloc

typedef struct Stack { // type Stack
    int *arr;
    int sp;
    int max_size;
    int (*pop)(struct Stack *);
    void (*push)(struct Stack *s, int);
} Stack; // tag

int pop(Stack *s) {
    int ret = 0;
    if (s->sp > 0) {
        s->sp--;
        ret = s->arr[s->sp];
    }
    return ret;
}

void push(Stack *s, int i) {
    if (s->sp < s->max_size) {
        s->arr[s->sp] = i;
        s->sp++;
    }
}

Stack *initStack(int tokensSize) {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (!s) {
        perror("Error: malloc failure");
        exit(1);
    }
    s->arr = (int *)malloc((unsigned long)tokensSize * sizeof(int));
    if (!(s->arr)) {
        perror("Error: malloc failure");
        exit(1);
    }

    // init members
    s->sp = 0;
    s->push = push;
    s->pop = pop;
    s->max_size = tokensSize;
    return s;
}

int isOp(const char *str) {
    // assuming minimum tokes is one char + null terminator
    return (str[1] == '\0' &&
            (str[0] == '+' || str[0] == '-' || str[0] == '*' || str[0] == '/'));
}

int calcRPN(char **tokens, int tokensSize) {
    int ret, curToken, toPush;
    Stack *s = initStack(tokensSize);
    ret = curToken = toPush = 0;

    while (curToken < tokensSize) { // not <= because 0-indexed
        char *str = tokens[curToken++];

        if (isOp(str)) {
            int rhs = s->pop(s);
            int lhs = s->pop(s);

            switch (str[0]) {
            case ('+'):
                toPush = lhs + rhs;
                break;
            case ('-'):
                toPush = lhs - rhs;
                break;
            case ('*'):
                toPush = lhs * rhs;
                break;
            default:
                if (rhs == 0) {
                    printf("Error: division by zero\n");
                    exit(1);
                }
                toPush = lhs / rhs;
                break;
            }
        } else {
            toPush = atoi(str);
        }
        s->push(s, toPush);
    }
    ret = s->pop(s);
    free(s->arr);
    free(s);
    return ret;
}

void evalTokens(char **tokens, int tokensSize) {
    if (!tokens) {
        return;
    }
    for (int i = 0; i < tokensSize; i++) {
        printf("%s ", tokens[i]);
    }
    printf(" = %d\n", calcRPN(tokens, tokensSize));
}

int main(void) {
    char *tokens[] = {"2", "1", "+", "3", "*"};
    int tokensSize = (int)(sizeof(tokens) / sizeof(tokens[0]));
    evalTokens(tokens, tokensSize); // 9

    char *tokens2[] = {"10", "6", "9",  "3", "+", "-11", "*",
                       "/",  "*", "17", "+", "5", "+"};
    tokensSize = (int)(sizeof(tokens2) / sizeof(tokens2[0]));
    evalTokens(tokens2, tokensSize); // 22

    return 0;
}
