import java.io.*;
import java.util.*;

public class millionairemadness {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        var input = br.readLine().split(" ");
        int m = Integer.parseInt(input[0]), n = Integer.parseInt(input[1]);

        if (m == 1 && n == 1) {
            System.out.println(0);
            return;
        }

        var vault = new int[m][n];
        for (int i = 0; i < m; i++) vault[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();

        int l = -1, r = (int) 1e9, mid;
        while (l + 1 < r) {
            mid = l + (r - l) / 2;

            if (bfs(mid, vault)) r = mid;
            else l = mid;
        }
        System.out.println(r);
    }

    static boolean bfs(int len, int[][] vault) {
        int[] dx = {1, 0, -1, 0}, dy = {0, 1, 0, -1};
        int m = vault.length, n = vault[0].length;

        var visited = new boolean[m][n];
        visited[0][0] = true;
        var q = new ArrayDeque<Pair<Integer, Integer>>();
        q.add(new Pair<>(0, 0));
        while (!q.isEmpty()) {
            var p = q.poll();
            int i = p.first, j = p.second;

            for (int k = 0; k < 4; k++) {
                int x = i + dx[k], y = j + dy[k];
                if (!(0 <= x && x < m && 0 <= y && y < n) || visited[x][y]) continue;

                if (vault[x][y] - vault[i][j] <= len) {
                    if (x == m - 1 && y == n - 1) return true;
                    visited[x][y] = true;
                    q.add(new Pair<>(x, y));
                }
            }
        }

        return false;
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
