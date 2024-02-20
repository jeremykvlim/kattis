#include <bits/stdc++.h>
using namespace std;

void perform(vector<int> &jugglers, stack<int> &zeroes, int i) {
    if (!jugglers[i]) zeroes.emplace(i);

    while (jugglers[i] > 1) {
        int zero = zeroes.empty() ? -1 : zeroes.top();
        if (!zeroes.empty()) zeroes.pop();

        int dist1 = i - zero, dist2 = jugglers[i] - 1;
        if (dist1 < jugglers[i]) {
            jugglers[i] -= dist1 + 1;
            if (i < jugglers.size() - 1) jugglers[i + 1] += dist1;
            if (~zero) jugglers[zero] = 1;
            if (!jugglers[i]) zeroes.emplace(i);
        } else {
            jugglers[i] = 1;
            if (i < jugglers.size() - 1) jugglers[i + 1] += dist2;
            if (~zero) jugglers[zero] = 1;
            jugglers[zero + dist2] = 0;
            zeroes.emplace(zero + dist2);
        }
    }
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<int> jugglers(s.size());
    for (int i = 0; i < s.size(); i++) jugglers[i] = s[i] - '0';

    stack<int> zeroes;
    for (int i = 0; i < s.size(); i++) perform(jugglers, zeroes, i);
    for (int i : jugglers) cout << i;
}
