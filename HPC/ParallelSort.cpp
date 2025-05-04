#include<iostream>
#include<omp.h>
#include<chrono>
#include<vector>

using namespace std;
using namespace std::chrono;

void bubblesort(vector<int> &arr)
{
    int n=arr.size();
    for(int i=n-1;i>=0;i--)
    {
        for(int j=0;j<=i-1;j++)
        {
            if(arr[j]>arr[j+1])
            swap(arr[j],arr[j+1]);
        }
    }
}

void parallel_bubblesort(vector<int> &arr)
{
    int n=arr.size();
    for(int i=n-1;i>=0;i--)
    {
        #pragma omp parallel for
        for(int j=0;j<=i-1;j++)
        {
            if(arr[j]>arr[j+1])
            swap(arr[j],arr[j+1]);
        }
    }
}

void merge(vector<int>&arr, int low, int mid ,int high)
{
    int n=arr.size();
    vector<int>res(n);
    int left=low;
    int right=mid+1;
    while(left<=mid && right<=high)
    {
        if(arr[left]<=arr[right])
        {
            res.push_back(arr[left]);
            left++;
        }

        else{
            res.push_back(arr[right]);
            right++;
        }

    }
    //remaining elements
    while(left<=mid)
    {
        res.push_back(arr[left]);
        left++;
    }

    while(right<=high)
    {
        res.push_back(arr[right]);
        right++;
    }

    for(int i=0;i<n;i++)
    {
        arr[i]=res[i-low];
    }

}

void mergeSort(vector<int>&arr, int low, int high)
{
    if(low>=high)
    return;

    if(low<high)
    {
        int mid= (low+high)/2;

        mergeSort(arr,low,mid);
        mergeSort(arr,mid+1,high);

        merge(arr,low,mid,high);
    }
}

void parallel_mergeSort(vector<int>&arr, int low, int high, int depth)
{
    if(low>=high)
    return;

    if(low<high)
    {
        int mid= (low+high)/2;

        if((high-low)>1000 && depth<4)
        {
        #pragma omp parallel sections
        {
            #pragma omp parallel section
            {
                parallel_mergeSort(arr,low,mid,depth+1);
            }

            #pragma omp parallel section
            {
                parallel_mergeSort(arr,mid+1,high,depth+1);
            }
        }
    }

    else{
        mergeSort(arr,low,mid);
        mergeSort(arr,mid+1,high);

    }
        
        merge(arr,low,mid,high);
    }
}


int main()
{

    const int size= 10000;
    vector<int> base(size);
    for(int i=0;i<size;i++)
    {
        base[i]= rand()%1000;
    }

    vector<int> arr=base;

    //Sequential Bubble Sort
    auto start= high_resolution_clock::now();
    bubblesort(arr);
    auto end= high_resolution_clock::now();
    auto seq_bubblesort= duration_cast<milliseconds>(end-start);


    //Parallel Bubble Sort
    arr=base;
    auto start=high_resolution_clock::now();
    parallel_bubblesort(arr);
    auto end= high_resolution_clock::now();
    auto par_bubblesort= duration_cast<milliseconds>(end-start);

    //Sequential Merge Sort
    arr=base;
    auto start=high_resolution_clock::now();
    mergeSort(arr,0, arr.size()-1);
    auto end= high_resolution_clock::now();
    auto seq_ms= duration_cast<milliseconds>(end-start);

    // Parallel Merge Sort
    arr=base;
    omp_set_num_threads(4);
    auto start=high_resolution_clock::now();
    parallel_mergeSort(arr,0,arr.size()-1,0);
    auto end=high_resolution_clock::now();
    auto para_ms= duration_cast<milliseconds>(end-start);

    cout<<"SORT and THEIR TIME (in milliseconds)"<<endl;
    cout<<"Sequential Bubble Sort= "<<seq_bubblesort.count()<<endl;
    cout<<"Parallel Bubble Sort= "<<par_bubblesort.count()<<endl;
    cout<<"Sequential Merge Sort= "<<seq_ms.count()<<endl;
    cout<<"Paralle Merge Sort= "<<para_ms.count()<<endl;

    

    return 0;

}