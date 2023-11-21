import java.io.*;
import java.util.*;

public class blockcrusher {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(new BufferedWriter(new OutputStreamWriter(System.out)));

        for (var line = br.readLine(); !line.equals("0 0"); line = br.readLine()) {
            var input = line.split(" ");
            int h = Integer.parseInt(input[0]), w = Integer.parseInt(input[1]);
            var block = new char[h][w];
            var strength = new int[h][w];

            for (int i = 0; i < h; i++) {
                block[i] = br.readLine().toCharArray();
                Arrays.fill(strength[i], Integer.MAX_VALUE);
            }

            var pq = new PriorityQueue<Digit>();
            for (int i = 0; i < w; i++)
                pq.offer(new Digit(0, i, block[0][i] - '0', null));


            int[] dx = {1, 1, 1, -1, -1, -1, 0, 0}, dy = {-1, 0, 1, -1, 0, 1, -1, 1};

            Digit min = null;
            while (!pq.isEmpty()) {
                var v = pq.poll();
                if (v.x == h - 1) min = (min == null) ? v : (min.strength < v.strength ? min : v);

                for (int i = 0; i < 8; i++) {
                    int x = v.x + dx[i], y = v.y + dy[i];
                    if (x >= 0 && y >= 0 && x < h && y < w && v.strength + block[x][y] - '0' < strength[x][y]) {
                        strength[x][y] = v.strength + block[x][y] - '0';
                        pq.offer(new Digit(x, y, strength[x][y], v));
                    }
                }
            }

            while (min != null) {
                block[min.x][min.y] = ' ';
                min = min.prev;
            }

            Arrays.stream(block).forEach(row -> pw.println(new String(row)));
            pw.println();
        }
        pw.flush();
    }

    static class Digit implements Comparable<Digit> {
        int x, y, strength;
        Digit prev;

        Digit(int x, int y, int s, Digit d) {
            this.x = x;
            this.y = y;
            strength = s;
            prev = d;
        }

        @Override
        public int compareTo(Digit d) {
            return strength - d.strength;
        }
    }
}

