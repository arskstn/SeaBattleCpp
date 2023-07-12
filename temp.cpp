#include <iostream>
#include <string>
#include <vector>

using namespace std;


void prmat(vector<vector<char> > matrix){
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

vector<vector<int>> find_ships(vector<vector<char>> pole) {
    vector<vector<int>> ships;
    for (int i = 0; i < pole.size(); i++) {
        for (int j = 0; j < pole[0].size(); j++) {
            if (pole[i][j] == 's') {
                vector<int> ship = {i, j};
                bool is_border_horizontal = true;
                bool is_border_vertical = true;
                for (int di = -1; di <= 1; di++) {
                    if (0 <= i + di < pole.size() && pole[i + di][j] == 's') {
                        is_border_horizontal = false;
                    }
                    if (0 <= j + di < pole[0].size() && pole[i][j + di] == 's') {
                        is_border_vertical = false;
                    }
                }
                if (is_border_horizontal || is_border_vertical) {
                    ships.push_back(ship);
                }
            }
        }
    }
    for (vector<int> ship : ships) {
        for (int i : ship) {
            cout << i << " ";
        }
        cout << endl;
    }
    return ships;
}



int main(){
    prmat(pole);
    vector<vector<int>> ships = find_ships(pole);
    return 0;
}