//
// Created by 2008817
//

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include "FinPlanningProgram.hpp"


void GetTheLastCompletedPeriod ();

int main() {
    // creat an instance our Financial Planning Program to work for a group of clients
    FinPlanningProgram fin_planning_program;

    // Input clients' information
    fin_planning_program.InputClients();

    // Input products' information
    fin_planning_program.InputProducts();

    // We want to know how much time it take to do a simulation
    clock_t start = clock();

    // Do the trading simulation
    fin_planning_program.TradingSimulation();

    // Timekeeper stop here
    clock_t finish = clock();
    std::cout << '\n' << "---------------------------The process takes " << (finish - start)/CLOCKS_PER_SEC << " s" << "\n" << std::endl;

    // Output the transaction records of each client to csv file
    fin_planning_program.OutputResult();

    return 0;
}