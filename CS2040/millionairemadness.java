import java.util.*;
import java.io.*;

public class millionairemadness {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split(" ");
        int m = Integer.parseInt(input[0]), n = Integer.parseInt(input[1]);

        var matrix = new int[m][n];
        int[] dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
        var visited = new boolean[m][n];
        for (int i = 0; i < m; i++) matrix[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();

        var pq = new PriorityQueue<Pair>();
        pq.add(new Pair(0, 0));
        int len = 0;
        while (!pq.isEmpty()) {
            var pair = pq.poll();
            int cost = -pair.first, x = pair.second / n, y = pair.second % n;
            if (visited[x][y]) continue;

            visited[x][y] = true;
            len = Math.max(len, cost);
            if (x == m - 1 && y == n - 1) break;


            for (int i = 0; i < 4; i++) {
                int xi = x + dx[i], yi = y + dy[i];
                if (xi < 0 || xi >= m || yi < 0 || yi >= n || visited[xi][yi]) continue;
                pq.add(new Pair(-Math.max(0, matrix[xi][yi] - matrix[x][y]), xi * n + yi));
            }
        }

        System.out.println(len);
    }

    static class Pair implements Comparable<Pair> {
        int first, second;

        Pair(int f, int s) {
            first = f;
            second = s;
        }

        @Override
        public int compareTo(Pair p) {
            if (p.first != first) return p.first - first;
            else return p.second - second;
        }
    }
}
