#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <random> //Для рандома
#include <ctime> //Для рандомного сида
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono; //Для сохранений, замера времени, задержек. Для этого же библиотека threads

ifstream default_field("default.txt"); //Значение расположения полей по умолчанию
ofstream scoreboard("scoreboard.txt", ios::app);  //Таблица Рекордов
fstream current_field("current.txt"); //Файл для сохранения текущей игры
string x; //Внутриигровая перменная, которая содержит весь экран в строке
bool flag = false; //Флаг игры. Когда опущен - игра в процессе. При окончании игры поднимается.
//Поле Игрока (Видимо)
vector<vector<char> > pole1 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
//Поле Компьютера (Видимо)
vector<vector<char> > pole2 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
//Поле Компьютера (Невидимо)
vector<vector<char> > pole2hidden = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
//Глобальные переменные, которые пригодятся для пересчетов позиции на полях игроков
string subs1;
string subs2;
//Константный размер поля
const int SIZE = 10;
//Глобальные переменные для количества подбитых игроков Игроком/Компьютером
int hitsqsbplr = 0;
int hitsqbpc = 0;
//Ход на данный момент. 0 - игрок, 1 - компьютер
int currmove = 0; //0 - player 1 - PC
//Переменные отслеживания причины хода (Неверный ввод, промах, другое)
//для Игрока
int w2s = 0;
//для Компьютера
int p2s = 0;
//Уровень сложности игры с компьютером от 0 до 2
int currdiff = 0;
//Имя игрока для таблицы рекордов
string username;
//Матричная реализация кораблей у игроков внутри игры (Нужно для функции замены потонувших кораблей)
vector<vector<pair<int, int>>> arrofplships;
vector<vector<pair<int, int>>> arrofpcships;
//тоже для этой реализации
vector<pair<int, int>> currplship;
vector<pair<int, int>> currpcship;

//Проверка на условие расстановки кораблей
bool right_place(const vector<vector<char>>& field, int row, int column, int length, bool isVertical) {
    if (isVertical) {
        if (row + length >= SIZE) return false; //Проверка на выход за границы поля
        //Проверка на пересечение и соприкосновение
        for (int i = row - 1; i <= row + length; ++i) {
            for (int j = column - 1; j <= column + 1; ++j) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                    if (field[i][j] == 's') return false;
                }
            }
        }
    } else {
        if (column + length >= SIZE) return false; //Проверка на выход за границы поля

        //Проверка на пересечения с другими кораблями и соприкосновения
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
    bool isVertical = rand() % 2 == 0; //Выбор случайной ориентации корабля
    int row, column;
    do {
        row = rand() % SIZE;
        column = rand() % SIZE;
    } while (!right_place(field, row, column, length, isVertical)); //Повторение, пока не найдено подходящее место для корабля

    //Расставление корабля
    if (isVertical) {

        if(currmove == 0){
            currpcship.clear();
        }
        else if(currmove == 1){
            currplship.clear();
        }
        //Добавление актуальной информации о кораблях и их положениях
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
    srand(time(0)); //Установка сида по времени для большей рандомности
    ship_place(field, 4); //1 корабль из 4 клеток
    ship_place(field, 3); //Дальше - по аналогии в соответствие с правилами игры
    ship_place(field, 3);
    ship_place(field, 2);
    ship_place(field, 2);
    ship_place(field, 2);
    ship_place(field, 1);
    ship_place(field, 1);
    ship_place(field, 1);
    ship_place(field, 1);
}

//Вычленение из now положения на поле
void get_pole(string now){
    int s1 = 245; //Индекс начала поля Игрока
    int s2 = 285; //Индекс начала поля Компьютера
    for(int i = 0; i < 10; i++){
        subs1 = now.substr(s1,10);
        subs2 = now.substr(s2, 10);
        for(int j = 0; j< 10; j++){
            pole1[i][j] = subs1[j];
        }
        for(int j = 0; j< 10; j++){
            pole2[i][j] = subs2[j];
        }
        s1 += 60;
        s2 += 60;
    }
}

//Отрисовка дисплея
void disp_call(string now){
    system("clear");
    int counter = 0;
    int disp_res_x = 60, disp_res_y = 16;
    for(int i = 0; i < disp_res_y; i++){
        for(int j = 0; j < disp_res_x; j++){
            char nowx = now[counter];
            if(nowx == 't'){
                cout << "-";
            }
            else if (nowx == 'e'){
                cout << "○";//■ ·
            }
            else if (nowx == 'z'){
                cout<<"◎";
            }
            else if (nowx == 'u'){
                cout<<"◉";
            }
            else if (nowx == 'm'){
                cout << "✗";
            }
            else if (nowx == 's'){
                cout << "◉";
            }
            else if (nowx == 'r'){
                cout << "★";
            }
            else{
                cout << nowx;
            }
            counter++;
        }
        cout << endl;
    }
}

//Перестроение измененного поля
string builder(int currmove, vector<vector<char> > field1, vector<vector<char> > field2){
    //Неизменная шапка по умолчанию
    string starting = "tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttPLAYERttttttttttttttttt";
    //Стрелка хода
    if(currmove == 0){
        starting += ">>";
    }
    else{
        starting += "<<";
    }
    //Неизменная шапка
    starting += "tttttttttttttttttPCttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttABCDEFGHIJttttttttttttttttttttttttttttttABCDEFGHIJttttttttt";
    //Добавление полей и разметки
    for (int k = 0; k < 10; k++){
        starting += to_string(k);
        for(int i = 0; i < 10; i++){
            starting += field1[k][i];
        }
        starting += to_string(k);
        starting += "tttttttttttttttttttttttttttt";
        starting += to_string(k);
        for(int i = 0; i < 10; i++){
            starting += field2[k][i];
        }
        starting += to_string(k);
        starting += "tttttttt";
    }
    starting += "tABCDEFGHIJttttttttttttttttttttttttttttttABCDEFGHIJttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt";
    return starting;
}

//Получение индекса из букв
int get_letter(string xo){
    char temper = toupper(xo[0]);
    if (temper == 'A'){
        return 0;
    }
    else if(temper == 'B'){
        return 1;
    }
    else if(temper == 'C'){
        return 2;
    }
    else if(temper == 'D'){
        return 3;
    }
    else if(temper == 'E'){
        return 4;
    }
    else if(temper == 'F'){
        return 5;
    }
    else if(temper == 'G'){
        return 6;
    }
    else if(temper == 'H'){
        return 7;
    }
    else if(temper == 'I'){
        return 8;
    }
    else if(temper == 'J'){
        return 9;
    }
    else{
        cout << "WARNING WRONG INPUT";
    }
}

//Конец игры при условии 20 сбитых кораблей
void end_game(){
    if (hitsqbpc == 20){
        //Рисунок победы Игрока
        string x1 = "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrPC WON!rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr";
        disp_call(x1);
        this_thread::sleep_for(chrono::milliseconds(6000));
    }
    else if (hitsqsbplr == 20){
        //Рисунок победы Компьютера
        string x2 = "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrPLAYER  WON!rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr";
        disp_call(x2);
        this_thread::sleep_for(chrono::milliseconds(6000));
    }
}

//Функция замены убитых кораблей
void dead_ship_replace(vector<vector<char> > &polenow, vector<vector<pair<int, int>>> arrofpole){
    //Прогон и проверка полностью сбитых кораблей
    for (int i = 0; i < arrofpole.size(); i++){
        int shipsegs = arrofpole[i].size();
        int hitsegs = 0;
        for(int j = 0; j < arrofpole[i].size(); j++){
            int currx = arrofpole[i][j].first;
            int curry = arrofpole[i][j].second;
            if (polenow[currx][curry] == 'z'){
                hitsegs += 1;
            }
        }
        //Замена в случае равенства поврежденных сегментов корабля и общего их количества
        if(hitsegs == shipsegs){
            for(int k = 0; k < arrofpole[i].size(); k++) {
                int currx = arrofpole[i][k].first;
                int curry = arrofpole[i][k].second;
                polenow[currx][curry] = 'u';
                //Все условия размещения ободка из пустых полей в зависимости от положения корабля
                if(currx == 0 && curry >= 1 && curry <= 8){
                    if(polenow[currx][curry-1] == 'e'){
                        polenow[currx][curry-1] = 'm';
                    }
                    if(polenow[currx+1][curry-1] == 'e'){
                        polenow[currx+1][curry-1] = 'm';
                    }
                    if(polenow[currx+1][curry] == 'e'){
                        polenow[currx+1][curry] = 'm';
                    }
                    if(polenow[currx+1][curry+1] == 'e'){
                        polenow[currx+1][curry+1] = 'm';
                    }
                    if(polenow[currx][curry+1] == 'e'){
                        polenow[currx][curry+1] = 'm';
                    }
                }
                else if(currx == 9 && curry >= 1 && curry <= 8){
                    if(polenow[currx][curry-1] == 'e'){
                        polenow[currx][curry-1] = 'm';
                    }
                    if(polenow[currx-1][curry-1] == 'e'){
                        polenow[currx-1][curry-1] = 'm';
                    }
                    if(polenow[currx-1][curry] == 'e'){
                        polenow[currx-1][curry] = 'm';
                    }
                    if(polenow[currx-1][curry+1] == 'e'){
                        polenow[currx-1][curry+1] = 'm';
                    }
                    if(polenow[currx][curry+1] == 'e'){
                        polenow[currx][curry+1] = 'm';
                    }
                }
                else if(curry == 0 && currx >= 1 && currx <= 8){
                    if(polenow[currx+1][curry] == 'e'){
                        polenow[currx+1][curry] = 'm';
                    }
                    if(polenow[currx+1][curry+1] == 'e'){
                        polenow[currx+1][curry+1] = 'm';
                    }
                    if(polenow[currx][curry+1] == 'e'){
                        polenow[currx][curry+1] = 'm';
                    }
                    if(polenow[currx-1][curry+1] == 'e'){
                        polenow[currx-1][curry+1] = 'm';
                    }
                    if(polenow[currx-1][curry] == 'e'){
                        polenow[currx-1][curry] = 'm';
                    }
                }
                else if(curry == 9 && currx >= 1 && currx <= 8){
                    if(polenow[currx+1][curry] == 'e'){
                        polenow[currx+1][curry] = 'm';
                    }
                    if(polenow[currx+1][curry-1] == 'e'){
                        polenow[currx+1][curry-1] = 'm';
                    }
                    if(polenow[currx][curry-1] == 'e'){
                        polenow[currx][curry-1] = 'm';
                    }
                    if(polenow[currx-1][curry-1] == 'e'){
                        polenow[currx-1][curry-1] = 'm';
                    }
                    if(polenow[currx-1][curry] == 'e'){
                        polenow[currx-1][curry] = 'm';
                    }
                }
                else if(currx == 0 && curry == 0){
                    if(polenow[currx][curry+1] == 'e'){
                        polenow[currx][curry+1] = 'm';
                    }
                    if(polenow[currx+1][curry+1] == 'e'){
                        polenow[currx+1][curry+1] = 'm';
                    }
                    if(polenow[currx+1][curry] == 'e'){
                        polenow[currx+1][curry] = 'm';
                    }
                }
                else if(currx == 0 && curry == 9){
                    if(polenow[currx][curry-1] == 'e'){
                        polenow[currx][curry-1] = 'm';
                    }
                    if(polenow[currx+1][curry-1] == 'e'){
                        polenow[currx+1][curry-1] = 'm';
                    }
                    if(polenow[currx+1][curry] == 'e'){
                        polenow[currx+1][curry] = 'm';
                    }
                }
                else if(currx == 9 && curry == 0){
                    if(polenow[currx+1][curry] == 'e'){
                        polenow[currx+1][curry] = 'm';
                    }
                    if(polenow[currx+1][curry+1] == 'e'){
                        polenow[currx+1][curry+1] = 'm';
                    }
                    if(polenow[currx][curry+1] == 'e'){
                        polenow[currx][curry+1] = 'm';
                    }
                }
                else if(currx == 9 && curry == 9){
                    if(polenow[currx][curry-1] == 'e'){
                        polenow[currx][curry-1] = 'm';
                    }
                    if(polenow[currx-1][curry-1] == 'e'){
                        polenow[currx-1][curry-1] = 'm';
                    }
                    if(polenow[currx-1][curry] == 'e'){
                        polenow[currx-1][curry] = 'm';
                    }
                }
                else if (currx >= 1 && currx <= 8 && curry >= 1 && curry <= 8){
                    if(polenow[currx-1][curry+1] == 'e'){
                        polenow[currx-1][curry+1] = 'm';
                    }
                    if(polenow[currx][curry+1] == 'e'){
                        polenow[currx][curry+1] = 'm';
                    }
                    if(polenow[currx+1][curry+1] == 'e'){
                        polenow[currx+1][curry+1] = 'm';
                    }
                    if(polenow[currx+1][curry] == 'e'){
                        polenow[currx+1][curry] = 'm';
                    }
                    if(polenow[currx+1][curry-1] == 'e'){
                        polenow[currx+1][curry-1] = 'm';
                    }
                    if(polenow[currx][curry-1] == 'e'){
                        polenow[currx][curry-1] = 'm';
                    }
                    if(polenow[currx-1][curry-1] == 'e'){
                        polenow[currx-1][curry-1] = 'm';
                    }
                    if(polenow[currx-1][curry] == 'e'){
                        polenow[currx-1][curry] = 'm';
                    }
                }
            }
        }
    }
}

//Генератор абсолютно случайного поля хода для компьютера
string pc_gen(){
    string abc = "ABCDEFGHIJ";
    string digits = "0123456789";
    string newone;
    random_device rand;
    mt19937 gen(rand());
    uniform_int_distribution<>dis(0, 9);
    int random_number = dis(gen);
    srand(time(0));
    newone += abc[rand() % 10];
    newone += digits[random_number];
    return newone;
}

//Отладочный вывод игрового поля в файл
void output_matrix_to_file(vector<vector<char>> matrix, string filename) {
    ofstream outfile(filename);
    for (vector<char> row : matrix) {
        for (char c : row) {
            outfile << c;
        }
        outfile << endl;
    }
    outfile.close();
}

//Отладочный вывод информации о кораблях в файл
void output_p_matrix_to_file(vector<vector<pair<int, int>>> matrix, string filename) {
    ofstream outfile(filename);
    for (vector<pair<int, int>> row : matrix) {
        for (auto c : row) {
            outfile << "(" << c.first << " " << c.second << ")" << " ";
        }
        outfile << endl;
    }
    outfile.close();
}

//Генерация абсолютно точных попаданий с естественным перебором
string pc_gen_evolved(){
    string abc = "ABCDEFGHIJ";
    string digits = "0123456789";
    string newone;
    random_device rand;
    int index;
    bool newflag = false;
    mt19937 gen(rand());
    uniform_int_distribution<>dis(0, 9);
    int random_number = dis(gen);
    srand(time(0));
    newone += abc[rand() % 10];
    newone += digits[random_number];
    //Генерируем, пока не найдется такой ход, который бы присутствовал в файле с информацией о кораблях Игрока
    //Нахождение численного индекса буквы хода
    auto it = find(abc.begin(), abc.end(), newone[0]);
    //Поиск индекса
    if (it != abc.end()) {
        index = distance(abc.begin(), it);
    }
    //Получение численного индекса хода
    int ff = newone[1] - '0';
    auto aha = make_pair(index, ff);
    //Поиск в матрице положения кораблей
    for (auto& a : arrofplships) {
        for (auto& b : a) {
            if(aha == b){
                newflag = true;
            }
        }
    }
    //Вывод хода при попадании, иначе, повтор функции при неудачной генерации
    if(newflag = true){
        newflag = false;
        return newone;
    }
    else{
        pc_gen_evolved();
    }
}

//Функция хода комптютера с проверкой попадания
void pc_move(){
    //Условие завершения игры, запуск функции конца игры
    if(hitsqsbplr == 20 || hitsqbpc == 20){
        flag = true;
        end_game();
    }
    //Проверка для вывода подходящего сообщения
    if(p2s == 0){
        cout << "PC is thinking" << endl;
    }
    else if(p2s == 1){
        cout << "PC has hit! PC is thinking" << endl;
    }
    //Искуственно созданное ожидания хода Компьютера
    this_thread::sleep_for(chrono::milliseconds(2000));
    string moved;
    //Генерация случайного хода при простой сложности
    if(currdiff == 0){
        moved = pc_gen();
    }
    //Теперь ход будет рандомным лишь в половине случаев
    else if(currdiff == 1){
        random_device rand;
        mt19937 gen(rand());
        uniform_int_distribution<>dis(1, 6);
        int random_number = dis(gen);
        srand(time(0));
        if(random_number % 2 == 0){
            moved = pc_gen();
        }
        else{
            moved = pc_gen_evolved();
        }
    }
    //Каждый 5 из 6 ходов - точное попадание Компьютера, настоящий HARDCORE, вдохновленный шедеврами Хидэтака Миядзаки
    else{
        random_device rand;
        mt19937 gen(rand());
        uniform_int_distribution<>dis(1, 6);
        int random_number = dis(gen);
        srand(time(0));
        if(random_number == 6){
            moved = pc_gen();
        }
        else{
            moved = pc_gen_evolved();
        }
    }
    //Вычленение целого числа из символьного типа
    int z = moved[1] - '0';
    //Случай промаха с переходом хода противнику
    if (pole1[z][get_letter(moved)] == 'e'){
        pole1[z][get_letter(moved)] = 'm';
        currmove = 0;
        p2s = 0;
    }
    //Случай попадания с повтором функции хода
    else if (pole1[z][get_letter(moved)] == 's'){
        pole1[z][get_letter(moved)] = 'z';
        //Замена потопленных кораблей
        dead_ship_replace(pole1, arrofplships);
        dead_ship_replace(pole2, arrofpcships);
        pc_move();
        p2s = 1;
    }
    //Случай ошибочного хода с повторной генерацией хода и выводом сообщения об ошибке
    else if(pole1[z][get_letter(moved)] == 'm' || pole1[z][get_letter(moved)] == 'z' || pole1[z][get_letter(moved)] == 'u'){
        pc_move();
        p2s = 3;
    }
}

//Проверка хода Игрока
void pos_move(){
    if(hitsqsbplr == 20 || hitsqbpc == 20){
        flag = true;
        end_game();
        return;
    }
    if(w2s == 0){
        cout << "Your move: ";
    }
    else{
        cout << endl << "Awesome hit! Your move: ";
    }
    string moved;
    cin >> moved;
    int z = moved[1] - '0';
    if (moved == "qq") {
        //do something like menu or quit the game
    } else if (pole2hidden[z][get_letter(moved)] == 'e') {
        pole2[z][get_letter(moved)] = 'm';
        pole2hidden[z][get_letter(moved)] = 'm';
        x = builder(currmove, pole1, pole2);
        disp_call(x);
        w2s = 0;
        p2s = 0;
        currmove = 1;
    } else if (pole2hidden[z][get_letter(moved)] == 's') {
        pole2[z][get_letter(moved)] = 'z';
        pole2hidden[z][get_letter(moved)] = 'z';
        dead_ship_replace(pole1, arrofplships);
        dead_ship_replace(pole2, arrofpcships);
        x = builder(currmove, pole1, pole2);
        disp_call(x);
        hitsqsbplr += 1;
        w2s = 1;
        pos_move();
    } else if (pole2hidden[z][get_letter(moved)] == 'm' || pole2hidden[z][get_letter(moved)] == 'z') {
        cout << "Wrong move, try again!" << endl;
        pos_move();
    }
}

void call_menu(){
    cout << "1. Новая игра!\n2. Продолжить игру!\n3. Об авторах\n4. Вывести требования\n5. Выход из игры" << endl;
    int chosen = 0;
    cout << "Выберите действие: ";
    cin >> chosen;
    cout << chosen << endl;
    cout << endl;
    switch(chosen){
        case 1: {
            cout << endl << "Ваше имя: ";
            cin >> username;
            cout << endl << "Выберите сложность от 0 до 2: ";
            cin >> currdiff;
            auto start = high_resolution_clock::now();
            flag = false;
            pole1 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
            pole2 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
            pole2hidden = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
            subs1 = "";
            subs2 = "";
            hitsqsbplr = 0;
            hitsqbpc = 0;
            currmove = 0;
            w2s = 0;
            p2s = 0;
            hitsqsbplr = 0;
            hitsqbpc = 0;
            arrofplships.clear();
            arrofpcships.clear();
            currplship.clear();
            currpcship.clear();
            getline(default_field, x);
            get_pole(x);
            currmove = 1;
            randomlyPlaceShips(pole2hidden);
            currmove = 0;
            randomlyPlaceShips(pole1);
            x = builder(0, pole1, pole2);
            disp_call(x);
            //pr_matrix(arrofpcships);
            //pr_matrix(arrofplships);
            while (!flag) {
                if (currmove == 0) {
                    output_matrix_to_file(pole1, "pole1.txt");
                    output_matrix_to_file(pole2, "pole2.txt");
                    output_matrix_to_file(pole2hidden, "pole3.txt");
                    output_p_matrix_to_file(arrofplships, "plshippole.txt");
                    output_p_matrix_to_file(arrofpcships, "pcshippole.txt");
                    pos_move();
                    if(hitsqsbplr == 20 || hitsqbpc == 20){
                        auto stop = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(stop - start);
                        int realdur = (duration.count() / 1000000) - 6;
                        scoreboard << username << " " << realdur << " seconds" << endl;
                        scoreboard.close();
                        call_menu();
                    }
                }
                else {
                    output_matrix_to_file(pole1, "pole1.txt");
                    output_matrix_to_file(pole2, "pole2.txt");
                    output_matrix_to_file(pole2hidden, "pole3.txt");
                    output_p_matrix_to_file(arrofplships, "plshippole.txt");
                    output_p_matrix_to_file(arrofpcships, "pcshippole.txt");
                    x = builder(currmove, pole1, pole2);
                    disp_call(x);
                    pc_move();
                    if(hitsqsbplr == 20 || hitsqbpc == 20){
                        auto stop = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(stop - start);
                        int realdur = (duration.count() / 1000000) - 6;
                        scoreboard << username << " " << realdur << endl;
                        scoreboard.close();
                        call_menu();
                    }
                }
            }
            call_menu();
            return;
        }

        case 2:
            cout << "continue game" << endl;
            call_menu();
            return;
        case 3:
            cout << endl << "Данная игра разработана на языке c++ в рамках выполнения практического задания для группы 8И23. Авторы: Клишина Ксения (8И23 гр.) и Костин Арсений (8Е21 гр.). 2023 г. Томский Политехнический Университет, г. Томск." << endl;
            call_menu();
            return;
        case 4:
            cout << "Данная игра выполняется в консоли. Разрешение окна игры 60*16 пикселей. Сохранение выполняется автоматически." << endl;
            call_menu();
            return;
        case 5:
            exit(0);
        default:
            cout << endl << "Вы ввели некорректное значение, попробуйте снова" << endl;
            call_menu();
    }
}

int main() {
    cout << "Добро пожаловать в Морской Бой!" << endl;
    call_menu();
    return 0;
}
