#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S;
    while (getline(cin, S)) {
        istringstream iss(S);
        deque<int> s{istream_iterator<int>{iss}, istream_iterator<int>{}};

        int d = s.front();
        s.pop_front();
        int p = s.front();
        s.pop_front();

        int dist = d + 1, n = s.size();
        if (n > d + 1)
            for (int i = 0; i <= d && s[i] <= p; i++, dist--);
        else {
            long long s1 = s[n - 1], s2 = s[n - 2], s3 = s[n - 3], s4 = s[n - 4], c1, c2;
            if (s2 * s4 != s3 * s3) {
                c1 = (s1 * s4 - s2 * s3) / (s2 * s4 - s3 * s3);
                c2 = (s1 * s3 - s2 * s2) / (s3 * s3 - s2 * s4);
            } else {
                c1 = !s2 ? 0 : s1 / s2;
                c2 = 0;
            }

            for (int i = 0; i <= d && s[i] <= p; i++, dist--)
                if (i >= n - 1) s.emplace_back(c1 * s[i] + c2 * s[i - 1]);
        }

        if (dist == d + 1) cout << "The spider is going to fall!\n";
        else if (dist <= 0) cout << "The spider may fall!\n";
        else cout << dist << "\n";
    }
}
