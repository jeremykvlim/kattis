import java.io.*;
import java.util.*;

public class lostoncampus {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int w = input[0], h = input[1];

        var grid = new char[h][];
        var dist = new int[h][w];
        var dq = new ArrayDeque<Pair<Integer, Integer>>();
        for (int i = 0; i < h; i++) {
            grid[i] = br.readLine().toCharArray();
            Arrays.fill(dist[i], Integer.MAX_VALUE);
            for (int j = 0; j < w; j++)
                if (grid[i][j] == '*') {
                    dist[i][j] = 0;
                    dq.addFirst(new Pair<>(i, j));
                }
        }

        int[] dr = {1, 0, -1, 0}, dc = {0, 1, 0, -1};
        while (!dq.isEmpty()) {
            var p = dq.removeFirst();
            int r = p.first, c = p.second;
            for (int k = 0; k < 4; k++) {
                int x = r + dr[k], y = c + dc[k];
                if (0 <= x && x < h && 0 <= y && y < w && grid[x][y] != '#') {
                    int d = grid[x][y] == 'D' ? 1 : 0;
                    if (dist[x][y] > dist[r][c] + d) {
                        dist[x][y] = dist[r][c] + d;
                        if (d == 1) dq.addLast(new Pair<>(x, y));
                        else dq.addFirst(new Pair<>(x, y));
                    }
                }
            }
        }

        int doors = Integer.MAX_VALUE;
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                if (grid[i][j] == 'E') doors = Math.min(doors, dist[i][j]);
        System.out.println(doors == Integer.MAX_VALUE ? "NOT POSSIBLE" : doors);
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
