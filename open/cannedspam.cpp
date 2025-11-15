#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p;
    cin >> p;
    cin.ignore();

    auto check = [&](const string &s, int k) {
        int run = 0;
        for (char c : s) {
            if (!isalpha(c)) run = 0;
            else {
                c = tolower(c);
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') run = 0;
                else if (++run > k) return true;
            }
        }
        return false;
    };

    queue<string> q;
    unordered_map<string, int> count;
    for (int i = 0; i < p; i++) {
        string s;
        getline(cin, s);
        for (char &c : s) c = tolower(c);

        if (check(s, 6) || count[s] > 0) cout << "no\n";
        else {
            cout << "yes\n";
            q.emplace(s);
            count[s]++;
            if (q.size() > 10) {
                auto t = q.front();
                q.pop();

                count[t]--;
            }
        }
    }
}
