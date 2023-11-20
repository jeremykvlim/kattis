import java.util.*;
import java.io.*;

public class promotions {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split(" ");
        int a = Integer.parseInt(input[0]), b = Integer.parseInt(input[1]), e = Integer.parseInt(input[2]), p = Integer.parseInt(input[3]),
                promo_a = 0, promo_b = 0, promo_no = 0;

        var outgoing = new ArrayList[e];
        var incoming = new ArrayList[e];
        var visiting = new boolean[e];
        var visited_by = new boolean[e];


        for (int i = 0; i < e; i++) {
            outgoing[i] = new ArrayList<>();
            incoming[i] = new ArrayList<>();
        }

        while (p-- > 0) {
            var edge = br.readLine().split(" ");
            int x = Integer.parseInt(edge[0]), y = Integer.parseInt(edge[1]);
            outgoing[x].add(y);
            incoming[y].add(x);
        }

        for (int i = 0; i < e; i++) {
            Arrays.fill(visiting, false);
            Arrays.fill(visited_by, false);
            int out = dfs(i, outgoing, visiting) - 1, in = dfs(i, incoming, visited_by) - 1;
            if (out >= e - a) promo_a++;
            if (out >= e - b) promo_b++;
            if (in >= b) promo_no++;
        }
        System.out.println(promo_a + "\n" + promo_b + "\n" + promo_no);
    }

    static int dfs(int v, ArrayList<Integer>[] adjList, boolean[] visited) {
        int count = 1;
        visited[v] = true;
        for (int i : adjList[v])
            if (!visited[i]) count += dfs(i, adjList, visited);

        return count;
    }
}
