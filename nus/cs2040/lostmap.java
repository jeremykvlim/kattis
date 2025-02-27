import java.io.*;
import java.util.*;

public class lostmap {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var adjMatrix = new int[n][n];
        for (int i = 0; i < n; i++) adjMatrix[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();

        for (var e : prim_dense(n, adjMatrix)) pw.println(e.first + " " + e.second);
        pw.flush();
    }

    static ArrayList<Pair<Integer, Integer>> prim_dense(int n, int[][] adjMatrix) {
        var dist = new int[n];
        Arrays.fill(dist, Integer.MAX_VALUE);
        dist[0] = 0;
        var visited = new boolean[n];
        var mst = new ArrayList<Pair<Integer, Integer>>(n - 1);
        for (int i = 0; i < n - 1; i++) mst.add(new Pair<>(-1, -1));
        for (int i = 0; i < n; i++) {
            int d = Integer.MAX_VALUE, j = 0;
            for (int k = 0; k < n; k++)
                if (!visited[k] && d > dist[k]) {
                    d = dist[k];
                    j = k;
                }

            visited[j] = true;
            for (int k = 0; k < n; k++)
                if (!visited[k] && dist[k] > adjMatrix[j][k]) {
                    dist[k] = adjMatrix[j][k];
                    mst.set(k - 1, new Pair<>(Math.min(j, k) + 1, Math.max(j, k) + 1));
                }
        }

        return mst;
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
