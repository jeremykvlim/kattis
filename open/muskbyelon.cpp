#include <bits/stdc++.h>
using namespace std;
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int t;
    cin >> t;
 
    for (int x = 1; x <= t; x++) {
        int n;
        cin >> n;
 
        auto parse = [&](string t) {
            int colon = t.find(':');
            return stoi(t.substr(0, colon)) * 60 + stoi(t.substr(colon + 1));
        };
        vector<pair<int, int>> frames(n);
        for (auto &[l, r] : frames) {
            string sl, sr;
            cin >> sl >> sr;
 
            l = parse(sl);
            r = parse(sr);
 
            string _;
            getline(cin, _);
        }
        sort(frames.begin(), frames.end());
        frames.erase(unique(frames.begin(), frames.end(), [](auto &p1, auto &p2) {
            if (p1.second >= p2.first - 1) {
                p1.second = max(p1.second, p2.second);
                return true;
            }
            return false;
        }), frames.end());
 
        pair<int, int> longest{0, -1e5};
        auto update = [&](int l, int r) {
            if (l > r) return;
            longest = max(longest, make_pair(r - l + 1, -l));
        };
        int start = 10 * 60, end = 18 * 60 + 59;
        for (auto &[l, r] : frames) {
            update(start, l - 1);
            start = max(start, r + 1);
            if (r >= end) break;
        }
        update(start, end);
 
        auto fmt = [&](int t) {
            int h = t / 60, m = t % 60;
            ostringstream oss;
            oss << setw(2) << setfill('0') << h << ':' << setw(2) << setfill('0') << m;
            return oss.str();
        };
        cout << "Day #" << x << ": Elon Musk will make Musk by Elon from " << fmt(-longest.second) << " for " << longest.first / 60 << " hours and " << longest.first % 60 << " minutes\n";
    }
}
