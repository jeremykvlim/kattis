import java.io.*;
import java.util.*;

public class lostmap {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var adjMatrix = new int[n][n];
        for (int i = 0; i < n; i++) adjMatrix[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();

        for (var e : prim_dense(n, adjMatrix).first) pw.println(e.first + " " + e.second);
        pw.flush();
    }

    static Tuple<ArrayList<Pair<Integer, Integer>>, Integer> prim_dense(int n, int[][] adjMatrix) {
        var dist = new int[n];
        Arrays.fill(dist, Integer.MAX_VALUE);
        dist[0] = 0;
        var visited = new boolean[n];
        var mst = new ArrayList<Pair<Integer, Integer>>(n - 1);
        for (int i = 0; i < n - 1; i++) mst.add(new Pair<>(-1, -1));

        int weight = 0;
        for (int __ = 0; __ < n; __++) {
            int u = -1;
            for (int v = 0; v < n; v++)
                if (!visited[v] && (u < 0 || dist[v] < dist[u])) u = v;

            visited[u] = true;
            weight += dist[u];
            for (int k = 0; k < n; k++)
                if (!visited[k] && dist[k] > adjMatrix[u][k]) {
                    dist[k] = adjMatrix[u][k];
                    mst.set(k - 1, new Pair<>(Math.min(u, k) + 1, Math.max(u, k) + 1));
                }
        }

        return new Tuple<>(mst, weight);
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
