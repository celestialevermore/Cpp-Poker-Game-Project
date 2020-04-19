////1. 포커게임
////작성자 : 중앙대학교 영어영문학과 김응엽
////Github domain : https://github.com/celestialevermore/Ryan
////과목 : 프로그래밍 
////교수 : 이 남 규 교수님 
////작성일 : 2019년 12월 07일
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CRT_SECURE_NO_WARNINGS
#define  NBOFPLAYERS 3 // 게임에 참가하는 플레이어의 수

enum { CLOV = 1, DIA, HEART, SPADE };// 무늬에 대한 수
enum { NOTYET = 0, SHOW, HIDDEN };// 카드의 현재 상태를 나타내기 위한 상수를 미리 정의
	   // 아직 받지 않은 패의 상태까지 나타내기 위한 상수 NOTYET은 사용 하지 않음.

#define  STARTMONEY 5000 // 처음 갖는 돈을 5000으로 미리 정의 해 놓음
#define  KINDOFPOKER 7 // 7 포카를 한다.
#define  MYTURN  1 // 사용자는 두번째에 한다.

typedef struct cards {
	int  num;
	int  shp;
	int  showornot;
}cd;// 구조체를 정의한다.

cd cds[52]; // 카드를 담을 배열을 선언. 전역 변수로 모든 함수에서 접근 가능.

cd players[NBOFPLAYERS][KINDOFPOKER];// 참가자들의 카드 구조체 배열 선언
int b_m[NBOFPLAYERS];

int stackedmoney; // 판돈
int money[NBOFPLAYERS]; // 플레이어들의 돈을 저장할 배열
//  이기고 진 횟수  , 처음 돈의 액수  , 나누어준 카드의 장수 등을 위한 변수
int wincnt[NBOFPLAYERS], losecnt[NBOFPLAYERS], betting, pdealcnt, tdealcnt;


void assign(void); // 카드의 번호와 무늬를 할당 해 주는 함수
void game(void); // 실제적인 게임 함수
void shuffle(void); // 카드를 섞는 함수(아래의 스왑 함수를 사용함)
void swap(cd* a, cd* b); // 카드를 섞을 때 사용 되는 함수
void deal(int, int);// 카드를 나눠 주는 함수
void bet(int); // 베팅 하는 함수
int betcal(int);// 컴퓨터의 베팅할 액수를 결정해 주는 함수
int winorlose(void); // 이겼는지 졌는지를 판단해 주는 함수
void show_cards(void);// 카드를 보여주는 함수
void result(void); // 결과를 보여 주는 함수
int made(int); // 원페어, 투페어.etc를 판단해 주는 함수
void rules(void);// 게임의 룰을 설명 해 주는 함수
void showall(void);// 모든 카드를 보여 주는 함수

// 짓기가 이루어 졌는지를 알려주는 함수들.
int royalflush(int);
int straightflush(int);
int poker(int);
int fullhouse(int);
int flush(int);
int straight(int);
int threeofakind(int);
int twopair(int);
int onepair(int);

void here(int a, int b); //게임진행
void gameover(char* str); //게임오버 및 종료처리
void mooni(int value);//무늬출력

char* str_exit[3] = {
	  "* 잔액이 부족합니다.한강물 요새 많이 춥습니다 수고하세요..\n",
	  "\n\n\n\n* 프로그램을 사용해 주셔서 감사합니다.\n",
	  "* 상대방의 잔액이 부족합니다. 프로그램을 종료합니다.\n",
};

// 메인 함수의 시작.
void main(void)
{

	assign();  // 카드 구조체의 초기화

	rules(); // 게임의 룰을 설명

	srand(time(NULL));  // 난수초기화

	game(); // 게임을 시작
}

// 게임의 룰을 설명 해줌.
void rules(void)
{
	printf("     **-----POKER GAME!!-----**\n");
	printf(" 곽철용의 하우스에 오신 여러분 환영합니다..\n");
	printf(" 7장의 카드를 가지고 하는 포커 게임입니다..\n");
	printf(" %d대의 컴퓨터와 게임을 진행하게 됩니다.\n", NBOFPLAYERS - 1);
	printf(" %d원의 시작금을 가지고 게임을 시작합니다.\n", STARTMONEY);
	printf(" 주의 : 1=A, 11=J, 12=Q, 13=K'\n\n");
}

// 카드의 무늬와 번호를 할당해 주는 함수-구조체의 초기화
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
	// 참가자들의 처음 시작 하는 돈의 액수를 초기화
	for (i = 0; i < NBOFPLAYERS; i++)money[i] = STARTMONEY;
}

// 실제 게임 함수
void game(void)
{
	int i, winner, go_on = 1;
	char ch;

	while (go_on)
	{
		shuffle();  // 카드를 섞는다.
		if (stackedmoney != 0) printf("*직전 게임에서 판돈이 누적되었습니다 : %d원 \n", stackedmoney);

		here(3, 1);
		here(1, 1); // 네번째 장의 카드를 나누어 준다.
		here(1, 1); // 다섯번째장의 카드를 나누어 준다.
		here(1, 1); // 여섯 번째장의 카드를 나누어 준다.
		here(1, 0); // 일곱 번째 마지막 카드를 나누어 준다.

		showall();

		winner = winorlose();  // 승자를 판단한다.

		if (winner != -1)
		{
			printf("* %d번님께서 승리하셨습니다.\n\n", winner + 1);
			money[winner] += stackedmoney;  // 승자에게 판돈을 더해준다.
			stackedmoney = 0;
		}

		for (i = 0; i < NBOFPLAYERS; i++) (i != winner) ? losecnt[i]++ : wincnt[i]++;


		printf("* 새 게임을 하시겠습니까?(y/n/r)(r - 결과) : ");
		getchar();
		ch = getchar();

		if (ch == 'n' || ch == 'N')
			gameover(str_exit[1]);
		else
			(ch == 'r' || ch == 'R') ? result() : printf("*새 게임을 시작 합니다.\n\n");

		tdealcnt = 0; pdealcnt = 0;
	}
}

void here(int a, int b)
{
	int i;

	deal(a, b);// a장의 카드를 나누어 준다.

	for (i = 0; i < NBOFPLAYERS; i++)   bet(i); // 베팅을 한다.

	printf("\n**************************************************\n");
	printf("* 현재 판돈은 %d원 입니다.\n", stackedmoney);
	printf("**************************************************\n\n");
}
// 카드를 섞는 함수. 난수를 발생시켜 그 난수를 이용한다.
void shuffle(void)
{
	int i;
	for (i = 0; i < 52; i++) swap(&cds[i], &cds[rand() % 52]);
}

// 두장의 카드를 바꾸는 함수
void swap(cd* a, cd* b)
{
	cd tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}
// 카드를 나누어 주는 함수
void deal(int CardstoDeal, int CardstoShow)
{
	int i, j, cnt = pdealcnt;

	printf("* %d번째 카드부터 %d장 나누어 드리겠습니다.\n", pdealcnt + 1, CardstoDeal);
	// 참가자들에게 카드를 나누어 준다.
	for (j = 0; j < NBOFPLAYERS; j++)
	{
		for (i = 0; i < CardstoDeal; i++)
		{
			players[j][pdealcnt + i] = cds[tdealcnt++];// 섞어 놓은 카드들에서 순서대로 나누어 준다.
			players[j][pdealcnt + i].showornot = HIDDEN;// 카드는 우선 안보여 주는 것으로 세팅한다.
		}

		if (CardstoShow)players[j][pdealcnt].showornot = SHOW;//받은 첫번째 카드를 CardstoShow인자에 따라 처리한다.
	}

	pdealcnt += CardstoDeal;

	show_cards();// 서로의 카드를 보여 준다.
}
// 지금까지 받은 카드를 보여주는 함수
void show_cards(void)
{
	int i, j;
	for (j = 0; j < NBOFPLAYERS; j++)
	{
		printf("      <Player %d님의 카드>\n", j + 1);

		for (i = 0; i < pdealcnt; i++)
		{
			// 상대방의 보여 주지 말아야 할 카드는 ***으로 나타냄.
			if (players[j][i].showornot == HIDDEN && (j != MYTURN))  printf(" *** ");
			else
			{
				mooni(players[j][i].shp);// 보여 주어야 하는 카드 : 자신의 카드와 상대방이 보여 주어야 하는 카드
				printf("%2d", players[j][i].num);//보여 주어야 하는 카드 이면 무늬와 숫자를 보여줌
			}
		}

		printf("\n\n");
	}
}

void showall(void)
{
	int i, j;

	printf("\n\n* 지금 까지 받은 카드를 모두 보여드리겠습니다.\n");
	for (i = 0; i < NBOFPLAYERS; i++)for (j = 0; j < 7; j++)players[i][j].showornot = SHOW;

	show_cards();
}

// 베팅하는 함수
void bet(int playernb)
{
	int bmoney, b, winner, i;
	// 사용자의 베팅 차례 : 베팅을 원할 경우 베팅할 금액을 입력 받는다.
	if (playernb == MYTURN)
	{
		printf("* 묻고 떠블로 기기? : 1\n* 새게임으로 기기? : 2\n* 다이치실? : 3: ");
		scanf("%d", &b);
		// 게임을 마치기를 원할 경우 게임을 마친다.
		// 새로운 게임을 원할 경우 승자는 상대방이 되고, 다시 게임을 시작한다.
		if (b == 2)
		{
			winner = MYTURN - 1;
			printf("\n* %d번님께서 승리!하셨습니다.\n", winner + 1);
			money[winner] += stackedmoney;
			stackedmoney = 0;

			for (i = 0; i < NBOFPLAYERS; i++)
			{
				if (i != winner)    losecnt[i]++;
				else      wincnt[i]++;
			}

			tdealcnt = 0;
			pdealcnt = 0;
			printf("\n* 자~새로운 판을 시작합니다~~~\n");
			game();
		}
		else if (b == 1)
		{
			printf("\n* %d번님 얼마나 베팅하시겠습니까? : ", playernb + 1);

			scanf("%d", &bmoney);// 베팅할 금액을 입력받는다.

			b_m[playernb] = bmoney;// 베팅한 돈을 저장 해 놓는다.
			bmoney += b_m[playernb - 1];// 상대방이 베팅한 돈과 자신이 베팅할 금액을 더한다.
			stackedmoney += bmoney;  // 판돈은 상대방이 베팅한 돈과 자신이 베팅한 돈만큼 늘어난다.
			money[playernb] -= bmoney;   // 돈도 그만큼 줄어든다.

			if (money[playernb] < 0) gameover(str_exit[0]);// 돈이 부족하면 프로그램을 종료한다.

			printf("\n\n* Player %d께서 %d원 받고, ", playernb + 1, b_m[0]);
			printf("%d원 베팅 하셨습니다.\n", bmoney - b_m[0]);

			money[playernb - 1] -= b_m[playernb];// 베팅 금액을 상대방이 받고, 상대방의 돈은 줄어든다.

			stackedmoney += b_m[playernb]; // 판돈은 그 만큼 늘어난다.

			if (money[playernb - 1] < 0) gameover(str_exit[2]);// 상대방의 잔액이 부족하면 프로그램을 종료한다.

			printf("* 상대방이 %d원을 받았습니다.\n", b_m[playernb]);
		}
		else  gameover(str_exit[1]);
	}
	// 컴퓨터가 베팅을 할 경우
	else {
		// 베팅할 금액을 결정한다.
		bmoney = betcal(playernb);
		b_m[playernb] = bmoney;
		stackedmoney += bmoney;
		money[playernb] -= bmoney;

		if (money[playernb] < 0)
		{
			printf(0);
			exit(1);
		}
		printf("* Player %d 님께서 베팅한 금액은 : %d원 입니다.\n", playernb + 1, bmoney);
	}
	// 각 플레이어의 잔액 상황을 알려준다.
	printf("* Player %d 님의 현 잔액은 %d원 입니다.\n", playernb + 1, money[playernb]);
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

// 승패를 나누는 함수
int winorlose(void)
{
	int a[NBOFPLAYERS], i, maxp = 0, winnersnb, nbofwinners = 0, shape;
	// 짓기를 실시한 내용을 가지고 승자를 정한다.
	for (i = 0; i < NBOFPLAYERS; i++)
	{
		printf("\n            <%d님의 카드 정보>\n", i + 1);
		a[i] = made(i);
		shape = a[i] % 10;

		switch (a[i] / 1000)
		{
		case 9:// 로얄 스트레이트 플러쉬는 무늬에 따라 승패를 정한다.
			printf("* 로얄 스트레이트 플러쉬입니다.\n*무늬는 - ");
			mooni(shape);//무늬를 출력한다.
			break;

		case 8:// 스트레이트 플러쉬는 시작 번호와 무늬에 따라 승패를 정한다.
			printf("* 스트레이트 플러쉬입니다. ");
			printf("* 시작 번호는 - %d입니다.\n* 무늬는 - ", (a[i] % 1000 - shape) / 10);
			mooni(shape);//무늬를 출력한다.
			break;

		case 7: // 포커의 경우 4장의 번호가 같으므로 무늬에 따라 승패를 정한다.
			printf("* 포카 입니다. \n* 무늬는 - \n ");
			mooni(shape);//무늬를 출력한다.
			break;

		case 6:// 풀 하우스의 경우 세장의 번호가 같은 카드의 숫자가 높은 패가 승리한다.
			printf("* 풀 하우스 입니다.\n");
			printf("* 세 장의 번호가 같은 카드의 번호는 - %d", a[i] % 100);
			break;

		case 5:// 플러쉬의 경우 무늬의 숫자가 높은 패가 승리한다.
			printf("* 플러쉬 입니다.\n* 무늬는 - ");
			mooni(shape);//무늬를 출력한다.
			break;

		case 4: // 스트레이트는 시작 번호가 큰 패가 승리한다.
			printf("* 스트레이트 입니다.\n");
			printf("* 처음 시작 번호는 - %d\n", a[i] % 100);
			break;

		case 3: // 같은 세 장의 번호가 큰 카드가 승리한다.
			printf("* Three of a kind입니다.\n* 같은 세장의 번호는 - ");
			printf(" %d", a[i] % 100);
			break;

		case 2:// 같은 두 쌍의 패 중 번호가 높은 패, 그 중에서도 무늬의 순위가 높은 패가 승리한다.
			printf("* 투페어 입니다.\n");
			printf("* 같은 두 장의 번호 중 큰 수는 - %d\n* 무늬는 - ", (a[i] % 1000 - shape) / 10);
			mooni(shape);//무늬를 출력한다.
			break;

		case 1:// 숫자가 높을 수록, 무늬가 높은 패를 가진 사람이 승리한다.
			printf("* 원페어 입니다.\n* 그 숫자는 - ");
			printf("%d\n* 두 장중 점수가 높은 무늬는 ", (a[i] % 1000 - shape) / 10);
			mooni(shape);//무늬를 출력한다.
			break;

		default:
			printf("개족보가 떠버렸습니다 ㅋㅋㅋㅋㅋ \n");
			break;
		};

		printf("\n\n");
	}

	for (i = 0; i < NBOFPLAYERS; i++)// 승자를 찾는다(최대값을 찾고, 그 숫자의 주인을 리턴)
	{
		if (a[i] > maxp)
		{
			maxp = a[i];
			winnersnb = i;
			nbofwinners = 1;
		}
		else if (a[i] == maxp)   nbofwinners++;
	}

	if (nbofwinners == 1)  return winnersnb; // 승자가 1명인 경우에만 승자의 번호를 리턴한다.
	else       return -1; // 공동 우승인 경우는 승자가 없는 것으로 간주한다.
}


void result(void) // 지금까지의 승패의 결과를 보여 준다.
{
	int i;

	printf("\n\n**-----Result-----**\n");
	for (i = 0; i < NBOFPLAYERS; i++)
	{
		printf("\n\n      <Player %d님의 결과>\n", i + 1);
		printf("* 승  : %10d 번\n", wincnt[i]);
		printf("* 패  : %10d 번\n", losecnt[i]);
		printf("* 잔액  : %10d 원\n\n", money[i]);
	}
}

// 짓기가 이루어진 숫자를 넘겨주는 함수
int made(int player)
{
	int p = 0;

	//높은 것 부터 지어내린다.
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

// 로얄 스트레이트 플러쉬인지를 판단하는 함수
// 5장의 모양이 같고 숫자가 10 J Q K A인 경우
int royalflush(int player)
{
	cd temp[4][7];
	int shps[4] = { CLOV,DIA,HEART,SPADE };
	int r, j, i, k;

	if (pdealcnt < 5) return 0;

	for (r = 0; r < 4; r++)// 네 가지의 무늬를 비교해 본다.
	{
		j = 0; // 임시 카드 배열을 위한 변수를 0으로 초기화
		for (i = 0; i < pdealcnt; i++) // 플레이어의 i번째 카드의 무늬와 r번째 카드의 무늬가 같으면
			if (players[player][i].shp == shps[r]) // r번째 무늬에 해당하는 배열의 j번째에 카드를 저장한다.
				temp[r][j++] = players[player][i];

		// 이 시점에서 카드가 무늬 별로 모여 있다.
		// 위에서 넘겨진 j는 같은 무늬인 카드의 장수이므로
		if (j >= 5)
		{ // j가 5보다 크거나 같아야만 함수를 진행시킨다.
			for (i = 0; i < j; i++)// 무늬가 같은 카드를 숫자에 따라
				for (k = 0; k < j - i; k++)// 내림차순으로 정렬시킨다
					if (temp[r][k].num < temp[r][k + 1].num) // 버블 정렬기법 사용
						swap(&temp[r][k], &temp[r][k + 1]);

			// 카드의 무늬별, 숫자별 정렬이 끝났으므로 로얄 스트레이트 플러쉬에
			// 해당하는지를 검사한다.
			if (
				temp[r][0].num == 13 && temp[r][1].num == 12 &&
				temp[r][2].num == 11 && temp[r][3].num == 10 &&
				temp[r][j - 1].num == 1
				)
				return 9000 + r;
		}

	}
	// 지금까지 리턴값이 없다는 것은 로얄s가 아니라는 뜻이므로 0을 리턴
	return 0;
}

// 스트레이트 플러쉬인지를 판단하는 함수
// 5장의 모양이 같으면서 숫자가 연속일 경우
int straightflush(int player)
{
	int i, j, r, k, l, shps[4] = { 1,2,3,4 }, cnt;
	cd temp[4][7];

	if (pdealcnt < 5) return 0;
	for (r = 0; r < 4; r++)
	{
		j = 0;

		for (i = 0; i < pdealcnt; i++)// 모양에 따라 숫자를 정렬을 시킨다.
			if (players[player][i].shp == shps[r]) temp[r][j++] = players[player][i];


		if (j >= 5)// j가 5이면, 같은 무늬의 카드가 5장 있다는 의미
		{
			for (i = 0; i < j; i++)for (k = 0; k < j - i; k++)
				if (temp[r][k].num < temp[r][k + 1].num) swap(&temp[r][k], &temp[r][k + 1]);


			// 내림 차순으로 정렬된 숫자를 검사한다.
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

// 포커인지 판단하는 함수
// 4장의 숫자가 같은 경우
int poker(int player)
{
	int i, j, cnt;

	if (pdealcnt < 4) return 0;

	// 카드를 숫자에 따라 내림차순으로 정렬한다.
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

// 풀 하우스 인지를 판단하는 함수
// 3장의 숫자가 같고, 2장의 숫자가 같은 경우
int fullhouse(int player)
{
	int i, j, cnt[7], num[7];

	if (pdealcnt < 5) return 0;

	// 배열의 초기화
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


// 플러쉬 인지 판단하는 함수
// 5장의 모양이 같은 경우
int flush(int player)
{
	cd temp[4][7];
	int shps[4] = { CLOV,DIA,HEART,SPADE };
	int r, j, i;

	if (pdealcnt < 5) return 0;

	// 네 가지의 무늬를 비교해 본다.
	for (r = 0; r < 4; r++)
	{
		j = 0; // 임시 카드 배열을 위한 변수를 0으로 초기화

		// 플레이어의 i번째 카드의 무늬와 r번째 카드의 무늬가 같으면
		// r번째 무늬에 해당하는 배열의 j번째에 카드를 저장한다.
		for (i = 0; i < pdealcnt; i++)
		{
			if (players[player][i].shp == shps[r]) temp[r][j++] = players[player][i];
			if (j == 5) return 5000 + temp[r][0].shp;
		}
	}

	return 0;
}

// 스트레이트인지 판단하는 함수
// 숫자가 5장 연속 인 경우
int straight(int player)
{
	int i, j;
	if (pdealcnt < 5) return 0;

	// 카드의 번호에 의해 정렬한다.
	for (i = 0; i < pdealcnt - 1; i++)
		for (j = 0; j < pdealcnt - i - 1; j++)
			if (players[player][i].num < players[player][i + 1].num)
				swap(&players[player][i], &players[player][i + 1]);

	// 정렬된 카드의 번호에 따라 1씩 증가하는 5장의 카드가 있는지 체크 한다.
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

// 세장의 숫자가 같은지를 체크하는 함수
int threeofakind(int player)
{
	int i, j = 0, cnt = 1;
	// 스트레이트를 검사하면서 이미 정렬이 되어 있음
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

// 투페어를 검사하는 함수
// 두장의 카드가 같은 2쌍이 있는 경우
int twopair(int player)
{
	int i, cnt = 0, num[2] = { 0 }, shp[2] = { 0 };

	for (i = 0; i < pdealcnt - 1; i++)// 이미 내림 차순으로 정렬이 되어 있음
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

// 원페어인 경우
int onepair(int player)
{
	int i;
	// 이미 숫자를 기준으로 내림차순 정렬이 되어 있음
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
	case CLOV:printf("♧"); break;
	case DIA:printf("◈"); break;
	case HEART:printf("♡"); break;
	default:printf("♤"); break;
	};
}