#include <vector>
#include <list>

class PmergeMe {
public:
    PmergeMe(const std::vector<int>& input);
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void sortAndDisplay();

private:
    std::vector<int> data_;
    template <typename Container>
    Container mergeInsertSort(Container input);
    template <typename Container>
    double measureTime(Container container);
    double measureTimeStdSort(std::vector<int> container);
    double measureTimeStdSort(std::list<int> container);
};
