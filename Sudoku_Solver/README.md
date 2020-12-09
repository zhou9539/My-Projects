# Sudoku puzzle solver using backtracking approach

Backtracking method is a type of brute force search. For solving a given Sudoku puzzle, the program firstly finds out all the empty square from 9x9 grid (usually starts with 1st row and 1st column). Then, when reaching a new empty square, the program fills in the possible digit (in the range from 1 to 9) and check whether it fit the rule of sudoku. If the digit is not allowed to be placed in here, the program will backtrack the previous cell and try another digit. If the digit meets Sudoku rules, the program head to the next cell, until the Sudoku puzzle is solved.

## A summary of the algorithm

![image](https://github.com/zhou9539/hello-world/blob/main/Sudoku%20Solver%20-%20Flow%20Chart.png)

## Why backtracking

There are several different approaches to solve the Sudoku puzzle by algorithm, including backtracking, stochastic search, constrain programming and exact cover. To be honest, backtracking might not be the best approach in many aspects, but I decided to use it for the following reasons.

Most importantly, backtracking looks attractive because it is easy to understand and implement. Nevertheless, its performance does not have to be worse than other methods when solving most of Sudoku puzzle. 

Time cost is basically unrelated the difficulty of the Sudoku puzzle. For some approach, like constrain programming, the time cost and the difficulty to implement will rise sharply when the difficulty of Sudoku increasing. I have tried to understand the constrain programming. Frankly, although it seems that I can still implement this approach for simple Sudoku, it is difficult to solve the difficult Sudoku.

Some Sudoku could be not solution or more than one solution, which can be identified using backtracking. When I considered whether to use stochastic search, I found it difficult to identify whether there are multiple solutions for Sudoku (should I stop to "shuffle" the digits when I find the first solution? If not, then when solving a “unproper” Sudoku, the program will run continually and output the same solution.) There might be some way to deal with this problem, but at least I have not found an acceptable solution.

## Algorithm efficiency

I think that the backtracking has a relatively high efficiency when solving Sudoku with fewer empty spaces. However, it has no advantage when solving the Sudoku with many empty spaces, because any one more empty space will greatly increase the number of backtracking, which means perform an extra large number of loops. In addition, even compared to the stochastic search (similar to brute-force approach), when solving some particular Sudoku, backtracking is not dominant because of its own characteristics. For example, some Sudoku with a lot of empty space in the first few rows could take it more time, because every time the program identifies a violation, it needs to backtrack to check the very previous cell. Therefore, backtracking may require more time to solve these special Sudoku puzzles, especially compared with some logic-based algorithms. Users can enter ex1.csv (the sample Sudoku) and ex2.csv (the designed Sudoku for backtracking) and compare the time cost (have to move these csv files to the debug folder manually as the cmake-build-debug will not be submitted).

<div align=center>![image](https://github.com/zhou9539/hello-world/blob/main/Sudoku_puzzle_hard_for_brute_force.svg.png)</div>

Despite these shortcomings, backtracking is still a qualified Sudoku solving approach. It can solve all the “proper” Sudoku puzzle, even some of them cannot be solve logically. Moreover, backtracking can identify “improper” Sudoku and its time cost is independent to the difficulty.
