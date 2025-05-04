#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

void bubbleSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

void mergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void parallelMergeSort(vector<int> &arr, int l, int r, int depth = 0)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        // Only parallelize if the array is large enough and depth is low
        if ((r - l) > 1000 && depth < 4)
        {
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, l, m, depth + 1);

                #pragma omp section
                parallelMergeSort(arr, m + 1, r, depth + 1);
            }
        }
        else
        {
            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

int main()
{
    const int size = 10000;
    vector<int> base(size);
    for (int i = 0; i < size; ++i)
        base[i] = rand() % 1000;

    vector<int> arr = base;

    // Sequential Bubble Sort
    auto start = high_resolution_clock::now();
    bubbleSort(arr);
    auto stop = high_resolution_clock::now();
    auto seq_bubble = duration_cast<milliseconds>(stop - start);

    // Sequential Merge Sort
    arr = base;
    start = high_resolution_clock::now();
    mergeSort(arr, 0, size - 1);
    stop = high_resolution_clock::now();
    auto seq_merge = duration_cast<milliseconds>(stop - start);

    // Parallel Merge Sort
    arr = base;
    omp_set_num_threads(4); // You can adjust this
    start = high_resolution_clock::now();
    parallelMergeSort(arr, 0, size - 1);
    stop = high_resolution_clock::now();
    auto par_merge = duration_cast<milliseconds>(stop - start);

    cout << "Sequential Bubble Sort Time: " << seq_bubble.count() << " ms" << endl;
    cout << "Sequential Merge Sort Time: " << seq_merge.count() << " ms" << endl;
    cout << "Parallel Merge Sort Time: " << par_merge.count() << " ms" << endl;

    return 0;
}
