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
            for (int i = 0; i < m; i++) {
                var line = br.readLine().split(" ");
                distances[i][0] = Double.parseDouble(line[0]);
                distances[i][1] = Double.parseDouble(line[1]);
            }

            var dist = new double[m];
            Arrays.fill(dist, Double.POSITIVE_INFINITY);
            dist[0] = 0;

            var pq = new PriorityQueue<Pair<Double, Integer>>();
            pq.add(new Pair<>(0., 0));

            var visited = new boolean[m];
            var total = 0.;

            while (!pq.isEmpty()) {
                var p = pq.poll();
                int v = p.second;

                if (visited[v]) continue;
                visited[v] = true;
                total += p.first;

                for (int u = 0; u < m; u++) {
                    if (visited[u]) continue;
                    var d = Math.hypot(distances[v][0] - distances[u][0], distances[v][1] - distances[u][1]);
                    if (dist[u] > d) {
                        dist[u] = d;
                        pq.add(new Pair<>(d, u));
                    }
                }
            }

            pw.format("%.3f\n", total);
        }
        pw.flush();
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
