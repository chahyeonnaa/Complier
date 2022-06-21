#include<stdio.h>

char lookahead;

void match(char token);
char nexttoken();
void expr();
void term();
void expr_rest();
void term_rest();
void factor();

void main()
{
	printf("ют╥б :");

	lookahead = nexttoken();

	expr();
	if (lookahead == '$')
		printf("\n");
	else
		printf(" error\n");
}

void match(char token)
{
	if (lookahead == token)
		lookahead = nexttoken();
	else
	{
		printf("error\n");
		exit(1);
	}
}

char nexttoken()
{
	char c;
	while (1)
	{
		c = getchar();
		if (c == ' ' || c == '\t' || c == '\n' || c == '\0')
			continue;
		return c;
	}
}
void expr()
{
	term(); printf("3 "); expr_rest();
}
void term()
{
	factor();
	printf("6 ");
	term_rest();
}
void factor()
{
	if (lookahead == '(')
	{
		match('(');
		expr();
		match(')');
		printf("7 ");
	}
	else if (lookahead >= 48 && lookahead <= 57)
	{
		match(lookahead);
	}
	else
	{
		printf("error");
		exit(1);
	}
}
void expr_rest()
{
	if (lookahead == '+')
	{
		match('+');
		term();
		printf("1 ");
		expr_rest();
	}
	else if (lookahead == '-')
	{
		match('-');
		term();
		printf("2 ");
		expr_rest();
	}
}
void term_rest()
{
	if (lookahead == '*')
	{
		match('*');
		factor();
		printf("4 ");
		term_rest();
	}
	else if (lookahead == '/')
	{
		match('/');
		factor();
		printf("5 ");
		term_rest();
	}
}
