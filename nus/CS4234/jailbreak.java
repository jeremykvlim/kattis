import java.io.*;
import java.util.*;

public class jailbreak {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            var input = br.readLine().split(" ");
            int h = Integer.parseInt(input[0]), w = Integer.parseInt(input[1]);
            var map = new char[h + 2][w + 2];
            
            Pair p1 = null, p2 = null;
            for (int i = 1; i <= h; i++) {
                var row = br.readLine();
                for (int j = 1; j <= w; j++) {
                    map[i][j] = row.charAt(j - 1);
                    if (map[i][j] == '$') {
                        if (p1 == null) p1 = new Pair(i, j);
                        else p2 = new Pair(i, j);
                    }
                }
            }
            
            var john = new int[h + 2][w + 2];
            bfs(new Pair(0, 0), map, john);
            int min = john[p1.first][p1.second] + john[p2.first][p2.second];

            var d1 = new int[h + 2][w + 2];
            bfs(p1, map, d1);
            var d2 = new int[h + 2][w + 2];
            bfs(p2, map, d2);
            for (int i = 1; i <= h; i++)
                for (int j = 1; j <= w; j++) {
                    int cur = john[i][j] + d1[i][j] + d2[i][j];
                    if (map[i][j] == '#') cur -= 2;
                    min = Math.min(min, cur);
                }
            System.out.println(min);
        }
    }

    static void bfs(Pair start, char[][] map, int[][] dist) {
        int[] dx = {1, -1, 0, 0}, dy = {0, 0, 1, -1};
        int h = map.length, w = map[0].length;

        for (var row : dist) Arrays.fill(row, Integer.MAX_VALUE);
        dist[start.first][start.second] = 0;

        var dq = new ArrayDeque<Pair>();
        dq.add(start);
        while (!dq.isEmpty()) {
            var p = dq.removeFirst();
            for (int i = 0; i < 4; i++) {
                int x = p.first + dx[i], y = p.second + dy[i];
                if (x < 0 || y < 0 || x >= h || y >= w || map[x][y] == '*' || dist[x][y] != Integer.MAX_VALUE) continue;
                if (map[x][y] == '#') {
                    dist[x][y] = dist[p.first][p.second] + 1;
                    dq.addLast(new Pair(x, y));
                } else {
                    dist[x][y] = dist[p.first][p.second];
                    dq.addFirst(new Pair(x, y));
                }
            }
        }
    }

    static class Pair {
        int first, second;

        Pair(int f, int s) {
            first = f;
            second = s;
        }
    }
}
