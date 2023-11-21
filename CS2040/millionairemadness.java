import java.util.*;
import java.io.*;

public class millionairemadness {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split(" ");
        int m = Integer.parseInt(input[0]), n = Integer.parseInt(input[1]);
        var matrix = new int[m][n];
        var dir = {0, 1, 0, -1, 1, 0, -1, 0};
        var visited = new boolean[m][n];
        for (int i = 0; i < m; i++) 
            matrix[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        
        var pq = new PriorityQueue<Pair>();
        pq.add(new Pair(0, 0));
        int ans = 0;
        while (!pq.isEmpty()) {
            var pair = pq.poll();
            int cost = -pair.first, x = pair.second / n, y = pair.second % n;
            if (visited[x][y]) continue;
            visited[x][y] = true;
            ans = Math.max(ans, cost);
            if (x == m - 1 && y == n - 1) break;
            int k = 0;
            while (k < 8) {
                int dx = x + dir[k++], dy = y + dir[k++];
                if (dx < 0 || dx >= m || dy < 0 || dy >= n || visited[dx][dy]) continue;
                pq.add(new Pair(-Math.max(0, matrix[dx][dy] - matrix[x][y]), dx * n + dy));
            }
        }
        System.out.println(ans);
    }

    static class Pair implements Comparable<Pair> {
        int first;
        int second;

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
