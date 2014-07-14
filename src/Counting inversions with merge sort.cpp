#include <cstdio>
#include <cstring>

/*
 * Merges the two sorted subarrays A[low...mid] and A[mid+1...high]
 * and counts the number of split inversions
 */
int mergeAndCountSplitInv(int *A, int *helper, int low, int mid, int high)	{
    // Copy both parts into the helper array
    memcpy(helper + low, A + low, (high - low + 1)*sizeof(int));

    int i = low, j = mid + 1, k = low, split = 0;
    // Copy the smallest values from either the left or the right side back
    // to the original array
    while (i <= mid && j <= high) {
        if(helper[i] <= helper[j])
            A[k] = helper[i++];
        else {
            A[k] = helper[j++];
            split += mid + 1 - i;
        }
        k++;
    }
    // Copy the rest of the left side of the array into the target array
    while (i <= mid)
        A[k++] = helper[i++];

    return split;
}

int auxCountInversion(int *A, int *helper, int low, int high)	{
    if(low < high)	{
        int mid = (low + high)/2;
        int left = auxCountInversion(A, helper, low, mid);
        int right = auxCountInversion(A, helper, mid+1, high);
        int split = mergeAndCountSplitInv(A, helper, low, mid, high);
        return left + right + split;
    }
    return 0;
}

int countInversion(int *A, int N)	{
    int *helper = new int[N];
    return auxCountInversion(A, helper, 0, N - 1);
}

int main()  {
    int *A, N, i;

    printf("Enter size of array : ");
    scanf("%d", &N);
    A = new int[N];
    printf("Enter %d integers :\n", N);
    for(i=0; i<N; i++)
        scanf("%d", &A[i]);

    printf("No. of inversions : %d\n", countInversion(A, N));
    return 0;
}