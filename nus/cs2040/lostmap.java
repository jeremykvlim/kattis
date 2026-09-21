import java.io.*;
import java.util.*;

public class lostmap {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var adjMatrix = new int[n][n];
        for (int i = 0; i < n; i++) adjMatrix[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();

        for (var e : prim_dense(n, adjMatrix).second) pw.println((e.first + 1) + " " + (e.second + 1));
        pw.flush();
    }

    static Tuple<Integer, Pair<Integer, Integer>[]> prim_dense(int n, int[][] adjMatrix) {
        var dist = new int[n];
        Arrays.fill(dist, Integer.MAX_VALUE);
        dist[0] = 0;
        var visited = new boolean[n];
        int len = 0;
        var mst = new Pair[n - 1];
        for (int __ = 0; __ < n; __++) {
            int u = -1;
            for (int v = 0; v < n; v++)
                if (!visited[v] && (u < 0 || dist[v] < dist[u])) u = v;

            visited[u] = true;
            len += dist[u];
            for (int v = 1; v < n; v++)
                if (!visited[v] && dist[v] > adjMatrix[u][v]) {
                    dist[v] = adjMatrix[u][v];
                    mst[v - 1] = new Pair<>(Math.min(u, v), Math.max(u, v));
                }
        }
        return new Tuple<>(len, mst);
    }

    record Tuple<T, U>(T first, U second) {}

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
