#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

BitcoinExchange::BitcoinExchange(const std::string &exchangeRateFilename) {
  exchangeRateReader_ = new ExchangeRateReader();
  if (!exchangeRateReader_->readFromCSV(exchangeRateFilename)) {
    delete exchangeRateReader_;
    throw std::runtime_error("Error: Invalid exchange rate file.");
  }
  data_map_ = exchangeRateReader_->getDataMap();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {
  exchangeRateReader_ = new ExchangeRateReader(*(other.exchangeRateReader_));
  data_map_ = other.data_map_;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
  if (this != &other) {
    delete exchangeRateReader_;
    exchangeRateReader_ = new ExchangeRateReader(*(other.exchangeRateReader_));
    data_map_ = other.data_map_;
  }
  return *this;
}

BitcoinExchange::~BitcoinExchange() { delete exchangeRateReader_; }

double BitcoinExchange::getExchangeRate(const std::string &date) const {
  std::map<std::string, double>::const_iterator it = data_map_.upper_bound(date);
  if (it != data_map_.begin()) {
    --it;
    return it->second;
  } else {
    std::cerr << "Error: " << date << " is not found." << std::endl;
    return -1.0;
  }
}

void BitcoinExchange::processInputFile(const std::string &inputFilename) const {
  std::ifstream file(inputFilename.c_str());
  if (!file.is_open()) {
    std::cerr << "Error: " << inputFilename << " could not open file."
              << std::endl;
    return;
  }

  std::string line;
  // ヘッダー行を取得しvalidateする
  std::getline(file, line);
  // ヘッダーフィールドの形式は”date | value”
  if (validateHeader(line) == false) {
    return;
  }

  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::string date_str;

    // 日付を取得
    std::getline(ss, date_str, ' ');
    // 日付の形式をvalidateする
    if (validateDateFormat(date_str) == false) {
      std::cerr << "Error: invalid date format: " << date_str << std::endl;
      continue;;
    }

    // 区切り文字を取得しフォーマットをチェック
    std::string delimiter;
    std::getline(ss, delimiter, ' ');
    if (delimiter != "|") {
      std::cerr << "Error: invalid delimiter format: " << delimiter << std::endl;
      continue;;
    }

    // valueを取得
    std::string value_str;
    std::getline(ss, value_str);
    // valueの形式をvalidateする
    double value;
    if (validateValueFormat(value_str, value) == false) {
      continue;;
    }
    double exchange_rate = getExchangeRate(date_str);
    if (exchange_rate < 0) {
      continue;
    }
    std::cout << date_str << " => " << value << " * " << exchange_rate << " = "
              << exchange_rate* value << std::endl;

  }
}

bool BitcoinExchange::validateHeader(const std::string &line) const {
  if (line != "date | value") {
    std::cerr << "Error: invalid header format: " << line << std::endl;
    return false;
  }
  return true;
}

bool BitcoinExchange::validateDateFormat(const std::string &date) const {
  // フォーマットは”YYYY_MM_DD”

  // 10文字であるかチェック
  if (date.size() != 10) {
    return false;
  }

  // 数字または4文字目と7文字目が'-'で構成されているかチェック
  for (size_t i = 0; i < date.size(); ++i) {
    if (i == 4 || i == 7) {
      if (date[i] != '-') {
        return false;
      }
    } else {
      if (!std::isdigit(date[i])) {
        return false;
      }
    }
  }

  // 年月日が数字に変換してチェック
  std::string year_str = date.substr(0, 4);
  std::string month_str = date.substr(5, 2);
  std::string day_str = date.substr(8, 2);
  // stoi()はC++11から追加された関数である
  // C++98では使えないので、stringstreamを経由する
  std::stringstream ss;
  ss << year_str << " " << month_str << " " << day_str;
  int year, month, day;
  ss >> year >> month >> day;

  // 年月日の範囲が正しいかチェック
  if (year < 0 || month < 1 || month > 12) {
    return false;
  }

  int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  // 閏年の場合
  if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
    days_in_month[1] = 29;
  }

  // 日が月の範囲内にあるかをチェック
  if (day < 1 || day > days_in_month[month - 1]) {
    return false;
  }

  // 未来の日付でないかをチェック
  // 今日の日付を取得
  // c++98の機能しか使えないので、c++11の機能を使わないようにする
  time_t t = time(NULL);
  tm *local_time = localtime(&t);
  int today_year = local_time->tm_year + 1900;
  int today_month = local_time->tm_mon + 1;
  int today_day = local_time->tm_mday;
  if (year > today_year || (year == today_year && month > today_month) ||
      (year == today_year && month == today_month && day > today_day)) {
    return false;
  }

  return true;
}

bool BitcoinExchange::validateValueFormat(const std::string &value_str, double &value_double) const {
  // A valid value must be either a float or a positive integer between 0 and 1000.
  // 0.0以上1000.0以下の値であるかチェック
  std::istringstream ss(value_str);
  ss >> value_double;
  if (value_double < 0.0) {
    std::cerr << "Error: not a positive number: " << value_str << std::endl;
    return false;
  }
  if (value_double > 1000.0) {
    std::cerr << "Error: too large a number: " << value_str << std::endl;
    return false;
  }
  return true;
}
