#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line)) {
        int eq = line.find('=');
        auto text = line.substr(0, eq), target = line.substr(eq + 1);

        int n = target.size();
        vector<int> pos(n, -1);
        int l = -1, r = -1, shortest = INT_MAX;
        for (int i = 0; i < text.size(); i++) {
            for (int j = n - 1; j; j--)
                if (text[i] == target[j] && pos[j - 1] != -1) pos[j] = pos[j - 1];

            if (text[i] == target[0]) pos[0] = i;
            if (text[i] == target[n - 1] && pos[n - 1] != -1) {
                int len = i - pos[n - 1] + 1;
                if (shortest > len || (shortest == len && l > pos[n - 1])) {
                    shortest = len;
                    l = pos[n - 1];
                    r = i;
                }
            }
        }
        cout << l << " " << r << "\n";
    }
}
