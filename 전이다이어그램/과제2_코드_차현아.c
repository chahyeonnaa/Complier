#include<stdio.h>
#include<ctype.h>

// (알파벳) or (알파벳 + 숫자) 토큰을 처리하기 위한 전역변수
int count = 0; 

int state = 0;
char nextchar();
int nexttoken();
char c;
void retraction(char c); // 입력 되돌리기 

void main()
{
	int num;
	printf("입력 :");
	while (1)
	{
		num = nexttoken();
		count = 0;
		if (num != -1) // c가 입력의 끝을 나타내는 문자인 '$'라면, -1을 리턴
		{
			printf("%d ", num); // 리턴 값 출력
		}

		if (c == '$') // c가 문자입력 종료를 나타내는 기호이면 종료
			break;
		else // 그렇지않으면 모두 invaid값이므로 리트랙션 한뒤, state = 11
		{
			retraction(c);
			state = 11;
		}
	}
	return;
}

char nextchar()
{
	while (1)
	{
		c = getchar();
		return c;
	}
}

int nexttoken()
{
	while (1)
	{
		switch (state)
		{
		case 0:
			c = nextchar();
			if (c == 'f') // c가 f면 state=1, count값 증가
			{
				state = 1;
				count++; // 9를 리턴할 수 있도록 만듦 
			}
			else if (c == 'i') // c가 i면 state=5
			{
				state = 5;
				count++;
			}
			else if (isalpha(c)) // c가 i도 아니고 f도 아닌 알파벳이라면 state=11
			{
				state = 11;
				count++;
			}
			else if (c == '$') // c가 입력의 끝을 나타내는 문자인 '$'라면, -1을 리턴
				return -1;
			else // 위 조건을 모두 만족하지 않으면 state=12
				state = 12;
			break;
		case 1:
			c = nextchar();
			if (c == 'o')
				state = 2;
			else if (isalnum(c)) // 문자나 숫자라면 state = 11
			{
				state = 11;
			}
			else // invalid 값이라면 state = 12
			{
				state = 12;
			}
			break;
		case 2:
			c = nextchar();
			if (c == 'r')
				state = 3;
			else if (isalnum(c)) // 문자나 숫자라면 state = 11
			{
				state = 11;
			}
			else 
			{
				state = 12;
			}
			break;
		case 3:
			c = nextchar();
			if (isalnum(c)) // 문자나 숫자라면 state = 11
				state = 11;
			else // 그 외의 값이라면, state = 4로 가서 1리턴(토큰 for 인식)
				state = 4;
			break;
		case 4:
			return 1;
			break;
		case 5:
			c = nextchar();
			if (c == 'f')
				state = 6;
			else if (c == 'n')
				state = 8;
			else if (isalnum(c)) // 문자나 숫자라면 state = 11
			{
				state = 11;
			}
			else // 그렇지 않으면 state = 12 로 가서 9 리턴
			{
				state = 12;
			}
			break;
		case 6:
			c = nextchar();
			if (isalnum(c))
				state = 11;
			else
				state = 7;
			break;
		case 7:
			return 2;
			break;
		case 8:
			c = nextchar();
			if (c == 't')
				state = 9;
			else if (isalnum(c))
			{
				state = 11;
			}
			else
			{
				state = 12;
			}
			break;
		case 9:
			c = nextchar();
			if (isalnum(c))
				state = 11;
			else
				state = 10;
			break;
		case 10:
			return 3;
			break;
		case 11:
			c = nextchar();
			if (isalnum(c)) // 문자나 숫자라면 다시 state = 1
			{
				count++;
				state = 11;
			}
			else // 리트랙션 한 뒤의 invalid값이라면 state = 12
				state = 12;
			break;
		case 12:
			if (count > 0) // 알파벳 또는 알파벳+ 숫자 조합이면 토큰이므로 9리턴
				return 9;

			if (c != '$') // 문자 입력 종료가 아니면 모두 invalid값이므로 오류 출력
			{
				printf("invalid char '%c' ", c);
				state = 0; // state = 0 으로 가서 남아있는 입력값 처리 
			}
			break;
		}
	}
}

void retraction(char c) {
	ungetc(c, stdin);
}