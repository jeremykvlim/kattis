#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<array<int, 2>> knight_dirs{{1,  2},
                                      {2,  1},
                                      {2,  -1},
                                      {1,  -2},
                                      {-1, -2},
                                      {-2, -1},
                                      {-2, 1},
                                      {-1, 2}},
                            rook_dirs{{1,  0},
                                      {0,  1},
                                      {-1, 0},
                                      {0,  -1}},
                          bishop_dirs{{1,  1},
                                      {-1, 1},
                                      {-1, -1},
                                      {1,  -1}};

    vector<vector<int>> knight(64);
    vector<array<vector<int>, 4>> rook(64), bishop(64);
    vector<array<int, 64>> symm(8);
    for (int sq = 0; sq < 64; sq++) {
        int r = sq >> 3, c = sq & 7;
        for (auto [dr, dc] : knight_dirs) {
            int rank = r + dr, file = c + dc;
            if (0 <= rank && rank < 8 && 0 <= file && file < 8) knight[sq].emplace_back((rank << 3) + file);
        }

        for (int i = 0; i < 4; i++) {
            auto add = [&](const array<int, 2> &d, auto &moves) {
                auto [dr, dc] = d;
                int rank = r + dr, file = c + dc;
                while (0 <= rank && rank < 8 && 0 <= file && file < 8) {
                    moves[i].emplace_back((rank << 3) + file);
                    rank += dr;
                    file += dc;
                }
            };
            add(rook_dirs[i], rook[sq]);
            add(bishop_dirs[i], bishop[sq]);
        }

        symm[0][sq] = (r << 3) + c;
        symm[1][sq] = (c << 3) + (7 - r);
        symm[2][sq] = ((7 - r) << 3) + (7 - c);
        symm[3][sq] = ((7 - c) << 3) + r;
        symm[4][sq] = (r << 3) + (7 - c);
        symm[5][sq] = ((7 - r) << 3) + c;
        symm[6][sq] = (c << 3) + r;
        symm[7][sq] = ((7 - c) << 3) + (7 - r);
    }

    array<char, 3> pieces{'K', '#', '#'};
    array<int, 3> pos;
    for (int r = 7, i = 1; ~r; r--)
        for (int c = 0; c < 8; c++) {
            char ch;
            cin >> ch;

            if (ch == '.') continue;

            if (ch == 'k') pos[0] = (r << 3) + c;
            else {
                pieces[i] = ch;
                pos[i] = (r << 3) + c;
                i++;
            }
        }

    auto piece_moves = [&](const array<int, 3> &p, int i) {
        int sq = p[i];

        vector<int> next_sqs;
        if (pieces[i] == 'N') {
            for (int next_sq : knight[sq]) {
                if (i && (next_sq == p[1] || next_sq == p[2]) && p[0] != next_sq) continue;
                next_sqs.emplace_back(next_sq);
            }
            return next_sqs;
        }

        auto move_KQRB = [&](const auto &base) -> void {
            for (const auto &dir : base)
                for (int next_sq : dir) {
                    if (i && (next_sq == p[1] || next_sq == p[2]) && p[0] != next_sq) break;
                    next_sqs.emplace_back(next_sq);
                    if ((i && p[0] == next_sq) || pieces[i] == 'K') break;
                }
        };

        if (pieces[i] == 'R' || pieces[i] == 'Q' || pieces[i] == 'K') move_KQRB(rook[sq]);
        if (pieces[i] == 'B' || pieces[i] == 'Q' || pieces[i] == 'K') move_KQRB(bishop[sq]);
        return next_sqs;
    };

    auto attacked = [&](const array<int, 3> &p, int i) -> bool {
        if (i) return max(abs((p[i] >> 3) - (p[0] >> 3)), abs((p[i] & 7) - (p[0] & 7))) == 1;

        for (int j : {1, 2})
            for (int sq : piece_moves(p, j))
                if (p[0] == sq) return true;
        return false;
    };

    auto legal_moves = [&](const array<int, 3> &p, int turn = 0) {
        vector<array<int, 3>> moves;
        vector<int> pieces_to_move{turn ? vector<int>{0} : vector<int>{1, 2}};
        for (int i : pieces_to_move)
            for (int sq : piece_moves(p, i)) {
                if (!turn && sq == p[0]) continue;
                auto q = p;
                q[i] = sq;
                if (pieces[i] == 'K' && attacked(q, i)) continue;
                moves.emplace_back(q);
            }
        return moves;
    };

    const int mask = 1 << 18;
    vector<int> encoded(mask, 1e9);
    for (int bk = 0; bk < 64; bk++)
        for (int w1 = 0; w1 < 64; w1++)
            for (int w2 = 0; w2 < 64; w2++)
                for (auto &boards : symm) encoded[(bk << 12) | (w1 << 6) | w2] = min(encoded[(bk << 12) | (w1 << 6) | w2], (boards[bk] << 12) | (boards[w1] << 6) | boards[w2]);

    auto decode = [&](int v) -> array<int, 3> {
        return {(v >> 12) & 63, (v >> 6) & 63, v & 63};
    };

    auto encode = [&](const array<int, 3> &p) {
        return encoded[(p[0] << 12) | (p[1] << 6) | p[2]];
    };

    vector<pair<int, int>> possible;
    int s = encode(pos);
    vector<array<bool, 2>> reachable(mask, {false, false});
    reachable[s][0] = true;
    deque<pair<int, int>> dq{{s, 0}};
    while (!dq.empty()) {
        auto [v, turn] = dq.front();
        dq.pop_front();

        possible.emplace_back(v, turn);

        for (const auto &q : legal_moves(decode(v), turn))
            if (q[1] != q[2]) {
                int u = encode(q), next_turn = turn ^ 1;
                if (!reachable[u][next_turn]) {
                    reachable[u][next_turn] = true;
                    dq.emplace_back(u, next_turn);
                }
            }
    }

    vector<array<vector<int>, 2>> adj_list(mask);
    vector<array<int, 2>> degree(mask, {0, 0});
    for (auto [v, turn] : possible) {
        auto p = decode(v);

        if (p[0] == p[1] || p[0] == p[2]) continue;

        for (const auto &q : legal_moves(p, turn)) {
            int u = encode(q), next_turn = turn ^ 1;
            if (reachable[u][next_turn]) {
                adj_list[u][next_turn].emplace_back(v);
                degree[v][turn]++;
            }
        }
    }

    vector<array<bool, 2>> wins(mask, {false, false});
    vector<array<int, 2>> dist(mask, {-1, -1});
    priority_queue<array<int, 3>, vector<array<int, 3>>, greater<>> pq;
    auto update = [&](int v, int turn, bool winning, int d = 0) {
        if (~dist[v][turn]) return;
        dist[v][turn] = d;
        wins[v][turn] = winning;
        pq.push({d, v, turn});
    };

    for (auto [v, turn] : possible) {
        auto p = decode(v);

        if (p[0] == p[1] || p[0] == p[2]) update(v, turn, turn == 1);
        else if (!degree[v][turn]) {
            if (turn == 1) update(v, turn, !attacked(p, 0));
            else update(v, turn, false);
        }
    }

    while (!pq.empty()) {
        auto [d, v, turn] = pq.top();
        pq.pop();

        if (d != dist[v][turn]) continue;

        for (int u : adj_list[v][turn]) {
            int next_turn = turn ^ 1;
            if (!wins[v][turn]) update(u, next_turn, true, d + 1);
            else if (!--degree[u][next_turn]) update(u, next_turn, false, d + 1);
        }
    }

    if (!wins[s][0]) {
        cout << "no-win";
        exit(0);
    }

    cout << "win\n" << dist[s][0] / 2 + 1 << "\n" << flush;

    for (;;) {
        for (const auto &p : legal_moves(pos)) {
            int v = encode(p);
            if (!~dist[v][1] || wins[v][1] || dist[v][1] + 1 != dist[s][0]) continue;

            int i = mismatch(p.begin(), p.end(), pos.begin()).first - p.begin(),
                    rank = pos[i] >> 3, file = pos[i] & 7,
                    next_rank = p[i] >> 3, next_file = p[i] & 7;
            cout << pieces[i] << " " << (char) (file + 'a') << (char) (rank + '1') << " " << (char) (next_file + 'a') << (char) (next_rank + '1') << "\n" << flush;

            if (!dist[v][1]) exit(0);
            pos = p;
            break;
        }

        string _, sq;
        cin >> _ >> _ >> sq;

        pos[0] = ((sq[1] - '1') << 3) + (sq[0] - 'a');
        s = encode(pos);
    }
}
