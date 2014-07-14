#include <cstdio>
#include <climits>
using namespace std;

int maxSum(int *A, int lo, int hi)  {
    int left = lo, right = lo, sum = INT_MIN, currentMaxSum = 0, maxLeft = lo, maxRight = lo;
    for(int i = lo; i < hi; i++)    {
        currentMaxSum += A[i];
        if(currentMaxSum > sum) {
            sum = currentMaxSum;
            right = i;
            maxLeft = left;
            maxRight = right;
        }
        if(currentMaxSum < 0)   {
            left = i+1;
            right = left;
            currentMaxSum = 0;
        }
    }
    printf("Maximum sum contiguous subarray :");
    for(int i = maxLeft; i <= maxRight; i++)
        printf(" %d", A[i]);
    printf("\n");
    return sum;
}

int minSum(int *A, int lo, int hi)  {
    int left = lo, right = lo, sum = INT_MAX, currentMinSum = 0, minLeft = lo, minRight = lo;
    for(int i = lo; i < hi; i++)    {
        currentMinSum += A[i];
        if(currentMinSum < sum) {
            sum = currentMinSum;
            right = i;
            minLeft = left;
            minRight = right;
        }
        if(currentMinSum > 0)   {
            left = i+1;
            right = left;
            currentMinSum = 0;
        }
    }
    printf("Minimum sum contiguous subarray :");
    for(int i = minLeft; i <= minRight; i++)
        printf(" %d", A[i]);
    printf("\n");
    return sum;
}

int main()  {
    int A[] = {3, 4, -3, -2, 6};
    int N = sizeof(A) / sizeof(int);

    printf("Maximum sum : %d\n", maxSum(A, 0, N));
    printf("Minimum sum : %d\n", minSum(A, 0, N));

    return 0;
}