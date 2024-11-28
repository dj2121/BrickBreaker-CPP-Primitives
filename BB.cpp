#include <iostream>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>


using namespace std;


//Editable size, please keep both values equal though.
int renderMatrix[50][50];
int boardSize;
//Editable Bat Size
int batsize = 6;
//Editable FPS, keep under 150
int fps=100;
int gameOver = 0;
string renderC("");


char wMessage[14] = "Brick Breaker";
char tMessage[14] = "Game Over";

struct velocity{
    float vertical;
    float horizontal;
} ballVelocity;


//Standard Stuff

//Standard C function
void clear_screen ( void )
{
  DWORD n;                         /* Number of characters written */
  DWORD size;                      /* number of visible characters */
  COORD coord = {0};               /* Top left screen position */
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  /* Get a handle to the console */
  HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );

  GetConsoleScreenBufferInfo ( h, &csbi );

  /* Find the number of characters to overwrite */
  size = csbi.dwSize.X * csbi.dwSize.Y;

  /* Overwrite the screen buffer with whitespace */
  FillConsoleOutputCharacter ( h, TEXT ( ' ' ), size, coord, &n );
  GetConsoleScreenBufferInfo ( h, &csbi );
  FillConsoleOutputAttribute ( h, csbi.wAttributes, size, coord, &n );

  /* Reset the cursor to the top left position */
  SetConsoleCursorPosition ( h, coord );
}



 void *boardMover(void *threadid)
 {
    long tid;
    tid = (long)threadid;
    char input;
    int i, j=boardSize;

    while(1)
    {
        j = boardSize;
        input = getch();
        switch (input)
        {
            case 'a':
            for(i=0; i<boardSize; i++)
            {
                if(renderMatrix[boardSize-2][i]==2)
                {
                    if(i<j) j=i;
                }
            }

            if(j>1 && j<boardSize)
            {
                for(i=0; i<boardSize; i++)
                {
                renderMatrix[boardSize-2][i]=0;
                }

                for(i=0; i<batsize; i++)
                {
                    renderMatrix[boardSize-2][i + j - 1] = 2;
                }
            }

            Sleep (100);
            break;

            case 'd':

            for(i=0; i<boardSize; i++)
            {
                if(renderMatrix[boardSize-2][i]==2)
                {
                    if(i<j) j=i;
                }
            }

            if(j>0 && j<(boardSize-batsize-1))
            {
                for(i=0; i<boardSize; i++)
                {
                renderMatrix[boardSize-2][i]=0;
                }

                for(i=0; i<batsize; i++)
                {
                    renderMatrix[boardSize-2][i + j + 1] = 2;
                }
            }

            Sleep (100);
            break;
        }
    }
    pthread_exit(NULL);
 }

void *render(void *threadid)
{
    int i, j;
    while(1)
    {
        clear_screen();
        for(i=1; i<(boardSize/2); i++)
        {
            cout << "  ";
        }

        if(gameOver){
            cout <<tMessage;
        }
        else{
            cout <<wMessage;
        }
        cout << endl;
        for(i=0; i<boardSize; i++)
        {
            for(j=0; j<boardSize; j++)
            {
                if((renderMatrix[i][j])!=0)
                {
                    if(renderMatrix[i][j]==5 || renderMatrix[i][j]==4)
                    {
                        renderC += "* ";
                    }
                    else if(renderMatrix[i][j]==6)
                    {
                        renderC += "$ ";
                    }
                    else if(renderMatrix[i][j]==2)
                    {
                        renderC += "##";
                    }
                    else if(renderMatrix[i][j]==3)
                    {
                        renderC += "0 ";
                    }
                }
                else
                {
                    renderC += "  ";
                }
            }
            renderC += "\n";
        }
        cout << renderC;
        renderC.clear();
        Sleep (1000/fps);
    }
    pthread_exit(NULL);
}

void changeV(int offrY, int offrX)
{
    if(renderMatrix[offrY][offrX] == 6 ) {
        gameOver = 1;
        fps = 1;
        ballVelocity.horizontal = 0;
        ballVelocity.vertical = 0;
    }
    else if(renderMatrix[offrY][offrX] == 5 ) {
        ballVelocity.vertical = -ballVelocity.vertical;

    }
    else if(renderMatrix[offrY][offrX] == 4 ) {
        ballVelocity.horizontal = -ballVelocity.horizontal;

    }
    else if(renderMatrix[offrY][offrX] == 2 ) {
        ballVelocity.vertical = -ballVelocity.vertical;
    }

}

void *ballController(void *threadid)
{
    int ballX, ballY, i, j, offX, offY;
    float vRatio;

    while(1){
            //get ball co-ordinates
            for(i=0; i<boardSize; i++)
            {
                for(j=0; j<boardSize; j++)
                {
                    if(renderMatrix[i][j]==3)
                    {
                        ballY = i;
                        ballX = j;
                    }
                }
            }

            renderMatrix[ballY][ballX] = 0;
            vRatio = ballVelocity.horizontal / ballVelocity.vertical;

            if(ballVelocity.vertical > 0)
            {
                if(ballVelocity.horizontal > 0)
                {
                    for(i = 1; i <= ballVelocity.vertical + 1; i++ )
                    {
                        offY = ballY + i;
                        offX = ballX + (int)(i * vRatio);
                        if(renderMatrix[offY][offX]!=0){
                                changeV(offY, offX);
                                break;
                        }
                    }
                }

                else if(ballVelocity.horizontal <= 0)
                {
                    for(i = 1; i <= ballVelocity.vertical + 1; i++ )
                    {
                        offY = ballY + i;
                        offX = ballX - (int)(i * vRatio);
                        if(renderMatrix[offY][offX]!=0){
                            changeV(offY, offX);
                            break;
                        }
                    }
                }
            }

            else if(ballVelocity.vertical <= 0)
            {
                if(ballVelocity.horizontal > 0)
                {
                    j = 1;
                    for(i = -1; i >= ballVelocity.vertical - 1; i-- )
                    {
                        offY = ballY + i;
                        offX = ballX - (int)(j * vRatio);
                        if(renderMatrix[offY][offX]!=0){
                            changeV(offY, offX);
                            break;
                        }
                        j++;
                    }
                }

                else if(ballVelocity.horizontal <= 0)
                {
                    for(i = -1; i >= ballVelocity.vertical - 1; i--)
                    {
                        offY = ballY + i;
                        offX = ballX + (int)(i * vRatio);
                        if(renderMatrix[offY][offX]!=0){
                            changeV(offY, offX);
                            break;
                        }
                    }
                }
            }
            ballX = ballX + ballVelocity.horizontal;
            ballY = ballY + ballVelocity.vertical;
            renderMatrix[ballY][ballX] = 3;
            Sleep (100);
    }
}



int initControl()
{
    ballVelocity.horizontal = 1;
    ballVelocity.vertical = 1.5;

    int i, j;

    for(i=0; i<boardSize; i++)
    {
        for(j=0; j<boardSize; j++)
        {
            renderMatrix[i][j] = 0;
        }
    }

    for(i = 0; i<boardSize; i++)
    {
        renderMatrix[0][i] = 5;
    }
    for(i = 0; i<boardSize; i++)
    {
        renderMatrix[boardSize-1][i] = 6;
    }

    for(i = 0; i<boardSize; i++)
    {
        renderMatrix[i][0] = 4;
    }

    for(i = 0; i<boardSize; i++)
    {
        renderMatrix[i][boardSize-1] = 4;
    }

    for(i=0; i<batsize; i++)
    {
        renderMatrix[boardSize-2][i + (boardSize/2) - (batsize/2)] = 2;
    }

    renderMatrix[10][boardSize/2] = 3;

    return 1;
}


//Main
int main(void)
{
    int col = sizeof(renderMatrix[0])/sizeof(int);
    int row=(sizeof(renderMatrix)/sizeof(renderMatrix[0]));

    if(row!=col)
    {
        cout << "Please make Rows and Columns equal";
        return 0;
    }

    boardSize = row;
    int init = initControl();
    if(init!=1)
    {
        cout << "Initialization Error";
        return 0;
    }

    int i;

    int num_tasks=5, tid;
    pthread_t threads[2];

    int rc;
    long t;

    rc = pthread_create(&threads[0], NULL, boardMover, (void *)0);
    rc = pthread_create(&threads[0], NULL, render, (void *)0);
    rc = pthread_create(&threads[0], NULL, ballController, (void *)0);
    pthread_exit(NULL);



    exit(0);
}

