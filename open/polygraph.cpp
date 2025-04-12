#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int x = 0; x < t; x++) {
        int n, m;
        cin >> n >> m;
        cin.ignore();

        vector<vector<string>> statements(m);
        for (auto &st : statements) {
            string s;
            getline(cin, s);

            istringstream iss(s);
            st = {istream_iterator<string>(iss), istream_iterator<string>()};
        }

        vector<dynamic_bitset<>> bitmasks(m, dynamic_bitset<>(n + 1));
        for (int i = 0; i < m; i++) {
            bitmasks[i].flip(stoi(statements[i][0]));
            for (int j = 2; j < statements[i].size(); j++) bitmasks[i].flip(stoi(statements[i][j]));

            if (statements[i][1] == "S" || statements[i][1] == "L") bitmasks[i].flip(0);
        }

        for (int i = 1; i <= n; i++) {
            for (auto m1 : bitmasks)
                if (m1[i]) {
                    dynamic_bitset<> temp(n + 1);
                    for (int j = 1; j < i; j++) temp[j] = true;

                    if ((m1 & temp).none()) {
                        for (auto &m2 : bitmasks)
                            if (m2[i] && m2 != m1) m2 ^= m1;
                        break;
                    }
                }
        }

        sort(bitmasks.begin(), bitmasks.end(), [](auto b1, auto b2) {return b1.to_string() < b2.to_string();});
        string cities(n, '-');
        for (auto mask : bitmasks) {
            int msb = mask.find_first();
            while (mask.find_next(msb) <= n) msb = mask.find_next(msb);

            if (msb && dynamic_bitset<>(n + 1, 1) << (msb - 1) == mask >> 1) cities[msb - 1] = mask[0] ? 'T' : 'L';
        }

        cout << "Case #" << x + 1 << ": ";
        for (char c : cities) cout << c << " ";
        cout << "\n";
    }
}
