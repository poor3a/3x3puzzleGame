#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define GRID_SIZE 3
#define TILE_SIZE 100
#define SCREEN_WIDTH (GRID_SIZE * TILE_SIZE + 100)
#define SCREEN_HEIGHT (GRID_SIZE * TILE_SIZE + 175)

typedef struct
{
    int number;
    bool selected;
} Tile;

Tile grid[GRID_SIZE][GRID_SIZE];
int emptyX = GRID_SIZE - 1, emptyY = GRID_SIZE - 1;
int selected_posx = 0, selected_posy = 0;
int startTime = 0, elapsedTime = 0;
int moves = 0;
bool win = false;
bool resetConfirmation = false, quitConfirmation = false;
bool menu = true;
char statusMessage[50] = "";

void InitializeMenu()
{
}
void InitializeGrid()
{
    int numbers[GRID_SIZE * GRID_SIZE - 1];
    for (int i = 0; i < GRID_SIZE * GRID_SIZE - 1; i++)
    {
        numbers[i] = i + 1;
    }

    srand(time(NULL) + 14035141022);
    for (int i = 0; i < GRID_SIZE * GRID_SIZE - 1; i++)
    {
        int j;
        do
        {
            j = rand() % (GRID_SIZE * GRID_SIZE - 1);
        } while (i == j);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }

    int index = 0;
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            if (y == GRID_SIZE - 1 && x == GRID_SIZE - 1)
            {
                grid[y][x].number = 0;
            }
            else
            {
                grid[y][x].number = numbers[index++];
            }
            grid[y][x].selected = false;
        }
    }
    emptyX = GRID_SIZE - 1;
    emptyY = GRID_SIZE - 1;
    selected_posx = 0;
    selected_posy = 0;
    elapsedTime = 0;
    win = false;
    resetConfirmation = false;
    quitConfirmation = false;
    statusMessage[0] = '\0';
}

void Draw_Grid()
{
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            Rectangle tileRect = {x * TILE_SIZE + 50, y * TILE_SIZE + 150, TILE_SIZE, TILE_SIZE};

            if (grid[y][x].number == 0)
            {
                DrawRectangleRec(tileRect, LIGHTGRAY);
                DrawRectangleLinesEx(tileRect, 3, BLACK);
            }
            else
            {
                DrawRectangleRec(tileRect, WHITE);
                if (win)
                    DrawRectangleLinesEx(tileRect, 4, GREEN);
                else if (grid[y][x].selected)
                    DrawRectangleLinesEx(tileRect, 4, RED);
                else
                    DrawRectangleLinesEx(tileRect, 3, BLACK);

                char numberText[3];
                snprintf(numberText, sizeof(numberText), "%d", grid[y][x].number);
                DrawText(numberText, tileRect.x + TILE_SIZE / 2 - 10, tileRect.y + TILE_SIZE / 2 - 10, 20, BLACK);
            }
        }
    }
}

void InputManager()
{

    for (int key = KEY_ONE; key <= KEY_EIGHT; key++)
    {
        if (IsKeyPressed(key))
        {
            for (int y = 0; y < GRID_SIZE; y++)
            {
                for (int x = 0; x < GRID_SIZE; x++)
                {
                    if (grid[y][x].number == key - KEY_ZERO)
                    {
                        grid[y][x].selected = true;
                        if (!(grid[y][x].number == grid[selected_posy][selected_posx].number))
                        {
                            grid[selected_posy][selected_posx].selected = false;
                            selected_posx = x;
                            selected_posy = y;
                        }
                    }
                }
            }
            statusMessage[0] = '\0';
        }
    }
}

void MoveTile(int key)
{
    int dx = 0, dy = 0;

    if (key == KEY_W || key == KEY_UP)
        dy = -1;
    else if (key == KEY_S || key == KEY_DOWN)
        dy = 1;
    else if (key == KEY_A || key == KEY_LEFT)
        dx = -1;
    else if (key == KEY_D || key == KEY_RIGHT)
        dx = 1;

    int newX = selected_posx + dx;
    int newY = selected_posy + dy;
    if (newX == emptyX && newY == emptyY)
    {
        moves++;
        emptyX = selected_posx;
        emptyY = selected_posy;
        grid[selected_posy][selected_posx].selected = false;
        grid[newY][newX].selected = true;
        grid[newY][newX].number = grid[selected_posy][selected_posx].number;
        grid[selected_posy][selected_posx].number = 0;
        selected_posx = newX;
        selected_posy = newY;
    }
}

void HandleResetAndQuit()
{
    if (resetConfirmation)
    {
        DrawText("Reset the game? (Y/N)", SCREEN_WIDTH / 2 - 150, 110, 20, RED);
        if (IsKeyPressed(KEY_Y))
        {
            moves = 0;
            startTime = (int)GetTime();
            InitializeGrid();
        }
        else if (IsKeyPressed(KEY_N))
        {
            resetConfirmation = false;
        }
    }
    else if (quitConfirmation)
    {
        DrawText("Quit the game? (Y/N)", SCREEN_WIDTH / 2 - 150, 110, 20, RED);
        if (IsKeyPressed(KEY_Y))
        {
            CloseWindow();
            exit(0);
        }
        else if (IsKeyPressed(KEY_N))
        {
            quitConfirmation = false;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_R))
        {
            resetConfirmation = true;
        }
        else if (IsKeyPressed(KEY_Q))
        {
            quitConfirmation = true;
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
                win = true;
                snprintf(statusMessage, sizeof(statusMessage), "You won!");
            }
        }
    }
}
void HandleMouseSelection()
{
    Vector2 mousePos = GetMousePosition();
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            Rectangle tileRect = {x * TILE_SIZE + 50, y * TILE_SIZE + 150, TILE_SIZE, TILE_SIZE};
            if (CheckCollisionPointRec(mousePos, tileRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                grid[selected_posy][selected_posx].selected = false;
                selected_posx = x;
                selected_posy = y;
                grid[y][x].selected = true;
                statusMessage[0] = '\0';
            }
        }
    }
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3x3 Puzzle Game");
    SetTargetFPS(60);
    InitializeGrid();

    while (!WindowShouldClose())
    {
        if (menu)
        {
            startTime = GetTime();
            ClearBackground(RAYWHITE);
            DrawText("welocome!", (SCREEN_WIDTH / 2) - 80, 30, 30, PURPLE);
            DrawText("press enter to start!", (SCREEN_WIDTH / 2) - 160, 80, 30, PURPLE);
            DrawText("you can select blocks by clicking,", (SCREEN_WIDTH / 2) - 165, 150, 20, BLACK);
            DrawText("or entering their number", (SCREEN_WIDTH / 2) - 100, 180, 20, BLACK);
            DrawText("and move them by (w,a,s,d) or arrows,", (SCREEN_WIDTH / 2) - 190, 210, 20, BLACK);
            DrawText("press R to reset and Q to exit!", (SCREEN_WIDTH / 2) - 150, 240, 20, BLACK);
            DrawText("created by pooria sadeghi", (SCREEN_WIDTH / 2) - 160, 380, 27, RED);
            if (IsKeyPressed(KEY_ENTER))
                menu = false;
        }
        else
        {
            if (!win && !resetConfirmation && !quitConfirmation)
            {
                InputManager();
                MoveTile(GetKeyPressed());
                CheckWinCondition();
                HandleMouseSelection();
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            char timerText[50];
            if (!win)
            {
                elapsedTime = (int)(GetTime() - startTime);
            }
            
            snprintf(timerText, sizeof(timerText), "Time: %d seconds", elapsedTime);
            DrawText(timerText, 10, 10, 20, BLACK);
            char moves_text[30];
            snprintf(moves_text, sizeof(moves_text), "moves: %d", moves);
            DrawText(moves_text, 200, 10, 20, BLACK);


            if (statusMessage[0] != '\0')
            {
                if (win)
                {
                    DrawText(statusMessage, 10, 40, 20, GREEN);
                    DrawText("press r to reset", 10, 80, 20, GREEN);
                }
                else
                    DrawText(statusMessage, 10, 40, 20, RED);
            }

            Draw_Grid();
            HandleResetAndQuit();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
