#include <cstdio>

struct SegmentTreeNode {
	int start, end; // this node is responsible for the segment [start...end]
	int count;
	bool pendingUpdate;

	SegmentTreeNode() : count(0), pendingUpdate(false) {}

	void assignLeaf(bool value) {}

	void merge(SegmentTreeNode& left, SegmentTreeNode& right) {
		count = (left.pendingUpdate ? (left.end - left.start + 1 - left.count) : left.count)
			    + (right.pendingUpdate ? (right.end - right.start + 1 - right.count) : right.count);
	}

	int query() {
		return count;
	}

	bool hasPendingUpdate() {
		return pendingUpdate;
	}

	void applyPendingUpdate() {
		count = (end - start + 1) - count;
		pendingUpdate = false;
	}

	void addUpdate(bool value) {
		pendingUpdate = !pendingUpdate;
	}

	bool getPendingUpdate() {
		return true;
	}
};

template<class InputType, class UpdateType, class OutputType>
class SegmentTree {
	SegmentTreeNode* nodes;
	int N;

public:
	SegmentTree(InputType arr[], int N) {
		this->N = N;
		nodes = new SegmentTreeNode[getSegmentTreeSize(N)];
		buildTree(arr, 1, 0, N-1);
	}

	~SegmentTree() {
		delete[] nodes;
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
		if (nodes[stIndex].start == start && nodes[stIndex].end == end) {
			SegmentTreeNode result = nodes[stIndex];
			if (result.hasPendingUpdate())
				result.applyPendingUpdate();
			return result;
		}

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

		if (nodes[stIndex].hasPendingUpdate()) {
			result.addUpdate(nodes[stIndex].getPendingUpdate());
			result.applyPendingUpdate();
		}
		return result;
	}

	void update(int stIndex, int start, int end, UpdateType value) {
		if (nodes[stIndex].start == start && nodes[stIndex].end == end) {
			nodes[stIndex].addUpdate(value);
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
};

bool tailsUp[100005];

int main() {
	int N, Q, cmd, A, B;

	scanf("%d %d", &N, &Q);
	SegmentTree<bool,bool,int> st(tailsUp, N);
	while (Q--) {
		scanf("%d %d %d", &cmd, &A, &B);
		if (cmd == 0)
			st.update(A, B, true);
		else
			printf("%d\n", st.query(A, B));
	}

	return 0;
}
