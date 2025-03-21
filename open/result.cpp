#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    while (cin >> s && s != "END") {
        vector<long long> nums;
        string ops;
        istringstream iss(s);

        long long x;
        char op;
        while (ss >> x) {
            nums.emplace_back(x);
            ss >> op;
            ops += op;
        }

        int size = nums.size();
        vector<vector<long long>> dp_max(size, vector<long long>(size, 0)), dp_min(size, vector<long long>(size, LLONG_MAX));
        for (int i = 0; i < size; i++) dp_min[i][i] = dp_max[i][i] = nums[i];

        for (int h = 1; h < size; h++)
            for (int i = 0, j = i + h; i + h < size; i++, j++)
                for (int k = i; k < j; k++) {
                    auto a = dp_max[i][k], b = dp_max[k + 1][j], c = dp_min[i][k], d = dp_min[k + 1][j];
                    if (ops[k] == '+') {
                        dp_max[i][j] = max({dp_max[i][j], a + b});
                        dp_min[i][j] = min({dp_min[i][j], c + d});
                    } else {
                        dp_max[i][j] = max({dp_max[i][j], a * b, c * d});
                        dp_min[i][j] = min({dp_min[i][j], a * b, c * d});
                    }
                }

        cout << dp_min[0].back() << " " << dp_max[0].back() << "\n";
    }
}
