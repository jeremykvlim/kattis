#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    while (getline(cin, s)) {
        stack<pair<int, int>> st;
        for (int i = s.size() - 1; ~i; i--)
            if (s[i] == 'V') st.emplace(1, 1);
            else if (s[i] == 'C') swap(st.top().first, st.top().second);
            else if (s[i] == 'U') {
                auto [a1, w1] = st.top();
                st.pop();
                auto [a2, w2] = st.top();
                st.pop();

                st.emplace(a1 + a2, max(w1, w2));
            }
        cout << st.top().first << "\n";
    }
}
