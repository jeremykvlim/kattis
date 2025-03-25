import java.io.*;
import java.util.*;

public class 10kindsofpeople {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = br.readLine().split(" ");
        int r = Integer.parseInt(input[0]), c = Integer.parseInt(input[1]);

        var world = new char[r][c];
        for (int i = 0; i < r; i++) world[i] = br.readLine().toCharArray();

        var dsu = new DisjointSets(r * c);

        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                if (i > 0 && world[i][j] == world[i - 1][j]) dsu.unite(i * c + j, (i - 1) * c + j);
                if (j > 0 && world[i][j] == world[i][j - 1]) dsu.unite(i * c + j, i * c + j - 1);
            }

        int n = Integer.parseInt(br.readLine());

        while (n-- > 0) {
            var query = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            int r1 = query[0], c1 = query[1], r2 = query[2], c2 = query[3];

            if (dsu.find((r1 - 1) * c + c1 - 1) == dsu.find((r2 - 1) * c + c2 - 1)) pw.println((world[r1 - 1][c1 - 1] == '0' ? "binary" : "decimal"));
            else pw.println("neither");
        }
        pw.flush();
    }

    static class DisjointSets {
        int[] sets;

        DisjointSets(int n) {
            sets = new int[n];
            for (int i = 0; i < n; i++) sets[i] = i;
        }

        boolean unite(int u, int v) {
            u = find(u);
            v = find(v);
            if (u != v) {
                sets[v] = u;
                return true;
            }
            return false;
        }

        int find(int v) {
            return sets[v] == v ? v : (sets[v] = find(sets[v]));
        }
    }
}
