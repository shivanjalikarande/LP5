#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

/*
Implement Min, Max, Sum and Average operations using Parallel Reduction
Measure the performance of sequential and parallel algorithms.
*/

// ============================= Sequential Operations =======================

int min_seq(vector<int>arr)
{
    int minVal = arr[0];
    for(int i=1;i<arr.size();i++)
    {
        if(minVal > arr[i])
        {
            minVal = arr[i];
        }
    }
    return minVal;
}

int max_seq(vector<int>arr)
{
    int maxVal = arr[0];
    for(int i=1;i<arr.size();i++)
    {
        if(arr[i] > maxVal)
        {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

int sum_seq(vector<int>arr)
{
    int sum = 0;
    for(int i=0;i<arr.size();i++)
    {
        sum += arr[i];
    }
    return sum;
}

double avg_seq(vector<int>arr)
{
    return double(sum_seq(arr) / arr.size());
}


//  ===================== Parallel Operatiosns==================

int min_parallel(vector<int>arr)
{
    int minVal = arr[0];
    #pragma omp parallel for reduction (min : minVal)
    for(int i=1;i<arr.size();i++)
    {
        if(minVal > arr[i])
        {
            minVal = arr[i];
        }
    }
    return minVal;
}


int max_parallel(vector<int>arr)
{
    int maxVal = arr[0];
    #pragma omp parallel for reduction (max : maxVal)
    for(int i=1;i<arr.size();i++)
    {
        if(arr[i] > maxVal)
        {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

int sum_parallel(vector<int>arr)
{
    int sum = 0;
    #pragma omp parallel for reduction (+ : sum)
    for(int i=0;i<arr.size();i++)
    {
        sum += arr[i];
    }
    return sum;
}

double avg_parallel(vector<int>arr)
{
    return double(sum_parallel(arr) / arr.size());
}


int main()
{
    int size;   //Above 10000000
    cout<<"Enter size of array: ";
    cin>>size;

    vector<int>arr(size, 0);

    for(int i=0;i<size;i++)
    {
        arr[i] = rand() % 1000;
    }


    double start, end;

    cout<<"\n\n============ SEQUENTIAL =================\n\n"<<endl;

    //1. min sequential
    start = omp_get_wtime();
    int minVal = min_seq(arr);
    end = omp_get_wtime();

    cout<<"Minimum Value: "<<minVal<<"\tTime: "<<(end - start)*1000<<"ms\n"<<endl;

    //2. max sequential
    start = omp_get_wtime();
    int maxVal = max_seq(arr);
    end = omp_get_wtime();

    cout<<"Maximum Value: "<<maxVal<<"\tTime: "<<(end - start)*1000<<"ms\n"<<endl;

    //3. sum sequential
    start = omp_get_wtime();
    int sumVal = sum_seq(arr);
    end = omp_get_wtime();

    cout<<"Sum Value: "<<sumVal<<"\tTime: "<<(end - start)*1000<<"ms\n"<<endl;

    //4. avg sequential
    start = omp_get_wtime();
    int avg = avg_seq(arr);
    end = omp_get_wtime();

    cout<<"Average Value: "<<avg<<"\tTime: "<<(end - start)*1000<<"ms\n"<<endl;

    cout<<"\n\n============ PARALLEL =================\n\n"<<endl;

    //5. min parallel
    start = omp_get_wtime();
    minVal = min_parallel(arr);
    end = omp_get_wtime();

    cout<<"Minimum Value: "<<minVal<<"\tTime: "<<(end - start)*1000<<"ms\n"<<endl;

    //6. max parallel
    start = omp_get_wtime();
    maxVal = max_parallel(arr);
    end = omp_get_wtime();

    cout<<"Maximum Value: "<<maxVal<<"\tTime: "<<(end - start)*1000<<"ms\n"<<endl;

    //7. sum parallel
    start = omp_get_wtime();
    sumVal = sum_parallel(arr);
    end = omp_get_wtime();

    cout<<"Sum Value: "<<sumVal<<"\tTime: "<<(end - start)*1000<<"ms\n"<<endl;

    //6. max parallel
    start = omp_get_wtime();
    avg = avg_parallel(arr);
    end = omp_get_wtime();

    cout<<"Average Value: "<<avg<<"\tTime: "<<(end - start)*1000<<"ms\n"<<endl;


    return 0;
}



/*

Enter size of array: 20000000


============ SEQUENTIAL =================


Minimum Value: 0        Time: 139.053ms

Maximum Value: 999      Time: 127.831ms

Sum Value: 1399158757   Time: 112.827ms

Average Value: 69       Time: 208.578ms



============ PARALLEL =================


Minimum Value: 0        Time: 125.183ms

Maximum Value: 999      Time: 173.344ms

Sum Value: 1399158757   Time: 190.386ms

Average Value: 69       Time: 203.199ms

*/