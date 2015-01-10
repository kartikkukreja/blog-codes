#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;

typedef long long ll;

struct SegmentTreeNode {
	int start, end; // this node is responsible for the segment [start...end]
	ll total;
	bool pendingUpdate;
	
	SegmentTreeNode() : total(0), pendingUpdate(false) {}
	
	void assignLeaf(ll value) {
		total = value;
	}
	
	void merge(SegmentTreeNode& left, SegmentTreeNode& right) {
		total = left.total + right.total;
	}
	
	ll query() {
		return total;
	}
	
	// For this particular problem, propagation is not required
	// if all elements in this segment are 1's
	bool isPropagationRequired() {
		return total > end-start+1;
	}
	
	void applyPendingUpdate() {
		total = (ll) sqrt(total);
		pendingUpdate = false;
	}
	
	// For this particular problem, the value of the update is dummy
	// and is just an instruction to square root the leaf value
	void addUpdate(bool value) {
		pendingUpdate = true;
	}
	
	// returns a dummy value
	bool getPendingUpdate() {
		return true;
	}
	
	void clearPendingUpdate() {
		pendingUpdate = false;
	}
};

// Had to declare it outside because the time limit on the problem
// is too strict to allow memory allocation/deallocation for
// each test case
SegmentTreeNode nodes[300000];

template<class InputType, class UpdateType, class OutputType>
class SegmentTree {
	//SegmentTreeNode* nodes;
	int N;
	
public:
	SegmentTree(InputType arr[], int N) {
		this->N = N;
		//nodes = new SegmentTreeNode[getSegmentTreeSize(N)];
		buildTree(arr, 1, 0, N-1);
	}
	
	~SegmentTree() {
		//delete[] nodes;
	}
	
	// get the value associated with the segment [start...end]
	OutputType query(int start, int end) {
		SegmentTreeNode result = query(1, start, end);
		return result.query();
	}
	
	// range update: update the range [start...end] by value
	// Exactly what is meant by an update is determined by the
	// problem statement and that logic is captured in segment tree node
	void update(int start, int end, UpdateType value) {
		update(1, start, end, value);
	}
	
private:
	void buildTree(InputType arr[], int stIndex, int start, int end) {
		// nodes[stIndex] is responsible for the segment [start...end]
		nodes[stIndex].start = start, nodes[stIndex].end = end;
		
		if (start == end) {
			// a leaf node is responsible for a segment containing only 1 element
			nodes[stIndex].assignLeaf(arr[start]);
			return;
		}
		
		int mid = (start + end) / 2,
			leftChildIndex = 2 * stIndex,
			rightChildIndex = leftChildIndex + 1;
			
		buildTree(arr, leftChildIndex, start, mid);
		buildTree(arr, rightChildIndex, mid + 1, end);
		nodes[stIndex].merge(nodes[leftChildIndex], nodes[rightChildIndex]);
	}
	
	int getSegmentTreeSize(int N) {
		int size = 1;
		for (; size < N; size <<= 1);
		return size << 1;
	}
	
	SegmentTreeNode query(int stIndex, int start, int end) {
		if (nodes[stIndex].start == start && nodes[stIndex].end == end)
			return nodes[stIndex];
		
		int mid = (nodes[stIndex].start + nodes[stIndex].end) >> 1,
			leftChildIndex = stIndex << 1,
			rightChildIndex = leftChildIndex + 1;			
		SegmentTreeNode result;
		
		if (start > mid)
			result = query(rightChildIndex, start, end);
		else if (end <= mid)
			result = query(leftChildIndex, start, end);
		else {
			SegmentTreeNode leftResult = query(leftChildIndex, start, mid),
							rightResult = query(rightChildIndex, mid+1, end);
			result.start = leftResult.start;
			result.end = rightResult.end;
			result.merge(leftResult, rightResult);
		}
		
		return result;
	}
	
	void update(int stIndex, int start, int end, UpdateType value) {
		if (nodes[stIndex].start == start && nodes[stIndex].end == end) {
			lazyPropagatePendingUpdateToSubtree(stIndex, value);
			return;
		}
		
		int mid = (nodes[stIndex].start + nodes[stIndex].end) >> 1,
			leftChildIndex = stIndex << 1,
			rightChildIndex = leftChildIndex + 1;
			
		if (start > mid)
			update(rightChildIndex, start, end, value);
		else if (end <= mid)
			update(leftChildIndex, start, end, value);
		else {
			update(leftChildIndex, start, mid, value);
			update(rightChildIndex, mid+1, end, value);
		}
		nodes[stIndex].merge(nodes[leftChildIndex], nodes[rightChildIndex]);
	}
	
	void lazyPropagatePendingUpdateToSubtree(int stIndex, UpdateType value) {
		nodes[stIndex].addUpdate(value);
		if (!nodes[stIndex].isPropagationRequired())
			return;
			
		if (nodes[stIndex].start == nodes[stIndex].end) {
			nodes[stIndex].applyPendingUpdate();
			return;
		}
		
		UpdateType pendingUpdate = nodes[stIndex].getPendingUpdate();
		nodes[stIndex].clearPendingUpdate();
		int mid = (nodes[stIndex].start + nodes[stIndex].end) >> 1,
			leftChildIndex = stIndex << 1,
			rightChildIndex = leftChildIndex + 1;
				
		lazyPropagatePendingUpdateToSubtree(leftChildIndex, pendingUpdate);
		lazyPropagatePendingUpdateToSubtree(rightChildIndex, pendingUpdate);		
		nodes[stIndex].merge(nodes[leftChildIndex], nodes[rightChildIndex]);
	}
};

ll A[100005];

int main() {
	int N, i, M, x, y, t = 1;
	
	while (scanf("%d", &N) != EOF) {
		for (i = 0; i < N; ++i)
			scanf("%lld", &A[i]);
		SegmentTree<ll,bool,ll> st(A, N);
		
		scanf("%d", &M);
		printf("Case #%d:\n", t);
		while (M--) {
			scanf("%d %d %d", &i, &x, &y);
			if (i == 0)
				st.update(min(x,y)-1, max(x,y)-1, true);
			else
				printf("%lld\n", st.query(min(x,y)-1, max(x,y)-1));
		}
		printf("\n");
		++t;
	}
	
	return 0;
}
