//
// Created by 2008817
//

#include "Client.hpp"
#include <cmath>
#include <iostream>


Client::Client(std::string name, std::string capital, std::string retirement_date) : name_(name) {
    // Convert the capital from string to double
    capital_ = static_cast<int>(std::stoi(capital));

    // Convert the retirement date from string to struct tm
    retirement_date_.tm_year = static_cast<int>(std::stoi(retirement_date.substr(0,4))) - 1900;
    retirement_date_.tm_mon = static_cast<int>(std::stoi(retirement_date.substr(4,2))) - 1;
    retirement_date_.tm_mday = static_cast<int>(std::stoi(retirement_date.substr(6,2)));
}


void Client::InvestProduct(Product product, std::tm transaction_date, int amount) {
    // Cannot exceed the maximum amount
    if (amount > product.maximum_investment) {
        amount = product.maximum_investment;
    }

    // Do nothing when capital less than minimum investment amount
    if (amount < product.minimum_investment) {
        return;
    }

    // Add the certain amount of product to owned product list
    owned_products_.push_back(std::make_tuple(product, transaction_date, amount));

    // Record this transaction
    TransactionRecord record_temp {transaction_date, product.institution_code, product.product_code, "Invest", amount};
    transaction_record_.push_back(record_temp);

    // Change the balance
    capital_ -= amount;
}


void Client::SellProduct(Product product, std::tm transaction_date) {
    int amount {0};
    std::vector<int> sell_list;

    for (int i {0}; i < owned_products_.size(); ++i) {
        // Extract the information from owned_products_ (a tuple)
        Product ith_product = std::get<0>(owned_products_.at(i));
        std::tm ith_product_transaction_date = std::get<1>(owned_products_.at(i));
        int ith_product_trading_amount = std::get<2>(owned_products_.at(i));

        if (product.product_code == ith_product.product_code) {
            // Convert from AER to monthly gross rate if necessary
            int completed_periods {0};
            double interest_rate {0};
            if (ith_product.annual_or_monthly_interest == "M") {
                // Convert from AER to monthly gross rate
                double n_temp {1.0/12.0};
                interest_rate = pow(1 + ith_product.aer_interest_rate, n_temp) - 1;

                // Calculate the completed month periods
                std::tm last_anniversary_date_tm_ = GetLastAnniversaryDate(ith_product_transaction_date, transaction_date, ith_product.annual_or_monthly_interest);
                completed_periods = GetCompletedPeriod(ith_product_transaction_date, last_anniversary_date_tm_, ith_product.annual_or_monthly_interest);

            } else if (ith_product.annual_or_monthly_interest == "A") {
                // interest rate is AER rate itself
                interest_rate = ith_product.aer_interest_rate;

                // Calculate the completed years
                std::tm last_anniversary_date_tm_ = GetLastAnniversaryDate(ith_product_transaction_date, transaction_date, ith_product.annual_or_monthly_interest);
                completed_periods = GetCompletedPeriod(ith_product_transaction_date, last_anniversary_date_tm_, ith_product.annual_or_monthly_interest);
            }

            /*
             * Products might be invested more than one time in different date
             * the trading amount should be the sum of each previous investment
             */
            int payoff = ith_product_trading_amount * (pow(1 + interest_rate, completed_periods) - 1);
            amount += payoff;

            // Add the gained money to client's capital
            capital_ += payoff;

            // Add the position of this product to sell list (to erase the product from owned_product list)
            sell_list.push_back(i);
        }
    }

    // Erase the product from owned_product list based on sell_list
    if (owned_products_.size() == 1) {
        owned_products_.clear();
    } else {
        for (int i {0}; i < sell_list.size(); ++i) {
            owned_products_.erase(owned_products_.begin() + (sell_list.at(i) - i));
        }
    }

    // Add a transaction record
    TransactionRecord record_temp {transaction_date, product.institution_code, product.product_code, "Sell", amount};
    transaction_record_.push_back(record_temp);
}


void Client::RedeemProduct(Product product, std::tm transaction_date) {
    int amount {0};
    std::vector<int> redeem_list;

    for (int i {0}; i < owned_products_.size(); ++i) {
        // Extract the information from owned_products_ (a tuple)
        Product ith_product = std::get<0>(owned_products_.at(i));
        std::tm ith_product_transaction_date = std::get<1>(owned_products_.at(i));
        int ith_product_trading_amount = std::get<2>(owned_products_.at(i));

        if (product.product_code == ith_product.product_code) {
            // Convert from AER to monthly gross rate if necessary
            int completed_periods {0};
            double interest_rate {0};
            if (ith_product.annual_or_monthly_interest == "M") {
                // Convert from AER to monthly gross rate
                double n_temp {1.0/12.0};
                interest_rate = pow(1 + ith_product.aer_interest_rate, n_temp) - 1;

                // Calculate the completed month periods
                std::tm last_anniversary_date_tm_ = GetLastAnniversaryDate(ith_product_transaction_date, transaction_date, ith_product.annual_or_monthly_interest);
                completed_periods = GetCompletedPeriod(ith_product_transaction_date, last_anniversary_date_tm_, ith_product.annual_or_monthly_interest);

            } else if (ith_product.annual_or_monthly_interest == "A") {
                // interest rate is AER rate itself
                interest_rate = ith_product.aer_interest_rate;

                // Calculate the completed years
                std::tm last_anniversary_date_tm_ = GetLastAnniversaryDate(ith_product_transaction_date, transaction_date, ith_product.annual_or_monthly_interest);
                completed_periods = GetCompletedPeriod(ith_product_transaction_date, last_anniversary_date_tm_, ith_product.annual_or_monthly_interest);
            }

            /*
             * Products might be invested more than one time in different date
             * the trading amount should be the sum of each previous investment
             */
            int payoff = ith_product_trading_amount * (pow(1 + interest_rate, completed_periods) - 1);
            amount += payoff;

            // Add the gained money to client's capital
            capital_ += payoff;

            // Add the position of this product to sell list (to erase the product from owned_product list)
            redeem_list.push_back(i);
        }
    }

    // Erase the product from owned_product list based on sell_list
    if (owned_products_.size() == 1) {
        owned_products_.clear();
    } else {
        for (int i {0}; i < redeem_list.size(); ++i) {
            owned_products_.erase(owned_products_.begin() + (redeem_list.at(i) - i));
        }
    }

    // Add a transaction record
    TransactionRecord record_temp {transaction_date, product.institution_code, product.product_code, "Redeem", amount};
    transaction_record_.push_back(record_temp);
}


int GetCompletedPeriod (std::tm date1, std::tm date2, const std::string& annual_or_monthly_interest) {
    /*
     * As date2 is already last anniversary we can simplify the algorithm
     * and do not have to care about the day
     */
    int result {0};

    // Annual case
    if (annual_or_monthly_interest == "A") {
        // As date2 is last anniversary
        result += (date2.tm_year - date1.tm_year);
    }

    // Monthly case
    if (annual_or_monthly_interest == "M") {
        if (date2.tm_year > date1.tm_year) {
            // 1 year == 12 month
            result += (date2.tm_year - date1.tm_year - 1)*12;
        }
        // The distance (number of months) from transaction date to the end of that year
        result += (11 - date1.tm_mon);
        // The number of months from the start of this year
        result += date2.tm_mon;
    }

    return result;
}