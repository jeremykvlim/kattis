#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<double>> Pab(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) cin >> Pab[i][j];

    vector<double> Pa(n), Pb(n), Pab_1D(n * n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) Pa[i] += Pab[i][j];

    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++) Pb[j] += Pab[i][j];
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) Pab_1D[i * n + j] = Pab[i][j];

    auto huffman = [&](vector<double> &P) -> double {
        priority_queue<double, vector<double>, greater<>> pq;
        for (auto p : P) pq.emplace(p);

        if (pq.size() <= 1) return 0;

        double sum = 0;
        while (pq.size() > 1) {
            auto p1 = pq.top();
            pq.pop();
            auto p2 = pq.top();
            pq.pop();

            auto p3 = p1 + p2;
            sum += p3;
            pq.emplace(p3);
        }
        return sum;
    };
    cout << fixed << setprecision(6) << huffman(Pa) + huffman(Pb) - huffman(Pab_1D);
}
