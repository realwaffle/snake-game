// Name: Fardad Hajirostami

#include "mylib.h"
#include "text.h"
#include "ball.h"
#include "apple.h"
#include "arrowr.h"
#include "arrowd.h"
#include "arrowl.h"
#include "arrowu.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "snake.h"
#include "gameover.h"
#include "ballred.h"

#define BUTTONS       *(volatile unsigned short*) 0x4000130
#define BUTTON_A      (1 << 0)
#define BUTTON_B      (1 << 1)
#define BUTTON_START  (1 << 2)
#define BUTTON_SELECT (1 << 3)
#define BUTTON_RIGHT  (1 << 4)
#define BUTTON_LEFT   (1 << 5)
#define BUTTON_UP     (1 << 6)
#define BUTTON_DOWN   (1 << 7)
#define BUTTON_R      (1 << 8)
#define BUTTON_L      (1 << 9)
#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

// State enum definition

typedef struct {
	int x;
	int y;
} OBJECT;

enum GBAState{
	GAMEOVER,
	WELCOME,
	START,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

// SNAKE DATA STRUCTURE: DOUBLY LINKED LIST //

struct Node{
	int x;
	int y;
	struct Node *next;
	struct Node *prev;
};

struct Node *head;
struct Node *tail;

// Prototypes
void removeTail();
void clear();
int exists(OBJECT player);
void addToHead(int x, int y);
void updateSnake(OBJECT player);
int isFood(OBJECT player);
void updateApple();

enum GBAState state;
OBJECT blueball;
OBJECT food;
int dir = 0;
int hit;
int appleOk;
int foodBool;
void delay(int n);
int delayint = 3;
int counter = 0;
int score = 0;
int main() {
	enum GBAState state = WELCOME;
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	char pointStr[1];
	while(1) {
		counter++;
		waitForVblank();
		switch(state) {
		case WELCOME:
			clear();
			fillScreen(BLACK);
			drawImage3(0, 0, 240, 160, snake);
			drawRect(130, 0, 240, 10, BLACK);
			sprintf(pointStr, "PRESS [ENTER] TO BEGIN");
			drawString(130,60,pointStr,WHITE);
			while (1) {
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
					break;
				}
			}
			break;
		case GAMEOVER:
			clear();
			fillScreen(BLACK);
			drawImage3(0, 0, 240, 160, gameover);
			sprintf(pointStr, "SCORE: %d", score);
			drawString(150,95,pointStr,WHITE);
			while(1) {
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					score = 0;
					state = START;
					break;
				}
			}
			break;
		case START:
			clear();
			fillScreen(BLACK);
			blueball.x = 60;
			blueball.y = 60;
			addToHead(blueball.x, blueball.y);
			drawImage3(blueball.y, blueball.x, 20, 20, ballred);
			blueball.x += 20;
			addToHead(blueball.x, blueball.y);
			drawImage3(blueball.y, blueball.x, 20, 20, ball);
			blueball.x += 20;
			addToHead(blueball.x, blueball.y);
			drawImage3(blueball.y, blueball.x, 20, 20, ball);
			blueball.x += 20;
			addToHead(blueball.x, blueball.y);
			drawImage3(blueball.y, blueball.x, 20, 20, arrowr);
			srand(counter);
			appleOk = 0;
			while (appleOk == 0) {
				food.y = (rand() % 8) * 20;
				food.x = (rand() % 12) * 20;
				if (exists(food) == 0){
					appleOk = 1;
					drawImage3(food.y, food.x, 20, 20, apple);
				}
			}
			sprintf(pointStr, "SCORE: %d", score);
			drawString(5,180,pointStr,WHITE);
			while (1) {
				if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
					state = RIGHT;
					break;
				}
				if (KEY_DOWN_NOW(BUTTON_UP)) {
					state = UP;
					break;
				}
				if (KEY_DOWN_NOW(BUTTON_DOWN)) {
					state = DOWN;
					break;
				}
			}
			break;
		case LEFT:
			while (1) {
				if (blueball.x <= 0) {
					state = GAMEOVER;
					break;
				}
				blueball.x -= 20;
				hit = exists(blueball);
				if (hit) {
					int isTail = 0;
					if (tail -> x == blueball.x) {
						if (tail -> y == blueball.y) {
							isTail = 1;
						}
					}
					if (isTail == 0) {
						state = GAMEOVER;
						break;
					}
				}
				foodBool = isFood(blueball);
				if (foodBool == 1) {
					score++;
					drawRect(4, 222, 13, 9, BLACK);
					addToHead(blueball.x, blueball.y);
					drawImage3(blueball.y, blueball.x + 20, 20, 20, ball);
					drawImage3(blueball.y, blueball.x, 20, 20, arrowl);
					updateApple();
					sprintf(pointStr, "SCORE: %d", score);
					drawString(5,180,pointStr,WHITE);
					delay(delayint);
				} else {
					int xRemove = tail -> x;
					int yRemove = tail -> y;
					updateSnake(blueball);
					drawRect(yRemove, xRemove, 20, 20, BLACK);
					drawImage3(tail -> y, tail -> x, 20, 20, ballred);
					drawImage3(blueball.y, blueball.x + 20, 20, 20, ball);
					drawImage3(blueball.y, blueball.x, 20, 20, arrowl);
					sprintf(pointStr, "SCORE: %d", score);
					drawString(5,180,pointStr,WHITE);
					delay(delayint);
				}
				if (dir == 2) {
					state = LEFT;
					break;
				}
				if (dir == 3) {
					state = UP;
					break;
				}
				if (dir == 4) {
					state = DOWN;
					break;
				}
			}
			break;
		case RIGHT:
			while (1) {
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
					break;
				}
				if (blueball.x >= 220) {
					state = GAMEOVER;
					break;
				}
				blueball.x += 20;
				hit = exists(blueball);
				if (hit) {
					int isTail = 0;
					if (tail -> x == blueball.x) {
						if (tail -> y == blueball.y) {
							isTail = 1;
						}
					}
					if (isTail == 0) {
						state = GAMEOVER;
						break;
					}
				}
				foodBool = isFood(blueball);
				if (foodBool == 1) {
					score++;
					drawRect(4, 222, 13, 9, BLACK);
					addToHead(blueball.x, blueball.y);
					drawImage3(blueball.y, blueball.x - 20, 20, 20, ball);
					drawImage3(blueball.y, blueball.x, 20, 20, arrowr);
					updateApple();
					sprintf(pointStr, "SCORE: %d", score);
					drawString(5,180,pointStr,WHITE);
					delay(delayint);
				} else {
					int xRemove = tail -> x;
					int yRemove = tail -> y;
					updateSnake(blueball);
					drawRect(yRemove, xRemove, 20, 20, BLACK);
					drawImage3(tail -> y, tail -> x, 20, 20, ballred);
					drawImage3(blueball.y, blueball.x - 20, 20, 20, ball);
					drawImage3(blueball.y, blueball.x, 20, 20, arrowr);
					sprintf(pointStr, "SCORE: %d", score);
					drawString(5,180,pointStr,WHITE);
					delay(delayint);
				}
				if (dir == 1) {
					state = RIGHT;
					break;
				}
				if (dir == 3) {
					state = UP;
					break;
				}
				if (dir == 4) {
					state = DOWN;
					break;
				}
			}
			break;
		case UP:
			while (1) {
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
					break;
				}
				if (blueball.y <= 0) {
					state = GAMEOVER;
					break;
				}
				blueball.y -= 20;
				hit = exists(blueball);
				if (hit) {
					int isTail = 0;
					if (tail -> x == blueball.x) {
						if (tail -> y == blueball.y) {
							isTail = 1;
						}
					}
					if (isTail == 0) {
						state = GAMEOVER;
						break;
					}
				}
				foodBool = isFood(blueball);
				if (foodBool == 1) {
					score++;
					drawRect(4, 222, 13, 9, BLACK);
					addToHead(blueball.x, blueball.y);
					drawImage3(blueball.y + 20, blueball.x, 20, 20, ball);
					drawImage3(blueball.y, blueball.x, 20, 20, arrowu);
					updateApple();
					sprintf(pointStr, "SCORE: %d", score);
					drawString(5,180,pointStr,WHITE);
					delay(delayint);
				} else {
					int xRemove = tail -> x;
					int yRemove = tail -> y;
					updateSnake(blueball);
					drawRect(yRemove, xRemove, 20, 20, BLACK);
					drawImage3(tail -> y, tail -> x, 20, 20, ballred);
					drawImage3(blueball.y + 20, blueball.x, 20, 20, ball);
					drawImage3(blueball.y, blueball.x, 20, 20, arrowu);
					sprintf(pointStr, "SCORE: %d", score);
					drawString(5,180,pointStr,WHITE);
					delay(delayint);
				}
				if (dir == 1) {
					state = LEFT;
					break;
				}
				if (dir == 2) {
					state = RIGHT;
					break;
				}
				if (dir == 4) {
					state = UP;
					break;
				}
			}
			break;
		case DOWN:
			while (1) {
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
					break;
				}
				if (blueball.y >= 140) {
					state = GAMEOVER;
					break;
				}
				blueball.y += 20;
				hit = exists(blueball);
				if (hit) {
					int isTail = 0;
					if (tail -> x == blueball.x) {
						if (tail -> y == blueball.y) {
							isTail = 1;
						}
					}
					if (isTail == 0) {
						state = GAMEOVER;
						break;
					}
				}
				foodBool = isFood(blueball);
				if (foodBool == 1) {
					score++;
					drawRect(4, 222, 13, 9, BLACK);
					sprintf(pointStr, "SCORE: %d", score);
					drawString(5,180,pointStr,WHITE);
					addToHead(blueball.x, blueball.y);
					drawImage3(blueball.y - 20, blueball.x, 20, 20, ball);
					drawImage3(blueball.y, blueball.x, 20, 20, arrowd);
					updateApple();
					delay(delayint);
				} else {
					int xRemove = tail -> x;
					int yRemove = tail -> y;
					updateSnake(blueball);
					drawRect(yRemove, xRemove, 20, 20, BLACK);
					drawImage3(tail -> y, tail -> x, 20, 20, ballred);
					drawImage3(blueball.y - 20, blueball.x, 20, 20, ball);
					drawImage3(blueball.y, blueball.x, 20, 20, arrowd);
					sprintf(pointStr, "SCORE: %d", score);
					drawString(5,180,pointStr,WHITE);
					delay(delayint);
				}
				if (dir == 1) {
					state = LEFT;
					break;
				}
				if (dir == 2) {
					state = RIGHT;
					break;
				}
				if (dir == 3) {
					state = DOWN;
					break;
				}
			}
			break;
		}
	}
	return 0;
}

void delay(int n) {
	volatile int x = 0;
	for (int i = 0; i < n * 10000; i++) {
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			dir = 0;
			score = 0;
			state = START;
			break;
		}
		if (KEY_DOWN_NOW(BUTTON_LEFT)) {
			dir = 1;
		}
		if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
			dir = 2;
		}
		if (KEY_DOWN_NOW(BUTTON_UP)) {
			dir = 3;
		}
		if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			dir = 4;
		}
		x++;
	}
}

void addToHead(int x, int y) {
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	if (head == NULL) {
		temp -> x = x;
		temp -> y = y;
		head = temp;
		tail = temp;
		return;
	}
	if (head -> next == NULL) {
		temp -> x = x;
		temp -> y = y;
		head -> prev = temp;
		temp -> next = head;
		head = temp;
		tail = head -> next;
		tail -> prev = head;
		head -> next = tail;
		return;
	}
	temp -> x = x;
	temp -> y = y;
	head -> prev = temp;
	temp -> next = head;
	head = temp;
	return;
}


void removeTail() {
	if (head -> next == NULL) {
		head = NULL;
		return;
	}
	if (tail -> prev == head) {
		head -> next = NULL;
		tail = head;
		return;
	}
	tail -> prev -> next = NULL;
	tail = tail -> prev;
	return;
}
void updateSnake(OBJECT player) {
	addToHead(player.x, player.y);
	removeTail();
	return;
}

void clear() {
	head = NULL;
	tail = NULL;
	head -> next = NULL;
	tail -> prev = NULL;
	return;
}

int exists(OBJECT player) {
	struct Node *curr = (struct Node*)malloc(sizeof(struct Node));
	curr = head;
	while (curr != NULL) {
		if (curr -> x == player.x) {
			if (curr -> y == player.y) {
				return 1;
			}
		}
		curr = curr -> next;
	}
	return 0;
}

int isFood(OBJECT player) {
	if (player.x == food.x) {
		if (player.y == food.y) {
				return 1;
		}
	}
	return 0;
}

void updateApple() {
	srand(counter);
	appleOk = 0;
	while (appleOk == 0) {
		food.y = (rand() % 8) * 20;
		food.x = (rand() % 12) * 20;
		if (exists(food) == 0){
			appleOk = 1;
			drawImage3(food.y, food.x, 20, 20, apple);
		}
	}
	return;
}