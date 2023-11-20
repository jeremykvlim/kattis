import java.util.*;
import java.io.*;

public class grasshopper {
    static String ans;

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        String line;
        while ((line = br.readLine()) != null) {
            var input = Arrays.stream(line.split(" ")).mapToInt(Integer::parseInt).toArray();
            int r = input[0], c = input[1], gr = input[2], gc = input[3], lr = input[4], lc = input[5];
            ans = "impossible";
            bfs(r, c, gr, gc, lr, lc);
            pw.println(ans);
        }
        pw.flush();
    }

    static void bfs(int r, int c, int gr, int gc, int lr, int lc) {
        int[] dr = {-2, -1, 1, 2, 2, 1, -1, -2}, dc = {1, 2, 2, 1, -1, -2, -2, -1};
        var visited = new boolean[r + 1][c + 1];
        var queue = new ArrayDeque<int[]>();
        queue.add(new int[]{gr, gc, 0});
        visited[gr][gc] = true;
        while (!queue.isEmpty()) {
            var square = queue.poll();
            int curr_r = square[0], curr_c = square[1], count = square[2];

            if (curr_r == lr && curr_c == lc) {
                ans = String.valueOf(count);
                return;
            }

            for (int i = 0; i < 8; i++) {
                int next_r = curr_r + dr[i], next_c = curr_c + dc[i];

                if (next_r >= 1 && next_r <= r && next_c >= 1 && next_c <= c && !visited[next_r][next_c]) {
                    visited[next_r][next_c] = true;
                    queue.add(new int[]{next_r, next_c, count + 1});
                }
            }
        }
    }
}
