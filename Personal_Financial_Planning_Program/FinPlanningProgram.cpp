//
// Created by 2008817
//

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include "FinPlanningProgram.hpp"
#include "CSVInputReader.hpp"


void FinPlanningProgram::InputClients() {
    // Default filename
    std::string default_filename {"../people.csv"};

    // Ask for the path of file
    std::string filename;
    std::cout << "Please enter the full path of people.csv file, or enter nothing to use default path (../people.csv)" << std::endl;

    // Considering some file names contain space
    getline(std::cin, filename);

    // Use default filename if enter nothing
    if (filename.empty())
    {
        filename = default_filename;
    }

    // Create an input file stream to the input file
    std::shared_ptr<std::ifstream> in_file {std::make_shared<std::ifstream>(filename)};

    // Check it opened okay - fail early if there's a problem.
    if (!in_file->good()){
        std::cerr << "Problem opening the file: " << filename << std::endl;
        //return 1;
    }

    // Create an instance of our CSVInputReader
    CSVInputReader reader {in_file};

    // Remove the title of the columns
    std::string temp;
    std::getline(*in_file, temp);

    // Input clients' information to separate instances of our Client structure
    int i {0};
    while(!in_file->eof()) {
        /*
         *  GetNextField function return one field at a time
         *  when GetNextField get the final field the eof() might still return false
         *  as eof() will return true when reaches one after the final byte
         *  so a extra loop that input noting will start and report error
         */
        std::unique_ptr<std::string> name, capital, retirement_date;
        name = reader.GetNextField();
        if (in_file->eof()) break;    // maybe fails to read anything and sets the eofbit here in the final loop
        capital = reader.GetNextField();
        retirement_date = reader.GetNextField();

        // Construct an instance of Client for each client
        Client client {*name, *capital, *retirement_date};
        // Add the new client to the client list
        clients_list_.push_back(client);

        // Next client
        ++i;
    }
}



void FinPlanningProgram::InputProducts() {
    // Default filename
    std::string default_filename {"../Feed.dat"};

    // Ask for the path of file
    std::string filename;
    std::cout << "Please enter the full path of Feed.dat file, or enter nothing to use default path (../Feed.dat)" << std::endl;

    // Considering some file names contain space
    getline(std::cin, filename);

    // Use default filename if enter nothing
    if (filename.empty())
    {
        filename = default_filename;
    }

    // Create an input file stream to the input file
    std::shared_ptr<std::ifstream> in_file {std::make_shared<std::ifstream>(filename)};

    // Check it opened okay - fail early if there's a problem.
    if (!in_file->good()){
        std::cerr << "Problem opening the file: " << filename << std::endl;
        //return 1;
    }

    // Create an instance of our FixedWidthInputReader
    FixedWidthInputReader reader {in_file};

     // Input product's information to separate instance of Product structure
    int i {0};
    while (!in_file->eof()) {
    // Creat an instance of Product
    Product product {};

    // Input relevant information of one product
    product.institution_code = *reader.GetNextField();
    if (in_file->eof()) break;    // maybe fails to read anything and sets the eofbit here in the final loop
    product.product_code = *reader.GetNextField();
    product.aer_interest_rate = static_cast<double>(std::stod(*reader.GetNextField()) / 10000);
    product.annual_or_monthly_interest = *reader.GetNextField();
    product.tie_in_period = static_cast<int>(std::stoi(*reader.GetNextField()));
    product.minimum_investment = static_cast<int>(std::stod(*reader.GetNextField())*100);    // 1 pound == 100 pence
    product.maximum_investment = static_cast<int>(std::stoi(*reader.GetNextField())*100);    // 1 pound == 100 pence
    product.available_date = DateString2TmStruct(*reader.GetNextField());
    product.maturity_date = DateString2TmStruct(*reader.GetNextField());

    // Add the new product to the product list
    product_list_.push_back(product);

    // Input the next product
    ++i;
    }
}


void FinPlanningProgram::TradingSimulation() {
    // Set the start date (5th April 2020) of the trading simulation
    std::tm simulation_start_date{};
    simulation_start_date.tm_year = 2020 - 1900;    // year 2020
    simulation_start_date.tm_mon = 3;    // April
    simulation_start_date.tm_mday = 5;    // 5th

    // Get the date that the simulation ends (the last client retires)
    std::vector<time_t> retirement_date_list;
    // We convert the retirement date of all clients from std::tm structure to time_t and store it in a list
    for (auto & client : clients_list_)
    {
        std::tm tm_temp {client.GetRetirementDate()};
        retirement_date_list.push_back(mktime(&tm_temp));
    }
    // Then get the date that the last client retires by comparing all the time_t
    std::vector<time_t>::iterator biggest_time_t= std::max_element(std::begin(retirement_date_list), std::end(retirement_date_list));
    int biggest_time_t_position {std::distance(std::begin(retirement_date_list), biggest_time_t)};
    std::tm simulation_end_date = clients_list_[biggest_time_t_position].GetRetirementDate();

    /*
     * Start the simulation here
     * Each loop means one single day between the start date and the ending date
     * 86400 means one day - 86400 seconds (60*60*24)
     *
     * !!!-----------------------------------------------------------------------------------------------------------!!!
     * NOTE: Here I am not simulating every day as it take more than 5 minutes to run in my laptop
     *       and I still can not improve it so far.
     *       In order to make the running time acceptable, I am simulating every 10 days,
     *       which will cause some products cannot be redeemed (as missing the maturity date).
     * !!!-----------------------------------------------------------------------------------------------------------!!!
     */
    time_t start_date {mktime(&simulation_start_date)};
    time_t end_date {mktime(&simulation_end_date)};
    for (time_t simulation_day {start_date}; simulation_day <= end_date; simulation_day += 86400*10) { // can set it daily by delete the number 10 here
        // Convert time_t to tm structure
        std::tm *simulation_day_tm_form_ptr = std::gmtime(&simulation_day);

        // Show the process
        std::cout << "simulation day now: " << simulation_day_tm_form_ptr->tm_year + 1900 << ", "
                    << simulation_day_tm_form_ptr->tm_mon + 1 << ", " << simulation_day_tm_form_ptr->tm_mday << std::endl;

        // Input the available products into available_product_list (vector)
        std::vector<Product> available_product_list;
        for (auto & product : product_list_) {
            time_t product_available_date = mktime(&product.available_date);
            time_t product_maturity_date = mktime(&product.maturity_date);
            if (simulation_day >= product_available_date || simulation_day < product_maturity_date) {
                available_product_list.push_back(product);
            }
        }

        // Operate each client's account every single day
        for (auto & client : clients_list_) {
            // Do nothing if the client has retired
            std::tm retirement_date_temp {client.GetRetirementDate()};
            if (simulation_day > mktime(&retirement_date_temp)) {
                continue;
            }

            // Sell all the owned products when the day that client retire
            if (simulation_day == mktime(&retirement_date_temp)) {
                for (int i {0}; i < client.GetOwnedProducts().size(); ++i) {
                    Product ith_product = std::get<0>(client.GetOwnedProducts().at(i));
                    client.SellProduct(ith_product, *simulation_day_tm_form_ptr);
                }
            }

            /*
             * If the client own some products we need to decide
             * whether sell them or not
             */
            if (!client.GetOwnedProducts().empty()) {
                // consider every owned products
                for (int i {0}; i < client.GetOwnedProducts().size(); ++i) {
                    // Extract the information from owned_products_ (a tuple)
                    Product ith_product = std::get<0>(client.GetOwnedProducts().at(i));
                    std::tm ith_product_transaction_date = std::get<1>(client.GetOwnedProducts().at(i));
                    int ith_product_trading_amount = std::get<2>(client.GetOwnedProducts().at(i));

                    // Redeem the owned products if they reach the maturity
                    time_t maturity_date = mktime(&ith_product.maturity_date);
                    if (simulation_day == maturity_date) {
                        client.RedeemProduct(ith_product, *simulation_day_tm_form_ptr);
                    }

                    // We need to know when can we sell the product
                    time_t ith_product_tie_in_until = mktime(&ith_product_transaction_date) + 86400*365*ith_product.tie_in_period;

                    /*
                     * Compute the last anniversary date
                     * if the simulation day is very close to the last anniversary we can
                     * consider to sell the owned product for higher rate product
                     * in the way we will not lose too much accrued interest
                     */
                    std::tm last_anniversary_date_tm_ = GetLastAnniversaryDate(ith_product_transaction_date, *simulation_day_tm_form_ptr, ith_product.annual_or_monthly_interest);
                    time_t last_anniversary_date = mktime(&last_anniversary_date_tm_);

                    // Sort the available product according to AER interest rate from greater to less
                    std::sort(available_product_list.begin(), available_product_list.end(), AerGreaterSort);

                    // Sell the owned products if better one is available
                    if (simulation_day > ith_product_tie_in_until                 // Product can not be sold within tie-in period
                        && ith_product.aer_interest_rate < available_product_list.at(0).aer_interest_rate    // We do not have to sell if there are not better products
                        ) {
                        /*
                         * For a monthly interest product we can accept
                         * the loss of accrued interest of 10 days for the better products
                         */
                        if (ith_product.annual_or_monthly_interest == "M"
                            && (simulation_day - last_anniversary_date) < 86400*10) {
                            client.SellProduct(ith_product, *simulation_day_tm_form_ptr);
                        }

                        /*
                         * For an annual interest product we can accept
                         * the loss of accrued interest of 30 days for the better products
                         */
                        if (ith_product.annual_or_monthly_interest == "A"
                            && (simulation_day - last_anniversary_date) < 86400*30) {
                            client.SellProduct(ith_product, *simulation_day_tm_form_ptr);
                        }

                    }
                }
            }

            /*
             * Invest the highest AER rate product if there are enough products available
             * and the client still have capital
             */
            if (client.GetCapital() == 0 || available_product_list.empty()) {
                continue;    // Go next client as this client can not invest any product today
            } else {
                /*
                 * We tend to invest the products with shorter tie-in period
                 * when there are only few product available in the market
                 * while we tend to invest the products with higher interest rate
                 * when there are enough products available
                 */
                if (available_product_list.size() < 5) {
                    // Sort the available product according to tie-in period from less to greater
                    std::sort(available_product_list.begin(), available_product_list.end(), TieInPeriodLessSort);

                    // Invest the products in the order of shorter tie-in period to longer
                    for (auto & product : available_product_list) {
                        client.InvestProduct(product, (*simulation_day_tm_form_ptr), client.GetCapital());
                    }
                } else {
                    // Sort the available product according to AER interest rate from greater to less
                    std::sort(available_product_list.begin(), available_product_list.end(), AerGreaterSort);

                    // High interest rate is more attractive in this situation
                    for (auto & product : available_product_list) {
                        client.InvestProduct(product, (*simulation_day_tm_form_ptr), client.GetCapital());
                    }

                }
            }
        }
    }
}


void FinPlanningProgram::OutputResult() {
    std::string folder_path;
    std::string filename;

    // Ask for the path of file
    std::cout << "Select a folder to save the output transaction records (the folder must exist), for example, D:\\output folder" << std::endl;

    // Considering some file names contain space
    getline(std::cin, folder_path);

    for (auto & client : clients_list_) {
        // Default filename
        const std::string  default_filename {"../" + client.GetName() + ".csv"};

        // Use default filename if enter nothing
        if (folder_path.empty())
        {
            filename = default_filename;
        } else {
            filename = folder_path + "\\" + client.GetName() + ".csv";
        }

        // Create an output file stream to the output file
        std::ofstream out_file;

        // Check it opened okay - fail early if there's a problem.
        if (!out_file.good()){
            std::cerr << "Problem opening the file: " << filename << std::endl;
            //return 1;
        }

        //  Open the file and write in information
        out_file.open(filename, std::ios::out);

        // Write in the transaction record in csv file
        for (auto & record : client.GetTransactionRecord()) {
            /*
             * tm_mon and tm_mday are int so we need to add "0"
             * when they less than 10 to keep the form YYYYMMDD
             */
            if (record.transaction_date.tm_mon < 9 && record.transaction_date.tm_mday <10) {
                out_file << record.transaction_date.tm_year + 1900
                         << "0" << record.transaction_date.tm_mon + 1
                         << "0" << record.transaction_date.tm_mday << ","
                         << record.institution_code << ","
                         << record.product_code << ","
                         << record.trading_operation << ","
                         << record.trading_amount << std::endl;
            } else if (record.transaction_date.tm_mon >= 9 && record.transaction_date.tm_mday <10) {
                out_file << record.transaction_date.tm_year + 1900
                         << record.transaction_date.tm_mon + 1
                         << "0" << record.transaction_date.tm_mday << ","
                         << record.institution_code << ","
                         << record.product_code << ","
                         << record.trading_operation << ","
                         << record.trading_amount << std::endl;
            } else if (record.transaction_date.tm_mon < 9 && record.transaction_date.tm_mday >=10) {
                out_file << record.transaction_date.tm_year + 1900
                         << "0" << record.transaction_date.tm_mon + 1
                         << record.transaction_date.tm_mday << ","
                         << record.institution_code << ","
                         << record.product_code << ","
                         << record.trading_operation << ","
                         << record.trading_amount << std::endl;
            } else {
                out_file << record.transaction_date.tm_year + 1900
                         << record.transaction_date.tm_mon + 1
                         << record.transaction_date.tm_mday << ","
                         << record.institution_code << ","
                         << record.product_code << ","
                         << record.trading_operation << ","
                         << record.trading_amount << std::endl;
            }
        }

        // Close the file
        out_file.close();
    }
}


std::tm DateString2TmStruct (std::string s) {
    std::tm result{};
    // Convert the retirement date from string to struct tm
    result.tm_year = static_cast<int>(std::stoi(s.substr(0,4)) - 1900);
    result.tm_mon = static_cast<int>(std::stoi(s.substr(4,2)));
    result.tm_mday = static_cast<int>(std::stoi(s.substr(6,2)));
    return result;
}


double CountDiffDay (time_t t1, time_t t2) {
    double seconds, days;
    seconds = std::difftime(t2, t1);
    days = seconds / 86400;    // one day have 86400 seconds (60*60*24)
    return days;
}


bool AerGreaterSort (Product a, Product b) { return a.aer_interest_rate > b.aer_interest_rate; }


bool TieInPeriodLessSort (Product a, Product b) { return  a.tie_in_period > b.tie_in_period; }


std::tm GetLastAnniversaryDate(std::tm date1, std::tm date2, const std::string& annual_or_monthly_interest) {
    std::tm result {};

    /*
     * In annual case, the month and day of the interest payment date
     * is always equal to the transaction date (date1)
     */
    if (annual_or_monthly_interest == "A") {
        /*
         * Then the year of last anniversary is equal to the year of the current date (date2)
         * if the transaction date (date1) falls before the current date (date2) - only
         * compare the month and day here
         */
        if (date1.tm_mon< date2.tm_mon || (date1.tm_mon == date2.tm_mon && date1.tm_mday <= date2.tm_mday)) {
            result.tm_year = date2.tm_year;
            result.tm_mon = date1.tm_mon;
            result.tm_mday = date1.tm_mday;
            return result;
        }

        /*
         * While the year of last anniversary is one year before the year of current date (date2)
         * if the transaction date (date1) falls after current date (date2) - only compare
         * the month and day here
         */
        if (date1.tm_mon > date2.tm_mon || (date1.tm_mon == date2.tm_mon  && date1.tm_mday > date2.tm_mday)) {
            result.tm_year = date2.tm_year - 1;
            result.tm_mon = date1.tm_mon;
            result.tm_mday = date1.tm_mday;
            return result;
        }
    }

    /*
     * In monthly case, the month and day are depends
     */
    if (annual_or_monthly_interest == "M") {

        if ((date2.tm_mon == 1 && (date2.tm_mday == 28 || date2.tm_mday == 29) && date1.tm_mday > 28)
            || ((date2.tm_mon == 3 || date2.tm_mon == 5 || date2.tm_mon == 8 || date2.tm_mon == 10) && date1.tm_mday > 30)) {
            /*
             * If the monthly anniversary falls after the end of the month
             * interest is paid on the last day of the month
             */
            result = date2;
            return result;
        }
        else if (date1.tm_mday < date2.tm_mday) {
            /*
             * If the day of transaction date is earlier than the current date
             * last anniversary is in this month on the same day of transaction date
             */
            result.tm_year = date2.tm_year;
            result.tm_mon = date2.tm_mon;
            result.tm_mday = date1.tm_mday;
            return result;
        }
        else if (date1.tm_mday > date2.tm_mday) {
            /*
             * If the day of transaction date is later than the current date
             * last anniversary is in last month on the same day of transaction date
             */
            if (date2.tm_mon == 0) {
                // If it is January now then last anniversary falls on December of last year
                result.tm_year = date2.tm_year - 1;
                result.tm_mon = 11;
            } else {
                result.tm_year = date2.tm_year;
                result.tm_mon = date2.tm_mon - 1;
            }
            result.tm_mday = date1.tm_mday;
            return result;
        }
    }

    // return as std::tm
    return result;
}