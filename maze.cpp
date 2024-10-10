#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>

class Node
{
    public:
        char dot;
        Node* up;
        Node* down;
        Node* left;
        Node* right;

        Node()
        {
            dot = '.';
            up = nullptr;
            down = nullptr;
            left = nullptr;
            right = nullptr;
        }
};

class StackNode
{
        public:
            int a;
            int b;
            StackNode* next;

            StackNode(int row, int col)
            {
                a = row;
                b = col;
                next = nullptr;
            }     

};

class List
{
    private:
        Node* head;
        int rows;
        int cols;

    public:
        int playerX;
        int playerY;
        int keyX;
        int keyY;
        int initial_dis;

        List(int row, int col)
        {
            head = nullptr;
            rows = row;
            cols = col;

            Grid();
        }

        void Grid()
        {
            Node* temp = nullptr;
            Node* prev = nullptr;

            for (int i = 0; i < rows; i++)
            {
                Node* first = nullptr; // first node in each row
                Node* curr_row = nullptr;

                for (int j = 0; j < cols; j++)
                {
                    Node* newNode = new Node();
                    
                    if (head == nullptr)
                    {
                        head = newNode;
                    }

                    if (first == nullptr)
                    {
                        first = newNode;
                    }

                    if (prev != nullptr)
                    {
                        prev->right = newNode;
                        newNode->left = prev;
                    }

                    if (temp!=nullptr)
                    {
                        temp->down = newNode;
                        newNode->up = temp;
                        temp = temp->right;
                    }

                    prev = newNode;

                    if (curr_row == nullptr)
                    {
                        curr_row = newNode;
                    }
                }
                temp = first;
                prev = nullptr;
            }
        }

        Node* current (int row, int col)
        {
            Node* curr = head;
            for (int i = 0; i < row; i++)
            {
                curr = curr->down;
            }
            for (int j = 0; j < col; j++)
            {
                curr = curr->right;
            }
            return curr;
        }

        void display()
        {
            mvprintw(0, 18, "Mode: Easy");
            mvprintw(1, 0, "Remaining Moves: ");
            mvprintw(1, 30, "Remaining Undos: ");
            mvprintw(2, 0, "Score: ");
            mvprintw(2, 30, "Key Status: ");
            mvprintw(3, 0, "Next drop in line: 'C', 'C', 'B', 'C', 'B'");
            mvprintw(5, 0, "Hint:");

            int offsetX = 10;
            int offsetY = 5;
            for (int i = 0; i < rows + 2; i++)
            {
                for (int j = 0; j < cols + 2; j++)
                {
                    if (i == 0 || i == rows + 1 || j == 0 || j == cols + 1)
                    {
                        mvaddch(i + 6 + offsetY, j * 2 + offsetX, '#');
                    }
                    else
                    {
                        char symbol = current(i - 1, j - 1)->dot;
                        mvaddch(i + 6 + offsetY, j * 2 + offsetX, symbol);
                        // if (symbol == 'K' || symbol == 'D')
                        // {
                        //     mvaddch(i + 6 + offsetY, j * 2 + offsetX, '.');
                        // }
                        // else
                        // {
                        //     mvaddch(i + 6 + offsetY, j * 2 + offsetX, symbol);
                        // }
                    }
                }
            }
            refresh();
        }

        int distance(int x1, int y1, int x2, int y2)
        {
            int dis;
            int x_axis;
            int y_axis;
            if (x1>x2)
            {
                x_axis = x1-x2;
            }
            else
            {
                x_axis = x2-x1;
            }
            if (y1>y2)
            {
                y_axis = y1-y2;
            }
            else
            {
                y_axis = y2-y1;
            }
            
            dis = (x_axis) + (y_axis);
            return dis;
        }

        void character(int row, int col, char ch)
        {
            current(row, col)->dot = ch;
        }

        void random()
        {
            srand(time(0));

            playerX = rand() % rows;
            playerY = rand() % cols;
            character(playerX, playerY, 'P');

            keyX = rand() % rows;
            keyY = rand() % cols;
            while (keyX == playerX && keyY == playerY)
            {
                keyX = rand() % rows;
                keyY = rand() % cols;
            }
            character(keyX, keyY, 'K');

            int doorX = rand() % rows;
            int doorY = rand() % cols;
            
            initial_dis = distance(playerX, playerY, keyX, keyY);
            
            while ((doorX == playerX && doorY == playerY) || (doorX == keyX && doorY == keyY))
            {
                doorX = rand() % rows;
                doorY = rand() % cols;
            }
            character(doorX, doorY, 'D');

            int bombX = rand() % rows;
            int bombY = rand() % cols;
            while ((bombX == playerX && bombY == playerY) || (bombX == keyX && bombY == keyY) || (bombX == doorX && bombY == doorY))
            {
                bombX = rand() % rows;
                bombY = rand() % cols;
            }
            character(bombX, bombY, 'B');

            int coinX = rand() % rows;
            int coinY = rand() % cols;
            while ((coinX == playerX && coinY == playerY) || (coinX == keyX && coinY == keyY) || (coinX == doorX && coinY == doorY) || (coinX == bombX && coinY == bombY))
            {
                coinX = rand() % rows;
                coinY = rand() % cols;
            }
            character(coinX, coinY, 'C');

            //int key_dis = distance(playerX, playerY, keyX, keyY);

            int door_dis = distance(keyX, keyY, doorX, doorY);

            //int total_dis = key_dis + door_dis;
        }

        void movement(int input)
        {
            character(playerX, playerY, '.');
            if (input == KEY_UP && playerX > 0)
            {
                playerX--;
            }
            else if (input == KEY_DOWN && playerX < rows - 1)
            {
                playerX++;
            }
            else if (input == KEY_RIGHT && playerY < cols - 1)
            {
                playerY++;
            }
            else if (input == KEY_LEFT && playerY > 0)
            {
                playerY--;
            }
            character(playerX, playerY, 'P');

            int key_dis = distance(playerX, playerY, keyX, keyY);

            if (key_dis > initial_dis)
            {
                mvprintw(5, 7, "Further away!!!");
            }
            else if (key_dis < initial_dis)
            {
                mvprintw(5, 7, "Getting closer");
            }

            display();

            
        }
};

void initialize()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

int main()
{
    int rows = 10;
    int cols = 10;

    initialize();

    List L1(rows, cols);

    L1.random();
    L1.display();

    int key;
    while ((key = getch()) != 27)
    {
        L1.movement(key);
    }

    endwin();

    return 0;
}

