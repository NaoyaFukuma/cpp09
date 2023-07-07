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

    std::cout << "Time to process a range of " << data_.size() << " elements with std::vector: " << timeVector << " us" << std::endl;
    std::cout << "Time to process a range of " << data_.size() << " elements with std::list: " << timeList << " us" << std::endl;
    std::cout << "Time to process a range of " << data_.size() << " elements with std::vector std::sort(): " << timeVector_StdSort << " us" << std::endl;
    std::cout << "Time to process a range of " << data_.size() << " elements with std::list std::sort(): " << timeList_StdSort << " us" << std::endl;
}

template <typename Container>
void PmergeMe::insertionSort(Container& arr, int begin, int end) {
    typedef typename Container::value_type T;
    typedef typename Container::iterator Iterator;
    Iterator itBegin = arr.begin();
    std::advance(itBegin, begin);
    Iterator itEnd = arr.begin();
    std::advance(itEnd, end);
    for (Iterator i = itBegin; i != itEnd; ++i) {
        T value = *i;
        Iterator j = i;
        Iterator prevJ = j;
        if (j != itBegin) --prevJ;
        while (j != itBegin && *prevJ > value) {
            *j = *prevJ;
            --j;
            if (j != itBegin) {
                --prevJ;
            }
        }
        *j = value;
    }
}

template <typename Container>
Container PmergeMe::mergeInsertSort(Container input) {
    typedef typename Container::value_type ValueType;
    typedef typename Container::iterator IteratorType;

    int numberOfElements = input.size();

    if (numberOfElements <= 1) {
        return input;
    }

    Container largerElements, smallerElements;
    IteratorType inputIterator = input.begin();

    while (inputIterator != input.end()) {
        ValueType firstValue = *inputIterator;
        ++inputIterator;

        if (inputIterator != input.end()) {
            ValueType secondValue = *inputIterator;
            if (firstValue < secondValue) {
                largerElements.push_back(secondValue);
                smallerElements.push_back(firstValue);
            } else {
                largerElements.push_back(firstValue);
                smallerElements.push_back(secondValue);
            }
            ++inputIterator;
        } else {
            largerElements.push_back(firstValue);
        }
    }

    insertionSort(largerElements, 0, largerElements.size());

    // std::cout << "Smaller elements: ";
    // for (IteratorType it = smallerElements.begin(); it != smallerElements.end(); ++it) {
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "Larger elements: ";
    // for (IteratorType it = largerElements.begin(); it != largerElements.end(); ++it) {
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;

    Container sortedResult;
    IteratorType largerElementsIterator = largerElements.begin();
    IteratorType smallerElementsIterator = smallerElements.begin();

    for (int index = 0; index < numberOfElements; ++index) {
        bool useLargerElement;

        // std::cout << "Small: " << *smallerElementsIterator << std::endl;
        // std::cout << "Large: " << *largerElementsIterator << std::endl;

        if (largerElementsIterator == largerElements.end()) {
            useLargerElement = false;
        } else if (smallerElementsIterator == smallerElements.end()) {
            useLargerElement = true;
        } else {
            useLargerElement = *largerElementsIterator < *smallerElementsIterator;
        }

        // std::cout << "Use larger element: " << useLargerElement << std::endl;

        if (useLargerElement) {
            sortedResult.push_back(*largerElementsIterator);
            ++largerElementsIterator;
        } else {
            IteratorType positionToInsert = std::lower_bound(sortedResult.begin(), sortedResult.end(), *smallerElementsIterator);
            sortedResult.insert(positionToInsert, *smallerElementsIterator);
            ++smallerElementsIterator;
        }
    }

    return sortedResult;
}




template <typename Container>
double PmergeMe::measureTime(Container container) {
    std::clock_t start = std::clock();
    Container result = mergeInsertSort(container);
    std::clock_t end = std::clock();

    std::cout << "After: ";
    for (typename Container::const_iterator it = result.begin(); it != result.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
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

    return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
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

    return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
}
