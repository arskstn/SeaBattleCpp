#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

ifstream fdef("default.txt"); //Текст, с полями по умолчанию
ofstream scoreboard("scoreboard.txt", ios::app);
fstream fnow("current.txt"); //Текст, в котором происходит ход игры.
string x; //Здесь хранится поле внутри кода игры
bool flag = false;
vector<vector<char> > pole1;
vector<vector<char> > pole2;
string subs1;
string subs2;

void fs_update(string now){
    int s1 = 245;
    int s2 = 285;
    for(int i = 0; i < 10; i++){
        subs1 = x.substr(s1,10);
        subs2 = x.substr(s2, 10);
        for(int j = 0; j< subs1.size(); j++){
            pole1[i][j] = subs1[j];
        }
        for(int j = 0; j< subs2.size(); j++){
            pole2[i][j] = subs2[j];
        }
        s1 += 60;
        s2 += 60;
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
            else if (nowx == 's'){
                cout<<"*";
            }
            else if (nowx == 'u'){
                cout<<"X";
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
    starting += "tttttttttttttttttPCttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttABCDEFGJIKttttttttttttttttttttttttttttttABCDEFGJIKttttttttt";
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
    starting += "tABCDEFGJIKttttttttttttttttttttttttttttttABCDEFGJIKttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt";
    string ah = to_string(starting.size());
    return ah;
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
            while (flag == false) {
                int currmove = 0; //0 - player 1 - PC
                string currinput = "";
                if (currmove == 0) {
                    //disp_call(x);
                    cout << endl << "Ваш ход: ";
                    cin >> currinput;
                    cout << builder(currmove, pole1, pole2);
                    exit(0);

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
