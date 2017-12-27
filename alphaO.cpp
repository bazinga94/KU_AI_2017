#include <stdio.h>
#include <conio.h>          //_getch�Լ� �������
#include <Windows.h>	    //gotoxy�Լ� �������

#define W_SIZE 19
#define H_SIZE 19    // 19X19 ũ�� ������
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32

int chk = -1;                                                           // ��/�� �� üũ�� ���� 1�̸� ���� ��, -1�̸� AI�� 
int winchk = 0;                                                         // winchk = 1�϶� ���� �¸�, -1�϶� AI�� �¸�
char *board[H_SIZE][W_SIZE] = {};                                       // ���񺸵��� ���� �迭


void create_board();                                                    // ���񺸵� �����Է� �������������������ۡ�
void draw_board();                                                      // ���񺸵� �׸���
void cursor(int stone[H_SIZE][W_SIZE], char key, int *x, int *y);    	// Ŀ���̵� + space�� �Է½� ���� �Լ�
void draw_stone();                                                      // ���� ��ǥ�� ��/�� ���� �׸��� �Լ�
void put_stone(int stone[H_SIZE][W_SIZE], int x, int y);                // stone�迭�� 1,-1 ���� �����ϰ� ���� �ѱ�� �Լ�
void create_stone(int stone[H_SIZE][W_SIZE]);                           // space �� �Է½� ���� ���񺸵忡 �׸��� �����ϴ� �Լ�
void erase_stone(int x, int y);                                         // ���� ������ �ڸ��� �ٽ� ���񺸵�� �ٲٴ� �Լ�
void gotoxy(int x, int y);                                              // Ŀ���̵��Լ�(�ۿ���)
void winner_check(int stone[H_SIZE][W_SIZE]);					    	// �¸� üũ �Լ�

void ai_put_stone(int stone[H_SIZE][W_SIZE], int Max_x, int Max_y);     // ai�� stone�迭�� �浹 -1���� ����
int ai_calculate(int stone[H_SIZE][W_SIZE]);                            // evaluation function
void ai_test(int stone[H_SIZE][W_SIZE], int *Max_x, int *Max_y);        // ai�� �� ���� ��ġ�� ã�� �Լ�

int main(void) {

	int stone[H_SIZE][W_SIZE] = { 0 };                     // ���� ���� �ʾ����� ��� ���� = 0, �� = 1, ������ = -1
	int x = 18;
	int y = 9;                                             //x�� y��ǥ
	char key;
	int Max_x = 9;                                         // ��ġȭ ���� ���� Ŭ���� stone[Max_x][Max_y]��
	int Max_y = 9;


	create_board();                            // ���񺸵带 ����                   
	draw_board();                              // ���񺸵� �׸���

	gotoxy(x, y);                              // ��ǥ�� ������ (18,9) ���� �̵�
	draw_stone();                              // chk���� '-1'�̹Ƿ� �浹 ���� ���� (AI�� ���� ����)

	while (winchk == 0) {

		key = _getch();                        // key���� Ű���� �Է°��� ����

		erase_stone(x, y);                     // ������ ���񺸵��� �� ����� (space�ٸ� ������ �ʾ�����) -> ���� ��ǥ�� ���񺸵� �ٽ� �׸���

		cursor(stone, key, &x, &y);            // Ű���� �Է� ���� ���� x,y��ǥ�� ��ȯ + space �Է½� stone �迭�� ���� ���� ����(1 ����)

		winner_check(stone);                   // �̱� ����� �ִ��� üũ���ִ� �Լ�

		ai_test(stone, &Max_x, &Max_y);        // ������ ���񺸵忡 �� ���� �ΰ� ��ġȭ��Ų ������ ���� ���� ��ǥ�� ����

		ai_put_stone(stone, Max_x, Max_y);     // ai�� ������ x,y ��ǥ�� -1 �� ����

		create_stone(stone);                   // ��ü ���񺸵带 �����ϰ� stone�� -1,1�� �κп� ���� �׸����� ����

		gotoxy(x, y);                          // �ٲ� x,y��ǥ���� ���� Ŀ�� �̵�!!

		winner_check(stone);                   // �̱� ����� �ִ��� üũ���ִ� �Լ�

		draw_stone();                          // �ٲ� x,y��ǥ���� �� ���� -> ������ ��ǥ�� ���� �׸���! 

	}
	getchar();
	system("pause");
	return 0;

}

void ai_test(int stone[H_SIZE][W_SIZE], int *Max_x, int *Max_y) {
	int i;
	int k;
	int AI;                                            // ���񺸵带 ��ġȭ ��Ų ��
	int temp = 0;                                      // ��ġȭ�� ���񺸵��� ���¸� ��� �����ϴ� ����


	if (chk == -1) {                                   // AI�� �����϶�
		for (i = 0; i <= 18; i++) {
			for (k = 0; k <= 18; k++) {
				if (stone[i][k] == 0) {
					stone[i][k] = -1;                      // stone�Լ��� ���� 0(���� ���� ����)�϶� ������ ���� ���� 

					AI = ai_calculate(stone);              // �� ��� �󸶳� ������ �� ���� ��ġ������ ��ȯ

					if (temp < AI) {                   // temp���� ������ ��ġ�� ����! , ������ ��ġ������ ���� �� �϶�

						temp = AI;
						*Max_x = i;                    // Max_x Max_y�� ��ǥ�� ����!
						*Max_y = k;
					}
					stone[i][k] = 0;                   // stone�Լ����� �� ����
				}
			}
		}
	}
}

int ai_calculate(int stone[H_SIZE][W_SIZE]) {

	int i;
	int k;

	int score = 0;                                                          // ai�� �����¸� ��ġȭ ���� ���� ��
	int total_score = 0;                                                    // ai�� ���񺸵���¸� ��ġȭ ���� ���� ��

	for (i = 0; i <= 18; i++) {
		for (k = 0; k <= 18; k++) {
			/////////����/////////
			if (stone[i][k - 2] == -1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == -1) {
				score = 1000;
				total_score = total_score + score;
			}  //ooooo �¸���
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 50;
				total_score = total_score + score;
			}  //oooo 4�� ����
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 1) {
				score = 40;
				total_score = total_score + score;
			}  //oooox 4�� ���� ����
			else if (stone[i][k - 2] == 1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == -1) {
				score = 40;
				total_score = total_score + score;
			}  //xoooo 4�� ���� ����
			else if (stone[i][k - 2] == 0 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 6;
				total_score = total_score + score;
			}  //ooo 3�� ����
			else if (stone[i][k - 2] == 0 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 1) {
				score = 4;
				total_score = total_score + score;
			}  //ooox 3�� ���� ����
			else if (stone[i][k - 2] == 1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 4;
				total_score = total_score + score;
			}  //xooo 3�� ���� ����
			else if (stone[i][k - 1] == 0 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 2;
				total_score = total_score + score;
			}  //oo 2�� ����
			else if (stone[i][k - 1] == 0 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 1) {
				score = 1;
				total_score = total_score + score;
			}  //oox 2�� ���� ����
			else if (stone[i][k - 1] == 1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == 0) {
				score = 1;
				total_score = total_score + score;
			}  //xoo 2�� ���� ����

			   /////////����/////////
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == -1) {
				score = 1000;
				total_score = total_score + score;
			}  //ooooo �¸���
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 50;
				total_score = total_score + score;
			}  //oooo 4�� ����
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 1) {
				score = 40;
				total_score = total_score + score;
			}  //oooox 4�� ���� ����
			else if (stone[i - 2][k] == 1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == -1) {
				score = 40;
				total_score = total_score + score;
			}  //xoooo 4�� ���� ����
			else if (stone[i - 2][k] == 0 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 6;
				total_score = total_score + score;
			}  //ooo 3�� ����
			else if (stone[i - 2][k] == 0 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 1) {
				score = 4;
				total_score = total_score + score;
			}  //ooox 3�� ���� ����
			else if (stone[i - 2][k] == 1 && stone[i - 1][k] == -1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 4;
				total_score = total_score + score;
			}  //xooo 3�� ���� ����
			else if (stone[i - 1][k] == 0 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 2;
				total_score = total_score + score;
			}  //oo 2�� ����
			else if (stone[i - 1][k] == 0 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 1) {
				score = 1;
				total_score = total_score + score;
			}  //oox 2�� ���� ����
			else if (stone[i - 1][k] == 1 && stone[i][k] == -1 && stone[i + 1][k] == -1 && stone[i + 2][k] == 0) {
				score = 1;
				total_score = total_score + score;
			}  //xoo 2�� ���� ����

			   /////////�밢��/////////
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == -1) {
				score = 1000;
				total_score = total_score + score;
			}  //ooooo �¸���
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 50;
				total_score = total_score + score;
			}  //oooo 4�� ����
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 1) {
				score = 40;
				total_score = total_score + score;
			}  //oooox 4�� ���� ����
			else if (stone[i - 2][k + 2] == 1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == -1) {
				score = 40;
				total_score = total_score + score;
			}  //xoooo 4�� ���� ����
			else if (stone[i - 2][k + 2] == 0 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 7;
				total_score = total_score + score;
			}  //ooo 3�� ����
			else if (stone[i - 2][k + 2] == 0 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 1) {
				score = 5;
				total_score = total_score + score;
			}  //ooox 3�� ���� ����
			else if (stone[i - 2][k + 2] == 1 && stone[i - 1][k + 1] == -1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 5;
				total_score = total_score + score;
			}  //xooo 3�� ���� ����
			else if (stone[i - 1][k + 1] == 0 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 3;
				total_score = total_score + score;
			}  //oo 2�� ����
			else if (stone[i - 1][k + 1] == 0 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 1) {
				score = 1;
				total_score = total_score + score;
			}  //oox 2�� ���� ����
			else if (stone[i - 1][k + 1] == 1 && stone[i][k] == -1 && stone[i + 1][k - 1] == -1 && stone[i + 2][k - 2] == 0) {
				score = 1;
				total_score = total_score + score;
			}  //xoo 2�� ���� ����

			   /////////�ݴ� �밢��/////////
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == -1) {
				score = 1000;
				total_score = total_score + score;
			}  //ooooo �¸���
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 50;
				total_score = total_score + score;
			}  //oooo 4�� ����
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 1) {
				score = 40;
				total_score = total_score + score;
			}  //oooox 4�� ���� ����
			else if (stone[i - 2][k - 2] == 1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == -1) {
				score = 40;
				total_score = total_score + score;
			}  //xoooo 4�� ���� ����
			else if (stone[i - 2][k - 2] == 0 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 7;
				total_score = total_score + score;
			}  //ooo 3�� ����
			else if (stone[i - 2][k - 2] == 0 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 1) {
				score = 5;
				total_score = total_score + score;
			}  //ooox 3�� ���� ����
			else if (stone[i - 2][k - 2] == 1 && stone[i - 1][k - 1] == -1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 5;
				total_score = total_score + score;
			}  //xooo 3�� ���� ����
			else if (stone[i - 1][k - 1] == 0 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 3;
				total_score = total_score + score;
			}  //oo 2�� ����
			else if (stone[i - 1][k - 1] == 0 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 1) {
				score = 1;
				total_score = total_score + score;
			}  //oox 2�� ���� ����
			else if (stone[i - 1][k - 1] == 1 && stone[i][k] == -1 && stone[i + 1][k + 1] == -1 && stone[i + 2][k + 2] == 0) {
				score = 1;
				total_score = total_score + score;
			}  //xoo 2�� ���� ����

			   /////////����!!/////////
			else if (stone[i][k - 2] == 1 && stone[i][k - 1] == 1 && stone[i][k] == 1 && stone[i][k + 1] == 1 && stone[i][k + 2] == -1) {
				score = 100;
				total_score = total_score + score;
			}  //oooox ���� �����
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == 1 && stone[i][k] == 1 && stone[i][k + 1] == 1 && stone[i][k + 2] == 1) {
				score = 100;
				total_score = total_score + score;
			}  //xoooo ���� �����
			else if (stone[i - 2][k] == 1 && stone[i - 1][k] == 1 && stone[i][k] == 1 && stone[i + 1][k] == 1 && stone[i + 2][k] == -1) {
				score = 100;
				total_score = total_score + score;
			}  //oooox ���� �����
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == 1 && stone[i][k] == 1 && stone[i + 1][k] == 1 && stone[i + 2][k] == 1) {
				score = 100;
				total_score = total_score + score;
			}  //xoooo ���� �����
			else if (stone[i - 2][k - 2] == 1 && stone[i - 1][k - 1] == 1 && stone[i][k] == 1 && stone[i + 1][k + 1] == 1 && stone[i + 2][k + 2] == -1) {
				score = 100;
				total_score = total_score + score;
			}  //oooox �ݴ� �밢�� �����
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == 1 && stone[i][k] == 1 && stone[i + 1][k + 1] == 1 && stone[i + 2][k + 2] == 1) {
				score = 100;
				total_score = total_score + score;
			}  //xoooo �ݴ� �밢�� �����
			else if (stone[i - 2][k + 2] == 1 && stone[i - 1][k + 1] == 1 && stone[i][k] == 1 && stone[i + 1][k - 1] == 1 && stone[i + 2][k - 2] == -1) {
				score = 100;
				total_score = total_score + score;
			}  //oooox �밢�� �����
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == 1 && stone[i][k] == 1 && stone[i + 1][k - 1] == 1 && stone[i + 2][k - 2] == 1) {
				score = 100;
				total_score = total_score + score;
			}  //xoooo �밢�� �����

			else if (stone[i][k - 1] == 1 && stone[i][k] == 1 && stone[i][k + 1] == 1 && stone[i][k + 2] == -1) {
				if (stone[i][k - 2] == -1) {
					score = 3;        //������ �����ִµ� ���� �Ϸ��� ���
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //ooox ���� �����
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == 1 && stone[i][k] == 1 && stone[i][k + 1] == 1) {
				if (stone[i][k + 2] == -1) {
					score = 3;        //������ �����ִµ� ���� �Ϸ��� ���
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //xooo ���� �����
			else if (stone[i - 1][k] == 1 && stone[i][k] == 1 && stone[i + 1][k] == 1 && stone[i + 2][k] == -1) {
				if (stone[i - 2][k] == -1) {
					score = 3;        //������ �����ִµ� ���� �Ϸ��� ���
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //ooox ���� �����
			else if (stone[i - 2][k] == -1 && stone[i - 1][k] == 1 && stone[i][k] == 1 && stone[i + 1][k] == 1) {
				if (stone[i + 2][k] == -1) {
					score = 3;        //������ �����ִµ� ���� �Ϸ��� ���
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //xooo ���� �����
			else if (stone[i - 1][k + 1] == 1 && stone[i][k] == 1 && stone[i + 1][k - 1] == 1 && stone[i + 2][k - 2] == -1) {
				if (stone[i - 2][k + 2] == -1) {
					score = 3;        //������ �����ִµ� ���� �Ϸ��� ���
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //ooox �밢�� �����
			else if (stone[i - 2][k + 2] == -1 && stone[i - 1][k + 1] == 1 && stone[i][k] == 1 && stone[i + 1][k - 1] == 1) {
				if (stone[i + 2][k - 2] == -1) {
					score = 3;        //������ �����ִµ� ���� �Ϸ��� ���
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //xooo �밢�� �����
			else if (stone[i - 1][k - 1] == 1 && stone[i][k] == 1 && stone[i + 1][k + 1] == 1 && stone[i + 2][k + 2] == -1) {
				if (stone[i - 2][k - 2] == -1) {
					score = 3;        //������ �����ִµ� ���� �Ϸ��� ���
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //ooox �ݴ� �밢�� �����
			else if (stone[i - 2][k - 2] == -1 && stone[i - 1][k - 1] == 1 && stone[i][k] == 1 && stone[i + 1][k + 1] == 1) {
				if (stone[i + 2][k + 2] == -1) {
					score = 3;        //������ �����ִµ� ���� �Ϸ��� ���
				}
				else {
					score = 30;
				}
				total_score = total_score + score;
			}  //xooo �ݴ� �밢�� �����

		}
	}
	return total_score;
}

void ai_put_stone(int stone[H_SIZE][W_SIZE], int Max_x, int Max_y) {

	if (winchk == 0) {                                   //�¸��� ���� ������ �ȵ� ���� �϶�
		if (stone[Max_x][Max_y] == 0)                    //���� �η��� �ϴ� ��ǥ�� '0'(�� ���� ����)���� Ȯ��
			if (chk == -1) {                             //chk=-1, ai �����϶�
				stone[Max_x][Max_y] = -1;                //��ǥ���� -1����

				chk = 1;                                 //���ʸ� ������ �ѱ��.
			}
	}
}

void cursor(int stone[H_SIZE][W_SIZE], char key, int *x, int *y) {

	switch (key) {                                     // ������ ����Ű�� �Է½� ��ǥ���� �̵�(x��ǥ�� 2ĭ�� �̵����־�� �Ѵ�)
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
		put_stone(stone, *x, *y);                // �����̽��� �Է½� 
		break;
	}

}


void put_stone(int stone[H_SIZE][W_SIZE], int x, int y) {

	if (stone[y][x / 2] == 0)                    //���� �η��� �ϴ� ��ǥ�� '0'(�� ���� ����)���� Ȯ��
		if (chk == 1) {                          //chk=1, �鵹�����϶�
			stone[y][x / 2] = 1;                 //��ǥ���� 1����

			chk = -1;                            //�浹���ʷ� �ѱ�

		}
	/*
	else {
	stone[y][x / 2] = -1;                //chk=1�� �ƴҶ�, �浹����

	chk = 1;                             //�鵹���ʷ� �ѱ� (2�ο��϶�)

	}
	*/
}


void draw_stone() {


	if (chk == 1 && winchk == 0) {							// �������� chk �� 1�̸� �鵹 ����, �� winchk�� 0 �϶���!!(��/�а� �������� �ʾ��� ��)
		printf("��");                                       //���� ����������, �ܼ�â���� �ݴ�� ���δ�...
		gotoxy(14, 19);
		printf("-�� ����-");
	}

	else if (chk == -1 && winchk == 0) {                    // �������� chk �� -1�̸� �浹 ����
		printf("��");
		gotoxy(14, 19);
		printf("-AI ����-");
	}
}

void erase_stone(int x, int y) {

	gotoxy(x, y);


	if (x == 0 && y == 0)                                           //���� �����ε� ��ü ���񺸵带 �˻��ؼ� ���� �����.
		printf("��");
	else if (x == 36 && y == 0)
		printf("��");
	else if (x == 0 && y == 18)
		printf("��");
	else if (x == 36 && y == 18)
		printf("��");
	else if (y == 0)
		printf("��");
	else if (y == 18)
		printf("��");
	else if (x == 0)
		printf("��");
	else if (x == 36)
		printf("��");
	else
		printf("��");


}


void create_stone(int stone[H_SIZE][W_SIZE]) {

	for (int i = 0; i <= 18; i++) {
		for (int j = 0; j <= 18; j++) {
			if (stone[j][i] == 1) {
				gotoxy(i * 2, j);
				printf("��");

			}
			else if (stone[j][i] == -1) {
				gotoxy(i * 2, j);
				printf("��");

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
				printf("���� �¸�!\n");
				
			}
			else if (stone[i][k - 2] == -1 && stone[i][k - 1] == -1 && stone[i][k] == -1 && stone[i][k + 1] == -1 && stone[i][k + 2] == -1) {
				winchk = -1;
				gotoxy(14, 19);
				printf("AI�� �¸�!\n");
				
			}
			if (stone[k - 2][i] == 1 && stone[k - 1][i] == 1 && stone[k][i] == 1 && stone[k + 1][i] == 1 && stone[k + 2][i] == 1) {
				winchk = 1;
				gotoxy(14, 19);
				printf("���� �¸�!\n");
				
			}
			else if (stone[k - 2][i] == -1 && stone[k - 1][i] == -1 && stone[k][i] == -1 && stone[k + 1][i] == -1 && stone[k + 2][i] == -1) {
				winchk = -1;
				gotoxy(14, 19);
				printf("AI�� �¸�!\n");
				
			}
		}
	}


	for (i = 2; i <= 16; i++) {
		for (k = 2; k <= 16; k++) {
			if (stone[k - 2][i - 2] == 1 && stone[k - 1][i - 1] == 1 && stone[k][i] == 1 && stone[k + 1][i + 1] == 1 && stone[k + 2][i + 2] == 1) {
				winchk = 1;
				gotoxy(14, 19);
				printf("���� �¸�!\n");
				
			}
			else if (stone[k - 2][i - 2] == -1 && stone[k - 1][i - 1] == -1 && stone[k][i] == -1 && stone[k + 1][i + 1] == -1 && stone[k + 2][i + 2] == -1) {
				winchk = -1;
				gotoxy(14, 19);
				printf("AI�� �¸�!\n");
				
			}
			if (stone[k - 2][i + 2] == 1 && stone[k - 1][i + 1] == 1 && stone[k][i] == 1 && stone[k + 1][i - 1] == 1 && stone[k + 2][i - 2] == 1) {
				winchk = 1;
				gotoxy(14, 19);
				printf("���� �¸�!\n");
				
			}
			else if (stone[k - 2][i + 2] == -1 && stone[k - 1][i + 1] == -1 && stone[k][i] == -1 && stone[k + 1][i - 1] == -1 && stone[k + 2][i - 2] == -1) {
				winchk = -1;
				gotoxy(14, 19);
				printf("AI�� �¸�!\n");
			
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

	board[0][0] = "��";

	for (int i = 1; i < W_SIZE - 1; i++) {
		board[0][i] = "��";
	}
	board[0][18] = "��";


	for (int i = 1; i < W_SIZE - 1; i++) {
		board[i][0] = "��";

		for (int j = 1; j < W_SIZE - 1; j++) {
			board[i][j] = "��";
		}
		board[i][18] = "��";
	}

	board[18][0] = "��";

	for (int i = 1; i < W_SIZE - 1; i++) {
		board[18][i] = "��";
	}
	board[18][18] = "��";

}

void gotoxy(int x, int y) {							// Ŀ���� ��ġ�� �����̴� �Լ�
	COORD Pos;										
	Pos.X = x;										// Pos.X�� �̵��� x��ǥ�� Pos.Y�� �̵��� y��ǥ
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}