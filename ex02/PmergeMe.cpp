#include "PmergeMe.hpp"
#include <iostream>
#include <ctime>
#include <list>
#include <vector>
#include <algorithm>

PmergeMe::PmergeMe(const std::vector<int>& input) : data_(input) {}

PmergeMe::PmergeMe(const PmergeMe& other) : data_(other.data_) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        data_ = other.data_;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::sortAndDisplay() {
    std::cout << "Before: ";
    for (std::vector<int>::const_iterator it = data_.begin(); it != data_.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    double timeVector = measureTime<std::vector<int> >(std::vector<int>(data_.begin(), data_.end()));
    double timeList = measureTime<std::list<int> >(std::list<int>(data_.begin(), data_.end()));
    double timeVector_StdSort = measureTimeStdSort(std::vector<int>(data_.begin(), data_.end()));
    double timeList_StdSort = measureTimeStdSort(std::list<int>(data_.begin(), data_.end()));

    std::cout << "Time to process a range of " << data_.size() << " elements with std::vector: " << timeVector << " s" << std::endl;
    std::cout << "Time to process a range of " << data_.size() << " elements with std::list: " << timeList << " s" << std::endl;
    std::cout << "Time to process a range of " << data_.size() << " elements with std::vector std::sort(): " << timeVector_StdSort << " s" << std::endl;
    std::cout << "Time to process a range of " << data_.size() << " elements with std::list std::sort(): " << timeList_StdSort << " s" << std::endl;
}

template <typename Container>
Container PmergeMe::mergeInsertSort(Container input) {
    // Base case: if size is 1 or 0, it's already sorted
    if (input.size() <= 1) {
        return input;
    }

    // Divide into pairs and select the larger elements
    Container firstHalf;
    typename Container::iterator it = input.begin();
    while (it != input.end()) {
        typename Container::iterator next_it = it;
        ++next_it;
        if (next_it != input.end()) {
            firstHalf.push_back(std::max(*it, *next_it));
            ++next_it;
        } else {
            firstHalf.push_back(*it);
        }
        it = next_it;
    }

    // Sort the first halves recursively
    firstHalf = mergeInsertSort(firstHalf);

    // Insert the other elements in sorted order
    it = input.begin();
    typename Container::iterator half_it = firstHalf.begin();
    while (half_it != firstHalf.end() && it != input.end()) {
        typename Container::iterator next_it = it;
        ++next_it;
        if (next_it != input.end()) {
            int to_insert = std::min(*it, *next_it);
            typename Container::iterator pos;
            for (pos = firstHalf.begin(); pos != firstHalf.end(); ++pos) {
                if (to_insert < *pos) {
                  break;
                }
            }
            firstHalf.insert(pos, to_insert);
            ++half_it;
        }
        it = next_it;
    }

    return firstHalf;
}


template <typename Container>
double PmergeMe::measureTime(Container container) {
    std::clock_t start = std::clock();
    mergeInsertSort(container);
    std::clock_t end = std::clock();

    std::cout << "After: ";
    for (typename Container::const_iterator it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
    return static_cast<double>(end - start) / CLOCKS_PER_SEC;
}


double PmergeMe::measureTimeStdSort(std::vector<int> container) {
    std::clock_t start = std::clock();
    std::sort(container.begin(), container.end());
    std::clock_t end = std::clock();

    std::cout << "After: ";
    for (std::vector<int>::const_iterator it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
    return static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

double PmergeMe::measureTimeStdSort(std::list<int> container) {
    std::clock_t start = std::clock();
    container.sort();
    std::clock_t end = std::clock();

    std::cout << "After: ";
    for (std::list<int>::const_iterator it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
    return static_cast<double>(end - start) / CLOCKS_PER_SEC;
}
