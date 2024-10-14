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

class Stack
{
    private:
        StackNode* top;

    public:
        Stack()
        {
            top = nullptr;
        }

        void push(int m, int n)
        {
            StackNode* node1 = new StackNode(m, n);
            if (top != nullptr)
            {
                node1->next = top;
            }
            top = node1;
        }

        StackNode* pop()
        {
            if (top == nullptr)
            {
                return nullptr;
            }
            StackNode* temp = top;
            top = top->next;
            return temp;
        }

        bool empty()
        {
            if (top == nullptr)
            {
                return true;
            }
            return false;
        }


};

class List
{
    private:
        Node* head;
        int rows;
        int cols;
        Stack stack;
        int undos;
        int prevX;
        int prevY;
        bool KEY;

    public:
        int playerX;
        int playerY;
        int keyX;
        int keyY;
        int initial_dis;
        int moves;
        int bombX;
        int bombY;
        int doorX;
        int doorY;
        int coinX1;
        int coinY1;
        int coinX2;
        int coinY2;
        int coinX3;
        int coinY3;
        int coinX4;
        int coinY4;

        List(int row, int col)
        {
            head = nullptr;
            rows = row;
            cols = col;
            prevX = -1;
            prevY = -1;
            undos = 6; // easy level
            KEY = false;

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
            mvprintw(1, 0, "Remaining Moves: %d", moves);
            mvprintw(1, 30, "Remaining Undos: %d", undos);
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

            doorX = rand() % rows;
            doorY = rand() % cols;
            
            initial_dis = distance(playerX, playerY, keyX, keyY);
            
            while ((doorX == playerX && doorY == playerY) || (doorX == keyX && doorY == keyY))
            {
                doorX = rand() % rows;
                doorY = rand() % cols;
            }
            character(doorX, doorY, 'D');

            bombX = rand() % rows;
            bombY = rand() % cols;
            while ((bombX == playerX && bombY == playerY) || (bombX == keyX && bombY == keyY) || (bombX == doorX && bombY == doorY))
            {
                bombX = rand() % rows;
                bombY = rand() % cols;
            }
            character(bombX, bombY, 'B');

            coinX1 = rand() % rows;
            coinY1 = rand() % cols;

            while ((coinX1 == playerX && coinY1 == playerY) || (coinX1 == keyX && coinY1 == keyY) || (coinX1 == doorX && coinY1 == doorY) || (coinX1 == bombX && coinY1 == bombY))
            {
                coinX1 = rand() % rows;
                coinY1 = rand() % cols;
            }
            character(coinX1, coinY1, 'C');

            coinX2 = rand() % rows;
            coinY2 = rand() % cols;

            while ((coinX2 == playerX && coinY2 == playerY) || (coinX2 == keyX && coinY2 == keyY) || (coinX2 == doorX && coinY2 == doorY) || (coinX2 == bombX && coinY2 == bombY))
            {
                coinX2 = rand() % rows;
                coinY2 = rand() % cols;
            }
            character(coinX2, coinY2, 'C');

            coinX3 = rand() % rows;
            coinY3 = rand() % cols;

            while ((coinX3 == playerX && coinY3 == playerY) || (coinX3 == keyX && coinY3 == keyY) || (coinX3 == doorX && coinY3 == doorY) || (coinX3 == bombX && coinY3 == bombY))
            {
                coinX3 = rand() % rows;
                coinY3 = rand() % cols;
            }
            character(coinX3, coinY3, 'C');

            coinX4 = rand() % rows;
            coinY4 = rand() % cols;

            while ((coinX4 == playerX && coinY4 == playerY) || (coinX4 == keyX && coinY4 == keyY) || (coinX4 == doorX && coinY4 == doorY) || (coinX4 == bombX && coinY4 == bombY))
            {
                coinX4 = rand() % rows;
                coinY4 = rand() % cols;
            }
            character(coinX4, coinY4, 'C');

            //int key_dis = distance(playerX, playerY, keyX, keyY);

            int door_dis = distance(keyX, keyY, doorX, doorY);

            moves = initial_dis + door_dis;

            //int total_dis = key_dis + door_dis;
        }

        void movement(int input)
        {
            if ((input == KEY_UP && playerX-1==prevX)||(input == KEY_DOWN && playerX+1==prevX)||(input == KEY_LEFT && playerY-1==prevY)||(input == KEY_RIGHT && playerY+1==prevY))
            {
                mvprintw(6, 0, "Press 'a' to use undo feature for moving back!!!");
                return;
            }

            stack.push(playerX,playerY);
            prevX = playerX;
            prevY = playerY;

            character(playerX, playerY, '.');

            int playerX1 = playerX;
            int playerY1 = playerY;

            if (input == KEY_UP && playerX > 0)
            {
                playerX1--;
            }
            else if (input == KEY_DOWN && playerX < rows - 1)
            {
                playerX1++;
            }
            else if (input == KEY_RIGHT && playerY < cols - 1)
            {
                playerY1++;
            }
            else if (input == KEY_LEFT && playerY > 0)
            {
                playerY1--;
            }

            if (playerX1 == doorX && playerY1 == doorY && KEY == false)
            {
                mvprintw(7, 0, "Find the key first in order to unlock!!!");
            }
            else
            {
                playerX = playerX1;
                playerY = playerY1;
            }
            if (playerX == keyX && playerY == keyY)
            {
                KEY = true;
                mvprintw(2,30,"Key Status: True");
            }

            if (playerX == bombX && playerY == bombY)
            {
                mvprintw(7,0,"BOMB EXPLODED!!! So the game is over!");
                mvprintw(8,0,"Press esc to quit the game");
            }

            if (playerX == doorX && playerY == doorY)
            {
                if (KEY)
                {
                    mvprintw(7,0,"Congtatulations. You unlocked the door and won the game!");
                    mvprintw(8,0,"Press esc to quit the game");
                }
                else
                {
                    mvprintw(7,0,"Find the key first in order to unlock!!!");
                }
            }
            else
            {
                mvprintw(7,0,"Find the key first in order to unlock!!!");
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
            initial_dis = key_dis;
            moves--;

            display();
            
        }

        void undo()
        {
            if (!stack.empty() && undos>0)
            {
                StackNode* prev = stack.pop();
                if (prev!=nullptr)
                {
                    character(playerX,playerY,'.');
                    playerX = prev->a;
                    playerY = prev->b;
                    character(playerX, playerY, 'P');
                    undos--;
                    delete prev;
                }
                display();
            }
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
        if (key == 'a')
        {
            L1.undo();
        }
        else
        {
            L1.movement(key);
        }
    }

    endwin();

    return 0;
}

