#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include "ExchangeRateReader.hpp"
#include <string>
#include <map>

class BitcoinExchange {
public:
    BitcoinExchange(const std::string &exchangeRateFilename);
    BitcoinExchange(const BitcoinExchange &other);
    BitcoinExchange &operator=(const BitcoinExchange &other);
    ~BitcoinExchange();

    double getExchangeRate(const std::string &date) const;
    void processInputFile(const std::string &inputFilename) const;

private:
    ExchangeRateReader *exchangeRateReader_;
    std::map<std::string, double> data_map_;

    bool validateHeader(const std::string &line) const;
    bool validateDateFormat(const std::string &date_str) const;
    bool validateValueFormat(const std::string &value_str,  double &value_double) const;
};

#endif
