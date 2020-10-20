//
// Created by Zhou Jiahao on 2020/10/20.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

/*
 * Function:       InputSudoku
 * Description:    This function aim to read data from csv file and write into an array
 * Calls:          None
 * Input:          sudoku - a 9*9 int array to store the Sudoku
 *                 address - a string to save the whole path of the csv file
 * Return:         Return 1 if succeed and return 0 if failed
 */
int InputSudoku(int (*sudoku)[9], const string& address)
{
    ifstream inFile(address, ios::out);

    // We need to give a signal if user input something that cannot be read
    if(inFile.fail())
    {
        cout << "Failed to read the file. Please try again" << "\n" << endl;
        return 0;   // zero means failed to read the file
    }

    string lineStr;
    int i {0}, j;
    char* end;
    while (getline(inFile, lineStr))
    {
        j = 0;    // ensure each row is written from the 1st column
        stringstream ss(lineStr);
        string str;

        // Separated by commas
        while (getline(ss, str, ','))
        {
            sudoku[i][j] = static_cast<int>(strtol(str.c_str(), &end, 9));  // string ---> int
            ++j;    // writing the next column
        }
        ++i;    // writing the next row
    }
    cout << "\n" << "File read successfully" << endl;
    return 1;   // one means file read successfully
}

/*
 * Function:       ShowSudoku
 * Description:    This function aim to print out the whole Sudoku
 * Calls:          None
 * Input:          sudoku - a 9*9 int array to store the Sudoku
 * Return:         None
 */
void ShowSudoku(int (*sudoku)[9])
{
    for (int i {0}; i < 9; ++i)
    {
        for (int j {0}; j < 9; ++j)
            cout << sudoku[i][j] << "  ";
        cout << "\n";
    }
}

/*
 * Function:       GetCoordOfBlank
 * Description:    Get the coordinates of the blank cell and count the number
 * Calls:          None
 * Input:          sudoku - a 9*9 int array to store the Sudoku
 *                 numOfBlank - an int to count the number
 * Return:         Return a vector listing the coordinates of blank
 */
vector<pair<int, int>> GetCoordOfBlank(int (*sudoku)[9], int &numOfBlank)
{
    vector<pair<int, int>> CoordOfBlank;    // more efficient to vector<vector<int>>
    for (int i {0}; i < 9; ++i)
    {
        for (int j {0}; j < 9; ++j)
        {
            // Add coordinates to the list if empty spaces are encountered
            if (0 == sudoku[i][j])
            {
                CoordOfBlank.emplace_back(i, j);    // i, j refer to the rows and columns
                ++numOfBlank;    // recording the number of blank (empty space)
            }
        }
    }
    return CoordOfBlank;
}

/*
 * Function:       FitOrNot
 * Description:    This function aim to check if digit comply with Sudoku rules
 * Calls:          None
 * Input:          digit - the possible digit
 *                 coordinate - a pair of the row and column
 *                 sudoku - a 9*9 array to store the Sudoku
 * Return:         Return zero if the one of the Sudoku rule are not met
 *                 Return one if all Sudoku rules are met
 */
int FitOrNot(int digit, pair<int, int> coordinate, int (*sudoku)[9])
{
    int x {coordinate.first};    // refers to the rows
    int y {coordinate.second};   // refers to the columns

    // I decided to combine the row and column checking command line in a loop to optimize efficiency
    for (int i {0}; i < 9; ++i)
    {
        if (digit == sudoku[x][i])    // check that no digit maybe repeated in any row
            return 0;                 // zero means that the row rule are not met
        if (digit == sudoku[i][y])    // check that no digit maybe repeated in any column
            return 0;                 // zero means that the column rule are not met
    }

    // Ensure the loop only run within the 3 * 3 region
    int X {x/3};    // X can only be 0, 1, 2, Referring to the number of boxes in rows
    int Y {y/3};    // Y can only be 0, 1, 2, Referring to the number of boxes in columns
    for (int i {3 * X}; i < 3 * X + 3; ++i)
    {
        for (int j {3 * Y}; j < 3 * Y + 3; ++j)
        {
            if (digit == sudoku[i][j])    // check that no digit maybe repeated in any 3 * 3 region
                return 0;                 // zero means that the 3 * 3 region rules are not met
        }
    }
    return 1;    // one means all Sudoku rules are met
}

/*
 * Function:       SolveSudoku
 * Description:    This function aim to solve the Sudoku by backtracking
 * Calls:          FitOrNot
 * Input:          sudoku - a 9*9 array to store the Sudoku
 *                 coord - a vector listing the coordinates of blank
 *                 numOfBlank - an int to count the number
 * Return:         Return an int to indicate the result
 *                 There is no solution if the return value less than 0
 *                 Solving process complete if the return value more than the number of blank
 */
int SolveSudoku(int (*sudoku)[9], vector<pair<int, int>> coord, int numOfBlank)
{
    int i {0};    // refers to the number of blank cells checked

    // Less than 0 means no solution
    // More than the number of blank means Sudoku solving has completed
    while (0 <= i && i < numOfBlank)
    {
        int x {coord[i].first};      // refers to the rows
        int y {coord[i].second};     // refers to the columns
        int digit {sudoku[x][y]};    // refers to the digit in that cell
        while(true)
        {
            ++digit;    // search forward
            if (9 >= digit)
            {
                if (FitOrNot(digit, coord[i], sudoku))    // true if all Sudoku rules are met
                {
                    sudoku[x][y] = digit;    // write the digit into Sudoku temporarily
                    ++i;                     // search forward next time
                    break;                   // go ahead to the next i
                } else
                    continue;                // check the next digit
            } else
            {
                sudoku[x][y] = 0;    // Zero the location where the check failed
                --i;                 // search backward next time
                break;               // go back to the previous i
            }
        }
    }
    return i;
}

/*
 * Function:       ShowResult
 * Description:    This function aim to print out the solved Sudoku
 * Calls:          ShowSudoku
 * Input:          sudoku - a 9*9 array to store the Sudoku
 *                 solvedOrNot - an int to indicate whether this Sudoku can be solved
 * Return:         None
 */
void ShowResult(int (*sudoku)[9], int solvedOrNot)
{
    if (0 > solvedOrNot)    // Less than 0 means no solution
        cout << "\nERROR: This Sudoku has no solution!" << endl;
    else
    {
        cout << "The solution is: " << endl;
        ShowSudoku(sudoku);    // print out the solved Sudoku
    }
}
