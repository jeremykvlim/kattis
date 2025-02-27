import java.io.*;
import java.util.*;

public class fire3 {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        var input = br.readLine().split(" ");
        int r = Integer.parseInt(input[0]), c = Integer.parseInt(input[1]);

        var maze = new char[r][c];
        var fire = new int[r][c];
        var joe = new int[r][c];
        var fire_q = new ArrayDeque<Pair<Integer, Integer>>();
        var joe_q = new ArrayDeque<Pair<Integer, Integer>>();

        for (int i = 0; i < r; i++) {
            Arrays.fill(fire[i], Integer.MAX_VALUE);
            Arrays.fill(joe[i], Integer.MAX_VALUE);
            maze[i] = br.readLine().toCharArray();
            for (int j = 0; j < c; j++)
                if (maze[i][j] == 'J') {
                    joe_q.add(new Pair<>(i, j));
                    joe[i][j] = 0;
                } else if (maze[i][j] == 'F') {
                    fire_q.add(new Pair<>(i, j));
                    fire[i][j] = 0;
                }
        }

        bfs(fire_q, fire, r, c, maze);
        bfs(joe_q, joe, r, c, maze);

        int fastest = Integer.MAX_VALUE;
        for (int i = 0; i < r; i++) {
            if (maze[i][0] != '#' && joe[i][0] < fire[i][0]) fastest = Math.min(fastest, joe[i][0] + 1);
            if (maze[i][c - 1] != '#' && joe[i][c - 1] < fire[i][c - 1]) fastest = Math.min(fastest, joe[i][c - 1] + 1);
        }

        for (int i = 0; i < c; i++) {
            if (maze[0][i] != '#' && joe[0][i] < fire[0][i]) fastest = Math.min(fastest, joe[0][i] + 1);
            if (maze[r - 1][i] != '#' && joe[r - 1][i] < fire[r - 1][i]) fastest = Math.min(fastest, joe[r - 1][i] + 1);
        }

        System.out.println(fastest == Integer.MAX_VALUE ? "IMPOSSIBLE" : fastest);
    }

    static void bfs(Queue<Pair<Integer, Integer>> q, int[][] d, int r, int c, char[][] maze) {
        int[] dx = {1, 0, -1, 0}, dy = {0, 1, 0, -1};
        while (!q.isEmpty()) {
            var p = q.poll();
            int i = p.first, j = p.second;
            for (int k = 0; k < 4; k++) {
                int x = i + dx[k], y = j + dy[k];
                if (0 <= x && x < r && 0 <= y && y < c && maze[x][y] != '#' && d[x][y] > d[i][j] + 1) {
                    d[x][y] = d[i][j] + 1;
                    q.add(new Pair<>(x, y));
                }
            }
        }
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }

}
