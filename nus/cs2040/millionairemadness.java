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
        var q = new ArrayDeque<Pair>();
        q.add(new Pair(0, 0));
        while (!q.isEmpty()) {
            var curr = q.poll();

            for (int i = 0; i < 4; i++) {
                int x = curr.first + dx[i], y = curr.second + dy[i];
                if (!(0 <= x && x < m && 0 <= y && y < n) || visited[x][y]) continue;

                if (vault[x][y] - vault[curr.first][curr.second] <= len) {
                    if (x == m - 1 && y == n - 1) return true;
                    visited[x][y] = true;
                    q.add(new Pair(x, y));
                }
            }
        }

        return false;
    }

    static class Pair {
        int first, second;

        Pair(int f, int s) {
            first = f;
            second = s;
        }
    }
}
