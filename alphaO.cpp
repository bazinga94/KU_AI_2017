#include <stdio.h>
#include <conio.h>          //_getch함수 헤더파일
#include <Windows.h>	    //gotoxy함수 헤더파일

#define W_SIZE 19
#define H_SIZE 19    // 19X19 크기 오목판
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32

int chk = -1;                                                           // 백/흑 돌 체크용 변수 1이면 내가 선, -1이면 AI선 
int winchk = 0;                                                         // winchk = 1일때 나의 승리, -1일때 AI의 승리
char *board[H_SIZE][W_SIZE] = {};                                       // 오목보드의 성분 배열


void create_board();                                                    // 오목보드 성분입력 ┠┨┼┗┛┷┏┓┯○●
void draw_board();                                                      // 오목보드 그리기
void cursor(int stone[H_SIZE][W_SIZE], char key, int *x, int *y);    	// 커서이동 + space바 입력시 실행 함수
void draw_stone();                                                      // 현재 좌표에 백/흑 돌을 그리는 함수
void put_stone(int stone[H_SIZE][W_SIZE], int x, int y);                // stone배열에 1,-1 값을 저장하고 턴을 넘기는 함수
void create_stone(int stone[H_SIZE][W_SIZE]);                           // space 바 입력시 돌을 오목보드에 그리고 저장하는 함수
void erase_stone(int x, int y);                                         // 돌이 지나간 자리를 다시 오목보드로 바꾸는 함수
void gotoxy(int x, int y);                                              // 커서이동함수(퍼왔음)
void winner_check(int stone[H_SIZE][W_SIZE]);					    	// 승리 체크 함수

void ai_put_stone(int stone[H_SIZE][W_SIZE], int Max_x, int Max_y);     // ai가 stone배열에 흑돌 -1값을 저장
int ai_calculate(int stone[H_SIZE][W_SIZE]);                            // evaluation function
void ai_test(int stone[H_SIZE][W_SIZE], int *Max_x, int *Max_y);        // ai가 둘 돌의 위치를 찾는 함수

int main(void) {

	int stone[H_SIZE][W_SIZE] = { 0 };                     // 돌을 두지 않았을때 모든 변수 = 0, 흰돌 = 1, 검은돌 = -1
	int x = 18;
	int y = 9;                                             //x와 y좌표
	char key;
	int Max_x = 9;                                         // 수치화 값이 가장 클때의 stone[Max_x][Max_y]값
	int Max_y = 9;


	create_board();                            // 오목보드를 구성                   
	draw_board();                              // 오목보드 그리기

	gotoxy(x, y);                              // 좌표를 시작점 (18,9) 으로 이동
	draw_stone();                              // chk값이 '-1'이므로 흑돌 먼저 생성 (AI가 먼저 시작)

	while (winchk == 0) {

		key = _getch();                        // key값에 키보드 입력값을 저장

		erase_stone(x, y);                     // 지나온 오목보드의 돌 지우기 (space바를 누르지 않았을때) -> 현재 좌표에 오목보드 다시 그리기

		cursor(stone, key, &x, &y);            // 키보드 입력 값에 따라 x,y좌표값 변환 + space 입력시 stone 배열에 돌의 상태 저장(1 저장)

		winner_check(stone);                   // 이긴 사람이 있는지 체크해주는 함수

		ai_test(stone, &Max_x, &Max_y);        // 임의의 오목보드에 흑 돌을 두고 수치화시킨 점수가 가장 높은 좌표값 저장

		ai_put_stone(stone, Max_x, Max_y);     // ai가 선택한 x,y 좌표에 -1 값 저장

		create_stone(stone);                   // 전체 오목보드를 조사하고 stone의 -1,1인 부분에 돌을 그린다음 저장

		gotoxy(x, y);                          // 바뀐 x,y좌표값에 따라 커서 이동!!

		winner_check(stone);                   // 이긴 사람이 있는지 체크해주는 함수

		draw_stone();                          // 바뀐 x,y좌표값에 돌 생성 -> 현재의 좌표에 돌을 그리기! 

	}
	getchar();
	system("pause");
	return 0;

}

void ai_test(int stone[H_SIZE][W_SIZE], int *Max_x, int *Max_y) {
	int i;
	int k;
	int AI;                                            // 오목보드를 수치화 시킨 값
	int temp = 0;                                      // 수치화된 오목보드의 상태를 잠시 저장하는 변수


	if (chk == -1) {                                   // AI의 차례일때
		for (i = 0; i <= 18; i++) {
			for (k = 0; k <= 18; k++) {
				if (stone[i][k] == 0) {
					stone[i][k] = -1;                      // stone함수의 값이 0(돌이 없는 상태)일때 임의의 돌을 삽입 

					AI = ai_calculate(stone);              // 그 결과 얼마나 유리한 수 인지 수치값으로 반환

					if (temp < AI) {                   // temp에는 이전의 수치값 저장! , 이전의 수치값보다 좋은 수 일때

						temp = AI;
						*Max_x = i;                    // Max_x Max_y에 좌표값 저장!
						*Max_y = k;
					}
					stone[i][k] = 0;                   // stone함수에서 돌 제거
				}
			}
		}
	}
}

int ai_calculate(int stone[H_SIZE][W_SIZE]) {

	int i;
	int k;

	int score = 0;                                                          // ai가 돌상태를 수치화 했을 때의 값
	int total_score = 0;                                                    // ai가 오목보드상태를 수치화 했을 때의 값

	for (i = 0; i <= 18; i++) {
		for (k = 0; k <= 18; k++) {
			/////////가로/////////
			if (stone[i][k - 2] == -1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == -1) {
				score = 1000;
				total_score = total_score + score;
			}  //ooooo 승리시
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 50;
				total_score = total_score + score;
			}  //oooo 4개 연속
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 1) {
				score = 40;
				total_score = total_score + score;
			}  //oooox 4개 연속 막힘
			else if (stone[i][k - 2] == 1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == -1) {
				score = 40;
				total_score = total_score + score;
			}  //xoooo 4개 연속 막힘
			else if (stone[i][k - 2] == 0 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 6;
				total_score = total_score + score;
			}  //ooo 3개 연속
			else if (stone[i][k - 2] == 0 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 1) {
				score = 4;
				total_score = total_score + score;
			}  //ooox 3개 연속 막힘
			else if (stone[i][k - 2] == 1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 4;
				total_score = total_score + score;
			}  //xooo 3개 연속 막힘
			else if (stone[i][k - 1] == 0 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 2;
				total_score = total_score + score;
			}  //oo 2개 연속
			else if (stone[i][k - 1] == 0 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 1) {
				score = 1;
				total_score = total_score + score;
			}  //oox 2개 연속 막힘
			else if (stone[i][k - 1] == 1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 1;
				total_score = total_score + score;
			}  //xoo 2개 연속 막힘

			   /////////세로/////////
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == -1) {
				score = 1000;
				total_score = total_score + score;
			}  //ooooo 승리시
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 50;
				total_score = total_score + score;
			}  //oooo 4개 연속
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 1) {
				score = 40;
				total_score = total_score + score;
			}  //oooox 4개 연속 막힘
			else if (stone[i - 2][k] == 1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == -1) {
				score = 40;
				total_score = total_score + score;
			}  //xoooo 4개 연속 막힘
			else if (stone[i - 2][k] == 0 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 6;
				total_score = total_score + score;
			}  //ooo 3개 연속
			else if (stone[i - 2][k] == 0 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 1) {
				score = 4;
				total_score = total_score + score;
			}  //ooox 3개 연속 막힘
			else if (stone[i - 2][k] == 1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 4;
				total_score = total_score + score;
			}  //xooo 3개 연속 막힘
			else if (stone[i - 1][k] == 0 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 2;
				total_score = total_score + score;
			}  //oo 2개 연속
			else if (stone[i - 1][k] == 0 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 1) {
				score = 1;
				total_score = total_score + score;
			}  //oox 2개 연속 막힘
			else if (stone[i - 1][k] == 1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 1;
				total_score = total_score + score;
			}  //xoo 2개 연속 막힘

			   /////////대각선/////////
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == -1) {
				score = 1000;
				total_score = total_score + score;
			}  //ooooo 승리시
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 50;
				total_score = total_score + score;
			}  //oooo 4개 연속
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 1) {
				score = 40;
				total_score = total_score + score;
			}  //oooox 4개 연속 막힘
			else if (stone[i - 2][k + 2] == 1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == -1) {
				score = 40;
				total_score = total_score + score;
			}  //xoooo 4개 연속 막힘
			else if (stone[i - 2][k + 2] == 0 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 7;
				total_score = total_score + score;
			}  //ooo 3개 연속
			else if (stone[i - 2][k + 2] == 0 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 1) {
				score = 5;
				total_score = total_score + score;
			}  //ooox 3개 연속 막힘
			else if (stone[i - 2][k + 2] == 1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 5;
				total_score = total_score + score;
			}  //xooo 3개 연속 막힘
			else if (stone[i - 1][k + 1] == 0 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 3;
				total_score = total_score + score;
			}  //oo 2개 연속
			else if (stone[i - 1][k + 1] == 0 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 1) {
				score = 1;
				total_score = total_score + score;
			}  //oox 2개 연속 막힘
			else if (stone[i - 1][k + 1] == 1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 1;
				total_score = total_score + score;
			}  //xoo 2개 연속 막힘

			   /////////반대 대각선/////////
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == -1) {
				score = 1000;
				total_score = total_score + score;
			}  //ooooo 승리시
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 50;
				total_score = total_score + score;
			}  //oooo 4개 연속
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 1) {
				score = 40;
				total_score = total_score + score;
			}  //oooox 4개 연속 막힘
			else if (stone[i - 2][k - 2] == 1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == -1) {
				score = 40;
				total_score = total_score + score;
			}  //xoooo 4개 연속 막힘
			else if (stone[i - 2][k - 2] == 0 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 7;
				total_score = total_score + score;
			}  //ooo 3개 연속
			else if (stone[i - 2][k - 2] == 0 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 1) {
				score = 5;
				total_score = total_score + score;
			}  //ooox 3개 연속 막힘
			else if (stone[i - 2][k - 2] == 1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 5;
				total_score = total_score + score;
			}  //xooo 3개 연속 막힘
			else if (stone[i - 1][k - 1] == 0 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 3;
				total_score = total_score + score;
			}  //oo 2개 연속
			else if (stone[i - 1][k - 1] == 0 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 1) {
				score = 1;
				total_score = total_score + score;
			}  //oox 2개 연속 막힘
			else if (stone[i - 1][k - 1] == 1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 1;
				total_score = total_score + score;
			}  //xoo 2개 연속 막힘

			   /////////수비!!/////////
			else if (stone[i][k - 2] == 1 && stone[i][k - 1] == 1 && stone[i][k] == 1 && stone[i][k + 1] == 1 && stone[i][k + 2] == -1) {
				score = 100;
				total_score = total_score + score;
			}  //oooox 가로 수비시
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == 1 && stone[i][k] == 1 && stone[i][k + 1] == 1 && stone[i][k + 2] == 1) {
				score = 100;
				total_score = total_score + score;
			}  //xoooo 가로 수비시
			else if (stone[i - 2][k] == 1 && stone[i - 1][k] == 1 && stone[i][k] == 1 && stone[i + 1][k] == 1 && stone[i + 2][k] == -1) {
				score = 100;
				total_score = total_score + score;
			}  //oooox 세로 수비시
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == 1 && stone[i][k] == 1 && stone[i + 1][k] == 1 && stone[i + 2][k] == 1) {
				score = 100;
				total_score = total_score + score;
			}  //xoooo 세로 수비시
			else if (stone[i - 2][k - 2] == 1 && stone[i - 1][k - 1] == 1 && stone[i][k] == 1 && stone[i + 1][k + 1] == 1 && stone[i + 2][k + 2] == -1) {
				score = 100;
				total_score = total_score + score;
			}  //oooox 반대 대각선 수비시
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == 1 && stone[i][k] == 1 && stone[i + 1][k + 1] == 1 && stone[i + 2][k + 2] == 1) {
				score = 100;
				total_score = total_score + score;
			}  //xoooo 반대 대각선 수비시
			else if (stone[i - 2][k + 2] == 1 && stone[i - 1][k + 1] == 1 && stone[i][k] == 1 && stone[i + 1][k - 1] == 1 && stone[i + 2][k - 2] == -1) {
				score = 100;
				total_score = total_score + score;
			}  //oooox 대각선 수비시
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == 1 && stone[i][k] == 1 && stone[i + 1][k - 1] == 1 && stone[i + 2][k - 2] == 1) {
				score = 100;
				total_score = total_score + score;
			}  //xoooo 대각선 수비시

			else if (stone[i][k - 1] == 1 && stone[i][k] == 1 && stone[i][k + 1] == 1 && stone[i][k + 2] == -1) {
				if (stone[i][k - 2] == -1) {
					score = 3;        //한쪽이 막혀있는데 수비 하려는 경우
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //ooox 가로 수비시
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == 1 && stone[i][k] == 1 && stone[i][k + 1] == 1) {
				if (stone[i][k + 2] == -1) {
					score = 3;        //한쪽이 막혀있는데 수비 하려는 경우
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //xooo 가로 수비시
			else if (stone[i - 1][k] == 1 && stone[i][k] == 1 && stone[i + 1][k] == 1 && stone[i + 2][k] == -1) {
				if (stone[i - 2][k] == -1) {
					score = 3;        //한쪽이 막혀있는데 수비 하려는 경우
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //ooox 세로 수비시
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == 1 && stone[i][k] == 1 && stone[i + 1][k] == 1) {
				if (stone[i + 2][k] == -1) {
					score = 3;        //한쪽이 막혀있는데 수비 하려는 경우
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //xooo 세로 수비시
			else if (stone[i - 1][k + 1] == 1 && stone[i][k] == 1 && stone[i + 1][k - 1] == 1 && stone[i + 2][k - 2] == -1) {
				if (stone[i - 2][k + 2] == -1) {
					score = 3;        //한쪽이 막혀있는데 수비 하려는 경우
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //ooox 대각선 수비시
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == 1 && stone[i][k] == 1 && stone[i + 1][k - 1] == 1) {
				if (stone[i + 2][k - 2] == -1) {
					score = 3;        //한쪽이 막혀있는데 수비 하려는 경우
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //xooo 대각선 수비시
			else if (stone[i - 1][k - 1] == 1 && stone[i][k] == 1 && stone[i + 1][k + 1] == 1 && stone[i + 2][k + 2] == -1) {
				if (stone[i - 2][k - 2] == -1) {
					score = 3;        //한쪽이 막혀있는데 수비 하려는 경우
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //ooox 반대 대각선 수비시
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == 1 && stone[i][k] == 1 && stone[i + 1][k + 1] == 1) {
				if (stone[i + 2][k + 2] == -1) {
					score = 3;        //한쪽이 막혀있는데 수비 하려는 경우
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //xooo 반대 대각선 수비시

		}
	}
	return total_score;
}

void ai_put_stone(int stone[H_SIZE][W_SIZE], int Max_x, int Max_y) {

	if (winchk == 0) {                                   //승리가 아직 선언이 안된 상태 일때
		if (stone[Max_x][Max_y] == 0)                    //돌을 두려고 하는 좌표가 '0'(돌 없는 상태)인지 확인
			if (chk == -1) {                             //chk=-1, ai 차례일때
				stone[Max_x][Max_y] = -1;                //좌표값에 -1선언

				chk = 1;                                 //차례를 나한테 넘긴다.
			}
	}
}

void cursor(int stone[H_SIZE][W_SIZE], char key, int *x, int *y) {

	switch (key) {                                     // 각각의 방향키를 입력시 좌표값을 이동(x좌표는 2칸씩 이동해주어야 한다)
	case UP:
		if (*y < 1)
			break;
		else
			*y = *y - 1;
		break;
	case DOWN:
		if (*y >= 18)
			break;
		else
			*y = *y + 1;
		break;
	case LEFT:
		if (*x < 2)
			break;
		else
			*x = *x - 2;
		break;
	case RIGHT:
		if (*x >= 36)
			break;
		else
			*x = *x + 2;
		break;
	case SPACE:
		put_stone(stone, *x, *y);                // 스페이스바 입력시 
		break;
	}

}


void put_stone(int stone[H_SIZE][W_SIZE], int x, int y) {

	if (stone[y][x / 2] == 0)                    //돌을 두려고 하는 좌표가 '0'(돌 없는 상태)인지 확인
		if (chk == 1) {                          //chk=1, 백돌차례일때
			stone[y][x / 2] = 1;                 //좌표값에 1선언

			chk = -1;                            //흑돌차례로 넘김

		}
	/*
	else {
	stone[y][x / 2] = -1;                //chk=1이 아닐때, 흑돌차례

	chk = 1;                             //백돌차례로 넘김 (2인용일때)

	}
	*/
}


void draw_stone() {


	if (chk == 1 && winchk == 0) {							// 전역변수 chk 가 1이면 백돌 차례, 단 winchk가 0 일때만!!(승/패가 결정되지 않았을 때)
		printf("●");                                       //백이 먼저이지만, 콘솔창에서 반대로 보인다...
		gotoxy(14, 19);
		printf("-내 차례-");
	}

	else if (chk == -1 && winchk == 0) {                    // 전역변수 chk 가 -1이면 흑돌 차례
		printf("○");
		gotoxy(14, 19);
		printf("-AI 차례-");
	}
}

void erase_stone(int x, int y) {

	gotoxy(x, y);


	if (x == 0 && y == 0)                                           //돌을 움직인뒤 전체 오목보드를 검사해서 돌을 지운다.
		printf("┏");
	else if (x == 36 && y == 0)
		printf("┓");
	else if (x == 0 && y == 18)
		printf("┗");
	else if (x == 36 && y == 18)
		printf("┛");
	else if (y == 0)
		printf("┯");
	else if (y == 18)
		printf("┷");
	else if (x == 0)
		printf("┠");
	else if (x == 36)
		printf("┨");
	else
		printf("┼");


}


void create_stone(int stone[H_SIZE][W_SIZE]) {

	for (int i = 0; i <= 18; i++) {
		for (int j = 0; j <= 18; j++) {
			if (stone[j][i] == 1) {
				gotoxy(i * 2, j);
				printf("●");

			}
			else if (stone[j][i] == -1) {
				gotoxy(i * 2, j);
				printf("○");

			}
		}
	}

}

void winner_check(int stone[H_SIZE][W_SIZE]) {


	int i;
	int k;

	for (i = 0; i <= 18; i++) {
		for (k = 2; k <= 16; k++) {
			if (stone[i][k - 2] == 1 && stone[i][k - 1] == 1 && stone[i][k] == 1 && stone[i][k + 1] == 1 && stone[i][k + 2] == 1) {
				winchk = 1;
				gotoxy(14, 19);
				printf("나의 승리!\n");
				
			}
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == -1) {
				winchk = -1;
				gotoxy(14, 19);
				printf("AI의 승리!\n");
				
			}
			if (stone[k - 2][i] == 1 && stone[k - 1][i] == 1 && stone[k][i] == 1 && stone[k + 1][i] == 1 && stone[k + 2][i] == 1) {
				winchk = 1;
				gotoxy(14, 19);
				printf("나의 승리!\n");
				
			}
			else if (stone[k - 2][i] == -1 && stone[k - 1][i] == -1 && stone[k][i] == -1 && stone[k + 1][i] == -1 && stone[k + 2][i] == -1) {
				winchk = -1;
				gotoxy(14, 19);
				printf("AI의 승리!\n");
				
			}
		}
	}


	for (i = 2; i <= 16; i++) {
		for (k = 2; k <= 16; k++) {
			if (stone[k - 2][i - 2] == 1 && stone[k - 1][i - 1] == 1 && stone[k][i] == 1 && stone[k + 1][i + 1] == 1 && stone[k + 2][i + 2] == 1) {
				winchk = 1;
				gotoxy(14, 19);
				printf("나의 승리!\n");
				
			}
			else if (stone[k - 2][i - 2] == -1 && stone[k - 1][i - 1] == -1 && stone[k][i] == -1 && stone[k + 1][i + 1] == -1 && stone[k + 2][i + 2] == -1) {
				winchk = -1;
				gotoxy(14, 19);
				printf("AI의 승리!\n");
				
			}
			if (stone[k - 2][i + 2] == 1 && stone[k - 1][i + 1] == 1 && stone[k][i] == 1 && stone[k + 1][i - 1] == 1 && stone[k + 2][i - 2] == 1) {
				winchk = 1;
				gotoxy(14, 19);
				printf("나의 승리!\n");
				
			}
			else if (stone[k - 2][i + 2] == -1 && stone[k - 1][i + 1] == -1 && stone[k][i] == -1 && stone[k + 1][i - 1] == -1 && stone[k + 2][i - 2] == -1) {
				winchk = -1;
				gotoxy(14, 19);
				printf("AI의 승리!\n");
			
			}
		}
	}
}


void draw_board() {


	printf("%s", board[0][0]);
	for (int i = 1; i < W_SIZE - 1; i++) {


		printf("%s", board[0][i]);

	}

	printf("%s", board[0][18]);
	printf("\n");

	for (int i = 1; i < W_SIZE - 1; i++) {

		printf("%s", board[i][0]);

		for (int j = 1; j < W_SIZE - 1; j++) {

			printf("%s", board[i][j]);

		}

		printf("%s", board[i][18]);
		printf("\n");
	}


	printf("%s", board[18][0]);
	for (int i = 1; i < W_SIZE - 1; i++) {

		printf("%s", board[18][i]);

	}

	printf("%s", board[18][18]);

	printf("\n");

}


void create_board() {

	board[0][0] = "┏";

	for (int i = 1; i < W_SIZE - 1; i++) {
		board[0][i] = "┯";
	}
	board[0][18] = "┓";


	for (int i = 1; i < W_SIZE - 1; i++) {
		board[i][0] = "┠";

		for (int j = 1; j < W_SIZE - 1; j++) {
			board[i][j] = "┼";
		}
		board[i][18] = "┨";
	}

	board[18][0] = "┗";

	for (int i = 1; i < W_SIZE - 1; i++) {
		board[18][i] = "┷";
	}
	board[18][18] = "┛";

}

void gotoxy(int x, int y) {							// 커서의 위치를 움직이는 함수
	COORD Pos;										
	Pos.X = x;										// Pos.X가 이동할 x좌표고 Pos.Y가 이동할 y좌표
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}