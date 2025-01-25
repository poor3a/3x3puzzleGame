
// مانی بابایی شماره دانشجویی 14035141022
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

typedef struct
{
    int number;
    boolean selected;
} Tile;

Tile grid[3][3];
int moves = 0;
int emptyX = 2;
int emptyY = 2;
int selectedx = 0;
int selectedy = 0;
boolean win = 0;
boolean reset = 0, quit = 0;
char BOLD_ON[7] = "\033[1m";
char BOLD_OFF[7] = "\033[0m";
#define Red 4
#define White 7
#define Green 2

void initialize_array()
{
    moves = 0;
    emptyX = 2;
    emptyY = 2;
    selectedx = 0;
    selectedy = 0;
    int numbers[8];
    for (int i = 0; i < 8; i++)
    {
        numbers[i] = i + 1;
    }
    srand(time(NULL) + 14035141022);
    for (int i = 0; i < 8; i++)
    {
        int j;
        do
        {
            j = rand() % (8);
        } while (i == j);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }

    int index = 0;
    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            if (y == 2 && x == 2)
            {
                grid[y][x].number = 0;
            }
            else
            {
                grid[y][x].number = numbers[index++];
            }
            grid[y][x].selected = 0;
        }
    }
}
void CheckWinCondition()
{
    if (((grid[0][0]).number == 1) && ((grid[0][1]).number == 2) && ((grid[0][2]).number == 3))
    {
        if (((grid[1][0]).number == 4) && ((grid[1][1]).number == 5) && ((grid[1][2]).number == 6))
        {
            if (((grid[2][0]).number == 7) && ((grid[2][1]).number == 8))
            {
                win = 1;
            }
        }
    }
}
void set_color(int x)
{
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(color, x);
}
int main()
{
    system("cls");
    printf("mani babaie ///// 14035141022\n");
    printf("welcome\n press any key to continue:");
    getch();
    system("cls");
    initialize_array();

    while (1)
    {
        CheckWinCondition();
        if (reset)
        {
            moves = 0;
            initialize_array();
            reset = 0;
        }
        if (quit)
        {
            system("cls");
            printf("bye!");
            return 0;
        }

        if (win)
        {
            system("cls");
            set_color(Green);
            printf("you won!\n press r to reset or q to exit");
            char input = _getch();
            if (input == 'r')
            {
                initialize_array();
                win = 0;
            }
            else if (input == 'q')
                return 0;
        }
        else
        {
            system("cls");
            printf("moves: %d\n",moves);
            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    if (grid[y][x].selected)
                    {
                        printf("%s", BOLD_ON);
                        set_color(Red);
                        printf("%d ", grid[y][x].number);
                        printf("%s", BOLD_OFF);
                        set_color(White);
                    }
                    else
                    {
                        printf("%d ", grid[y][x].number);
                    }
                }
                printf("\n");
            }
            int dx = 0;
            int dy = 0;

            char input = _getch();
            if ((int)input >= 49 && (int)input <= 56)
            {
                for (int y = 0; y < 3; y++)
                {
                    for (int x = 0; x < 3; x++)
                    {
                        if (grid[y][x].number == (int)(input - 48))
                        {
                            grid[y][x].selected = 1;
                            grid[selectedy][selectedx].selected = 0;
                            selectedx = x;
                            selectedy = y;
                        }
                    }
                }
            }
            else
            {
                switch (input)
                {
                case 'a':
                    dx = -1;
                    break;
                case 's':
                    dy = 1;
                    break;
                case 'w':
                    dy = -1;
                    break;
                case 'd':
                    dx = 1;
                    break;
                case 'q':
                    quit = 1;
                    break;
                case 'r':
                    reset = 1;
                    break;
                default:
                    break;
                }
            }

            int newX = selectedx + dx;
            int newY = selectedy + dy;
            if (newX == emptyX && newY == emptyY)
            {
                moves ++;
                emptyX = selectedx;
                emptyY = selectedy;
                grid[selectedy][selectedx].selected = 0;
                grid[newY][newX].selected = 1;
                grid[newY][newX].number = grid[selectedy][selectedx].number;
                grid[selectedy][selectedx].number = 0;
                selectedx = newX;
                selectedy = newY;
            }
        }
    }
}