#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    char c;
    struct Node *next;
} Node;

Node *top;

Node *makeNode(char x)
{
    Node *p = (Node *)malloc(sizeof(Node));
    p->c = x;
    p->next = NULL;
    return p;
}

void initStack()
{
    top = NULL;
}

int stackEmpty()
{
    return top == NULL;
}

void push(char x)
{
    Node *p = makeNode(x);
    p->next = top;
    top = p;
}

char pop()
{
    if (stackEmpty())
        return ' ';
    char x = top->c;
    Node *tmp = top;
    top = top->next;
    free(tmp);
    return x;
}

int match(char a, char b)
{
    if (a == '(' && b == ')')
        return 1;
    if (a == '[' && b == ']')
        return 1;
    if (a == '{' && b == '}')
        return 1;
    if (a == '"' && b == '"')
        return 1;
    return 0;
}

int isOpeningBrace(char c)
{
    if (c == '(' || c == '{' || c == '[' || c == '"')
        return 1;
    else
        return 0;
}

int isClosingBrace(char c)
{
    if (c == ')' || c == '}' || c == ']' || c == '"')
        return 1;
    else
        return 0;
}

int count(int a, int b, int c)
{
    int d = 0;
    if (a) d++;
    if (b) d++;
    if (c) d++;
    return d;
}

int checkJson(char *fileName)
{
    FILE *f = fopen(fileName, "r+");
    char c;
    int a = 0, b = 0, d = 0, e = 0;
    initStack();
    while (1)
    {
        c = fgetc(f);
        if (c == EOF) break;
        if (isOpeningBrace(c))
            push(c);
        if (isClosingBrace(c))
        {
            char x = pop();
            if (!match(x, c))
            {
                return 0;
            }
        }
    }
    initStack();
    while (1)
    {
        c = fgetc(f);
        if (c == EOF) break;
        if (c == '"' && a == 0 && b == 0 && d == 0)
        {
            push(c);
            a = 1;
        }
        if (c == '"' && a == 1 && b == 0 && d == 0)
        {
            char x = pop();
            if (!match(x, c)) 
            {
                return 0;
            }
            b = 1;
            a = 0;
        }
        if (c == ':' && a == 0 && b == 1 && d == 0)
        {
            b = 0;
            d = 1;
            continue;
        }
        if ((c == ',' || isClosingBrace(c)) && a == 0 && b == 0 && d == 1)
        {
            d = 0;
            if (a || b || c || d)
                return 0;
            continue;
        }
        if (c == '"' && a == 0 && b == 0 && d == 1 && e == 0)
        {
            push(c);
            e = 1;
        }
        if (c == '"' && a == 0 && b == 0 && d == 1 && e == 1)
        {
            char x = pop();
            if (!match(x, c)) 
            {
                return 0;
            }
            e = 0;
        }
        if (isOpeningBrace(c) && a == 0 && b == 0 && d == 1)
        {
            d = 0;
            continue;
        }
        if (count(a, b, d) > 1)
            return 0;
    }
    if (!a && !b && !e && !d)
        return 1;
    fclose(f);
}

int main()
{
    printf("%d\n", checkJson("input.json"));
    return 0;
}