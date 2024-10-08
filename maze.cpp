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

class List
{
    private:
        Node* head;
        int rows;
        int cols;

    public:
        int playerX;
        int playerY;

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

            int keyX = rand() % rows;
            int keyY = rand() % cols;
            while (keyX == playerX && keyY == playerY)
            {
                keyX = rand() % rows;
                keyY = rand() % cols;
            }
            character(keyX, keyY, 'K');

            int doorX = rand() % rows;
            int doorY = rand() % cols;
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
           // display();
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

