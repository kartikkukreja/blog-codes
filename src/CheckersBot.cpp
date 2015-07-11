#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#define INF 1e9
#define EPS 1e-3
#define ULL unsigned long long
using namespace std;

namespace kartikkukreja_logic {
	const int gc_initial_depth = 10, gc_branch_factor = 25;
	int g_current_depth;

	struct State {
	    ULL b, w, bk, wk;
	    double score;
	    vector<int> hops;

	    State () : b(0), w(0), bk(0), wk(0), score(INF) { }
	    State (const State & other)	: b(other.b), w(other.w), bk(other.bk), wk(other.wk), score(other.score) { hops = other.hops; }
	    State (char board[8][9]) : score(INF)	{
	        b = w = bk = wk = 0;
	    	int u, v; char c;
	        for (int i = 0; i < 64; ++i) {
	        	u = i >> 3; v = i % 8; c = board[u][v];
	        	if (c == 'b' || c == 'B') {
	        		b |= ((ULL)1)<<i;
	        		if (c == 'B')
	        			bk |= ((ULL)1)<<i;
	        	} else if (c == 'w' || c == 'W') {
	        		w |= ((ULL)1)<<i;
	        		if (c == 'W')
	        			wk |= ((ULL)1)<<i;
	        	}
	        }
	    }
	    State& operator=( const State& rhs )	{
	        b = rhs.b; w = rhs.w;
	        bk = rhs.bk; wk = rhs.wk;
	        score = rhs.score;
	        hops = rhs.hops;
	        return *this;
	    }
	    double random01() { return rand() / (double) RAND_MAX; }
	    double state_score()	{
	        if (abs(score-INF) > EPS) return score;
	        score = 0;
	        ULL bit = 1, bitk, nk;
	        int ii, jj, nr, k, idiff, jdiff, mx;

	        for (int i = 0; i < 64; ++i, bit <<= 1) {
	            if (b & bit) {
	                score += 17;
	                if (bk & bit) {
	                    score += 15;
	                    ii = i>>3; jj = i%8;
	                    if (ii > 1 && ii < 6 && jj > 1 && jj < 6) score++;
	                    bitk = 1;
	                    nr = 10;
	                    for (k = 0; k < 64; ++k, bitk <<= 1) {
	                        if (wk & bitk) {
	                            idiff = abs(ii - (k>>3)); jdiff = abs(jj - k%8);
	                            mx = (idiff > jdiff ? idiff : jdiff);
	                            if (mx < nr) nr = mx;
	                        }
	                    }
	                    if (nr <= 2) score += 2;
	                    else if (nr <= 4) score++;
	                }
	                else {
	                    score += i>>4;
	                    nk = b & ~bk;
	                    if ((nk & (nk-1)) == 0) score += i>>3;
	                }
	                if ((i>>3) == 0 && w != wk) score += 2;
	            } else if (w & bit) {
	                score -= 17;
	                if (wk & bit) {
	                    score -= 15;
	                    ii = i>>3; jj = i%8;
	                    if (ii > 1 && ii < 6 && jj > 1 && jj < 6) score--;
	                    bitk = 1;
	                    nr = 10;
	                    for (k = 0; k < 64; ++k, bitk<<=1) {
	                        if (bk & bitk) {
	                            idiff = abs(ii - (k>>3)); jdiff = abs(jj - k%8);
	                            mx = (idiff > jdiff ? idiff : jdiff);
	                            if (mx < nr) nr = mx;
	                        }
	                    }
	                    if (nr <= 2) score -= 2;
	                    else if(nr <= 4) score--;
	                } else {
	                    score -= (64-i-1)>>4;
	                    nk = w & ~wk;
	                    if ((nk & (nk-1)) == 0) score -= (64-i-1)>>3;
	                }
	                if ((i>>3) == 7 && b != bk) score -= 2;
	            }
	        }
	        return score + random01();
	    }
	    bool empty (int loc) { return !(b & (((ULL)1)<<loc)) && !(w & (((ULL)1)<<loc)); }
	    bool opp_at (int loc, bool black)	{ return (black ? (w & (((ULL)1)<<loc)) : (b & (((ULL)1)<<loc))); }
	    // _hops contains [src, jump, dest]+ while hops contains [src, dest]+
		State make_move (bool black, vector<int> & _hops)	{
	        State copy;
	        copy.b = b, copy.w = w, copy.bk = bk, copy.wk = wk;
	        int nhops = _hops.size(), src = _hops[0], jump, dest = _hops[nhops-1];
	        ULL srcbit = ((ULL)1)<<src, jumpbit, destbit = ((ULL)1)<<dest;

	        if (black) {
				copy.b |= destbit; copy.b &= ~srcbit; copy.bk &= ~srcbit;
				if ((dest>>3) == 7) copy.bk |= destbit;
			} else {
				copy.w |= destbit; copy.w &= ~srcbit; copy.wk &= ~srcbit;
				if ((dest>>3) == 0) copy.wk |= destbit;
			}
	        copy.hops.push_back(src);

	        for (int i = 1; i < nhops; i += 2) {
	        	jump = _hops[i];
	        	if (jump != -1) {
		        	jumpbit = ((ULL)1)<<jump;
			        if (black) { copy.w &= ~jumpbit; copy.wk &= ~jumpbit; }
					else { copy.b &= ~jumpbit; copy.bk &= ~jumpbit; }
			    }
		        copy.hops.push_back(_hops[i+1]);
		    }
	        return copy;
	    }
	    void generate_jumps(bool king, bool black, int i, int j, vector<State> & moves, vector<int> & _hops) {
	    	int jump, dest; bool jumpTaken = false;
	    	ULL prev_b = b, prev_w = w, prev_bk = bk, prev_wk = wk, bit;

	    	if ((black && i == 7) || (!black && i == 0)) king = true;
			if ((i > 1) && (king || !black)) { // going up
	            jump = ((i-1)<<3)+(j-1), dest = jump-9;
	            if (j > 1 && empty(dest) && opp_at(jump, black)) {
	            	_hops.push_back(jump); _hops.push_back(dest); jumpTaken = true;
	            	bit = ((ULL)1)<<jump; b &= ~bit, w &= ~bit, bk &= ~bit, wk &= ~bit;
	            	generate_jumps(king, black, i-2, j-2, moves, _hops);
	            	b = prev_b, w = prev_w, bk = prev_bk, wk = prev_wk;
	            	_hops.pop_back(); _hops.pop_back();
	            }
	            jump = ((i-1)<<3)+(j+1), dest = jump-7;
	            if (j < 6 && empty(dest) && opp_at(jump, black)) {
	            	_hops.push_back(jump); _hops.push_back(dest); jumpTaken = true;
	            	bit = ((ULL)1)<<jump; b &= ~bit, w &= ~bit, bk &= ~bit, wk &= ~bit;
	            	generate_jumps(king, black, i-2, j+2, moves, _hops);
	            	b = prev_b, w = prev_w, bk = prev_bk, wk = prev_wk;
	            	_hops.pop_back(); _hops.pop_back();
	            }
	        }
	        if ((i < 6) && (king || black)) {  // going down
	            jump = ((i+1)<<3)+(j-1), dest = jump+7;
	            if (i < 6 && j > 1 && empty(dest) && opp_at(jump,black)) {
	            	_hops.push_back(jump); _hops.push_back(dest); jumpTaken = true;
	            	bit = ((ULL)1)<<jump; b &= ~bit, w &= ~bit, bk &= ~bit, wk &= ~bit;
	            	generate_jumps(king, black, i+2, j-2, moves, _hops);
	            	b = prev_b, w = prev_w, bk = prev_bk, wk = prev_wk;
	            	_hops.pop_back(); _hops.pop_back();
	            }
	            jump = ((i+1)<<3)+(j+1), dest = jump+9;
	            if (i < 6 && j < 6 && empty(dest) && opp_at(jump,black)) {
	            	_hops.push_back(jump); _hops.push_back(dest); jumpTaken = true;
	            	bit = ((ULL)1)<<jump; b &= ~bit, w &= ~bit, bk &= ~bit, wk &= ~bit;
	            	generate_jumps(king, black, i+2, j+2, moves, _hops);
	            	b = prev_b, w = prev_w, bk = prev_bk, wk = prev_wk;
	            	_hops.pop_back(); _hops.pop_back();
	            }
	        }
	        if (!jumpTaken && _hops.size() > 2)
	        	moves.push_back(make_move(black, _hops));
	    }
	    void generate_moves (bool king, bool black, int i, int j, vector<State> & moves)	{
	    	vector<int> _hops; _hops.push_back((i<<3)+j); _hops.push_back(-1);
	        int dest;

			if ((i > 0) && (king || !black)) { // going up
	            dest = ((i-1)<<3)+(j-1);
	            if (j > 0 && empty(dest)) {
	        		_hops.push_back(dest);
					moves.push_back(make_move(black, _hops));
					_hops.pop_back();
				}
	            dest = ((i-1)<<3)+(j+1);
	            if (j < 7 && empty(dest)) {
	        		_hops.push_back(dest);
					moves.push_back(make_move(black, _hops));
					_hops.pop_back();
				}
	        }
	        if ((i < 7) && (king || black)) {  // going down
	            dest = ((i+1)<<3)+(j-1);
	            if (j > 0 && empty(dest)) {
	        		_hops.push_back(dest);
					moves.push_back(make_move(black, _hops));
					_hops.pop_back();
				}
	            dest = ((i+1)<<3)+(j+1);
	            if (j < 7 && empty(dest)) {
	        		_hops.push_back(dest);
					moves.push_back(make_move(black, _hops));
					_hops.pop_back();
				}
	        }
	    }
	    void print() {
	    	printf("b=%llu, w=%llu, bk=%llu, wk=%llu, score=%lf\n", b, w, bk, wk, score);
	    	for (int i = 0; i < 64; ++i) {
	    		ULL bit = ((ULL)1) << i; char c;
				if (bit & bk) c = 'B';
				else if (bit & wk) c = 'W';
				else if (bit & b) c = 'b';
				else if (bit & w) c = 'w';
				else c = '_';
				printf("%c", c);
				if ((i+1) % 8 == 0) printf("\n");
	    	}
	    }
	};

	struct MoveSort {
		bool black;
		MoveSort(bool black_) : black(black_) {}
		bool operator () (const State & b1, const State & b2) {
			return black ? (b1.score > b2.score) : (b1.score < b2.score);
		}
	};

	void simulate (bool black, State& board, State* play, double* outcome, int alpha, int beta, int depth) {
	    if (board.w == 0) { *outcome = 800; return; }
	    if (board.b == 0) { *outcome = -800; return; }
	    if (depth == 0) { *outcome = board.state_score(); return; }

	    // iterate over all pieces
	    vector<State> moves;
	    ULL & pieces = (black ? board.b : board.w);
	    ULL & kings = (black ? board.bk : board.wk);
	    ULL bit = 1;

        vector<int> _hops;
	    for (int i = 0; i < 64; ++i, bit<<=1)
	        if (pieces & bit) {
                    _hops.push_back(i);
                    board.generate_jumps(kings & bit, black, i >> 3, i % 8, moves, _hops);
                    _hops.pop_back();
                }
        if (moves.empty()) {
            bit = 1;
            for (int i = 0; i < 64; ++i, bit<<=1)
                if (pieces & bit)
                    board.generate_moves(kings & bit, black, i>>3, i%8, moves);
        }

	    if (moves.empty()) {
	        *outcome = (black ? -800 : 800);
	        return;
	    }

        int nmoves = moves.size();
	    for (int i = 0; i < nmoves; ++i) moves[i].state_score();
	    sort(moves.begin(), moves.end(), MoveSort(black));

	  	double bestOutcome = (black ? -1000 : 1000), simOutcome;
	  	int limit = (nmoves < gc_branch_factor ? nmoves : gc_branch_factor);
	  	for (int c = 0; c < limit; c++) {
	  		//moves[c].print();
			simOutcome = 0;
		    simulate(!black, moves[c], NULL, &simOutcome, alpha, beta, depth-1);
		    if ((black && simOutcome > bestOutcome) || (!black && simOutcome < bestOutcome)) {
		      if (depth == g_current_depth) *play = moves[c];
		      bestOutcome = simOutcome;
		    }
		    if ((black && simOutcome > alpha) || (!black && simOutcome < beta)) {
		        if (black) alpha = simOutcome;
		        else beta = simOutcome;
		    }
		    if (beta <= alpha) break;
		}

	  	*outcome = bestOutcome;
	  	return;
	}
	void next_move (char _board[8][9], bool black) {
		State board(_board), play; double outcome;
		g_current_depth = gc_initial_depth;
		simulate (black, board, &play, &outcome, -800, 800, g_current_depth);

		int nhops = play.hops.size(), src = play.hops[0];
		printf("%d\n%d %d\n", nhops - 1, src >> 3, src % 8);

		for (int i = 1; i < nhops; ++i) {
			src = play.hops[i];
			printf("%d %d\n", src >> 3, src % 8);
		}
	}
};

int main() {
	char player, board[8][9];
	int boardSize;

	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);

	scanf("%c%*c%d%*c", &player, &boardSize);
	if (boardSize != 8) return -1;

	for (int i = 0; i < 8; ++i)
		scanf("%s%*c", board[i]);
	kartikkukreja_logic::next_move(board, player == 'b');

	return 0;
}
