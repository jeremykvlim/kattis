import java.io.*;
import java.util.*;

public class promotions {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int a = input[0], b = input[1], e = input[2], p = input[3];

        var outgoing = new ArrayList[e];
        var incoming = new ArrayList[e];
        
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

        var visiting = new boolean[e];
        var visited_by = new boolean[e];
        int promo_a = 0, promo_b = 0, promo_no = 0;
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
        for (int u : adjList[v])
            if (!visited[u]) count += dfs(u, adjList, visited);

        return count;
    }
}
