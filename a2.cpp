#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>


std::mt19937 mtRand(0xdeaddead);

const int VARIANTS = 150;


bool isSorted(std::vector<int> a) {
    for (int i = 1; i < a.size(); ++i) {
        if (a[i] < a[i-1]) {
            return false;
        }
    } 
    return true;
}


void insertionSort(std::vector<int>& a, int begin, int end) {
    for (int i = begin + 1; i < end; ++i) {
        int j = i;
        while (j > begin && a[j] < a[j-1]) {
            std::swap(a[j-1], a[j]);
            --j;
        }
    }
}


void combinedMergeSort(std::vector<int>& a, int begin, int end, int limit) {
    if (end - begin <= std::max(limit, 1)) {
        insertionSort(a, begin, end);
        return;
    }

    int mid = begin + (end - begin) / 2;
 
    combinedMergeSort(a, begin, mid, limit);
    combinedMergeSort(a, mid, end, limit);
 
    std::vector<int> tmp(end - begin);
 
    int i = 0; 
    int j = 0;
    while (i != mid - begin || j != end - mid) {
        if (i == mid - begin) {
            tmp[i + j] = a[mid + j];
            ++j;
        } else if (j == end - mid) {
            tmp[i + j] = a[begin + i];
            ++i;
        } else {
            if (a[mid + j] < a[begin + i]) {
                tmp[i + j] = a[mid + j];
                ++j;
            } else {
                tmp[i + j] = a[begin + i];
                ++i;
            }
        }
    }
 
    for (int i = 0; i < end - begin; ++i) {
        a[begin + i] = tmp[i];
    }
}


enum Mode {
    RANDOM, REVERSE, ALMOST_SORTED
};


class ArrayGenerator {
private:
    int max_length;
    std::vector<int> array;

    // void shuffle() {
    //     for (int i = 0; i < max_length; ++i) {
    //         int j = mtRand() % max_length;
    //         std::swap(array[i], array[j]);
    //     }
    // }

    void limited_shuffle() {
        for (int k = 0; k < max_length / 20; ++k) {
            int i = mtRand() % max_length;
            int j = mtRand() % max_length;
            std::swap(array[i], array[j]);
        }
    }

public:
    ArrayGenerator() {}

    ArrayGenerator(int _max_length, Mode mode) {
        max_length = _max_length;
        array.resize(max_length);

        for (int i = 0; i < max_length; ++i) {
            array[i] = mtRand() % 6000;
        }

        switch (mode)
        {
        case RANDOM:
            break;

        case REVERSE:
            std::sort(array.rbegin(), array.rend());
            break;

        case ALMOST_SORTED:
            std::sort(array.begin(), array.end());
            limited_shuffle();
            break;
        
        default:
            break;
        }
    }

    std::vector<int> getSubArray(int length) {
        std::vector<int> sub_array(length);
        
        int begin = mtRand() % (max_length - length + 1);

        for (int i = 0; i < length; ++i) {
            sub_array[i] = array[begin + i];
        }

        return sub_array;
    }
};


class SortTester {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    int64_t stop() {
        auto elapsed = std::chrono::high_resolution_clock::now() - start_time;
        int64_t msec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        return msec;
    }
};


int main() {
    std::fstream fout("data.csv", std::ios::out);

    std::vector<ArrayGenerator> rev_gen(VARIANTS, ArrayGenerator(10000, Mode::REVERSE));
    std::vector<ArrayGenerator> rand_gen(VARIANTS, ArrayGenerator(10000, Mode::RANDOM));
    std::vector<ArrayGenerator> as_gen(VARIANTS, ArrayGenerator(10000, Mode::ALMOST_SORTED));

    SortTester timer;

    for (int len = 500; len <= 10000; len += 100) {
        fout << len << ',';

        std::cout << len << '\n';


        const std::vector<int> limits = {1, 10, 50, 100};
        for (auto& sort_limit : limits) {

            int64_t rev_time = 0;
            for (auto& gen : rev_gen) {
                auto arr = gen.getSubArray(len);
                timer.start();
                combinedMergeSort(arr, 0, len, sort_limit); 
                if (!isSorted(arr)) {
                    throw std::runtime_error("array is not sorted");
                }
                rev_time += timer.stop();
            }
            rev_time /= VARIANTS;

            int64_t rand_time = 0;
            for (auto& gen : rand_gen) {
                auto arr = gen.getSubArray(len);
                timer.start();
                combinedMergeSort(arr, 0, len, sort_limit); 
                if (!isSorted(arr)) {
                    throw std::runtime_error("array is not sorted");
                }
                rand_time += timer.stop();
            }
            rand_time /= VARIANTS;

            int64_t as_time = 0;
            for (auto& gen : as_gen) {
                auto arr = gen.getSubArray(len);
                timer.start();
                combinedMergeSort(arr, 0, len, sort_limit); 
                if (!isSorted(arr)) {
                    throw std::runtime_error("array is not sorted");
                }
                as_time += timer.stop();
            }
            as_time /= VARIANTS;


            fout << rev_time << ',' << rand_time << ',' << as_time;
            if (sort_limit != limits.back()) {
                fout << ',';
            }
        }
        fout << "\n";

    }

    return 0;

}
