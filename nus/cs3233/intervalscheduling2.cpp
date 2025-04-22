#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    cin.ignore();

    auto parse = [&](string t) {
        int colon = t.find(':');
        return stoi(t.substr(0, colon)) * 60 + stoi(t.substr(colon + 1));
    };

    vector<int> open(n), close(n);
    for (int i = 0; i < n; i++) {
        string interval;
        getline(cin, interval);

        open[i] = parse(interval.substr(0, 5));
        close[i] = parse(interval.substr(8, 5));
    }

    vector<int> o(n), c(n);
    auto plan = [&](int m) -> bool {
        o[0] = open[0];
        c[0] = max(close[0], o[0] + m);
        for (int i = 1; i < n; i++) {
            if (c[i - 1] > open[i]) return false;
            o[i] = c[i - 1];

            if (i == n - 1) {
                if (o[n - 1] + m > close[n - 1]) return false;
                c[i] = close[n - 1];
            } else c[i] = max(close[i], o[i] + m);
        }
        return true;
    };

    int l = 0, r = close[n - 1] - open[0], m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        if (plan(m)) l = m;
        else r = m;
    }
    plan(l);
    
    auto fmt = [&](int t) {
        int h = t / 60, m = t % 60;
        ostringstream oss;
        oss << setw(2) << setfill('0') << h << ':' << setw(2) << setfill('0') << m;
        return oss.str();
    };
    for (int i = 0; i < n; i++) cout << fmt(o[i]) << " - " << fmt(c[i]) << "\n";
}
