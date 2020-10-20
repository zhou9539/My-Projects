//
// Created by 2008817
//

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include"Backtracking.hpp"

using namespace std;

int main()
{
     string address;    // avoid repeated declare in the loop

    // Repeatedly run with different puzzles without re-compilation
    while (true)
    {
        cout << "Please enter the full path of the Sudoku (csv file), or enter nothing to exit." << endl;
        getline(cin, address);    // Considering some file names contain space

        if (address.empty())
        {
            cout << "There is not any input, program exits!!!" << endl;
            break;    // We need to provide a way to stop running next puzzles
        } else
        {
            int sudoku[9][9];

            // Writing Sudoku into an 9 * 9 array
            if (InputSudoku(sudoku, address))
            {
                cout << "The sudoku is: " << endl;
                ShowSudoku(sudoku);    // print out the original Sudoku
            } else
            {
                continue;   // ask the user to re-enter
            }

            // I decided to measure the time cost of the Sudoku solving process, dexcluding the file reaing process
            clock_t start = clock();

            int numOfBlank {0};    // refers to the number of empty spaces in the Sudoku
            vector<pair<int, int>> coord = GetCoordOfBlank(sudoku, numOfBlank);    // get coordinates of empty spaces

            // Solve and fill in digits in Sudoku and return a value indicating whether Sudoku has solution
            int SolvedOrNot = SolveSudoku(sudoku, coord, numOfBlank);

            // Users may want to know how many empty spaces there is, but they are too lazy to count
            cout <<  "The number of empty space is " << numOfBlank << "\n";
            // Print out the solved Sudoku if there is a solution
            ShowResult(sudoku, SolvedOrNot);

            // This timekeeper only records the time required to resolve an individual Sudoku
            clock_t finish = clock();
            cout << '\n' << "---------------------------The process takes " << finish - start << " ms" << "\n" << endl;
        }
    }
    return 0;
}