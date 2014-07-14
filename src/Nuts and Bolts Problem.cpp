#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;
 
class Bolt; // forward declaration of class Bolt
 
class Nut {
        int size;
public:
        Nut (int size) {
                this->size = size;
        }
        void to_str(char* str) {
                sprintf(str, "NUT(%d)", size);
        }
        // comparators are implemented as friend functions so that
        // they have access to the private member 'size', which is
        // otherwise inaccessible
 
        // A Nut can be compared against a Bolt and vice-versa
        // but a Nut cannot be compared against another Nut
        // and a Bolt cannot be compared against another Bolt
        friend int ::compare (const Nut*, const Bolt*);
        friend int ::compare (const Bolt*, const Nut*);
};
 
class Bolt {
        int size;
public:
        Bolt (int size) {
                this->size = size;
        }
        void to_str(char* str) {
                sprintf(str, "BOLT(%d)", size);
        }
        friend int ::compare (const Nut*, const Bolt*);
        friend int ::compare (const Bolt*, const Nut*);
};
 
// Compare a bolt with a nut
// Returns -1 if the bolt is smaller than the nut,
// 0 if they are the same size, 1 if the bolt is larger
int compare (const Bolt* a, const Nut* b) {
        if (a->size < b->size)
                return -1;
        else if (a->size == b->size)
                return 0;
        return 1;
}
 
// Compare a nut with a bolt
// Returns -1 if the nut is smaller than the bolt,
// 0 if they are the same size, 1 if the nut is larger
int compare (const Nut* a, const Bolt* b) {
        if (a->size < b->size)
                return -1;
        else if (a->size == b->size)
                return 0;
        return 1;
}
 
// partition A[lo...hi] by comparing with B
// creating a template avoids duplicating the code
template <typename U, typename V>
int partition(U** A, int lo, int hi, V* B)      {
        int i, j, cmp;
        U* temp;
    // partition A[lo...hi-1] into A[lo...x] and A[x+1...hi-1]
    // such that A[lo...x] has all elements smaller than B
    // and A[x+1...hi-1] has all elements larger than B
    // After the loop ends, A[hi] will match B
        for (i = j = lo; j < hi; j++)   {
                cmp = compare(A[j], B);
                if (cmp < 0)    {
                        temp = A[i]; A[i] = A[j]; A[j] = temp;
                        i++;
                } else if (cmp == 0)    {
            temp = A[j]; A[j] = A[hi]; A[hi] = temp;
            j--;
                }
        }
        // swap A[i] with A[hi]
        temp = A[i]; A[i] = A[hi]; A[hi] = temp;
        return i;
}
 
void sort(Nut** nuts, Bolt** bolts, int lo, int hi)     {
        if (lo >= hi)
                return;
        // choose a random nut to partition the bolts in the interval [lo...hi]
        int choice = lo + (rand() % (hi - lo + 1));
        Nut *pNut = nuts[choice];
        // partition the bolts[lo...hi] according to the chosen nut
        int pivot = partition(bolts, lo, hi, pNut);
        // partition the nuts in the interval [lo, hi] according to the bolt at position pivot
        // the matching nut will end up at position pivot
        partition(nuts, lo, hi, bolts[pivot]);
        // recursively sort nuts and bolts in ranges [lo...pivot-1] and [pivot+1...hi]
        sort(nuts, bolts, lo, pivot-1);
        sort(nuts, bolts, pivot+1, hi);
}
 
void sort(Nut** nuts, Bolt** bolts, int N)      {
        srand(time(NULL));
        sort(nuts, bolts, 0, N-1);
}
 
int main()      {
        int N, i, size;
        Nut **nuts;
        Bolt **bolts;
        char str[100];
 
        printf("Enter the number of nuts (or equivalently bolts) : ");
        scanf("%d", &N);
        nuts = new Nut*[N];
        bolts = new Bolt*[N];
 
        printf("Enter sizes of %d nuts :\n", N);
        for (i = 0; i < N; i++) {
                scanf("%d", &size);
                nuts[i] = new Nut(size);
        }
 
        printf("Enter sizes of %d bolts (nuts and bolts must have 1-1 correspondence) :\n", N);
        for (i = 0; i < N; i++) {
                scanf("%d", &size);
                bolts[i] = new Bolt(size);
        }
 
        // sort the nuts and bolts together
        sort(nuts, bolts, N);
 
        printf("Matching nuts and bolts :\n");
        for (i = 0; i < N; i++) {
                nuts[i]->to_str(str);
                printf("%s ", str);
                bolts[i]->to_str(str);
                printf("%s\n", str);
        }
 
        return 0;
}