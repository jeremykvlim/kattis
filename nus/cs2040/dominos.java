import java.io.*;
import java.util.*;

public class dominos {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            var inputs = br.readLine().split(" ");
            int n = Integer.parseInt(inputs[0]), m = Integer.parseInt(inputs[1]);

            var sets = new int[n + 1];
            for (int i = 1; i <= n; i++) sets[i] = i;

            var visited = new boolean[n + 1];
            int disjoint = n;
            while (m-- > 0) {
                var edge = br.readLine().split(" ");
                int x = Integer.parseInt(edge[0]), y = Integer.parseInt(edge[1]),
                    x_set = find(x, sets), y_set = find(y, sets);

                if (!visited[y]) {
                    visited[y] = true;
                    n--;
                }

                if (x_set != y_set) {
                    sets[y_set] = x_set;
                    disjoint--;
                }
            }

            pw.println(Math.max(disjoint, n));
        }
        pw.flush();
    }

    static int find(int p, int[] sets) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
    }
}
