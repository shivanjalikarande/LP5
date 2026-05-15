#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

void generateArray(vector<int>arr, int size)
{
    for(int i=0;i<size;i++)
    {
        arr[i] = rand() % 1000;
    }
}


void bubbleSortSequential(vector<int>&arr)
{
    int n = arr.size();
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if(arr[j] > arr[j+1])
            {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}


void bubbleSortParallel(vector<int>&arr)
{
    int flag = 0;
    int n = arr.size();

    while(flag == 0)
    {
        flag = 1;
        #pragma omp parallel for reduction(&&: flag)
        for(int j=0;j<n-1;j+=2)
        {
            if(arr[j] > arr[j+1])
            {
                swap(arr[j], arr[j+1]);
                flag = 0;
            }
        }
        #pragma omp parallel for reduction(&&: flag)
        for(int j=1;j<n-1;j+=2)
        {
            if(arr[j] > arr[j+1])
            {
                swap(arr[j], arr[j+1]);
                flag = 0;
            }
        }
    }

}


void merge(int low, int mid, int right, vector<int>&arr)
{
    vector<int>temp(right - low + 1);

    int i = low;
    int j = mid + 1;
    int k = 0;
    
    while(i <= mid && j <= right)
    {
        if(arr[i] < arr[j])
        {
            temp[k] = arr[i];
            k++;
            i++;
        }
        else
        {
            temp[k] = arr[j];
            k++;
            j++;
        }
    }

    while(i <= mid)
    {
        temp[k] = arr[i];
        i++;
        k++;
    }
    while(j <= right)
    {
        temp[k] = arr[j];
        j++;
        k++;
    }

    int ind = 0;
    for(int ind=0;ind<=k;ind++)
    {
        arr[low + ind] = temp[ind];
    }
}

void mergeSortSequential(int left, int right, vector<int>&arr)
{
    if(left < right)
    {
        int mid = left + (right - left)/2;
        
        mergeSortSequential(left, mid, arr);
        mergeSortSequential(mid + 1, right, arr);

        merge(left, mid, right, arr);
    }
}

void mergeSortParallel(int left, int right, vector<int>&arr)
{
    if(left < right)
    {
        if(right - left < 1000)
        {
            mergeSortSequential(left, right, arr);
            return;
        }

        int mid = left + (right - left)/2;

        #pragma omp task shared(arr)
        mergeSortParallel(left, mid, arr);

        #pragma omp task shared(arr)
        mergeSortParallel(mid+1, right, arr);

        #pragma omp taskwait
        merge(left, mid, right, arr);
    }
}


int main()
{
    int size;
    cout<<"\nEnter size of array: ";
    cin>>size;

    vector<int>arr1(size, 0);
    vector<int>arr2(size, 0);
    vector<int>arr3(size, 0);
    vector<int>arr4(size, 0);

    generateArray(arr1,size);

    double start, end;

    start = omp_get_wtime();
    bubbleSortSequential(arr1);
    end = omp_get_wtime();

    cout<<"\nSequential Bubble Sort: "<<(end-start)<<"secs"<<endl;

    generateArray(arr2,size);

    start = omp_get_wtime();
    bubbleSortParallel(arr2);
    end = omp_get_wtime();

    cout<<"\nParallel Bubble Sort: "<<(end-start)<<"secs"<<endl;

    generateArray(arr3,size);
    start = omp_get_wtime();
    mergeSortSequential(0, arr3.size()-1, arr3);
    end = omp_get_wtime();

    cout<<"\nSequential Merge Sort: "<<(end-start)<<"secs"<<endl;

    generateArray(arr4,size);
    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            mergeSortParallel(0, arr4.size()-1, arr4);
        }
    }
    end = omp_get_wtime();

    cout<<"\nParallel Merge Sort: "<<(end-start)<<"secs"<<endl;





    return 0;
}