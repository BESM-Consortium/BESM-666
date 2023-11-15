const int SIZE = 14; // N queens.

// I don't know why you can't create board[SIZE][SIZE],
// this fucking shit swears at variable.
// So, you need to change the number in both places.

int board[14][14];
int results_count = 0; // Number of solves.

int tryQueen(int a, int b)
{
    for(int i = 0; i < a; ++i)
    {
        if(board[i][b])
        {
            return 0;
        }
    }

    for(int i = 1; i <= a && b-i >= 0; ++i)
    {
        if(board[a-i][b-i])
        {
            return 0;
        }
    }

    for(int i = 1; i <= a && b+i < SIZE; i++)
    {
        if(board[a-i][b+i])
        {
            return 0;
        }
    }

    return 1;
}

void setQueen(int a)
{   
    for(int i = 0; i < SIZE; ++i)
    {
        if(tryQueen(a, i))
        {
            board[a][i] = 1;
            setQueen(a+1);
            board[a][i] = 0;
        }
    }

    return;
}

int start()
{
    setQueen(0);

    return 1;
}