//%%writefile vector.cu
#include<iostream>
#include<bits/stdc++.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

__global__ void add(int *A, int *B, int *C, int size)
{
    int tid= blockIdx.x * blockDim.x + threadIdx.x;
    if(tid<size)
    {
        C[tid]=A[tid]+B[tid];
    }
}

void initialize(int* vec, int size)
{
    for(int i=0;i<size;i++)
    {
        cout<<"Enter element "<<i+1<<"of the vector: ";
        cin>>vec[i];
    }
}

void print(int* vec, int size)
{
    for(int i=0;i<size;i++)
    {
        cout<<vec[i]<<" ";

    }
    cout<<endl;

}

void Seq_Addition(int *A, int *B, int *C, int size)
{
    for(int i=0;i<size;i++)
    {
        C[i]=A[i]+B[i];
    }

}

int main()
{

    int N;
    cout<<"Enter the size of vectors= "<<endl;
    cin>>N;
    int* A, * B, * C;

    int vectorsize=N;
    size_t vectorBytes = vectorsize*sizeof(int);
    
    A = new int[vectorsize];
    B = new int[vectorsize];
    C = new int[vectorsize];

    initialize(A, N);
    initialize(B, N);

    cout << "Vector A: ";
    print(A, N);
    cout << "Vector B: ";
    print(B, N);

    int *X, *Y, *Z;
    cudaMalloc(&X,vectorBytes);
    cudaMalloc(&Y,vectorBytes);
    cudaMalloc(&Z,vectorBytes);

    cudaMemcpy(X,A,vectorBytes,cudaMemcpyHostToDevice);
    cudaMemcpy(Y,B,vectorBytes,cudaMemcpyHostToDevice);

    int threadsPerBlock= 256;
    int blocksPerGrid= (N+threadsPerBlock-1)/threadsPerBlock;

    //Sequential Addition
    auto start = high_resolution_clock::now();
    Seq_Addition(A, B, C, N);
    auto stop = high_resolution_clock::now();
    auto seq_duration = duration_cast<microseconds>(stop - start);

    cout << "Sequential Addition: ";
    print(C, N);

    //Parallel Addition
    start=high_resolution_clock::now();
    add<<<blocksPerGrid,threadsPerBlock>>>(X,Y,Z,N);
    cudaMemcpy(C,Z,vectorBytes, cudaMemcpyDeviceToHost);
    stop=high_resolution_clock::now();
    auto par_duration = duration_cast<microseconds>(stop - start);

    cout << "Parallel Addition: ";
    print(C, N);

    cout << "Sequential Addition Time: " << seq_duration.count() << " microseconds" << endl;
    cout << "Parallel Addition Time: " << par_duration.count() << " microseconds" << endl;

    

    cudaFree(X);
    cudaFree(Y);
    cudaFree(Z);
    return 0;
}

