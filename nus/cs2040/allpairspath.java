import java.io.*;
import java.util.*;

public class allpairspath {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        for (var input = br.readLine(); !input.equals("0 0 0"); input = br.readLine()) {
            var line = input.split(" ");
            int n = Integer.parseInt(line[0]), m = Integer.parseInt(line[1]), q = Integer.parseInt(line[2]);

            var dist = new int[n][n];
            for (var row : dist) Arrays.fill(row, Integer.MAX_VALUE);
            for (int i = 0; i < n; i++) dist[i][i] = 0;

            while (m-- > 0) {
                var edge = br.readLine().split(" ");
                int u = Integer.parseInt(edge[0]), v = Integer.parseInt(edge[1]), w = Integer.parseInt(edge[2]);
                dist[u][v] = Math.min(dist[u][v], w);
            }

            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    for (int k = 0; k < n; k++)
                        if (dist[j][i] != Integer.MAX_VALUE && dist[i][k] != Integer.MAX_VALUE)
                            dist[j][k] = Math.min(dist[j][k], dist[j][i] + dist[i][k]);

            for (int i = 0; i < n; i++)
                if (dist[i][i] < 0)
                    for (int j = 0; j < n; j++)
                        for (int k = 0; k < n; k++)
                            if (dist[j][i] != Integer.MAX_VALUE && dist[i][k] != Integer.MAX_VALUE)
                                dist[j][k] = -Integer.MAX_VALUE;


            while (q-- > 0) {
                var query = br.readLine().split(" ");
                int u = Integer.parseInt(query[0]), v = Integer.parseInt(query[1]), w = dist[u][v];
                pw.println(w == Integer.MAX_VALUE ? "Impossible" : (w == -Integer.MAX_VALUE ? "-Infinity" : w));
            }
            pw.println();
        }
        pw.close();
    }
}
