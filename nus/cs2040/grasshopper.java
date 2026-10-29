import java.io.*;
import java.util.*;

public class grasshopper {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        for (var line = br.readLine(); line != null; line = br.readLine()) {
            var input = Arrays.stream(line.split(" ")).mapToInt(Integer::parseInt).toArray();
            int r = input[0], c = input[1], gr = input[2], gc = input[3], lr = input[4], lc = input[5];

            var hops = "impossible";
            int[] dr = {-2, -1, 1, 2, 2, 1, -1, -2}, dc = {1, 2, 2, 1, -1, -2, -2, -1};
            var visited = new boolean[r + 1][c + 1];
            var queue = new ArrayDeque<int[]>();
            queue.add(new int[]{gr, gc, 0});
            visited[gr][gc] = true;
            while (!queue.isEmpty()) {
                var square = queue.poll();
                int r1 = square[0], c1 = square[1], count = square[2];

                if (r1 == lr && c1 == lc) {
                    hops = String.valueOf(count);
                    break;
                }

                for (int k = 0; k < 8; k++) {
                    int r2 = r1 + dr[k], c2 = c1 + dc[k];
                    if (0 < r2 && r2 <= r && 0 < c2 && c2 <= c && !visited[r2][c2]) {
                        visited[r2][c2] = true;
                        queue.add(new int[]{r2, c2, count + 1});
                    }
                }
            }

            pw.println(hops);
        }
        pw.flush();
    }
}
