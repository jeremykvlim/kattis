import java.io.*;
import java.util.*;

public class lostmap {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var dist = new int[n][n];

        for (int i = 0; i < n; i++)
            dist[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();

        var adjMatrix = new int[n][2];
        for (int i = 1; i < n; i++)
            adjMatrix[i][0] = dist[0][i];

        for (int i = 1; i < n; i++) {
            int v = 1, min = Integer.MAX_VALUE;
            for (int u = 1; u < n; u++) {
                if (adjMatrix[u][0] < min) {
                    min = adjMatrix[u][0];
                    v = u;
                }
            }
            adjMatrix[v][0] = Integer.MAX_VALUE;
            pw.println((adjMatrix[v][1] + 1) + " " + (v + 1));
            for (int u = 1; u < n; u++) {
                if (adjMatrix[u][0] != Integer.MAX_VALUE && u != v && adjMatrix[u][0] > dist[v][u]) {
                    adjMatrix[u][0] = dist[v][u];
                    adjMatrix[u][1] = v;
                }
            }
        }
        pw.flush();
    }
}
