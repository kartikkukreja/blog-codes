#include <cstdio>
#include <algorithm>
#include <queue>
using namespace std;

class Interval {
public:
    int start, finish;

    bool operator < (const Interval& x) const   {
        if (finish != x.finish)
            return finish < x.finish;
        return start < x.start;
    }
} *intervals;

int main()  {
    int N, i, last_finished;

    scanf("%d", &N); //Number of intervals
    intervals = new Interval[N];
    for (i = 0; i < N; i++)
        scanf("%d %d", &intervals[i].start, &intervals[i].finish);

    // sort intervals in non-decreasing order of finish times
    sort(intervals, intervals + N);

    queue <int> opt;
    // construct the optimal solution
    for (i = 0, last_finished = 0; i < N; i++)
        if (intervals[i].start >= last_finished)    {
            opt.push(i);
            last_finished = intervals[i].finish;
        }

    // optimal solution
    printf("%d\n", opt.size());
    while (!opt.empty())    {
        i = opt.front();
        opt.pop();
        printf("%d %d\n", intervals[i].start, intervals[i].finish);
    }

    return 0;
}