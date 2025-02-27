import java.io.*;
import java.util.*;

public class blockcrusher {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        for (var line = br.readLine(); !line.equals("0 0"); line = br.readLine()) {
            var input = line.split(" ");
            int h = Integer.parseInt(input[0]), w = Integer.parseInt(input[1]);

            var block = new char[h][w];
            int[][] strength = new int[h][w], prev_x = new int[h][w], prev_y = new int[h][w];
            for (int i = 0; i < h; i++) {
                block[i] = br.readLine().toCharArray();
                Arrays.fill(strength[i], Integer.MAX_VALUE);
                Arrays.fill(prev_x[i], -1);
                Arrays.fill(prev_y[i], -1);
            }

            var pq = new PriorityQueue<Triple<Integer, Integer, Integer>>();
            for (int i = 0; i < w; i++) {
                strength[0][i] = block[0][i] - '0';
                pq.add(new Triple<>(strength[0][i], 0, i));
            }

            int[] dx = {1, 1, 1, -1, -1, -1, 0, 0}, dy = {-1, 0, 1, -1, 0, 1, -1, 1};
            int r = -1, c = -1, lowest = Integer.MAX_VALUE;
            while (!pq.isEmpty()) {
                var t = pq.poll();
                int s = t.first, i = t.second, j = t.third;

                if (i == h - 1 && lowest > s) {
                    r = i;
                    c = j;
                    lowest = s;
                }

                for (int k = 0; k < 8; k++) {
                    int x = i + dx[k], y = j + dy[k];
                    if (0 <= x && x < h && 0 <= y && y < w && strength[x][y] > s + block[x][y] - '0') {
                        strength[x][y] = s + block[x][y] - '0';
                        prev_x[x][y] = i;
                        prev_y[x][y] = j;
                        pq.add(new Triple<>(strength[x][y], x, y));
                    }
                }
            }
            while (r != -1 && c != -1) {
                block[r][c] = ' ';
                int temp = prev_y[r][c];
                r = prev_x[r][c];
                c = temp;
            }
            Arrays.stream(block).forEach(row -> pw.println(new String(row)));
            pw.println();
        }
        pw.flush();
    }

    record Triple<T extends Comparable<T>, U extends Comparable<U>, V extends Comparable<V>>(T first, U second, V third) implements Comparable<Triple<T, U, V>> {
        @Override
        public int compareTo(Triple<T, U, V> t) {
            int cmp = first.compareTo(t.first);
            if (cmp != 0) return cmp;
            cmp = second.compareTo(t.second);
            return (cmp == 0) ? third.compareTo(t.third) : cmp;
        }
    }
}
