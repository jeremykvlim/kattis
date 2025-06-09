#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<bool> lights(n);
    for (int i = 0; i < n; i++) {
        char c;
        cin >> c;

        lights[i] = c == '1';
    }
    auto temp = lights;

    vector<int> b;
    auto flip = [&](int i) {
        bool l = lights[(i - 1 + n) % n], r = lights[(i + 1) % n];
        if (lights[i] != (l ^ r)) {
            lights[i] = l ^ r;
            b.emplace_back(i);
        }
    };

    for (int _ = 0; _ < 2; _++)
        for (int i = 0; i < n; i++)
            if (lights[i]) flip(i);

    int i = 0;
    while (i < n && (lights[i] | lights[(i - 1 + n) % n])) i++;
    if (i == n) {
        lights = temp;
        b.clear();
        for (int _ = 0; _ < 2; _++)
            for (int j = 1; j <= n; j++)
                if (lights[j % n]) flip(j % n);

        i = 0;
        while (i < n && (lights[i] | lights[(i - 1 + n) % n])) i++;
        if (i == n) {
            cout << -1;
            exit(0);
        }
    }

    for (int _ = 0; _ < n; _++, ++i %= n)
        if (lights[i]) {
            flip((i - 1 + n) % n);
            flip(i);
            flip((i - 1 + n) % n);
            flip((i + 1) % n);
        }

    cout << b.size() << "\n";
    for (int bi : b) cout << bi << "\n";
}
