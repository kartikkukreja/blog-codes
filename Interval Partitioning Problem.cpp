#include <cstdio>
#include <algorithm>
using namespace std;

/*
 * Indexed min priority queue
 * Supports insertion in O(log N), deletion of any key (regardless of whether
 * the key is the minimum key or not) in O(log N) and changes to key values
 * in O(log N), where N is the number of elements currently in the PQ
 */
class MinIndexedPQ {
    int NMAX, N, *heap, *index, *keys;

    void swap(int i, int j) {
        int t = heap[i]; heap[i] = heap[j]; heap[j] = t;
        index[heap[i]] = i; index[heap[j]] = j;
    }

    void bubbleUp(int k)    {
        while(k > 1 && keys[heap[k/2]] > keys[heap[k]])   {
            swap(k, k/2);
            k = k/2;
        }
    }

    void bubbleDown(int k)  {
        int j;
        while(2*k <= N) {
            j = 2*k;
            if(j < N && keys[heap[j]] > keys[heap[j+1]])
                j++;
            if(keys[heap[k]] <= keys[heap[j]])
                break;
            swap(k, j);
            k = j;
        }
    }

public:
    // Create an empty MinIndexedPQ which can contain atmost NMAX elements
    MinIndexedPQ(int NMAX)  {
        this->NMAX = NMAX;
        N = 0;
        keys = new int[NMAX + 1];
        heap = new int[NMAX + 1];
        index = new int[NMAX + 1];
        for(int i = 0; i <= NMAX; i++)
            index[i] = -1;
    }

    ~MinIndexedPQ() {
        delete [] keys;
        delete [] heap;
        delete [] index;
    }

    // check if the PQ is empty
    bool isEmpty()  {
        return N == 0;
    }

    // check if i is an index on the PQ
    bool contains(int i)    {
        return index[i] != -1;
    }

    // return the number of elements in the PQ
    int size()  {
        return N;
    }

    // associate key with index i; 0 < i < NMAX
    void insert(int i, int key) {
        N++;
        index[i] = N;
        heap[N] = i;
        keys[i] = key;
        bubbleUp(N);
    }

    // returns the index associated with the minimal key
    int minIndex()  {
        return heap[1];
    }

    // returns the minimal key
    int minKey()    {
        return keys[heap[1]];
    }

    // delete the minimal key and return its associated index
    // Warning: Don't try to read from this index after calling this function
    int deleteMin() {
        int min = heap[1];
        swap(1, N--);
        bubbleDown(1);
        index[min] = -1;
        heap[N+1] = -1;
        return min;
    }

    // returns the key associated with index i
    int keyOf(int i)    {
        return keys[i];
    }

    // change the key associated with index i to the specified value
    void changeKey(int i, int key)  {
        keys[i] = key;
        bubbleUp(index[i]);
        bubbleDown(index[i]);
    }

    // decrease the key associated with index i to the specified value
    void decreaseKey(int i, int key)    {
        keys[i] = key;
        bubbleUp(index[i]);
    }

    // increase the key associated with index i to the specified value
    void increaseKey(int i, int key)    {
        keys[i] = key;
        bubbleDown(index[i]);
    }

    // delete the key associated with index i
    void deleteKey(int i)   {
        int ind = index[i];
        swap(ind, N--);
        bubbleUp(ind);
        bubbleDown(ind);
        index[i] = -1;
    }
};

class Interval {
public:
    int start, finish;

    bool operator < (const Interval& x) const   {
        if (start != x.start)
            return start < x.start;
        return finish < x.finish;
    }
} *intervals;

int main()  {
    int N, i, j, d, *schedule;

    scanf("%d", &N); // Number of intervals
    intervals = new Interval[N];
    for (i = 0; i < N; i++)
        scanf("%d %d", &intervals[i].start, &intervals[i].finish);

    // sort intervals in non-decreasing order of start times
    sort(intervals, intervals + N);

    schedule = new int[N];
    MinIndexedPQ pq(N);

    d = 0;
    schedule[0] = 0;
    pq.insert(0, intervals[0].finish);

    for (i = 1; i < N; i++) {
        j = pq.minIndex();
        if (intervals[i].start >= pq.keyOf(j))  {
            schedule[i] = j;
            pq.increaseKey(j, intervals[i].finish);
        } else  {
            d++;
            schedule[i] = d;
            pq.insert(d, intervals[i].finish);
        }
    }

    printf("%d\n", d + 1); // Minimum number of resources required
    for (i = 0; i < N; i++) // Assignment of resources to each interval
        printf("%d %d %d\n", intervals[i].start, intervals[i].finish, schedule[i]);

    return 0;
}