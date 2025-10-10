#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, N;
    cin >> T >> N;

    if (!N) {
        cout << 0;
        exit(0);
    }

    if (T < 20) {
        cout << "impossible";
        exit(0);
    }

    auto sum = 0LL;
    vector<int> t(N);
    for (int i = 0; i < N; i++) {
        double n;
        cin >> n;

        sum += t[i] = 20 + 10 * ceil(n / 10);
        if (t[i] > T) {
            cout << "impossible";
            exit(0);
        }
    }

    vector<int> count(T + 1);
    int l = (sum + T - 1) / T - 1, r = N, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        fill(count.begin(), count.end(), 0);
        count[0] = m;
        auto valid = [&]() {
            int time = 0;
            for (int ti : t) {
                while (time <= T && !count[time]) time++;
                if (time > T || time + ti > T) return false;
                count[time]--;
                count[time + ti]++;
            }
            return true;
        };

        if (valid()) r = m;
        else l = m;
    }
    cout << r;
}
