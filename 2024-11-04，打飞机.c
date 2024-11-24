#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#define N 500

// 以左上角为原点，向右为x轴正方向，向下为y轴正方向

int enx[N] = { 0 };  // 敌机的横坐标
int eny[N] = { 0 };
int ebx[N] = { 0 };  //  敌机的子弹的横坐标
int eby[N] = { 0 };  
int px = 13;  // 玩家的横坐标
int py = 20;
int pbx[100] = { 0 };  // 玩家的子弹的横坐标
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

// 刷新敌机
int reflesh() {
	static int times = 0;
	double difficulty = 120;
	difficulty = (difficulty * pow(0.5, t / 60) < 0.9) ? 0.9 : difficulty * pow(0.5, t / 60); //半衰期60秒
	times++;
	if (times > difficulty) {
		times = 0;
		//填充新机
		for (int k = 0; k < N; k++) {
			if (enx[k] == 0) {
				enx[k] = 100;
				break;
			}
		}
	}
	
	//分配位置
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

// 敌机移动
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
	// 发射子弹
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

// 敌机子弹移动
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

// 玩家移动
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
		printf("\r    暂停中……按y继续游戏");
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

// 我方子弹移动
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

// 显示
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
			// 打印敌机
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
			//打印敌机的子弹
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
			//打印玩家的飞机
			if (px == i && py == j) {
				printf("A");
				continue;
			}
			//打印玩家的子弹
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
			//打印空格
			printf(" ");			
		}
		if (j == 18) {
			printf("  剩余敌人：%.f     ", remain_enemy);
		}
		if (j == 20) {
			printf("  杀敌数量：%.f     ", kill);
		}
		if (j == 22) {
			printf("  存活时间：%.f     ", t);
		}
		printf("                                         \n");
	}
	printf("                                                     \n");
	printf("                                                       ");
	return 0;
}

// 隐藏光标并移动光标到左上角顶格
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
			printf("\n    真可惜呀！差点就赢了呢。            \n");
			if (kill > most_kill) {
				most_kill = kill;
				printf("    新记录！最高杀敌数量：%.f       \n", most_kill);
			}
			if (t > most_live) {
				most_live = t;
				printf("    新记录！最长存活时间：%.f秒      \n", most_live);
			}
			break;
		}
		if (remain_enemy < 1) {
			total_kill += kill;
			total_live += t;
			printf("    怎么可以开挂呀？你好坏！           \n");
			if (kill > most_kill) {
				most_kill = kill;
				printf("    新记录！最高杀敌数量：%.f     \n", most_kill);
			}
			if (t > most_live) {
				most_live = t;
				printf("    新记录！最长存活时间：%.f秒    \n", most_live);
			}
			break;;
		}
	
	}
	game_num++;
	return 0;
}

// 加载数据
int load_data() {
	FILE* file = fopen("打飞机.yjy.txt", "r");
	if (file == NULL) {
		file = fopen("打飞机.yjy.txt", "w");
		fprintf(file, "%f %f %f %f %f", most_kill, total_kill, most_live, total_live, game_num);
		fclose(file);
		file = fopen("打飞机.txt", "r");
	}
	fscanf(file, "%lf %lf %lf %lf %lf", &most_kill, &total_kill, &most_live, &total_live, &game_num);
	fclose(file);
	return 0;
}

// 保存数据
int save_data() {
	FILE* file = fopen("打飞机.yjy.txt", "w");
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
	printf("    ** 打飞机 **\n");
	printf("    ************\n");
	
	while (1) {
		printf("\n");
		printf("    请切换成英文输入法                      \n");
		printf("    wasd移动，空格射击，y暂停               \n");
		printf("    按k开始游戏                             \n");
		printf("    =====================       \n");
		printf("    游戏总场次：  %.f           \n", game_num);
		printf("    最高杀敌数量：%.f           \n", most_kill);
		printf("    最长存活时间：%.f           \n", most_live);
		printf("    累计杀敌数量：%.f           \n", total_kill);		
		printf("    累计存活时间：%.f           \n", total_live);
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