#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

//**************
clock_t startTime, currentTime;
int elapsedTime;
char direction;
char prevdirection;
#define period 130
#define width 25
#define height 18

//**************
typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Snake {
    Point pos[width * height];
    Point food;
    int size;
} Snake;

//**************
Point point;
Snake snake;

//**************
void welcome();
void init_snake(); // Change the function name to init_snake
void set_food();   // Change the function name to set_food
void snake_move(char direction);
void set_border();
int check_collision();
int check_wasd();

//主代碼
int main() {
    //初始化
    welcome();
    init_snake(); // Change the function name to init_snake
    set_food();   // Change the function name to set_food

    // 初始方向
    direction = ' ';
    prevdirection = ' ';

    // 初始化時間
    startTime = clock();

    while (1) {
        // 獲取當前時間
        currentTime = clock();
        elapsedTime = (currentTime - startTime) / CLOCKS_PER_SEC;

        set_border();

        // 目前遊戲時間
        printf("遊戲時間: %d 秒\n", elapsedTime);

        if (_kbhit()) {
            prevdirection = direction;
            direction = _getch();
        }

        snake_move(direction);

        printf("您目前的長度:%d\n", snake.size);

        //螢幕刷新率調整
        Sleep(period);

        if (check_collision() && check_wasd()) {
            printf("\n您輸了!\n");
            printf("您的遊戲時間:%d秒\n", elapsedTime);
            printf("您最終的長度:%d\n", snake.size);
            printf("\n");
            main();
        }
    }
    return 0;
}

//**************
// 遊戲開始畫面
void welcome() {
    printf("歡迎來到貪吃蛇!");
    printf("\n請使用WASD輸入法進行操作!");
    printf("\n按任意鍵繼續......");
    _getch(); // 等待玩家輸入
}

//初始化蛇位置
void init_snake() {
    //頭
    snake.pos[0].x = width / 2;
    snake.pos[0].y = height / 2;
    //蛇的初始化長度
    snake.size = 1;

    //初始化食物位置
    set_food(); // Change the function name to set_food
}

// 設置食物
void set_food() {
    srand((unsigned)time(NULL));
    while (1) {
        snake.food.x = rand() % (width - 2) + 1;
        snake.food.y = rand() % (height - 2) + 1;

        int collision = 0; // 座標相等檢查

        for (int i = 0; i < snake.size; i++) {
            if (snake.food.x == snake.pos[i].x && snake.food.y == snake.pos[i].y) {
                // 食物座標等於蛇身座標.
                collision = 1;
            }
            else if (snake.food.x == 1 || snake.food.x == width || snake.food.y == 1 || snake.food.y == height) {
                //食物座標等於邊界
                collision = 1;
            }
        }

        if (collision == 0) {
            // 座標不相等，輸出食物座標
            break;
        }
    }
}

// 處理移動
void snake_move(char direction) {
    //如果吃到食物
    if (snake.pos[0].x == snake.food.x && snake.pos[0].y == snake.food.y) {
        snake.size++;
        set_food();
    }

    //蛇向前移動
    for (int i = snake.size - 1; i > 0; i--) {
        snake.pos[i] = snake.pos[i - 1];
    }

    switch (direction) {
    case 'W':
    case 'w':
    case 72:
        snake.pos[0].y--;
        break;
    case 'A':
    case 'a':
    case 75:
        snake.pos[0].x--;
        break;
    case 'S':
    case 's':
    case 80:
        snake.pos[0].y++;
        break;
    case 'D':
    case 'd':
    case 77:
        snake.pos[0].x++;
        break;
    default:
        break;
    }
}

//設置邊界
void set_border() {
    //清除屏幕
    //system("cls");
    printf("\033[1;1H\033[2J");

    //上邊界
    for (int i = 1; i <= width; i++) {
        printf("X");
    }
    printf("\n");

    //左右邊界
    for (int i = 1; i <= height - 2; i++) {
        for (int j = 1; j <= width; j++) {
            if (j == 1 || j == width) {
                printf("X");
            }
            else {
                if (i == snake.food.y && j == snake.food.x) {
                    printf("$");
                }
                else {
                    int isSnakeBodyPart = 0;
                    for (int k = 0; k < snake.size; k++) {
                        //當前位置有無蛇身
                        if (snake.pos[k].x == j && snake.pos[k].y == i) {
                            isSnakeBodyPart = 1;
                            if (k == 0) {
                                printf("O");
                            }
                            else {
                                printf("o");
                            }
                        }
                    }
                    if (!isSnakeBodyPart) {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }

    //下邊界
    for (int i = 1; i <= width; i++) {
        printf("X");
    }
    printf("\n");
}

//檢查是否出局
int check_collision() {
    //如果撞到牆
    if (snake.pos[0].x <= 1 || snake.pos[0].x >= width || snake.pos[0].y <= 0 || snake.pos[0].y >= height - 1) {
        return 1;
    }
    //如果撞到身體
    for (int i = snake.size - 1; i > 0; i--) {
        if (snake.pos[0].x == snake.pos[i].x && snake.pos[0].y == snake.pos[i].y) {
            return 1;
        }
    }
    return 0;
}

//檢查輸入是否相對
int check_wasd() {
    if ((direction == 'A' || direction == 'a') && (prevdirection == 'D' || prevdirection == 'd') ||
        (direction == 'D' || direction == 'd') && (prevdirection == 'A' || prevdirection == 'a') ||
        (direction == 'W' || direction == 'w') && (prevdirection == 'S' || prevdirection == 's') ||
        (direction == 'S' || direction == 's') && (prevdirection == 'W' || prevdirection == 'w')) {
        return 0;
    }
    return 1;
}
