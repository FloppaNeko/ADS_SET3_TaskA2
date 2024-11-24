#include <iostream>
#include <vector>


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


int main() {
    int n;
    std::cin >> n;

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    combinedMergeSort(a, 0, n, 15);

    for (int i = 0; i < n; ++i) {
        std::cout << a[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}