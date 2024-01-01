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

        int[] sets = new int[r * c], rank = new int[r * c];
        for (int i = 0; i < sets.length; i++) sets[i] = i;

        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                if (i > 0 && world[i][j] == world[i - 1][j]) union(i * c + j, (i - 1) * c + j, sets, rank);
                if (j > 0 && world[i][j] == world[i][j - 1]) union(i * c + j, i * c + j - 1, sets, rank);
            }

        int n = Integer.parseInt(br.readLine());

        while (n-- > 0) {
            var query = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            int r1 = query[0], c1 = query[1], r2 = query[2], c2 = query[3];

            if (find((r1 - 1) * c + c1 - 1, sets) == find((r2 - 1) * c + c2 - 1, sets)) System.out.println((world[r1 - 1][c1 - 1] == '0' ? "binary" : "decimal"));
            else System.out.println("neither");
        }
    }

    static void union(int curr, int prev, int[] sets, int[] rank) {
        curr = find(curr, sets);
        prev = find(prev, sets);

        if (curr != prev) {
            sets[rank[curr] < rank[prev] ? curr : prev] = rank[curr] < rank[prev] ? prev : curr;
            rank[curr] = rank[curr] == rank[prev] ? rank[curr]++ : rank[curr];
        }
    }

    static int find(int p, int[] sets) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
    }
}
