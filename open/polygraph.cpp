#include <bits/stdc++.h>
using namespace std;

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
            copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(st));
        }

        vector<bitset<501>> bitmasks(m);
        for (int i = 0; i < m; i++) {
            bitmasks[i].flip(stoi(statements[i][0]));
            for (int j = 2; j < statements[i].size(); j++) bitmasks[i].flip(stoi(statements[i][j]));

            if (statements[i][1] == "S" || statements[i][1] == "L") bitmasks[i] ^= 1;
        }

        for (int i = 1; i <= n; i++) {
            for (auto mask : bitmasks)
                if (mask[i]) {
                    bitset<501> temp;
                    for (int j = 1; j < i; j++) temp.set(j);

                    if ((mask & temp).none()) {
                        for (auto &bm : bitmasks)
                            if (bm[i] && bm != mask) bm ^= mask;
                        break;
                    }
                }
        }

        sort(bitmasks.begin(), bitmasks.end(), [](auto b1, auto b2) {return b1.to_string() < b2.to_string();});
        string cities(n, '-');
        for (auto b : bitmasks) {
            int msb = b._Find_first();
            while (b._Find_next(msb) != 501) msb = b._Find_next(msb);

            if (msb && (bitset<501>(1) << (msb - 1)) == b >> 1) cities[msb - 1] = b[0] ? 'T' : 'L';
        }

        cout << "Case #" << x + 1 << ": ";
        for (char c : cities) cout << c << " ";
        cout << "\n";
    }
}
