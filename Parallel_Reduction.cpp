#include<iostream>
#include<omp.h>
#include<chrono>
#include<vector>

using namespace std;
using namespace std::chrono;

long long arr_max(vector<long long > arr)
{
    long long maximum=arr[0];
    #pragma omp parallel for reduction(max:maximum)
    for(long long i=0;i<arr.size();i++)
    {
        if(arr[i]>maximum)
        maximum=arr[i];
    }

    return maximum;
}

long long arr_max_seq(vector<long long> arr)
{
    long long maximum=arr[0];
    for(long long i=0;i<arr.size();i++)
    {
        if(arr[i]>maximum)
        maximum=arr[i];
    }
    return maximum;

}

long long arr_min(vector<long long > arr)
{
    long long minimum=arr[0];
    #pragma omp parallel for reduction(min:minimum)
    for(long long i=0;i<arr.size();i++)
    {
        if(arr[i]<minimum)
        minimum=arr[i];
    }

    return minimum;
}

long long arr_min_seq(vector<long long> arr)
{
    long long minimum=arr[0];
    for(long long i=0;i<arr.size();i++)
    {
        if(arr[i]<minimum)
        minimum=arr[i];
    }
    return minimum;
}

long long arr_sum(vector<long long>arr)
{
    long long sum=0;
    #pragma omp parallel for reduction(+:sum)
    for(long long i=0;i<arr.size();i++)
    {
        sum+=arr[i];
    }
    return sum;
}

long long arr_sum_seq(vector<long long>arr)
{
    long long sum=0;
    //#pragma omp parallel for reduction(+:sum)
    for(long long i=0;i<arr.size();i++)
    {
        sum+=arr[i];
    }
    return sum;
}

double arr_avg(vector<long long> arr)
{
    long long sum = 0;
    
    #pragma omp parallel for reduction(+ : sum)
    for (long long i = 0; i < arr.size(); i++)
    {
        sum += arr[i];
    }
    double avg = sum / arr.size();
    return avg;
}

double arr_avg_seq(vector<long long> arr)
{
    long long sum = 0;
    
    #pragma omp parallel for reduction(+ : sum)
    for (long long i = 0; i < arr.size(); i++)
    {
        sum += arr[i];
    }
    double avg = sum / arr.size();
    return avg;
}

int main()
{
    vector<long long>arr={1,2,3,4,5,7,2,6,0};
    long long N=90000000;

    for(long long i=0;i<N;i++)
    {
        arr.push_back(rand()%1000);
    }

    cout<<"Operations: Sequential and Parallel Reduction: "<<endl;
    cout<<"1. Minimum"<<endl;
   
    auto start= high_resolution_clock::now();
    long long res= arr_min_seq(arr);
    auto end= high_resolution_clock::now();
    cout<<"Minimum = "<<res<<endl;
    auto t= duration_cast<milliseconds>(end-start);
    cout<<"Time sequential= "<<t.count()<<endl;

    start= high_resolution_clock::now();
    res= arr_min(arr);
    end= high_resolution_clock::now();
    cout<<"Minimum = "<<res<<endl;
    t= duration_cast<milliseconds>(end-start);
    cout<<"Time Parallel= "<<t.count()<<endl;

    cout<<endl;

    cout<<"2. Maximum"<<endl;
   
    start= high_resolution_clock::now();
    res= arr_max_seq(arr);
    end= high_resolution_clock::now();
    cout<<"Maximum = "<<res<<endl;
    t= duration_cast<milliseconds>(end-start);
    cout<<"Time sequential= "<<t.count()<<endl;

    start= high_resolution_clock::now();
    res= arr_max(arr);
    end= high_resolution_clock::now();
    cout<<"Maximum = "<<res<<endl;
    t= duration_cast<milliseconds>(end-start);
    cout<<"Time Parallel= "<<t.count()<<endl;

    cout<<endl;

    cout<<"3. Sum"<<endl;
   
    start= high_resolution_clock::now();
    res= arr_sum_seq(arr);
    end= high_resolution_clock::now();
    cout<<"Sum = "<<res<<endl;
    t= duration_cast<milliseconds>(end-start);
    cout<<"Time sequential= "<<t.count()<<endl;

    start= high_resolution_clock::now();
    res= arr_sum(arr);
    end= high_resolution_clock::now();
    cout<<"Sum = "<<res<<endl;
    t= duration_cast<milliseconds>(end-start);
    cout<<"Time Parallel= "<<t.count()<<endl;

    cout<<endl;

    cout<<"4. Average"<<endl;
   
    start= high_resolution_clock::now();
    double res1= arr_avg_seq(arr);
    end= high_resolution_clock::now();
    cout<<"Average = "<<res1<<endl;
    t= duration_cast<milliseconds>(end-start);
    cout<<"Time sequential= "<<t.count()<<endl;

    start= high_resolution_clock::now();
    res1= arr_avg(arr);
    end= high_resolution_clock::now();
    cout<<"Average = "<<res1<<endl;
    t= duration_cast<milliseconds>(end-start);
    cout<<"Time Parallel= "<<t.count()<<endl;

    return 0;

}