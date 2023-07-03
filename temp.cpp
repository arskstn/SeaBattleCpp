#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<char> > pole1 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
vector<vector<char> > pole2 = {{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'},{'e','e','e','e','e','e','e','e','e','e'}};
string subs1;
string subs2;

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
    std::cout << "Матрица из векторов типа char:\n";
    for(const auto& row: matrix) {
        for(const auto& element: row) {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }
}

int letind(string temper){
    string xo = temper.substr(1,2);
    int baza = stoi(xo.substr(1));
    return baza;
}


int main(){
    string now;
    cin >> now;
    cout << letind(now);

}