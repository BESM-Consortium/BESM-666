const int SIZE = 12; // Размер.

// Я хз почему нельзя создать board[SIZE][SIZE],
// это ебучее говно ругается на variable переменную.
// Короче нужно менять чиселку в обоих местах.

int board[12][12];
int results_count = 0; // Количество решений.
 
// Функция tryQueen() - проверяет нет ли уже установленных ферзей,
// по вертикали, диагоналям.
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
 
// Функция setQueen() - пробует найти результаты решений.
void setQueen(int a) // a - номер очередной строки в которую нужно поставить очередного ферзя.
{   
    for(int i = 0; i < SIZE; ++i)
    {
        // Здесь проверяем, что если поставим в board[a][i] ферзя (единицу),
        // то он будет единственным в этой строке, столбце и диагоналях.
        if(tryQueen(a, i))
        {
            board[a][i] = 1;
            setQueen(a+1);
            board[a][i] = 0;
        }
    }
    
    return; // Опционально.
}
 
int start()
{
    setQueen(0);
    
    return 1;
}