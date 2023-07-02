#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

ifstream fdef("default.txt"); //Текст, с полями по умолчанию
ofstream sboard("scoreboard.txt");//Текст, с таблицей рекордов
ofstream sboard1("currtime.txt");
fstream fnow("current.txt"); //Текст, в котором происходит ход игры.
string x; //Здесь хранится поле внутри кода игры
bool flag = false;
vector<vector<char> > pole1;
vector<vector<char> > pole2;
string subs1;
string subs2;

void print_field(vector<vector<char> > ppole){
    sboard << endl;
    for(const auto& row: ppole) {
        for(const auto& element: row) {
            sboard << element << " ";
        }
        sboard << "\n";
    }
    sboard.close();
}

void fs_update(string now){
    for(int i = 0; i < 10; i++){
        int s1 = 245;
        int s2 = 285;
        subs1 = x.substr(s1,10);
        subs2 = x.substr(s2, 10);
        sboard << subs1 << endl;
        sboard1 << subs2 << endl;
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
            //fs_update(x);
            //print_field(pole1);



            while (flag == false) {
                int currmove = 0; //0 - player 1 - PC
                string currinput = "";
                if (currmove == 0) {
                    //стрелочка на игрока
                    cout << endl << "Ваш ход: ";
                    cin >> currinput;
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
            cout << "Данная игра выполняется в консоли. Разрешение окна игры 60*20 пикселей. Сохранение происходит каждые 20 секунд автоматически." << endl;
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
