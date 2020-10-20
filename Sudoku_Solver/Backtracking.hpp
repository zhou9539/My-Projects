//
// Created by Zhou Jiahao on 2020/10/20.
//

#ifndef INC_2008817_BACKTRACKING_HPP
#define INC_2008817_BACKTRACKING_HPP


#include <vector>
#include <string>

using namespace std;

int InputSudoku(int (*sudoku)[9], const string& address);
void ShowSudoku(int (*sudoku)[9]);
vector<pair<int, int>> GetCoordOfBlank(int (*sudoku)[9], int& numOfBlank);
int FitOrNot(int digit, pair<int, int> coordinate, int (*sudoku)[9]);
int SolveSudoku(int (*sudoku)[9], vector<pair<int, int>> coord, int numOfBlank);
void ShowResult(int (*sudoku)[9], int solvedOrNot);


#endif //INC_2008817_BACKTRACKING_HPP
