#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

#define FIELD_WIDTH (30)
#define FIELD_HEIGHT (16)
#define BOMB_COUNT (100)
typedef struct {
	bool bomb;
	bool mine;
	bool flag;
	bool wrong;
}CELL;
CELL cells[FIELD_HEIGHT][FIELD_WIDTH];

int cursorX, cursorY;

int getAdjacentBombsCount(int _x, int _y) {
	int n = 0;
	for (int y = -1; y <= 1; y++)
		for (int x = -1; x <= 1; x++) {
			if ((x == 0) && (y == 0))
				continue;
			int x2 = _x + x;
			int y2 = _y + y;

			if (x2 < 0 || x2 >= FIELD_WIDTH || y2 < 0 || y2 >= FIELD_HEIGHT)
				continue;
			if (cells[y2][x2].bomb)
				n++;
		}

	return n;
}

void autoEraseMines(int _x, int _y) {
	if ((cells[_y][_x].bomb)
		|| !(cells[_y][_x].mine)
		|| _x < 0
		|| _x >= FIELD_WIDTH
		|| _y < 0
		|| _y >= FIELD_HEIGHT
		|| getAdjacentBombsCount(_x, _y) > 0)
		return;
	cells[_y][_x].mine = false;

	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			if ((x == 0) && (y == 0))
				continue;
			int x2 = _x + x;
			int y2 = _y + y;
			autoEraseMines(x2, y2);
		}
	}
}


int main() {
	srand((unsigned int)time(NULL));
	for (int y = 0;y < FIELD_HEIGHT; y++)
		for (int x = 0; x < FIELD_WIDTH;x++) {
			cells[y][x].mine  =  true;
			cells[y][x].bomb  =  false;
			cells[y][x].flag  =  false;
			cells[y][x].wrong =  false;
		}
	int n = 0;
	do {
		int x = rand() % FIELD_WIDTH;
		int y = rand() % FIELD_HEIGHT;
		if (!cells[y][x].bomb) {
			cells[y][x].bomb = true;
			n++;
		}
	} while (n < BOMB_COUNT);

	

	bool explosion = false;
	bool clear = false;
	while (1) {
		system("cls");
		for (int y = 0;y < FIELD_HEIGHT; y++) {
			for (int x = 0; x < FIELD_WIDTH;x++)
				if (x == cursorX
					&& y == cursorY)
				{
					if (!explosion)
						printf("");
					else
						printf("¦");
				}
				else if (cells[y][x].flag)
					printf("¥");
				else if (cells[y][x].mine)
					printf("¡");
				else if (cells[y][x].bomb)
					printf("œ");
				else if (cells[y][x].wrong)
					printf("~");
				else {
					int n = getAdjacentBombsCount(x, y);
					if (n == 0) {
						printf("E");
						
					}
					else {
						char str[] = "‚O";
						str[1] += n;
						printf("%s", str);
					}
				}
			printf("\n");
		}



		if (explosion) {
			printf("GAME OVER");
			exit(0);
			break;
		}
		if (clear) {
			printf("CLEAR!!");
			exit(0);
			break;
		}
		switch (_getch()) {
		case'w': if (cursorY > 0)				cursorY--; break;
		case's': if (cursorY < FIELD_HEIGHT - 1)cursorY++; break;
		case'a': if (cursorX > 0)               cursorX--; break;
		case'd': if (cursorX < FIELD_WIDTH - 1) cursorX++; break;
		case'f':cells[cursorY][cursorX].flag = !cells[cursorY][cursorX].flag; ; break;
		case' ':
			cells[cursorY][cursorX].mine = false;
			if (cells[cursorY][cursorX].bomb) {
				explosion = true;
				
				printf("\a");
				for (int y = 0; y < FIELD_HEIGHT; y++) {
					for (int x = 0; x < FIELD_WIDTH; x++) {
						bool temp = cells[y][x].flag;
						cells[y][x].mine =
							cells[y][x].flag = false;
						if (temp != cells[y][x].bomb)
							cells[y][x].wrong = true;
					}
				}printf("\n");
				break;
			}
			{
				clear = true;
				for (int y = 0;y < FIELD_HEIGHT; y++)
					for (int x = 0; x < FIELD_WIDTH;x++)
						if (
							(!cells[y][x].bomb)
							&& (cells[y][x].mine)
							) {
							clear = false;
						}
				if (clear) {
					printf("\a");
				
				}
			}
			for (int y = -1; y <= 1; y++)
				for (int x = -1; x <= 1; x++) {
					if ((x == 0) && (y == 0))
						continue;
					int x2 = cursorX + x;
					int y2 = cursorY + y;
					autoEraseMines(x2, y2);
				}
			break;
		}
	}
}