import java.io.*;
import java.util.*;

public class islandhopping {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int n = Integer.parseInt(br.readLine());
        while (n-- > 0) {
            int m = Integer.parseInt(br.readLine());
            var distances = new double[m][2];
            for (int i = 0; i < m; i++){
                var line = br.readLine().split(" ");
                distances[i][0] = Double.parseDouble(line[0]);
                distances[i][1] = Double.parseDouble(line[1]);
            }

            var min = new double[m];
            Arrays.fill(min, Double.POSITIVE_INFINITY);
            min[0] = 0;

            var pq = new PriorityQueue<Pair>();
            pq.add(new Pair(0, 0));

            var visited = new boolean[m];
            var total = 0.0;

            while (!pq.isEmpty()) {
                var curr = pq.poll();
                if (visited[curr.first]) continue;
                visited[curr.first] = true;
                total += curr.second;

                for (int i = 0; i < m; i++) {
                    if (visited[i]) continue;
                    var dist = Math.hypot(distances[curr.first][0] - distances[i][0], distances[curr.first][1] - distances[i][1]);
                    if (dist < min[i]) {
                        min[i] = dist;
                        pq.add(new Pair(i, dist));
                    }
                }
            }

            pw.format("%.3f\n", total);
            pw.flush();
        }
    }

    static class Pair implements Comparable<Pair> {
        int first;
        double second;

        public Pair(int f, double s){
            first = f;
            second = s;
        }

        @Override
        public int compareTo(Pair p) {
            return Double.compare(this.second, p.second);
        }
    }
}
