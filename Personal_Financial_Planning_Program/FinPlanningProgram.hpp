//
// Created by 2008817
//

#ifndef ASSIGNMENT_2_FINPLANNINGPROGRAM_HPP
#define ASSIGNMENT_2_FINPLANNINGPROGRAM_HPP

#include <string>
#include <vector>
#include "Client.hpp"


class FinPlanningProgram {
public:
    // Constructor
    FinPlanningProgram() = default;

    // Input clients' information from csv file then save it as private variable of Class Client
    void InputClients();

    // Input products information from dat file then save as Product structure
    void InputProducts();

    // Do a trading simulation from 5th April 2020
    void TradingSimulation();

    // Output a csv file containing transaction records of simulation for each client
    void OutputResult();

private:
    std::vector<Client> clients_list_;    // Will hold  all client's account (instances of Client)
    std::vector<Product> product_list_;   // Will hold  all product information (instances of Product structure)
};


/*
 * Convert the date string in fix width form to tm structure
 *
 * Args:
 *   string - date string YYYYMMDD
 * Returns:
 *   tm - a tm structure with 3 elements: tm_year, tm_mon, tm_mday
 */
std::tm DateString2TmStruct (std::string);


/*
 * Count how many days between two dates
 *
 * Args:
 *   time_t - the first date
 *   time_t - the second date
 * Returns:
 *   double - the number of day between them
 */
double CountDiffDay (time_t, time_t);


/*
 * Designed a method to greater sort the products according to AER interest rate
 *
 * Args:
 *   Product - the former one
 *   Product - the later one
 * Returns:
 *   bool - true true if the former's AER interest rate greater than the later
 */
bool AerGreaterSort (Product, Product);


/*
 * Designed a method to less sort the products according to tie-in period
 *
 * Args:
 *   Product - the former one
 *   Product - the later one
 * Returns:
 *   bool - true true if the former's tie-in period less than the later
 */
bool TieInPeriodLessSort (Product, Product);


/*
 * Calculate the the previously closest interest payment date
 *
 * Args:
 *   tm - the product transaction date - date1
 *   tm - current date (simulation_day) - date2
 *   string - "A" means annual interest payment
 *            "M: means monthly interest payment
 * Returns:
 *   std::tm - the previous closest interest payment date
 */
std::tm GetLastAnniversaryDate(std::tm, std::tm, const std::string&);


#endif //ASSIGNMENT_2_FINPLANNINGPROGRAM_HPP
