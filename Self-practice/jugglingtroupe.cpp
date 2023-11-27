#include <bits/stdc++.h>
using namespace std;

void perform(vector<int> &jugglers, stack<int> &zeroes, int i) {
    if (!jugglers[i]) zeroes.push(i);

    while (jugglers[i] > 1) {
        int zero = zeroes.empty() ? -1 : zeroes.top();
        if (!zeroes.empty()) zeroes.pop();

        int dist = i - zero;

        if (dist < jugglers[i]) {
            jugglers[i] -= dist + 1;
            if (i < jugglers.size() - 1) jugglers[i + 1] += dist;
            if (zero >= 0) jugglers[zero] = 1;
            if (!jugglers[i]) zeroes.push(i);
        } else {
            int move = jugglers[i] - 1;
            jugglers[i] -= move;
            if (i < jugglers.size() - 1) jugglers[i + 1] += move;
            zeroes.push(zero + move);
            if (zero >= 0) jugglers[zero] = 1;
            jugglers[zero + move] = 0;
        }
    }
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    vector<int> jugglers(s.size());
    stack<int> zeroes;
    for (int i = 0; i < s.size(); i++) jugglers[i] = s[i] - '0';
    for (int i = 0; i < s.size(); i++) perform(jugglers, zeroes, i);
    for (int i : jugglers) cout << i;
}
