#include <bits/stdc++.h>
using namespace std;

vector<vector<unsigned>> I(int size) {
    vector<vector<unsigned>> I(size, vector<unsigned>(size, 0));
    for (int i = 0; i < size; i++) I[i][i] = 1;

    return I;
}

vector<vector<unsigned>> matmul(vector<vector<unsigned>> &a, vector<vector<unsigned>> &b) {
    int r1 = a.size(), r2 = b.size(), c2 = b[0].size();

    vector<vector<unsigned>> c(r1, vector<unsigned>(c2, 0));
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c2; j++)
            for (int k = 0; k < r2; k++) c[i][j] += a[i][k] * b[k][j];

    return c;
}

vector<vector<unsigned>> matpow(vector<vector<unsigned>> a, long long n) {
    int size = a.size();
    auto b = I(size);

    for (; n; n >>= 1) {
        if (n & 1) b = matmul(a, b);
        a = matmul(a, a);
    }

    return b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<unsigned>> r(33, vector<unsigned>(33, 0));
    for (int i = 0; i < k; i++) cin >> r[0][i];
    r[0][32] = 1;

    stack<pair<vector<vector<unsigned>>, long long>> st;
    st.emplace(I(33), 1);
    while (n--) {
        string s;
        cin >> s;

        if (s == "add") {
            int x, y, z;
            char _;
            cin >> _ >> x >> _ >> _ >> y >> _ >> _ >> z;

            auto temp = I(33);
            temp[x][x]--;
            temp[y][x]++;
            temp[z][x]++;

            st.top().first = matmul(st.top().first, temp);
        } else if (s == "addi") {
            int x, y;
            unsigned c;
            char _;
            cin >> _ >> x >> _ >> _ >> y >> _ >> c;

            auto temp = I(33);
            temp[x][x]--;
            temp[y][x]++;
            temp[32][x] += c;

            st.top().first = matmul(st.top().first, temp);
        } else if (s == "move") {
            int x, y;
            char _;
            cin >> _ >> x >> _ >> _ >> y;

            auto temp = I(33);
            temp[x][x]--;
            temp[y][x]++;

            st.top().first = matmul(st.top().first, temp);
        } else if (s == "li") {
            int x;
            unsigned c;
            char _;
            cin >> _ >> x >> _ >> c;

            auto temp = I(33);
            temp[x][x]--;
            temp[32][x] += c;

            st.top().first = matmul(st.top().first, temp);
        } else if (s == "for") {
            unsigned c;
            cin >> c;

            st.emplace(I(33), c);
        } else {
            auto temp2 = matpow(st.top().first, st.top().second);

            st.pop();
            st.top().first = matmul(st.top().first, temp2);
        }
    }

    r = matmul(r, st.top().first);
    for (int i = 0; i < k; i++) cout << r[0][i] << " ";
}
