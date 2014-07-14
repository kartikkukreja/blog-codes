#include <cstdio>
#include <algorithm>
using namespace std;
 
class Job {
public:
    int deadline, length;
 
    bool operator < (const Job& x) const   {
        return deadline < x.deadline;
    }
} *jobs;
 
int main()  {
    int N, i, last_finished, s, lateness, temp;
 
    scanf("%d", &N); // Number of jobs
    jobs = new Job[N];
    for (i = 0; i < N; i++) // deadline and length of each job
        scanf("%d %d", &jobs[i].deadline, &jobs[i].length);
    scanf("%d", &s); // time at which the jobs become available
 
    // sort jobs in non-decreasing order of deadline
    sort(jobs, jobs + N);
 
    // print the optimal solution
    printf("Optimal schedule :\n");
    for (i = 0, last_finished = s, lateness = 0; i < N; i++)	{
    	// job i runs from last_finished to last_finished + jobs[i].length
    	temp = last_finished + jobs[i].length;
    	printf("Job %d : [%d, %d]\n", i, last_finished, temp);
    	last_finished = temp;
    	
    	// If last_finished > jobs[i].deadline
	// then lateness of job i = last_finished - jobs[i].deadline
	// otherwise lateness of job i = 0
	temp = max(last_finished - jobs[i].deadline, 0);
	if (lateness < temp)
		lateness = temp;
    }
    printf("Maximum lateness of schedule : %d\n", lateness);
 
    return 0;
}