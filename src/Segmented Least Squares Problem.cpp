#include <cstdio>
#include <algorithm>
#include <stack>
#include <limits>
#define MAXN 1000
#define INF numeric_limits<double>::infinity()
using namespace std;

struct Point {
	int x, y;
	bool operator < (const Point& that) const {
		return x < that.x;
	}
} points[MAXN + 1];

// Used for computing E[i][j] which is the square error of a segment 
// that is best fit to the points {points[i], points[i+1], ..., points[j]}

// cumulative_x[i] is sum(points[j].x) for 1 <= j <= i
// cumulative_y[i] is sum(points[j].y) for 1 <= j <= i
// cumulative_xy[i] is sum(points[j].x * points[j].y) for 1 <= j <= i
// cumulative_xSqr[i] is sum(points[j].x * points[j].x) for 1 <= j <= i

// slope[i][j] is the slope of the segment that is best fit to
// the points {points[i], points[i+1], ..., points[j]}

// intercept[i][j] is the y-intercept of the segment that is best fit to
// the points {points[i], points[i+1], ..., points[j]}

// E[i][j] is the square error of the segment that is best fit to
// the points {points[i], points[i+1], ..., points[j]}

long long cumulative_x[MAXN + 1], cumulative_y[MAXN + 1], cumulative_xy[MAXN + 1], cumulative_xSqr[MAXN + 1];
double slope[MAXN + 1][MAXN + 1], intercept[MAXN + 1][MAXN + 1], E[MAXN + 1][MAXN + 1];

// OPT[i] is the optimal solution (minimum cost) for the points {points[1], points[2], ..., points[i]}
double OPT[MAXN + 1];

// [opt_segment[i], i] is the last segment in the optimal solution 
// for the points {points[1], points[2], ..., points[i]}
int opt_segment[MAXN + 1];

int main()	{
	int N, i, j, k, interval;
	long long x_sum, y_sum, xy_sum, xsqr_sum, num, denom;
	double tmp, mn, C;
	
	printf("Enter the number of points : ");
	scanf("%d", &N);
	printf("Enter %d points :\n", N);
	for (i = 1; i <= N; i++)
		scanf("%d %d", &points[i].x, &points[i].y);
	printf("Enter the cost of creating a new segment : ");
	scanf("%lf", &C);
	
	// sort the points in non-decreasing order of x coordinate
	sort (points + 1, points + N + 1);
	
	// precompute the error terms
	cumulative_x[0] = cumulative_y[0] = cumulative_xy[0] = cumulative_xSqr[0] = 0;
	for (j = 1; j <= N; j++)	{
		cumulative_x[j] = cumulative_x[j-1] + points[j].x;
		cumulative_y[j] = cumulative_y[j-1] + points[j].y;
		cumulative_xy[j] = cumulative_xy[j-1] + points[j].x * points[j].y;
		cumulative_xSqr[j] = cumulative_xSqr[j-1] + points[j].x * points[j].x;
		
		for (i = 1; i <= j; i++)	{
			interval = j - i + 1;
			x_sum = cumulative_x[j] - cumulative_x[i-1];
			y_sum = cumulative_y[j] - cumulative_y[i-1];
			xy_sum = cumulative_xy[j] - cumulative_xy[i-1];
			xsqr_sum = cumulative_xSqr[j] - cumulative_xSqr[i-1];
			
			num = interval * xy_sum - x_sum * y_sum;
			if (num == 0)
				slope[i][j] = 0.0;
			else {
				denom = interval * xsqr_sum - x_sum * x_sum;
				slope[i][j] = (denom == 0) ? INF : (num / double(denom));				
			}
            		intercept[i][j] = (y_sum - slope[i][j] * x_sum) / double(interval);
            
           		for (k = i, E[i][j] = 0.0; k <= j; k++)	{
            			tmp = points[k].y - slope[i][j] * points[k].x - intercept[i][j];
            			E[i][j] += tmp * tmp;
            		}
		}
	}
	
	// find the cost of the optimal solution
	OPT[0] = opt_segment[0] = 0;
	for (j = 1; j <= N; j++)	{
		for (i = 1, mn = INF, k = 0; i <= j; i++)	{
			tmp = E[i][j] + OPT[i-1];
			if (tmp < mn)	{
				mn = tmp;
				k = i;
			}
		}
		OPT[j] = mn + C;
		opt_segment[j] = k;
	}
	
	printf("\nCost of the optimal solution : %lf\n", OPT[N]);
	
	// find the optimal solution
	stack <int> segments;
	for (i = N, j = opt_segment[N]; i > 0; i = j-1, j = opt_segment[i])	{
		segments.push(i);
		segments.push(j);
	}
	
	printf("\nAn optimal solution :\n");
	while (!segments.empty())	{
		i = segments.top(); segments.pop();
		j = segments.top(); segments.pop();
		printf("Segment (y = %lf * x + %lf) from points %d to %d with square error %lf.\n", 
				slope[i][j], intercept[i][j], i, j, E[i][j]);
	}
	
	return 0;
}