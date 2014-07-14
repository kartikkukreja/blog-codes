#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

typedef pair <int, int> ii;
typedef vector <int> vi;
typedef vector <ii> vii;

// comparator for min-heap
// By default, heap functions create a max-heap
bool comp(ii x, ii y)   {
    return x > y;
}

int main()  {
    int N = 3;  // number of sorted vectors
    vi* A = new vi[N];  // array of sorted vectors
    int *indices = new int[N]; // stores the index of the next element to be added to the heap in each array

    A[0].push_back(3); A[0].push_back(5); A[0].push_back(6); A[0].push_back(10);
    A[1].push_back(1); A[1].push_back(2); A[1].push_back(7);
    A[2].push_back(2); A[2].push_back(4); A[2].push_back(6); A[2].push_back(8);

    // initially, first elements of each vector are added to the heap so the index
    // of next element to be added to the heap for each vector is 1.
    for (int i = 0; i < N; i++)
        indices[i] = 1;

    // vector that forms the heap. pairs of elements and vector number form the elements of the heap
    // vector number is used to identify the vector this element was added from so that the next element
    // can be added from the same vector
    vii heap;

    // first element of each vector is added to the heap
    for (int i = 0; i < N; i++)
        heap.push_back(make_pair(A[i][0], i));

    make_heap(heap.begin(), heap.end(), comp);

    // while the heap is not empty, remove the minimum element from the heap, print it
    // and add the next element from the vector this element was added to the heap
    while(!heap.empty())    {
        pair <int, int> mn = heap.front();
        printf("%d ", mn.first);

        pop_heap(heap.begin(), heap.end(), comp);
        heap.pop_back();

        int pos = mn.second;

        if (indices[pos] < A[pos].size())   {
            heap.push_back(make_pair(A[pos][indices[pos]++], pos));
            push_heap(heap.begin(), heap.end(), comp);
        }
    }

    return 0;
}