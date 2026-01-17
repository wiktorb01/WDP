#include <bits/stdc++.h>
using namespace std;


// Struct describing maximal intervals
struct Interval {
    int l, r;
    double quality;
    // Constructs an interval and calculates its quality
    Interval(vector<pair<int,int>>& vec, int _l, int _r) : l(_l), r(_r) {
        this->quality = (double)(vec[_r].first - vec[_l].first) / sqrt((double)(_r-_l+1));
    }
};


signed main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    // Input
    int N, U;
    cin >> N >> U;
    vector<pair<int,int>> vec(N);
    auto y = vec | views::elements<1>; 
    for ( auto& u : vec ) {
        cin >> u.first >> u.second;
    }

    // Constructs deque of all maximal intervals present in our sequence of pairs
    // It will be sorted by first and second values (only 1 maximal interval can start at a given index)
    deque<Interval> intervals;
    int l=0;
    deque<int> min_dq, max_dq; // Holds indexes of the next minimal and maximal values of the current interval
    for(int r=0; r<N; r++) {
        // Update min_dq (makes sure the values under indexes of deque are monotonic and adds r)
        while(!min_dq.empty() && y[min_dq.back()] >= y[r]) min_dq.pop_back();
        min_dq.push_back(r);
        while(!max_dq.empty() && y[max_dq.back()] <= y[r]) max_dq.pop_back();
        max_dq.push_back(r);

        // If the interval is invalid it adds the last interval as the maximal interval 
        // and increments the left index until current interval is valid
        if(y[max_dq.front()] - y[min_dq.front()] > U) {
            intervals.emplace_back(vec, l, r-1);
        }
        while(y[max_dq.front()] - y[min_dq.front()] > U) {
            if(l == min_dq.front()) min_dq.pop_front();
            if(l == max_dq.front()) max_dq.pop_front();
            l++;
        }
    }
    if(N != 0) intervals.emplace_back(vec, l, N-1);

    // Intervals that could be assigned to the current index
    deque<Interval> activeIntervals;
    for(int i=0; i<N; i++) {
        // Remove intervals that ended before i
        while(!activeIntervals.empty() && activeIntervals.front().r < i){ 
            activeIntervals.pop_front();
        }

        // Remove last intervals if quality is worse then the new interval (if it covers i) 
        if(!intervals.empty()) {
            Interval next = intervals.front();
            while(!activeIntervals.empty() && next.l <= i && next.quality > activeIntervals.back().quality) {
                activeIntervals.pop_back();
            }
            if(next.l <= i) {
                activeIntervals.push_back(intervals.front());
                intervals.pop_front();
            }
        }

        // Output
        if(!activeIntervals.empty())cout << activeIntervals.front().l+1 << " " << activeIntervals.front().r+1 << "\n";    
    }

    return 0;
}
