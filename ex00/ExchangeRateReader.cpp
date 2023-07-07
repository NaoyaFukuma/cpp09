#include "ExchangeRateReader.hpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

ExchangeRateReader::ExchangeRateReader() {}

ExchangeRateReader::ExchangeRateReader(const ExchangeRateReader &other)
    : data_map_(other.data_map_) {}

ExchangeRateReader &
ExchangeRateReader::operator=(const ExchangeRateReader &other) {
  if (this != &other) {
    data_map_ = other.data_map_;
  }
  return *this;
}

ExchangeRateReader::~ExchangeRateReader() {}

bool ExchangeRateReader::readFromCSV(const std::string &filename) {
  std::ifstream file(filename.c_str());
  if (!file.is_open()) {
    std::cerr << "Error: " << filename << " could not open file." << std::endl;
    return false;
  }

  std::string line;
  // ヘッダー行を取得しvalidateする
  std::getline(file, line);
  // ヘッダーフィールドの形式は”date,exchange_rate”
  if (validateHeader(line) == false) {
    return false;
  }

  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::string date_str;

    // 日付を取得
    std::getline(ss, date_str, ',');
    // 日付の形式をvalidateする
    if (validateDateFormat(date_str) == false) {
      std::cerr << "invalid date format: " << date_str << std::endl;
      return false;
    }
    // レートを取得
    std::string exchange_rate_str;
    std::getline(ss, exchange_rate_str);
    // レートの形式をvalidateする
    if (validateExchangeRate(exchange_rate_str) == false) {
      std::cerr << "invalid exchange rate format: " << exchange_rate_str
                << std::endl;
      return false;
    }
    ss.clear();
    ss.str("");
    ss.str(exchange_rate_str);
    double exchange_rate;
    ss >> exchange_rate;
    data_map_.insert(std::make_pair(date_str, exchange_rate));
  }

  return true;
}

const std::map<std::string, double> &ExchangeRateReader::getDataMap() const {
  return data_map_;
}

bool ExchangeRateReader::validateHeader(const std::string &header) {
  return header == "date,exchange_rate";
}

bool ExchangeRateReader::validateDateFormat(const std::string &date) {
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
  std::time_t t = std::time(NULL);
  std::tm *local_time = std::localtime(&t);
  int today_year = local_time->tm_year + 1900;
  int today_month = local_time->tm_mon + 1;
  int today_day = local_time->tm_mday;
  if (year > today_year || (year == today_year && month > today_month) ||
      (year == today_year && month == today_month && day > today_day)) {
    return false;
  }

  // data_map_の最後の日付よりも新しい日付であるかをチェック
  if (!data_map_.empty()) {
    std::string last_date = data_map_.rbegin()->first;
    std::string last_year_str = last_date.substr(0, 4);
    std::string last_month_str = last_date.substr(5, 2);
    std::string last_day_str = last_date.substr(8, 2);
    ss.clear();
    ss.str("");
    ss << last_year_str << " " << last_month_str << " " << last_day_str;
    int last_year, last_month, last_day;
    ss >> last_year >> last_month >> last_day;

    if (year < last_year || (year == last_year && month < last_month) ||
        (year == last_year && month == last_month && day <= last_day)) {
      return false;
    }
  }
  return true;
}

bool ExchangeRateReader::validateExchangeRate(const std::string &value) {
  std::stringstream ss(value);
  double val;
  ss >> val;

  // ssが最後まで読み込まれ、かつ数値が適切な範囲にあるかを確認
  if (!ss.eof() || ss.fail() || val < 0.0) {
    return false;
  }

  return true;
}
