%{
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG	0

#define	 MAXSYM	100
#define	 MAXSYMLEN	20
#define	 MAXTSYMLEN	15
#define	 MAXTSYMBOL	MAXSYM/2

#define STMTLIST 500

typedef struct nodeType {
	int token;
	int tokenval;
	struct nodeType *son;
	struct nodeType *brother;
	} Node;

#define YYSTYPE Node*
	
int tsymbolcnt=0;
int errorcnt=0;

FILE *yyin;
FILE *fp;

extern char symtbl[MAXSYM][MAXSYMLEN];
extern int maxsym;
extern int lineno;

void DFSTree(Node*);
Node * MakeOPTree(int, Node*, Node*);
Node * MakeLongTree(int, Node*, Node*,Node*);
Node * MakeLabel(int, Node *operand1);
Node * MakeNode(int, int);
Node * MakeListTree(Node*, Node*);
void codegen(Node* );
void prtcode(int, int);

void	dwgen();
int	gentemp();
void	assgnstmt(int, int);
void	numassgn(int, int);
void	addstmt(int, int, int);
void	substmt(int, int, int);
int		insertsym(char *);
%}

%token	LGAL RGAL MUL DIV ADD SUB ASSGN LPARA RPARA JUMP WHILE FOR GT LT GE LE ID NUM STMTEND START DONE END IF LOVE STAR ELSE ID2



%%
program	: START stmt_list END	{ if (errorcnt==0) {codegen($2); dwgen();} }
		;

stmt_list: 	stmt_list stmt 	{$$=MakeListTree($1, $2);}
		|	stmt			{$$=MakeListTree(NULL, $1);}
		| 	error STMTEND	{ errorcnt++; yyerrok;}
		;

stmt	: 	ID ASSGN expr STMTEND	{ $1->token = ID2; $$=MakeOPTree(ASSGN, $1, $3);}
		| 	ID JUMP ID STMTEND { $$=MakeOPTree(JUMP, $1, $3);}
		|   IF LGAL com_pare RGAL LPARA stmt_list RPARA ELSE stmt_list DONE { $$=MakeLongTree(IF, $3, MakeLabel(LOVE, $6), MakeLabel(STAR, $9));}
		|   WHILE LGAL com_pare RGAL LPARA stmt_list RPARA { $$=MakeLongTree(WHILE, MakeLabel(STAR, NULL), $3, MakeLabel(LOVE, $6));}
		;

com_pare : expr GT expr { $$=MakeOPTree(GT, $1, $3); }
		 | expr LT expr { $$=MakeOPTree(LT, $1, $3); }
		 | expr LE expr { $$=MakeOPTree(LE, $1, $3); }
		 | expr GE expr { $$=MakeOPTree(GE, $1, $3); }
		 ;

expr	: 	expr ADD term	{ $$=MakeOPTree(ADD, $1, $3); }
		|	expr SUB term	{ $$=MakeOPTree(SUB, $1, $3); }
		|	term
		;

term	:  term MUL factor { $$=MakeOPTree(MUL, $1, $3); }
		|  term DIV factor { $$=MakeOPTree(DIV, $1, $3); }
		|  factor
		;

factor  :   LGAL expr RGAL { $$=$2;}
		|	ID		{ /* ID node is created in lex */ }
		|	NUM		{ /* NUM node is created in lex */ }
		;


%%
int main(int argc, char *argv[]) 
{
	printf("\nsample CBU compiler v2.0\n");
	printf("(C) Copyright by Jae Sung Lee (jasonlee@cbnu.ac.kr), 2022.\n");
	printf("2020039009_ChaHyeona.\n");
	
	if (argc == 2)
		yyin = fopen(argv[1], "r");
	else {
		printf("Usage: cbu2 inputfile\noutput file is 'a.asm'\n");
		return(0);
		}
		
	fp=fopen("a.asm", "w");
	
	yyparse();
	
	fclose(yyin);
	fclose(fp);

	if (errorcnt==0) 
		{ printf("Successfully compiled. Assembly code is in 'a.asm'.\n");}
}

yyerror(s)
char *s;
{
	printf("%s (line %d)\n", s, lineno);
}

Node * MakeLabel(int op, Node *operand1)
{
	Node * node;
	node = (Node *) malloc(sizeof (Node));
	node->token = op;
	node->tokenval =op; 
	node->son = operand1;
    node->brother = NULL;
	return node;
}

Node * MakeLongTree(int op, Node* operand1, Node* operand2, Node* operand3)
{
	Node * newnode;
	newnode = (Node *)malloc(sizeof (Node));

	newnode->token = op;
	newnode->tokenval = op;

	newnode->son = operand1;
	newnode->brother = NULL;

	operand1->brother= operand2;
	operand2->brother= operand3;

	return newnode;
}

Node * MakeOPTree(int op, Node* operand1, Node* operand2)
{
Node * newnode;

	newnode = (Node *)malloc(sizeof (Node));
	newnode->token = op;
	newnode->tokenval = op;
	newnode->son = operand1;
	newnode->brother = NULL;
	operand1->brother = operand2;
	return newnode;
}

Node * MakeNode(int token, int operand)
{
Node * newnode;

	newnode = (Node *) malloc(sizeof (Node));
	newnode->token = token;
	newnode->tokenval = operand; 
	newnode->son = newnode->brother = NULL;
	return newnode;
}

Node * MakeListTree(Node* operand1, Node* operand2)
{
Node * newnode;
Node * node;

	if (operand1 == NULL){
		newnode = (Node *)malloc(sizeof (Node));
		newnode->token = newnode-> tokenval = STMTLIST;
		newnode->son = operand2;
		newnode->brother = NULL;
		return newnode;
		}
	else {
		node = operand1->son;
		while (node->brother != NULL) node = node->brother;
		node->brother = operand2;
		return operand1;
		}
}

void codegen(Node * root)
{
	DFSTree(root);
}

void DFSTree(Node * n)
{
	if (n==NULL) return;
	DFSTree(n->son);
	prtcode(n->token, n->tokenval);
	DFSTree(n->brother);
	
}

void prtcode(int token, int val)
{
	switch (token) {
	case ID:
		fprintf(fp,"RVALUE %s\n", symtbl[val]);
		break;
	case ID2:
		fprintf(fp, "LVALUE %s\n", symtbl[val]);
		break;
	case NUM:
		fprintf(fp, "PUSH %d\n", val);
		break;
	case GE:
		fprintf(fp, "-\n GOMINUS memloc\n");
		break;
	case LE:
		fprintf(fp, "-\n GOPLUS memloc\n");
		break;
	case LT:
		fprintf(fp, "-\n COPY \n GOPLUS memloc\n GOFALSE memloc\n");
		break;
	case GT:
		fprintf(fp, "-\n COPY \n GOMINUS memloc\n GOFALSE memloc\n");
		break;
	case LOVE:
		fprintf(fp, "LABEL memloc \n"); 
		break;
	case STAR:
		fprintf(fp, "LABEL loc \n"); 
		break;
	case JUMP:
		fprintf(fp, "GOTO loc \n"); 
		break;
	case ADD:
		fprintf(fp, "+\n");
		break;
	case MUL:
		fprintf(fp, "*\n");
		break;
	case DIV:
		fprintf(fp, "/\n");
		break;
	case SUB:
		fprintf(fp, "-\n");
		break;
	case ASSGN:
		fprintf(fp, ":=\n");
		break;
	case STMTLIST:
	default:
		break;
	};
}


/*
int gentemp()
{
char buffer[MAXTSYMLEN];
char tempsym[MAXSYMLEN]="TTCBU";

	tsymbolcnt++;
	if (tsymbolcnt > MAXTSYMBOL) printf("temp symbol overflow\n");
	itoa(tsymbolcnt, buffer, 10);
	strcat(tempsym, buffer);
	return( insertsym(tempsym) ); // Warning: duplicated symbol is not checked for lazy implementation
}
*/
void dwgen()
{
int i;
	fprintf(fp, "HALT\n");
	fprintf(fp, "$ -- END OF EXECUTION CODE AND START OF VAR DEFINITIONS --\n");

// Warning: this code should be different if variable declaration is supported in the language 
	for(i=0; i<maxsym; i++) 
		fprintf(fp, "DW %s\n", symtbl[i]);
	fprintf(fp, "END\n");
}

