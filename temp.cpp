#include <iostream>
#include <string>
#include <vector>

using namespace std;
vector<vector<char> > pole1 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
vector<vector<char> > pole2 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
vector<vector<char> > pole2hidden = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
string subs1;
string subs2;
const int SIZE = 10;
int currmove = 0; //0 - player 1 - PC
int w2s = 0;
int p2s = 0;
int toHit = 0;
int hitOnes = 0;
string currinput;
vector<vector<pair<int, int>>> arrofplships;
vector<vector<pair<int, int>>> arrofpcships;
vector<pair<int, int>> currplship;
vector<pair<int, int>> currpcship;

void start_pole(vector<vector<char>>& field) {
    for (int i = 0; i < SIZE; ++i) {
        vector<char> row(SIZE, 'e'); // заполняем каждую клетку пустым полем
        field.push_back(row);
    }
}

//Проверка на условие расстановки кораблей
bool right_place(const vector<vector<char>>& field, int row, int column, int length, bool isVertical) {
    if (isVertical) {
        if (row + length >= SIZE) return false; // проверка, чтобы корабли не выходили за границы поля

        // проверка, чтобы не было пересечений с другими кораблями и корабли не соприкасались
        for (int i = row - 1; i <= row + length; ++i) {
            for (int j = column - 1; j <= column + 1; ++j) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                    if (field[i][j] == 's') return false;
                }
            }
        }
    } else {
        if (column + length >= SIZE) return false; // проверка, чтобы корабль не выходил за границы поля

        // проверка, чтобы не было пересечений с другими кораблями и корабли не соприкасались
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = column - 1; j <= column + length; ++j) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                    if (field[i][j] == 's') return false;
                }
            }
        }
    }

    return true;
}

//Алгоритм размещения кораблей
void ship_place(vector<vector<char>>& field, int length) {
    bool isVertical = rand() % 2 == 0; // выбираем случайную ориентацию корабля

    int row, column;
    do {
        row = rand() % SIZE;
        column = rand() % SIZE;
    } while (!right_place(field, row, column, length, isVertical)); // повторяем, пока не найдем подходящее место для корабля

    // расставляем корабль
    if (isVertical) {
        if(currmove == 0){
            currpcship.clear();
        }
        else if(currmove == 1){
            currplship.clear();
        }
        for (int i = row; i < row + length; ++i) {
            field[i][column] = 's';
            if(currmove == 0){
                currpcship.push_back(make_pair(i, column));
            }
            else if(currmove == 1){
                currplship.push_back(make_pair(i, column));
            }
        }
        if(currmove == 0){
            arrofpcships.push_back(currpcship);
        }
        else if(currmove == 1){
            arrofplships.push_back(currplship);
        }
    }
    else {
        if(currmove == 0){
            currpcship.clear();
        }
        else if(currmove == 1){
            currplship.clear();
        }
        for (int j = column; j < column + length; ++j) {
            field[row][j] = 's';
            if(currmove == 0){
                currpcship.push_back(make_pair(row, j));
            }
            else if(currmove == 1){
                currplship.push_back(make_pair(row, j));
            }
        }
        if(currmove == 0){
            arrofpcships.push_back(currpcship);
        }
        else if(currmove == 1){
            arrofplships.push_back(currplship);
        }
    }
}

//Расстановка кораблей компьютером
void randomlyPlaceShips(vector<vector<char>>& field) {
    srand(time(0)); // инициализируем генератор случайных чисел
    // расставляем корабли по количеству
    ship_place(field, 4); // 1 корабль из 4 клеток
    ship_place(field, 3); // 2 корабля из 3 клеток
    ship_place(field, 3);
    ship_place(field, 2); // 3 корабля из 2 клеток
    ship_place(field, 2);
    ship_place(field, 2);
    ship_place(field, 1); // 4 корабля из 1 клетки
    ship_place(field, 1);
    ship_place(field, 1);
    ship_place(field, 1);
}


int main(){

    return 0;
}