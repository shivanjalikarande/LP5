#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

// To run in parallel - instructions/operations must be independent

/*  #pragma omp parallel for

pragma - special instruction to compiler
omp - open multiprocessing -> Tells compiler this pragma belogs to openmp
parallel - create multiple threads
for - parallelize upcoming for loop

*/


class ParallelSort
{
    public:
        vector<int>arr;

        ParallelSort(vector<int> a)
        {
            arr = a;
        }

        void printArray(vector<int>nums)
        {
            int limit = 20;
            for(int i=0;i<limit;i++)
            {
                cout<<nums[i]<<" ";
            }
            cout<<endl;
        }

        //1. Bubble Sort

        //1.a. Sequential Bubble Sort - T.C = O(n^2)
        void bubbleSort_seq()
        {
            int n = arr.size();
            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n-i-1;i++)
                {
                    if(arr[j] > arr[j+1])
                    {
                        swap(arr[j], arr[j+1]);
                    }
                }
            }
        }

        //1.b. Parallel Bubble Sort  - T.C. = O(n^2) , Same TC but reduces no of comparisons. Hence faster
        /*void bubbleSort_parallel()
        {
            int n = arr.size();
            for(int phase=0;phase<n;phase++)
            {
                //even phase (phase = 0, 2, 4, 6,..)
                #pragma omp parallel for
                for(int i=0;i<n;i+=2)
                {
                    if(arr[i] > arr[i+1])
                    {
                        swap(arr[i], arr[i+1]);
                    }
                }

                //Odd phase (phase = 1, 3, 5, 7,..)
                #pragma omp parallel for
                for(int i=1;i<n;i+=2)
                {
                    if(arr[i] > arr[i+1])
                    {
                        swap(arr[i], arr[i+1]);
                    }
                }
            }
        }
        */

        void bubbleSort_parallel()
        {
            int n=arr.size();
            for(int i=0;i<n;i++)
            {
                #pragma omp parallel for
                for(int j=i%2;j<n-1;j+=2)
                {
                    if(arr[j]>arr[j+1])
                    {
                        swap(arr[j],arr[j+1]);
                    }
                }
            }
        }

        //2. Merge Sort
        //a. Sequential Merge Sort

        void merge()



};


int main()
{
    int size;
    cout<<"\nEnter the size of the array: ";
    cin>>size;

    vector<int>arr(size, 0);

    cout<<"\nGenerating the random array elements of given array size...";
    for(int i=0;i<size;i++)
    {
        arr[i] = rand() % 1000;
    }

    ParallelSort ps1(arr);

    cout<<"Array Generated is : "<<endl;
    ps1.printArray(arr);

    double start, end;

    //Sequential Bubble Sort
    start = omp_get_wtime();
    ps1.bubbleSort_seq();
    end = omp_get_wtime();

    cout<<"Sequential Bubble Sort Time: "<<(end - start)<<"\n"<<endl;

    //Parallel Buuble Sort
    ParallelSort ps2(arr);

    start = omp_get_wtime();
    ps2.bubbleSort_parallel();
    end = omp_get_wtime();

    cout<<"Parallel Bubble Sort Time: "<<(end - start)<<"\n"<<endl;

    //Sequential Merge Sort
    ParallelSort ps3(arr);

    start = omp_get_wtime();
    ps2.bubbleSort_parallel();
    end = omp_get_wtime();

    cout<<"Parallel Bubble Sort Time: "<<(end - start)<<"\n"<<endl;

    //Paralllel Merge Sort
    ParallelSort ps2(arr);

    start = omp_get_wtime();
    ps2.bubbleSort_parallel();
    end = omp_get_wtime();

    cout<<"Parallel Bubble Sort Time: "<<(end - start)<<"\n"<<endl;

    


    //Parallel Bubble Sort



    return 0;
}