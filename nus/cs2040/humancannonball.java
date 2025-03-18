import java.io.*;
import java.util.*;

public class humancannonball {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var src = Arrays.stream(br.readLine().split(" ")).mapToDouble(Double::parseDouble).toArray();
        var dest = Arrays.stream(br.readLine().split(" ")).mapToDouble(Double::parseDouble).toArray();
        int n = Integer.parseInt(br.readLine());
        var coords = new double[n + 2][2];
        coords[0] = src;
        coords[n + 1] = dest;
        for (int i = 1; i <= n; i++) coords[i] = Arrays.stream(br.readLine().split(" ")).mapToDouble(Double::parseDouble).toArray();

        var adjMatrix = new double[n + 2][n + 2];
        for (int i = 1; i < n + 2; i++)
            adjMatrix[0][i] = Math.hypot(coords[0][0] - coords[i][0], coords[0][1] - coords[i][1]) / 5;

        for (int i = 1; i < n + 1; i++)
            for (int j = 1; j < n + 2; j++)
                adjMatrix[i][j] = Math.min(2 + Math.abs(Math.hypot(coords[i][0] - coords[j][0], coords[i][1] - coords[j][1]) - 50) / 5,
                                           Math.hypot(coords[i][0] - coords[j][0], coords[i][1] - coords[j][1]) / 5);

        var time = new double[n + 2];
        Arrays.fill(time, Double.POSITIVE_INFINITY);
        time[0] = 0;
        var pq = new PriorityQueue<Pair<Double, Integer>>();
        pq.add(new Pair<>(0., 0));
        while (!pq.isEmpty()) {
            var p = pq.poll();
            var d = p.first;
            int v = p.second;

            if (d != time[v]) continue;

            for (int u = 0; u < n + 2; u++)
                if (time[u] > time[v] + adjMatrix[v][u]) {
                    time[u] = time[v] + adjMatrix[v][u];
                    pq.add(new Pair<>(time[u], u));
                }
        }
        pw.format("%3f", time[n + 1]);
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
