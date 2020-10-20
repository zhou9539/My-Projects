//
// Created by 2008817
//

#ifndef ASSIGNMENT_2_CLIENT_HPP
#define ASSIGNMENT_2_CLIENT_HPP

#include <string>
#include <vector>
#include <ctime>
#include "FixedWidthInputReader.hpp"


struct Product;
struct TransactionRecord;

class Client {
public:
    // Constructor
    Client() = default;
    Client(std::string name, std::string capital, std::string retirement_date);

    // Functions to get private variable from outside
    std::string GetName() { return name_;}
    int GetCapital() { return capital_;}
    std::tm GetRetirementDate() { return retirement_date_;}
    std::vector<std::tuple<Product, std::tm, int>> GetOwnedProducts() { return owned_products_;}
    std::vector<TransactionRecord> GetTransactionRecord () { return transaction_record_;}

    /*
     * Simulated invest curtain amount of product
     *
     * Args:
     *   Product - the product to invest
     *   std::tm - the transaction date
     *   int - the amount
     * Returns:
     *   None
     */
    void InvestProduct(Product, std::tm, int);

    /*
     * Simulated sell curtain amount of product
     *
     * Args:
     *   Product - the product to sell
     *   std::tm - the transaction date
     */
    void SellProduct(Product, std::tm);

    /*
     * Redeem the product when reach maturity
     *
     * Args:
     *   Product - the product to sell
     *   std::tm - the transaction date
     */
    void RedeemProduct(Product, std::tm);

private:
    // Client's basic information
    std::string name_;
    int capital_ {0};
    std::tm retirement_date_{};

    // Will hold the owned products
    std::vector<std::tuple<Product, std::tm, int>> owned_products_;

    // Will hold every transaction record (invest, sell, redeem)
    std::vector<TransactionRecord> transaction_record_;
};


struct Product {
    std::string institution_code;
    std::string product_code;
    double aer_interest_rate;
    std::string annual_or_monthly_interest;
    int tie_in_period;
    int minimum_investment;
    int maximum_investment;
    std::tm available_date{};
    std::tm maturity_date{};
};

struct TransactionRecord {
    std::tm transaction_date{};
    std::string institution_code;
    std::string product_code;
    std::string trading_operation;
    int trading_amount;
};

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

/*
 * Calculate the completed period after the investment date
 *
 * Args:
 *   tm - the product transaction date - date1
 *   tm - last anniversary - date2
 *   const string& - showing annual or monthly interest product
 * Returns:
 *   int - the completed period
 */
int GetCompletedPeriod (std::tm, std::tm, const std::string&);

#endif //ASSIGNMENT_2_CLIENT_HPP
