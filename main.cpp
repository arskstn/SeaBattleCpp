#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

ifstream fdef("default.txt"); //Текст, с полями по умолчанию
ofstream scoreboard("scoreboard.txt", ios::app);
string x; //Здесь хранится поле внутри кода игры
bool flag = false;
vector<vector<char> > pole1 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
vector<vector<char> > pole2 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
vector<vector<char> > pole2hidden = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
string subs1;
string subs2;

void placeShips(vector<vector<char> >& matrix) {
    srand(time(nullptr));
    int shipLengths[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    for (int i = 0; i < 10; i++) {
        int length = shipLengths[i];
        bool validPlacement = false;
        while (!validPlacement) {
            int x = rand() % (matrix.size() - length + 1);
            int y = rand() % (matrix.size() - length + 1);
            validPlacement = true;
            for (int j = 0; j < length; j++) {
                if (matrix[x+j][y] != 'e' || matrix[x][y+j] != 'e') {
                    validPlacement = false;
                    break;
                }
            }
            if (validPlacement) {
                for (int j = 0; j < length; j++) {
                    if (matrix[x+j][y] == 'e') {
                        matrix[x+j][y] = 's';
                    }
                    if (matrix[x][y+j] == 'e') {
                        matrix[x][y+j] = 's';
                    }
                }
            }
        }
    }
}

void fs_update(string now){
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



void printMatrix(vector<vector<char> > matrix) {
    for(const auto& row: matrix) {
        for(const auto& element: row) {
            cout << element << " ";
        }
        cout << "\n";
    }
}

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

string builder(int currmove, vector<vector<char> > field1, vector<vector<char> > field2){
    string starting = "tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttPLAYERttttttttttttttttt";
    if(currmove == 0){
        starting += "<<";
    }
    else{
        starting += ">>";
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

int letind(string xo){
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

void pos_move(string moved){
    int z = moved[1] - '0';
    if (pole2hidden[z][letind(moved)] == 'e'){
        pole2[z][letind(moved)] = 'm';
    }
    else if (pole2hidden[z][letind(moved)] == 's'){
        pole2[z][letind(moved)] = 'z';
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
            getline(fdef, x);
            disp_call(x);
            fs_update(x);
            placeShips(pole2hidden);
            while (flag == false) {
                int currmove = 0; //0 - player 1 - PC
                string currinput = "";
                if (currmove == 0) {
                    cout << endl << "Ваш ход: ";
                    cin >> currinput;
                    int z = currinput[1] - '0';
                    pos_move(currinput);
                    x = builder(currmove, pole1, pole2);
                    disp_call(x);
                    currmove += 1;
                }
                else {
                    //стрелочка на комп
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
