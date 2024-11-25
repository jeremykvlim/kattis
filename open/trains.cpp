#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;

    while (T--) {
        int N;
        cin >> N;

        vector<vector<array<int, 3>>> adj_list;
        unordered_map<string, int> compress;
        while (N--) {
            int S;
            cin >> S;

            vector<pair<int, string>> stations(S);
            int prev_i = -1, prev_t = -1;
            for (auto &[t, name]  : stations) {
                string time;
                cin >> time >> name;

                int colon = time.find(':');
                t = stoi(time.substr(0, colon)) * 60 + stoi(time.substr(colon + 1));

                if (!compress.count(name)) {
                    compress[name] = compress.size();
                    adj_list.emplace_back();
                }

                if (!~prev_i && !~prev_t) {
                    prev_i = compress[name];
                    prev_t = t;
                    continue;
                }

                int curr_i = compress[name], curr_t = prev_t + t;
                adj_list[prev_i].push_back({curr_i, prev_t, curr_t});
                prev_i = curr_i;
                prev_t = curr_t % 1440;
            }
        }

        string source, destination;
        cin >> source >> destination;

        int src = compress[source], dest = compress[destination];
        sort(adj_list[src].begin(), adj_list[src].end(), [&](auto a, auto b) { return a[1] < b[1]; });
        vector<pair<int, int>> trains;
        for (auto [_, dsrc, asrc] : adj_list[src]) {
            vector<int> arrive(compress.size(), INT_MAX);
            arrive[src] = dsrc;
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
            pq.emplace(dsrc, src);
            while (!pq.empty()) {
                auto [av, v] = pq.top();
                pq.pop();

                if (av != arrive[v]) continue;
                if (v == dest) break;

                for (auto [u, du, au] : adj_list[v]) {
                    while (du < av) {
                        du += 1440;
                        au += 1440;
                    }

                    if (arrive[u] > au) {
                        arrive[u] = au;
                        pq.emplace(au, u);
                    }
                }
            }

            while (!trains.empty() && trains.back().second >= arrive[dest]) trains.pop_back();
            trains.emplace_back(dsrc, arrive[dest]);
        }

        while (!trains.empty() && trains.back().second == trains.front().second + 1440) trains.pop_back();
        for (auto [depart, arrive] : trains) {
            int h1 = depart / 60, m1 = depart % 60;
            auto dh = to_string(h1), dm = to_string(m1);
            if (dh.size() < 2) dh = "0" + dh;
            if (dm.size() < 2) dm = "0" + dm;

            int travel = arrive - depart, h2 = travel / 60, m2 = travel % 60;
            auto th = to_string(h2), tm = to_string(m2);
            if (tm.size() < 2) tm = "0" + tm;

            cout << dh << ":" << dm << " " << th << ":" << tm << "\n";
        }
    }
}
