#include<stdio.h>
#include<ctype.h>

// (���ĺ�) or (���ĺ� + ����) ��ū�� ó���ϱ� ���� ��������
int count = 0; 

int state = 0;
char nextchar();
int nexttoken();
char c;
void retraction(char c); // �Է� �ǵ����� 

void main()
{
	int num;
	printf("�Է� :");
	while (1)
	{
		num = nexttoken();
		count = 0;
		if (num != -1) // c�� �Է��� ���� ��Ÿ���� ������ '$'���, -1�� ����
		{
			printf("%d ", num); // ���� �� ���
		}

		if (c == '$') // c�� �����Է� ���Ḧ ��Ÿ���� ��ȣ�̸� ����
			break;
		else // �׷��������� ��� invaid���̹Ƿ� ��Ʈ���� �ѵ�, state = 11
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
			if (c == 'f') // c�� f�� state=1, count�� ����
			{
				state = 1;
				count++; // 9�� ������ �� �ֵ��� ���� 
			}
			else if (c == 'i') // c�� i�� state=5
			{
				state = 5;
				count++;
			}
			else if (isalpha(c)) // c�� i�� �ƴϰ� f�� �ƴ� ���ĺ��̶�� state=11
			{
				state = 11;
				count++;
			}
			else if (c == '$') // c�� �Է��� ���� ��Ÿ���� ������ '$'���, -1�� ����
				return -1;
			else // �� ������ ��� �������� ������ state=12
				state = 12;
			break;
		case 1:
			c = nextchar();
			if (c == 'o')
				state = 2;
			else if (isalnum(c)) // ���ڳ� ���ڶ�� state = 11
			{
				state = 11;
			}
			else // invalid ���̶�� state = 12
			{
				state = 12;
			}
			break;
		case 2:
			c = nextchar();
			if (c == 'r')
				state = 3;
			else if (isalnum(c)) // ���ڳ� ���ڶ�� state = 11
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
			if (isalnum(c)) // ���ڳ� ���ڶ�� state = 11
				state = 11;
			else // �� ���� ���̶��, state = 4�� ���� 1����(��ū for �ν�)
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
			else if (isalnum(c)) // ���ڳ� ���ڶ�� state = 11
			{
				state = 11;
			}
			else // �׷��� ������ state = 12 �� ���� 9 ����
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
			if (isalnum(c)) // ���ڳ� ���ڶ�� �ٽ� state = 1
			{
				count++;
				state = 11;
			}
			else // ��Ʈ���� �� ���� invalid���̶�� state = 12
				state = 12;
			break;
		case 12:
			if (count > 0) // ���ĺ� �Ǵ� ���ĺ�+ ���� �����̸� ��ū�̹Ƿ� 9����
				return 9;

			if (c != '$') // ���� �Է� ���ᰡ �ƴϸ� ��� invalid���̹Ƿ� ���� ���
			{
				printf("invalid char '%c' ", c);
				state = 0; // state = 0 ���� ���� �����ִ� �Է°� ó�� 
			}
			break;
		}
	}
}

void retraction(char c) {
	ungetc(c, stdin);
}