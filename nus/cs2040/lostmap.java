import java.io.*;
import java.util.*;

public class lostmap {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var adjMatrix = new int[n][n];
        for (int i = 0; i < n; i++) adjMatrix[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();

        for (var e : prim_dense(n, adjMatrix)) pw.println(e.v + " " + e.u);
        pw.flush();
    }

    static Edge[] prim_dense(int n, int[][] adjMatrix) {
        var dist = new int[n];
        Arrays.fill(dist, Integer.MAX_VALUE);
        dist[0] = 0;
        var visited = new boolean[n];
        var mst = new Edge[n - 1];
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
                    mst[k - 1] = new Edge(Math.min(j, k) + 1, Math.max(j, k) + 1);
                }
        }

        return mst;
    }

    static class Edge {
        int v, u;

        Edge(int f, int s) {
            v = f;
            u = s;
        }
    }
}
