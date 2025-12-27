#include <stdio.h>
#include <string.h>
//Categories: Numbers, open brackets, closed brackets, operators.
int is_operator(char c){return (c == '*' || c == '+' || c== '-' || c == '/');}
int is_number(char c){return (c >= '0' && c<='9');}
char type(char c) {
    if (c == '*' || c == '+' ||c == '-' || c == '/')
        return '*';
    if (c == '(' || c == ')')
        return c;
    if (is_number(c))
        return 'n';

    return ' ';
}
char next(FILE *source,int prev_is_num) {
    char c = fgetc(source);
    while (c== ' ' ) // if c is a blank space, proceed.
    {
        c = fgetc(source);
        prev_is_num = 0;
    }

    if (is_number(c)){
        if (prev_is_num) {
            prev_is_num = 0;
            while (is_number(c))
                c = fgetc(source);
            while (c == ' ')
                c = fgetc(source);
            return type(c);
        }
        return 'n';
    }
    else if (c == '\n' || c == EOF)
        return c;
    return type(c);
}

int is_valid(char a, char b) {
    if (b == '(') {
        if (a == '(' || a == '*' || a == '\n' || a == '\0')
            return 1;
        return 0;
    }
    if (b == 'n') {
        if (a == '*' || a == '(' || a == '\n' || a == '\0')
            return 1;
        return 0;
    }
    if (b == ')') {
        if (a == ')' || a == 'n')
            return 1;
        return 0;
    }
    if (b == '*') {
        if (a == ')' || a == 'n')
            return 1;
        return 0;
    }
    if (b == '\n' || b == EOF)
        if (a == '*' || a == '(')
            return 0;
    return 1;
}
int main() {
    FILE *source, *dest;

    source = fopen("../Input.txt","r");
    if (source == NULL) {
        printf("File cannot be opened.");
        return -1;
    }

    dest = fopen("../Input-v.txt","w");
    if (dest == NULL) {
        printf("File cannot be opened.");
        return -1;
    }
    char res, prev;
    int expr, brackets, found_error;
    expr = 1;
    brackets = 0;
    found_error = 0;
    prev = '\0';
    res = '\0';
    do {
        if (prev == 'n') {
            res = next(source,1);
        }
        else if (prev == EOF)
            break;
        else
            res = next(source, 0);

        if (res == '(')
            brackets = brackets + 1;
        else if (res == ')')
            brackets = brackets -1;
        if (!found_error && (!is_valid(prev,res) || brackets < 0)) {
            fprintf(dest,"Expression %d contains error.\n", expr);
            found_error = 1;
        }

        if (res == '\n') {
            if (brackets != 0 && !found_error)
                fprintf(dest, "Expression %d contains error.\n", expr);

            expr = expr + 1;
            found_error = 0;
            brackets = 0;
        }
            prev = res;
    } while (1);
}
