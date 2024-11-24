#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#define N 500

// �����Ͻ�Ϊԭ�㣬����Ϊx������������Ϊy��������

int enx[N] = { 0 };  // �л��ĺ�����
int eny[N] = { 0 };
int ebx[N] = { 0 };  //  �л����ӵ��ĺ�����
int eby[N] = { 0 };  
int px = 13;  // ��ҵĺ�����
int py = 20;
int pbx[100] = { 0 };  // ��ҵ��ӵ��ĺ�����
int pby[100] = { 0 };
int game_over = 0;
double t0 = 0;
double t = 0;
double remain_enemy = 1000;
double kill = 0;
double most_kill = 0;
double total_kill = 0;
double most_live = 0;
double total_live = 0;
double game_num = 0;

// ˢ�µл�
int reflesh() {
	static int times = 0;
	double difficulty = 120;
	difficulty = (difficulty * pow(0.5, t / 60) < 0.9) ? 0.9 : difficulty * pow(0.5, t / 60); //��˥��60��
	times++;
	if (times > difficulty) {
		times = 0;
		//����»�
		for (int k = 0; k < N; k++) {
			if (enx[k] == 0) {
				enx[k] = 100;
				break;
			}
		}
	}
	
	//����λ��
	for (int k = 0; k < N; k++) {
		
		if (enx[k] == 100) {
			int x = 0;
			int cn = 0;
			while (1) {
				x = rand() % 25 + 1;
				int c = 0;
				for (int i = 0; i < N; i++) {
					if (enx[i] == x && eny[i] == 0) {
						c = 1;
						break;
					}
				}
				if (c == 0) {
					enx[k] = x;
				}
				break;
			}
			
		}
	}
	return 0;	
}

// �л��ƶ�
int enemy_move() {
	static int times = 0;
	times++;
	if (times == 60) {
		times = 0;
		for (int k = 0; k < N; k++) {
			if (enx[k]) {
				eny[k]++;
				if (enx[k] == px && eny[k] == py) {
					game_over = 1;
				}
				if (eny[k] == 26) {
					enx[k] = 0;
					eny[k] = 0;
					remain_enemy--;
				}
			}
		}
	}
	// �����ӵ�
	static int times2 = 0;
	times2++;
	if (times2 == 240) {
		times2 = 0;
		for (int k = 0; k < N; k++) {
			if (enx[k]) {
				for (int b = 0; b < N; b++) {
					if (ebx[b] == 0) {
						ebx[b] = enx[k];
						eby[b] = eny[k];
						break;
					}
				}
			}
		}
	}
	return 0;
}

// �л��ӵ��ƶ�
int enemy_bullet() {
	static times = 0;
	times++;
	if (times == 20) {
		times = 0;
		for (int k = 0; k < N; k++) {
			if (ebx[k]) {
				eby[k]++;
				if (ebx[k] == px && eby[k] == py) {
					game_over = 1;
				}
				if (eby[k] == 26) {
					ebx[k] = 0;
					eby[k] = 0;
				}
			}
		}
	}
	return 0;
}

// ����ƶ�
int player_move() {
	
	char ch = 0;
	if (kbhit() == 0) {
		return 0;
	}
	ch = getch();
	
	if (ch == 'w' && py > 1) {
		py--;
	}
	if (ch == 'a' && px > 1) {
		px--;
	}
	if (ch == 's' && py < 25) {
		py++;
	}
	if (ch == 'd' && px < 25) {
		px++;
	}
	if (ch == ' ') {
		for (int i = 0; i < 100; i++) {
			if (pbx[i] == 0) {
				pbx[i] = px;
				pby[i] = py;
				break;
			}
		}
	}
	if (ch == 'y') {
		printf("\r    ��ͣ�С�����y������Ϸ");
		while (1) {
			if (kbhit() && getch() == 'y') {
				break;
			}
		}
	}
	for (int k = 0; k < N; k++) {
		if (px == enx[k] && py == eny[k] || px == ebx[k] && py == eby[k]) {
			game_over = 1;
		}
	}
	return 0;
}

// �ҷ��ӵ��ƶ�
int player_bullet() {
	for (int i = 0; i < 100; i++) {
		if (pbx[i]) {
			pby[i]--;
			for (int k = 0; k < N; k++) {
				if (pbx[i] == enx[k] && (pby[i] + 1 == eny[k])) {
					pbx[i] = 0;
					pby[i] = 0;
					enx[k] = 0;
					eny[k] = 0;
					kill++;
					remain_enemy--;
				}
			}
			if (pby[i] == 0) {
				pbx[i] = 0;
				pby[i] = 0;
			}
		}
	}
	return 0;
}

// ��ʾ
int show() {
	for (int j = 0; j < 27; j++) {
		printf("    ");
		for (int i = 0; i < 27; i++) {
			if (j == 0 || j == 26) {
				printf("=");
				continue;
			}
			else if (i == 0 || i == 26) {
				printf("|");
				continue;
			}
			// ��ӡ�л�
			int c = 0;
			for (int k = 0; k < N; k++) {				
				if (enx[k] == i && eny[k] == j) {
					printf("O");
					c = 1;
					break;
				}		
			}
			if (c) {
				continue;
			}
			//��ӡ�л����ӵ�
			for (int k = 0; k < N; k++) {
				if (ebx[k] == i && eby[k] == j) {
					printf("*");
					c = 1;
					break;
				}
			}
			if (c) {
				continue;
			}
			//��ӡ��ҵķɻ�
			if (px == i && py == j) {
				printf("A");
				continue;
			}
			//��ӡ��ҵ��ӵ�
			for (int k = 0; k < 100; k++) {
				if (pbx[k] == i && pby[k] == j) {
					printf("+");
					c = 1;
					break;
				}
			}
			if (c) {
				continue;
			}
			//��ӡ�ո�
			printf(" ");			
		}
		if (j == 18) {
			printf("  ʣ����ˣ�%.f     ", remain_enemy);
		}
		if (j == 20) {
			printf("  ɱ��������%.f     ", kill);
		}
		if (j == 22) {
			printf("  ���ʱ�䣺%.f     ", t);
		}
		printf("                                         \n");
	}
	printf("                                                     \n");
	printf("                                                       ");
	return 0;
}

// ���ع�겢�ƶ���굽���ϽǶ���
int cursor() {
	CONSOLE_CURSOR_INFO ci = { 1,0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
	COORD p = { 0,0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	return 0;
}

int game() {

	game_over = 0;
	for (int i = 0; i < N; i++) {
		enx[i] = 0;
		eny[i] = 0;
		ebx[i] = 0;
		eby[i] = 0;
	}
	for (int i = 0; i < 100; i++) {
		pbx[i] = 0;
		pby[i] = 0;
	}
	px = 13;
	py = 20;
	t0 = time(NULL);
	kill = 0;
	remain_enemy = 1000;

	while (1) {
		
		t = time(NULL) - t0;

		reflesh();	
		enemy_move();		
		enemy_bullet();		
		player_move();	
		player_bullet();
		cursor();	
		show();

		if (game_over) {
			total_kill += kill;
			total_live += t;
			printf("\n    ���ϧѽ������Ӯ���ء�            \n");
			if (kill > most_kill) {
				most_kill = kill;
				printf("    �¼�¼�����ɱ��������%.f       \n", most_kill);
			}
			if (t > most_live) {
				most_live = t;
				printf("    �¼�¼������ʱ�䣺%.f��      \n", most_live);
			}
			break;
		}
		if (remain_enemy < 1) {
			total_kill += kill;
			total_live += t;
			printf("    ��ô���Կ���ѽ����û���           \n");
			if (kill > most_kill) {
				most_kill = kill;
				printf("    �¼�¼�����ɱ��������%.f     \n", most_kill);
			}
			if (t > most_live) {
				most_live = t;
				printf("    �¼�¼������ʱ�䣺%.f��    \n", most_live);
			}
			break;;
		}
	
	}
	game_num++;
	return 0;
}

// ��������
int load_data() {
	FILE* file = fopen("��ɻ�.yjy.txt", "r");
	if (file == NULL) {
		file = fopen("��ɻ�.yjy.txt", "w");
		fprintf(file, "%f %f %f %f %f", most_kill, total_kill, most_live, total_live, game_num);
		fclose(file);
		file = fopen("��ɻ�.txt", "r");
	}
	fscanf(file, "%lf %lf %lf %lf %lf", &most_kill, &total_kill, &most_live, &total_live, &game_num);
	fclose(file);
	return 0;
}

// ��������
int save_data() {
	FILE* file = fopen("��ɻ�.yjy.txt", "w");
	fprintf(file, "%f %f %f %f %f", most_kill, total_kill, most_live, total_live, game_num);
	fclose(file);
	return 0;
}

int main()
{	
	

	char ch = 0;
	srand(time(NULL));
	
	load_data();  

	
	printf("    ************\n");
	printf("    ** ��ɻ� **\n");
	printf("    ************\n");
	
	while (1) {
		printf("\n");
		printf("    ���л���Ӣ�����뷨                      \n");
		printf("    wasd�ƶ����ո������y��ͣ               \n");
		printf("    ��k��ʼ��Ϸ                             \n");
		printf("    =====================       \n");
		printf("    ��Ϸ�ܳ��Σ�  %.f           \n", game_num);
		printf("    ���ɱ��������%.f           \n", most_kill);
		printf("    ����ʱ�䣺%.f           \n", most_live);
		printf("    �ۼ�ɱ��������%.f           \n", total_kill);		
		printf("    �ۼƴ��ʱ�䣺%.f           \n", total_live);
		printf("    =====================       \n");
		while (1) {
			ch = getch();
			if (ch == 'k') {
				game();
				save_data();
				break;
			}
		}
	}

	return 0;
}