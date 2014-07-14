#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <deque>
using namespace std;

/*
 * Searches for @value in [@first, @last) with linear interpolation of values
 * and returns an iterator i in [@first, @last) s.t. *i == @value
 * Returns @last if @value is not found in the range
 * T must have definitions for these operators: -, <, ==
 */
template<class RandomAccessIterator, class T>
RandomAccessIterator interpolation_search(RandomAccessIterator first,
                                          RandomAccessIterator last,
                                          const T& value) {
    RandomAccessIterator middle, not_found = last;
    last -= 1;
    T* lo = &(*first);
    T* hi = &(*last);
    T* mid;

    if (value < *lo)
        return not_found;
    if (!(value < *hi))
        first = last;

    while (first < last)  {
        middle = first + ((last - first) * (value - *lo)) / (*hi - *lo);
        mid = &(*middle);
        if (*mid < value)    {
            first = middle + 1;
            lo = mid;
        } else if (value < *mid)    {
            last = middle - 1;
            hi = mid;
        } else
            return middle;
    }
    return (value == *first) ? first : not_found;
}

int main()  {
    int A[] = {0, 0, 2, 2, 2}, N = 5;

    int *result = interpolation_search(&A[0], &A[N], 2);
    if (result == &A[N])
        printf("Not found\n");
    else
        printf("Found at position: %d\n", result - A);

    vector<int> vec(&A[0], &A[N]);

    vector<int>::iterator res = interpolation_search(vec.begin(), vec.end(), 1);
    if (res == vec.end())
        printf("Not found\n");
    else
        printf("Found at position: %d\n", res - vec.begin());

    deque<int> deck(&A[0], &A[N]);

    deque<int>::iterator r = interpolation_search(deck.begin(), deck.end(), 0);
    if (r == deck.end())
        printf("Not found\n");
    else
        printf("Found at position: %d\n", r - deck.begin());

    return 0;
}