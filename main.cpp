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
const int SIZE = 10;

void outputMatrixToFile(const std::vector<std::vector<char>>& matrix, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        int rows = matrix.size();
        int cols = matrix[0].size();

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                file << matrix[i][j] << " ";
            }
            file << std::endl;
        }

        file.close();
    } else {
        std::cout << "Невозможно открыть файл " << filename << " для записи." << std::endl;
    }
}

void initializeField(std::vector<std::vector<char>>& field) {
    for (int i = 0; i < SIZE; ++i) {
        std::vector<char> row(SIZE, 'e'); // заполняем каждую клетку пустым полем
        field.push_back(row);
    }
}

bool isValidPlacement(const std::vector<std::vector<char>>& field, int row, int col, int length, bool isVertical) {
    if (isVertical) {
        if (row + length >= SIZE) return false; // проверка, чтобы корабли не выходили за границы поля

        // проверка, чтобы не было пересечений с другими кораблями и корабли не соприкасались
        for (int i = row - 1; i <= row + length; ++i) {
            for (int j = col - 1; j <= col + 1; ++j) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                    if (field[i][j] == 's') return false;
                }
            }
        }
    } else {
        if (col + length >= SIZE) return false; // проверка, чтобы корабль не выходил за границы поля

        // проверка, чтобы не было пересечений с другими кораблями и корабли не соприкасались
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col + length; ++j) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                    if (field[i][j] == 's') return false;
                }
            }
        }
    }

    return true;
}

void placeShip(std::vector<std::vector<char>>& field, int length) {
    bool isVertical = rand() % 2 == 0; // выбираем случайную ориентацию корабля

    int row, col;
    do {
        row = rand() % SIZE;
        col = rand() % SIZE;
    } while (!isValidPlacement(field, row, col, length, isVertical)); // повторяем, пока не найдем подходящее место для корабля

    // расставляем корабль
    if (isVertical) {
        for (int i = row; i < row + length; ++i) {
            field[i][col] = 's';
        }
    } else {
        for (int j = col; j < col + length; ++j) {
            field[row][j] = 's';
        }
    }
}

void randomlyPlaceShips(std::vector<std::vector<char>>& field) {
    std::srand(std::time(0)); // инициализируем генератор случайных чисел

    // расставляем корабли по количеству
    placeShip(field, 4); // 1 корабль из 4 клеток
    placeShip(field, 3); // 2 корабля из 3 клеток
    placeShip(field, 3);
    placeShip(field, 2); // 3 корабля из 2 клеток
    placeShip(field, 2);
    placeShip(field, 2);
    placeShip(field, 1); // 4 корабля из 1 клетки
    placeShip(field, 1);
    placeShip(field, 1);
    placeShip(field, 1);
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
        pole2hidden[z][letind(moved)] = 'z';
    }
    else{
        cout << "wrong move";
    }
}

void replaceDestroyedShips(std::vector<std::vector<char>>& pole2) {
    int rows = pole2.size();
    int columns = pole2[0].size();

    // Проверка горизонтальных последовательностей
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns - 4; j++) {
            if (pole2[i][j] == 'e' && pole2[i][j+1] == 'z' && pole2[i][j+2] == 'z' && pole2[i][j+3] == 'z' && pole2[i][j+4] == 'e') {
                for (int k = j; k <= j + 4; k++) {
                    pole2[i][k] = 'u';
                }
                if (i > 0) pole2[i-1][j] = 'm'; // символ m сверху
                if (i < rows - 1) pole2[i+1][j] = 'm'; // символ m снизу
                if (j > 0 && pole2[i][j-1] != 'u') pole2[i][j-1] = 'm'; // символ m слева
                if (j + 4 < columns - 1 && pole2[i][j+5] != 'u') pole2[i][j+5] = 'm'; // символ m справа
            }
        }
    }

    // Проверка вертикальных последовательностей
    for (int i = 0; i < rows - 4; i++) {
        for (int j = 0; j < columns; j++) {
            if (pole2[i][j] == 'e' && pole2[i+1][j] == 'z' && pole2[i+2][j] == 'z' && pole2[i+3][j] == 'z' && pole2[i+4][j] == 'e') {
                for (int k = i; k <= i + 4; k++) {
                    pole2[k][j] = 'u';
                }
                if (j > 0) pole2[i][j-1] = 'm'; // символ m слева
                if (j < columns - 1) pole2[i][j+1] = 'm'; // символ m справа
                if (i > 0 && pole2[i-1][j] != 'u') pole2[i-1][j] = 'm'; // символ m сверху
                if (i + 4 < rows - 1 && pole2[i+5][j] != 'u') pole2[i+5][j] = 'm'; // символ m снизу
            }
        }
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
            initializeField(pole2hidden);
            //randomlyPlaceShips(pole2hidden);
            //TO BE FIXED
            while (!flag) {
                int currmove = 0; //0 - player 1 - PC
                string currinput;
                if (currmove == 0) {
                    cout << endl << "Ваш ход: ";
                    cin >> currinput;
                    pos_move(currinput);
                    if(true){
                        replaceDestroyedShips(pole2);
                        x = builder(currmove, pole1, pole2);
                        outputMatrixToFile(pole2, "sss.txt");
                        outputMatrixToFile(pole2hidden, "sss1.txt");
                        disp_call(x);
                    }
                    else{
                        replaceDestroyedShips(pole2);
                        x = builder(currmove, pole1, pole2);
                        disp_call(x);
                        currmove = 1;
                    }
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
