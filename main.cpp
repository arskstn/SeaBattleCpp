#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <chrono>
#include <thread>

/*
 * 1. FIX messages about PC hits and misses
 * 2. Finish working on ship replacement function
 */

using namespace std;

ifstream default_field("default.txt"); //Default field position
ofstream scoreboard("scoreboard.txt", ios::app);
fstream current_field("current.txt"); //Save file for current game position
string x; //In-game variable for field positions
bool flag = false; //Is the game over yet
bool toRepl = false; //Part of dead ship replacement function
vector<vector<char> > pole1 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
vector<vector<char> > pole2 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
vector<vector<char> > pole2hidden = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
string subs1;
string subs2;
const int SIZE = 10;
string result = "";
int hitsqsbplr = 0;
int hitsqbpc = 0;
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

void pr_matrix(vector<vector<pair<int, int>>> z){
    for (int i = 0; i < z.size(); i++){
        for(int j = 0; j < z[i].size(); j++){
            cout << z[i][j].first << " " << z[i][j].second << ";";
        }
        cout << endl;
    }
}

//Заполнение поля пустыми клетками
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

//Вычленение из now положение на поле
void get_pole(string now){
    int s1 = 245;
    int s2 = 285;
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
                cout << "·";
            }
            else if (nowx == 'z'){
                cout<<"*";
            }
            else if (nowx == 'u'){
                cout<<"X";
            }
            else if (nowx == 'm'){
                cout << "о";
            }
            else{
                cout << nowx;
            }
            counter++;
        }
        cout << endl;
    }
}

//Построение измененного поля
string builder(int currmove, vector<vector<char> > field1, vector<vector<char> > field2){
    string starting = "tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttPLAYERttttttttttttttttt";
    if(currmove == 0){
        starting += ">>";
    }
    else{
        starting += "<<";
    }
    starting += "tttttttttttttttttPCttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttABCDEFGHIJttttttttttttttttttttttttttttttABCDEFGHIJttttttttt";
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

void end_game(){
    if (hitsqbpc == 20){
        cout << "PC WON!";
    }
    else if (hitsqsbplr == 20){
        cout << "PLAYER WON!";
    }
}


void dead_ship_replace(vector<vector<char> > &polenow, vector<vector<pair<int, int>>> arrofpole){
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
        if(hitsegs == shipsegs){
            for(int k = 0; k < arrofpole[i].size(); k++) {
                int currx = arrofpole[i][k].first;
                int curry = arrofpole[i][k].second;
                polenow[currx][curry] = 'u';
                try{
                    if(polenow[currx-1][curry+1] == 'e'){
                        polenow[currx-1][curry+1] = 'm';
                    }
                }
                catch(...){
                    continue;
                }
                try{
                    if(polenow[currx][curry+1] == 'e'){
                        polenow[currx][curry+1] = 'm';
                    }
                }
                catch(...){
                    continue;
                }
                try{
                    if(polenow[currx+1][curry+1] == 'e'){
                        polenow[currx+1][curry+1] = 'm';
                    }
                }
                catch(...){
                    continue;
                }
                try{
                    if(polenow[currx+1][curry] == 'e'){
                        polenow[currx+1][curry] = 'm';
                    }
                }
                catch(...){
                    continue;
                }
                try{
                    if(polenow[currx+1][curry-1] == 'e'){
                        polenow[currx+1][curry-1] = 'm';
                    }
                }
                catch(...){
                    continue;
                }
                try{
                    if(polenow[currx][curry-1] == 'e'){
                        polenow[currx][curry-1] = 'm';
                    }
                }
                catch(...){
                    continue;
                }
                try{
                    if(polenow[currx-1][curry-1] == 'e'){
                        polenow[currx-1][curry-1] = 'm';
                    }
                }
                catch(...){
                    continue;
                }
                try{
                    if(polenow[currx-1][curry] == 'e'){
                        polenow[currx-1][curry] = 'm';
                    }
                }
                catch(...){
                    continue;
                }
            }
        }
    }
}

/*
void dead_ship_replace(vector<vector<char>>& polenow, vector<vector<pair<int, int>>> arrofpole) {
    for (int i = 0; i < arrofpole.size(); i++) {
        int shipsegs = arrofpole[i].size();
        int hitsegs = 0;
        for (int j = 0; j < arrofpole[i].size(); j++) {
            int currx = arrofpole[i][j].first;
            int curry = arrofpole[i][j].second;
            if (polenow[currx][curry] == 'z') {
                hitsegs += 1;
            }
        }
        if (hitsegs == shipsegs) {
            cout << "WOWZA!WOWZA!WOWZA!WOWZA!WOWZA!WOWZA!WOWZA!" << endl;
            for (int k = 0; k < arrofpole[i].size(); k++) {
                int currx = arrofpole[i][k].first;
                int curry = arrofpole[i][k].second;
                polenow[currx][curry] = 'u';

                // Draw a one block border around the hit ship
                if (k == 0) {
                    polenow[currx][curry - 1] = 'm';
                } else if (k == arrofpole[i].size() - 1) {
                    polenow[currx][curry + 1] = 'm';
                } else {
                    polenow[currx][curry - 1] = 'm';
                    polenow[currx][curry + 1] = 'm';
                }

                // Check if the ship is vertical
                if (arrofpole[i][0].first == arrofpole[i][1].first) {
                    // Draw a one block border below the ship
                    for (int m = curry; m < curry + shipsegs; m++) {
                        polenow[currx][m] = 'm';
                    }
                } else {
                    // Draw a one block border to the right of the ship
                    for (int m = currx; m < currx + shipsegs; m++) {
                        polenow[m][curry] = 'm';
                    }
                }
            }
        }
    }
}
*/
//Проверка хода
void pos_move(){
    if(w2s == 0){
        cout << "Your move: ";
    }
    else{
        cout << endl << "Awesome hit! Your move: ";
    }

    string moved;
    if(hitsqsbplr == 20 || hitsqbpc == 20){
        flag = true;
        end_game();
    }
    else{
        cin >> moved;
        int z = moved[1] - '0';
        if(moved == "qq"){
            //do something like menu or quit the game
        }
        else if (pole2hidden[z][get_letter(moved)] == 'e'){
            pole2[z][get_letter(moved)] = 'm';
            pole2hidden[z][get_letter(moved)] = 'm';
            x = builder(currmove, pole1, pole2);
            disp_call(x);
            w2s = 0;
            p2s = 0;
            currmove = 1;
        }
        else if (pole2hidden[z][get_letter(moved)] == 's'){
            pole2[z][get_letter(moved)] = 'z';
            pole2hidden[z][get_letter(moved)] = 'z';
            dead_ship_replace(pole1, arrofplships);
            dead_ship_replace(pole2, arrofpcships);
            x = builder(currmove, pole1, pole2);
            disp_call(x);
            hitsqsbplr += 1;
            w2s = 1;
            pos_move();
        }
        else if (pole2hidden[z][get_letter(moved)] == 'm' || pole2hidden[z][get_letter(moved)] == 'z'){
            cout << "Wrong move, try again!" << endl;
            pos_move();
        }
    }
}

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

void pc_move(){
    if(p2s == 0){
        cout << "You've missed! PC is thinking" << endl;
    }
    else if(p2s == 1){
        cout << "PC has hit! PC is thinking" << endl;
    }

    this_thread::sleep_for(chrono::milliseconds(2000));
    string moved;
    moved = pc_gen();
    int z = moved[1] - '0';
    if (pole1[z][get_letter(moved)] == 'e'){
        pole1[z][get_letter(moved)] = 'm';
        currmove = 0;
        p2s = 0;
    }
    else if (pole1[z][get_letter(moved)] == 's'){
        pole1[z][get_letter(moved)] = 'z';
        dead_ship_replace(pole1, arrofplships);
        dead_ship_replace(pole2, arrofpcships);
        pc_move();
        p2s = 1;
    }
    else if(pole1[z][get_letter(moved)] == 'm' || pole1[z][get_letter(moved)] == 'z' || pole1[z][get_letter(moved)] == 'u'){
        pc_move();
        p2s = 3;
    }
}



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


void call_menu(){
    cout << "1. Новая игра!\n2. Продолжить игру!\n3. Об авторах\n4. Вывести требования\n5. Выход из игры" << endl;
    int chosen = 0;
    cout << "Выберите действие: ";
    cin >> chosen;
    cout << chosen << endl;
    cout << endl;
    switch(chosen){
        case 1: {
            getline(default_field, x);
            get_pole(x);
            currmove = 1;
            //start_pole(pole2hidden);
            randomlyPlaceShips(pole2hidden);
            currmove = 0;
            start_pole(pole1);
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
                    x = builder(currmove, pole1, pole2);
                    disp_call(x);
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
                    x = builder(currmove, pole1, pole2);
                    disp_call(x);
                }
            }
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
            cout << "Данная игра выполняется в консоли. Разрешение окна игры 60*16 пикселей. Сохранение выполняется пользователем по команде 'save'." << endl;
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
