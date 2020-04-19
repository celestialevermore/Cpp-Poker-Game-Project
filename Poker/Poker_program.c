////1. ��Ŀ����
////�ۼ��� : �߾Ӵ��б� ������а� ������
////Github domain : https://github.com/celestialevermore/Ryan
////���� : ���α׷��� 
////���� : �� �� �� ������ 
////�ۼ��� : 2019�� 12�� 07��
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CRT_SECURE_NO_WARNINGS
#define  NBOFPLAYERS 3 // ���ӿ� �����ϴ� �÷��̾��� ��

enum { CLOV = 1, DIA, HEART, SPADE };// ���̿� ���� ��
enum { NOTYET = 0, SHOW, HIDDEN };// ī���� ���� ���¸� ��Ÿ���� ���� ����� �̸� ����
	   // ���� ���� ���� ���� ���±��� ��Ÿ���� ���� ��� NOTYET�� ��� ���� ����.

#define  STARTMONEY 5000 // ó�� ���� ���� 5000���� �̸� ���� �� ����
#define  KINDOFPOKER 7 // 7 ��ī�� �Ѵ�.
#define  MYTURN  1 // ����ڴ� �ι�°�� �Ѵ�.

typedef struct cards {
	int  num;
	int  shp;
	int  showornot;
}cd;// ����ü�� �����Ѵ�.

cd cds[52]; // ī�带 ���� �迭�� ����. ���� ������ ��� �Լ����� ���� ����.

cd players[NBOFPLAYERS][KINDOFPOKER];// �����ڵ��� ī�� ����ü �迭 ����
int b_m[NBOFPLAYERS];

int stackedmoney; // �ǵ�
int money[NBOFPLAYERS]; // �÷��̾���� ���� ������ �迭
//  �̱�� �� Ƚ��  , ó�� ���� �׼�  , �������� ī���� ��� ���� ���� ����
int wincnt[NBOFPLAYERS], losecnt[NBOFPLAYERS], betting, pdealcnt, tdealcnt;


void assign(void); // ī���� ��ȣ�� ���̸� �Ҵ� �� �ִ� �Լ�
void game(void); // �������� ���� �Լ�
void shuffle(void); // ī�带 ���� �Լ�(�Ʒ��� ���� �Լ��� �����)
void swap(cd* a, cd* b); // ī�带 ���� �� ��� �Ǵ� �Լ�
void deal(int, int);// ī�带 ���� �ִ� �Լ�
void bet(int); // ���� �ϴ� �Լ�
int betcal(int);// ��ǻ���� ������ �׼��� ������ �ִ� �Լ�
int winorlose(void); // �̰���� �������� �Ǵ��� �ִ� �Լ�
void show_cards(void);// ī�带 �����ִ� �Լ�
void result(void); // ����� ���� �ִ� �Լ�
int made(int); // �����, �����.etc�� �Ǵ��� �ִ� �Լ�
void rules(void);// ������ ���� ���� �� �ִ� �Լ�
void showall(void);// ��� ī�带 ���� �ִ� �Լ�

// ���Ⱑ �̷�� �������� �˷��ִ� �Լ���.
int royalflush(int);
int straightflush(int);
int poker(int);
int fullhouse(int);
int flush(int);
int straight(int);
int threeofakind(int);
int twopair(int);
int onepair(int);

void here(int a, int b); //��������
void gameover(char* str); //���ӿ��� �� ����ó��
void mooni(int value);//�������

char* str_exit[3] = {
	  "* �ܾ��� �����մϴ�.�Ѱ��� ��� ���� ����ϴ� �����ϼ���..\n",
	  "\n\n\n\n* ���α׷��� ����� �ּż� �����մϴ�.\n",
	  "* ������ �ܾ��� �����մϴ�. ���α׷��� �����մϴ�.\n",
};

// ���� �Լ��� ����.
void main(void)
{

	assign();  // ī�� ����ü�� �ʱ�ȭ

	rules(); // ������ ���� ����

	srand(time(NULL));  // �����ʱ�ȭ

	game(); // ������ ����
}

// ������ ���� ���� ����.
void rules(void)
{
	printf("     **-----POKER GAME!!-----**\n");
	printf(" ��ö���� �Ͽ콺�� ���� ������ ȯ���մϴ�..\n");
	printf(" 7���� ī�带 ������ �ϴ� ��Ŀ �����Դϴ�..\n");
	printf(" %d���� ��ǻ�Ϳ� ������ �����ϰ� �˴ϴ�.\n", NBOFPLAYERS - 1);
	printf(" %d���� ���۱��� ������ ������ �����մϴ�.\n", STARTMONEY);
	printf(" ���� : 1=A, 11=J, 12=Q, 13=K'\n\n");
}

// ī���� ���̿� ��ȣ�� �Ҵ��� �ִ� �Լ�-����ü�� �ʱ�ȭ
void assign(void)
{
	int i, num;
	for (i = 0; i < 52; i++)
	{
		num = i % 13 + 1;

		if (i < 13)  cds[i].shp = CLOV;
		else if (i < 26) cds[i].shp = DIA;
		else if (i < 39) cds[i].shp = HEART;
		else   cds[i].shp = SPADE;

		cds[i].num = num;
	}
	// �����ڵ��� ó�� ���� �ϴ� ���� �׼��� �ʱ�ȭ
	for (i = 0; i < NBOFPLAYERS; i++)money[i] = STARTMONEY;
}

// ���� ���� �Լ�
void game(void)
{
	int i, winner, go_on = 1;
	char ch;

	while (go_on)
	{
		shuffle();  // ī�带 ���´�.
		if (stackedmoney != 0) printf("*���� ���ӿ��� �ǵ��� �����Ǿ����ϴ� : %d�� \n", stackedmoney);

		here(3, 1);
		here(1, 1); // �׹�° ���� ī�带 ������ �ش�.
		here(1, 1); // �ټ���°���� ī�带 ������ �ش�.
		here(1, 1); // ���� ��°���� ī�带 ������ �ش�.
		here(1, 0); // �ϰ� ��° ������ ī�带 ������ �ش�.

		showall();

		winner = winorlose();  // ���ڸ� �Ǵ��Ѵ�.

		if (winner != -1)
		{
			printf("* %d���Բ��� �¸��ϼ̽��ϴ�.\n\n", winner + 1);
			money[winner] += stackedmoney;  // ���ڿ��� �ǵ��� �����ش�.
			stackedmoney = 0;
		}

		for (i = 0; i < NBOFPLAYERS; i++) (i != winner) ? losecnt[i]++ : wincnt[i]++;


		printf("* �� ������ �Ͻðڽ��ϱ�?(y/n/r)(r - ���) : ");
		getchar();
		ch = getchar();

		if (ch == 'n' || ch == 'N')
			gameover(str_exit[1]);
		else
			(ch == 'r' || ch == 'R') ? result() : printf("*�� ������ ���� �մϴ�.\n\n");

		tdealcnt = 0; pdealcnt = 0;
	}
}

void here(int a, int b)
{
	int i;

	deal(a, b);// a���� ī�带 ������ �ش�.

	for (i = 0; i < NBOFPLAYERS; i++)   bet(i); // ������ �Ѵ�.

	printf("\n**************************************************\n");
	printf("* ���� �ǵ��� %d�� �Դϴ�.\n", stackedmoney);
	printf("**************************************************\n\n");
}
// ī�带 ���� �Լ�. ������ �߻����� �� ������ �̿��Ѵ�.
void shuffle(void)
{
	int i;
	for (i = 0; i < 52; i++) swap(&cds[i], &cds[rand() % 52]);
}

// ������ ī�带 �ٲٴ� �Լ�
void swap(cd* a, cd* b)
{
	cd tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}
// ī�带 ������ �ִ� �Լ�
void deal(int CardstoDeal, int CardstoShow)
{
	int i, j, cnt = pdealcnt;

	printf("* %d��° ī����� %d�� ������ �帮�ڽ��ϴ�.\n", pdealcnt + 1, CardstoDeal);
	// �����ڵ鿡�� ī�带 ������ �ش�.
	for (j = 0; j < NBOFPLAYERS; j++)
	{
		for (i = 0; i < CardstoDeal; i++)
		{
			players[j][pdealcnt + i] = cds[tdealcnt++];// ���� ���� ī��鿡�� ������� ������ �ش�.
			players[j][pdealcnt + i].showornot = HIDDEN;// ī��� �켱 �Ⱥ��� �ִ� ������ �����Ѵ�.
		}

		if (CardstoShow)players[j][pdealcnt].showornot = SHOW;//���� ù��° ī�带 CardstoShow���ڿ� ���� ó���Ѵ�.
	}

	pdealcnt += CardstoDeal;

	show_cards();// ������ ī�带 ���� �ش�.
}
// ���ݱ��� ���� ī�带 �����ִ� �Լ�
void show_cards(void)
{
	int i, j;
	for (j = 0; j < NBOFPLAYERS; j++)
	{
		printf("      <Player %d���� ī��>\n", j + 1);

		for (i = 0; i < pdealcnt; i++)
		{
			// ������ ���� ���� ���ƾ� �� ī��� ***���� ��Ÿ��.
			if (players[j][i].showornot == HIDDEN && (j != MYTURN))  printf(" *** ");
			else
			{
				mooni(players[j][i].shp);// ���� �־�� �ϴ� ī�� : �ڽ��� ī��� ������ ���� �־�� �ϴ� ī��
				printf("%2d", players[j][i].num);//���� �־�� �ϴ� ī�� �̸� ���̿� ���ڸ� ������
			}
		}

		printf("\n\n");
	}
}

void showall(void)
{
	int i, j;

	printf("\n\n* ���� ���� ���� ī�带 ��� �����帮�ڽ��ϴ�.\n");
	for (i = 0; i < NBOFPLAYERS; i++)for (j = 0; j < 7; j++)players[i][j].showornot = SHOW;

	show_cards();
}

// �����ϴ� �Լ�
void bet(int playernb)
{
	int bmoney, b, winner, i;
	// ������� ���� ���� : ������ ���� ��� ������ �ݾ��� �Է� �޴´�.
	if (playernb == MYTURN)
	{
		printf("* ���� ����� ���? : 1\n* ���������� ���? : 2\n* ����ġ��? : 3: ");
		scanf("%d", &b);
		// ������ ��ġ�⸦ ���� ��� ������ ��ģ��.
		// ���ο� ������ ���� ��� ���ڴ� ������ �ǰ�, �ٽ� ������ �����Ѵ�.
		if (b == 2)
		{
			winner = MYTURN - 1;
			printf("\n* %d���Բ��� �¸�!�ϼ̽��ϴ�.\n", winner + 1);
			money[winner] += stackedmoney;
			stackedmoney = 0;

			for (i = 0; i < NBOFPLAYERS; i++)
			{
				if (i != winner)    losecnt[i]++;
				else      wincnt[i]++;
			}

			tdealcnt = 0;
			pdealcnt = 0;
			printf("\n* ��~���ο� ���� �����մϴ�~~~\n");
			game();
		}
		else if (b == 1)
		{
			printf("\n* %d���� �󸶳� �����Ͻðڽ��ϱ�? : ", playernb + 1);

			scanf("%d", &bmoney);// ������ �ݾ��� �Է¹޴´�.

			b_m[playernb] = bmoney;// ������ ���� ���� �� ���´�.
			bmoney += b_m[playernb - 1];// ������ ������ ���� �ڽ��� ������ �ݾ��� ���Ѵ�.
			stackedmoney += bmoney;  // �ǵ��� ������ ������ ���� �ڽ��� ������ ����ŭ �þ��.
			money[playernb] -= bmoney;   // ���� �׸�ŭ �پ���.

			if (money[playernb] < 0) gameover(str_exit[0]);// ���� �����ϸ� ���α׷��� �����Ѵ�.

			printf("\n\n* Player %d���� %d�� �ް�, ", playernb + 1, b_m[0]);
			printf("%d�� ���� �ϼ̽��ϴ�.\n", bmoney - b_m[0]);

			money[playernb - 1] -= b_m[playernb];// ���� �ݾ��� ������ �ް�, ������ ���� �پ���.

			stackedmoney += b_m[playernb]; // �ǵ��� �� ��ŭ �þ��.

			if (money[playernb - 1] < 0) gameover(str_exit[2]);// ������ �ܾ��� �����ϸ� ���α׷��� �����Ѵ�.

			printf("* ������ %d���� �޾ҽ��ϴ�.\n", b_m[playernb]);
		}
		else  gameover(str_exit[1]);
	}
	// ��ǻ�Ͱ� ������ �� ���
	else {
		// ������ �ݾ��� �����Ѵ�.
		bmoney = betcal(playernb);
		b_m[playernb] = bmoney;
		stackedmoney += bmoney;
		money[playernb] -= bmoney;

		if (money[playernb] < 0)
		{
			printf(0);
			exit(1);
		}
		printf("* Player %d �Բ��� ������ �ݾ��� : %d�� �Դϴ�.\n", playernb + 1, bmoney);
	}
	// �� �÷��̾��� �ܾ� ��Ȳ�� �˷��ش�.
	printf("* Player %d ���� �� �ܾ��� %d�� �Դϴ�.\n", playernb + 1, money[playernb]);
}

int betcal(int player)
{
	int p, q;
	p = made(player);
	q = p / 1000;
	if (q == 9)return 1000;
	if (q == 8)return 700;
	if (q == 7)return 600;
	if (q == 6)return 500;
	if (q == 5)return 400;
	if (q == 4)return 300;
	if (q == 3)return 200;
	if (q == 2)return 100;
	else    return 50;
}

// ���и� ������ �Լ�
int winorlose(void)
{
	int a[NBOFPLAYERS], i, maxp = 0, winnersnb, nbofwinners = 0, shape;
	// ���⸦ �ǽ��� ������ ������ ���ڸ� ���Ѵ�.
	for (i = 0; i < NBOFPLAYERS; i++)
	{
		printf("\n            <%d���� ī�� ����>\n", i + 1);
		a[i] = made(i);
		shape = a[i] % 10;

		switch (a[i] / 1000)
		{
		case 9:// �ξ� ��Ʈ����Ʈ �÷����� ���̿� ���� ���и� ���Ѵ�.
			printf("* �ξ� ��Ʈ����Ʈ �÷����Դϴ�.\n*���̴� - ");
			mooni(shape);//���̸� ����Ѵ�.
			break;

		case 8:// ��Ʈ����Ʈ �÷����� ���� ��ȣ�� ���̿� ���� ���и� ���Ѵ�.
			printf("* ��Ʈ����Ʈ �÷����Դϴ�. ");
			printf("* ���� ��ȣ�� - %d�Դϴ�.\n* ���̴� - ", (a[i] % 1000 - shape) / 10);
			mooni(shape);//���̸� ����Ѵ�.
			break;

		case 7: // ��Ŀ�� ��� 4���� ��ȣ�� �����Ƿ� ���̿� ���� ���и� ���Ѵ�.
			printf("* ��ī �Դϴ�. \n* ���̴� - \n ");
			mooni(shape);//���̸� ����Ѵ�.
			break;

		case 6:// Ǯ �Ͽ콺�� ��� ������ ��ȣ�� ���� ī���� ���ڰ� ���� �а� �¸��Ѵ�.
			printf("* Ǯ �Ͽ콺 �Դϴ�.\n");
			printf("* �� ���� ��ȣ�� ���� ī���� ��ȣ�� - %d", a[i] % 100);
			break;

		case 5:// �÷����� ��� ������ ���ڰ� ���� �а� �¸��Ѵ�.
			printf("* �÷��� �Դϴ�.\n* ���̴� - ");
			mooni(shape);//���̸� ����Ѵ�.
			break;

		case 4: // ��Ʈ����Ʈ�� ���� ��ȣ�� ū �а� �¸��Ѵ�.
			printf("* ��Ʈ����Ʈ �Դϴ�.\n");
			printf("* ó�� ���� ��ȣ�� - %d\n", a[i] % 100);
			break;

		case 3: // ���� �� ���� ��ȣ�� ū ī�尡 �¸��Ѵ�.
			printf("* Three of a kind�Դϴ�.\n* ���� ������ ��ȣ�� - ");
			printf(" %d", a[i] % 100);
			break;

		case 2:// ���� �� ���� �� �� ��ȣ�� ���� ��, �� �߿����� ������ ������ ���� �а� �¸��Ѵ�.
			printf("* ����� �Դϴ�.\n");
			printf("* ���� �� ���� ��ȣ �� ū ���� - %d\n* ���̴� - ", (a[i] % 1000 - shape) / 10);
			mooni(shape);//���̸� ����Ѵ�.
			break;

		case 1:// ���ڰ� ���� ����, ���̰� ���� �и� ���� ����� �¸��Ѵ�.
			printf("* ����� �Դϴ�.\n* �� ���ڴ� - ");
			printf("%d\n* �� ���� ������ ���� ���̴� ", (a[i] % 1000 - shape) / 10);
			mooni(shape);//���̸� ����Ѵ�.
			break;

		default:
			printf("�������� �����Ƚ��ϴ� ���������� \n");
			break;
		};

		printf("\n\n");
	}

	for (i = 0; i < NBOFPLAYERS; i++)// ���ڸ� ã�´�(�ִ밪�� ã��, �� ������ ������ ����)
	{
		if (a[i] > maxp)
		{
			maxp = a[i];
			winnersnb = i;
			nbofwinners = 1;
		}
		else if (a[i] == maxp)   nbofwinners++;
	}

	if (nbofwinners == 1)  return winnersnb; // ���ڰ� 1���� ��쿡�� ������ ��ȣ�� �����Ѵ�.
	else       return -1; // ���� ����� ���� ���ڰ� ���� ������ �����Ѵ�.
}


void result(void) // ���ݱ����� ������ ����� ���� �ش�.
{
	int i;

	printf("\n\n**-----Result-----**\n");
	for (i = 0; i < NBOFPLAYERS; i++)
	{
		printf("\n\n      <Player %d���� ���>\n", i + 1);
		printf("* ��  : %10d ��\n", wincnt[i]);
		printf("* ��  : %10d ��\n", losecnt[i]);
		printf("* �ܾ�  : %10d ��\n\n", money[i]);
	}
}

// ���Ⱑ �̷���� ���ڸ� �Ѱ��ִ� �Լ�
int made(int player)
{
	int p = 0;

	//���� �� ���� �������.
	if (p = royalflush(player)) return p;
	else if (p = straightflush(player)) return p;
	else if (p = poker(player))   return p;
	else if (p = fullhouse(player))  return p;
	else if (p = flush(player))   return p;
	else if (p = straight(player))  return p;
	else if (p = threeofakind(player)) return p;
	else if (p = twopair(player))  return p;
	else if (p = onepair(player))  return p;

	return 0;
}

// �ξ� ��Ʈ����Ʈ �÷��������� �Ǵ��ϴ� �Լ�
// 5���� ����� ���� ���ڰ� 10 J Q K A�� ���
int royalflush(int player)
{
	cd temp[4][7];
	int shps[4] = { CLOV,DIA,HEART,SPADE };
	int r, j, i, k;

	if (pdealcnt < 5) return 0;

	for (r = 0; r < 4; r++)// �� ������ ���̸� ���� ����.
	{
		j = 0; // �ӽ� ī�� �迭�� ���� ������ 0���� �ʱ�ȭ
		for (i = 0; i < pdealcnt; i++) // �÷��̾��� i��° ī���� ���̿� r��° ī���� ���̰� ������
			if (players[player][i].shp == shps[r]) // r��° ���̿� �ش��ϴ� �迭�� j��°�� ī�带 �����Ѵ�.
				temp[r][j++] = players[player][i];

		// �� �������� ī�尡 ���� ���� �� �ִ�.
		// ������ �Ѱ��� j�� ���� ������ ī���� ����̹Ƿ�
		if (j >= 5)
		{ // j�� 5���� ũ�ų� ���ƾ߸� �Լ��� �����Ų��.
			for (i = 0; i < j; i++)// ���̰� ���� ī�带 ���ڿ� ����
				for (k = 0; k < j - i; k++)// ������������ ���Ľ�Ų��
					if (temp[r][k].num < temp[r][k + 1].num) // ���� ���ı�� ���
						swap(&temp[r][k], &temp[r][k + 1]);

			// ī���� ���̺�, ���ں� ������ �������Ƿ� �ξ� ��Ʈ����Ʈ �÷�����
			// �ش��ϴ����� �˻��Ѵ�.
			if (
				temp[r][0].num == 13 && temp[r][1].num == 12 &&
				temp[r][2].num == 11 && temp[r][3].num == 10 &&
				temp[r][j - 1].num == 1
				)
				return 9000 + r;
		}

	}
	// ���ݱ��� ���ϰ��� ���ٴ� ���� �ξ�s�� �ƴ϶�� ���̹Ƿ� 0�� ����
	return 0;
}

// ��Ʈ����Ʈ �÷��������� �Ǵ��ϴ� �Լ�
// 5���� ����� �����鼭 ���ڰ� ������ ���
int straightflush(int player)
{
	int i, j, r, k, l, shps[4] = { 1,2,3,4 }, cnt;
	cd temp[4][7];

	if (pdealcnt < 5) return 0;
	for (r = 0; r < 4; r++)
	{
		j = 0;

		for (i = 0; i < pdealcnt; i++)// ��翡 ���� ���ڸ� ������ ��Ų��.
			if (players[player][i].shp == shps[r]) temp[r][j++] = players[player][i];


		if (j >= 5)// j�� 5�̸�, ���� ������ ī�尡 5�� �ִٴ� �ǹ�
		{
			for (i = 0; i < j; i++)for (k = 0; k < j - i; k++)
				if (temp[r][k].num < temp[r][k + 1].num) swap(&temp[r][k], &temp[r][k + 1]);


			// ���� �������� ���ĵ� ���ڸ� �˻��Ѵ�.
			for (i = 0; i < j; i++)
			{
				cnt = 1;
				k = temp[r][i].num;
				for (l = i; l < j; l++)
				{
					if (temp[r][l].num == k)
					{
						cnt++;
						k--;
					}
					else break;
				}
				if (cnt == 5) return 8000 + temp[r][i].num * 10 + r;
			}
			//for end
		}
	}
	return 0;
}

// ��Ŀ���� �Ǵ��ϴ� �Լ�
// 4���� ���ڰ� ���� ���
int poker(int player)
{
	int i, j, cnt;

	if (pdealcnt < 4) return 0;

	// ī�带 ���ڿ� ���� ������������ �����Ѵ�.
	for (i = 0; i < pdealcnt - 1; i++)for (j = 0; j < pdealcnt - i - 1; j++)
		if (players[player][j].num < players[player][j + 1].num)
			swap(&players[player][j], &players[player][j + 1]);

	for (j = 0; j < pdealcnt; j++)
	{
		cnt = 1;
		for (i = j + 1; i < pdealcnt; i++)
		{
			if (players[player][j].num == players[player][i].num)
			{
				cnt++;
				if (cnt == 3)   return 7000 + players[player][j].num;
			}
			else   cnt = 1;

			break;
		}
	}
	return 0;
}

// Ǯ �Ͽ콺 ������ �Ǵ��ϴ� �Լ�
// 3���� ���ڰ� ����, 2���� ���ڰ� ���� ���
int fullhouse(int player)
{
	int i, j, cnt[7], num[7];

	if (pdealcnt < 5) return 0;

	// �迭�� �ʱ�ȭ
	for (i = 0; i < pdealcnt; i++)
	{
		cnt[i] = 0;
		num[i] = 0;
	}

	for (i = 0; i < pdealcnt; i++)
		for (j = 0; j < pdealcnt; j++)
			if (players[player][i].num == players[player][j].num)
			{
				cnt[i]++;
				num[i] = players[player][i].num;
			}

	for (i = 0; i < pdealcnt; i++)if (cnt[i] == 3)
		for (j = 0; j < pdealcnt; j++)if (cnt[j] == 2) return 6000 + num[i];

	return 0;
}


// �÷��� ���� �Ǵ��ϴ� �Լ�
// 5���� ����� ���� ���
int flush(int player)
{
	cd temp[4][7];
	int shps[4] = { CLOV,DIA,HEART,SPADE };
	int r, j, i;

	if (pdealcnt < 5) return 0;

	// �� ������ ���̸� ���� ����.
	for (r = 0; r < 4; r++)
	{
		j = 0; // �ӽ� ī�� �迭�� ���� ������ 0���� �ʱ�ȭ

		// �÷��̾��� i��° ī���� ���̿� r��° ī���� ���̰� ������
		// r��° ���̿� �ش��ϴ� �迭�� j��°�� ī�带 �����Ѵ�.
		for (i = 0; i < pdealcnt; i++)
		{
			if (players[player][i].shp == shps[r]) temp[r][j++] = players[player][i];
			if (j == 5) return 5000 + temp[r][0].shp;
		}
	}

	return 0;
}

// ��Ʈ����Ʈ���� �Ǵ��ϴ� �Լ�
// ���ڰ� 5�� ���� �� ���
int straight(int player)
{
	int i, j;
	if (pdealcnt < 5) return 0;

	// ī���� ��ȣ�� ���� �����Ѵ�.
	for (i = 0; i < pdealcnt - 1; i++)
		for (j = 0; j < pdealcnt - i - 1; j++)
			if (players[player][i].num < players[player][i + 1].num)
				swap(&players[player][i], &players[player][i + 1]);

	// ���ĵ� ī���� ��ȣ�� ���� 1�� �����ϴ� 5���� ī�尡 �ִ��� üũ �Ѵ�.
	for (i = 0; i < pdealcnt - 4; i++)
		if (
			(players[player][i].num == players[player][i + 1].num - 1) &&
			(players[player][i + 1].num - 1 == players[player][i + 2].num - 2) &&
			(players[player][i + 2].num - 2 == players[player][i + 2].num - 3) &&
			(players[player][i + 3].num - 3 == players[player][i + 4].num - 4)
			)
			return 4000 + players[player][i].num;

	return 0;
}

// ������ ���ڰ� �������� üũ�ϴ� �Լ�
int threeofakind(int player)
{
	int i, j = 0, cnt = 1;
	// ��Ʈ����Ʈ�� �˻��ϸ鼭 �̹� ������ �Ǿ� ����
	for (i = j; i < pdealcnt; i++)
	{
		if (players[player][i].num == players[player][i + 1].num)
		{
			cnt++;
			if (cnt == 3) return 3000 + players[player][i].num;
		}
		else
		{
			j++;
			cnt = 1;
		}
	}

	return 0;
}

// ���� �˻��ϴ� �Լ�
// ������ ī�尡 ���� 2���� �ִ� ���
int twopair(int player)
{
	int i, cnt = 0, num[2] = { 0 }, shp[2] = { 0 };

	for (i = 0; i < pdealcnt - 1; i++)// �̹� ���� �������� ������ �Ǿ� ����
		if (players[player][i].num == players[player][i + 1].num)
		{
			num[cnt] = players[player][i].num;
			shp[cnt++] = players[player][i].shp;
			i++;

			if ((cnt == 2) && (shp[0] > shp[1]))return 2000 + num[0] * 10 + shp[0];
			else              return 2000 + num[0] * 10 + shp[1];
		}

	return 0;
}

// ������� ���
int onepair(int player)
{
	int i;
	// �̹� ���ڸ� �������� �������� ������ �Ǿ� ����
	for (i = 0; i < pdealcnt - 1; i++)
	{
		if (players[player][i].num == players[player][i + 1].num)
			if (players[player][i].shp == SPADE || players[player][i + 1].shp == SPADE)
				return 1000 + players[player][i].num * 10 + SPADE;

			else if (players[player][i].shp == HEART || players[player][i + 1].shp == HEART)
				return 1000 + players[player][i].num * 10 + HEART;

			else if (players[player][i].shp == DIA || players[player][i + 1].shp == DIA)
				return 1000 + players[player][i].num * 10 + DIA;

			else return 1000 + players[player][i].num * 10 + CLOV;
	}
	return 0;
}

void gameover(char* str)
{
	printf("%s", str);
	exit(1);
}


void mooni(int value)
{
	switch (value)
	{
	case CLOV:printf("��"); break;
	case DIA:printf("��"); break;
	case HEART:printf("��"); break;
	default:printf("��"); break;
	};
}