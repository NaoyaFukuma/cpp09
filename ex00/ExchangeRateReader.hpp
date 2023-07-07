#ifndef EXCHANGE_RATE_READER_H
#define EXCHANGE_RATE_READER_H

#include <string>
#include <map>

class ExchangeRateReader {
public:
    ExchangeRateReader();
    ExchangeRateReader(const ExchangeRateReader& other);
    ExchangeRateReader& operator=(const ExchangeRateReader& other);
    ~ExchangeRateReader();

    bool readFromCSV(const std::string& filename);
    const std::map<std::string, double>& getDataMap() const;

private:
    std::map<std::string, double> data_map_;

    bool validateHeader(const std::string& header);
    bool validateDateFormat(const std::string& date);
    bool validateExchangeRate(const std::string& exchange_rate);
};

#endif // EXCHANGE_RATE_READER_H
