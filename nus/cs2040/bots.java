import java.io.*;
import java.util.*;

public class bots {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int n = input[0], m = input[1];

        var adjListRegular = new ArrayList[n];
        var adjListTranspose = new ArrayList[n];
        for (int i = 0; i < n; i++) {
            adjListRegular[i] = new ArrayList<>();
            adjListTranspose[i] = new ArrayList<>();
        }

        while (m-- > 0) {
            input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            int p = input[0], q = input[1];

            adjListRegular[p].add(q);
            adjListTranspose[q].add(p);
        }

        var visited = new boolean[n];
        var s = new Stack<Integer>();
        for (int i = 0; i < n; i++)
            if (!visited[i]) dfs1(i, visited, adjListRegular, s);

        var component = new int[n];
        Arrays.fill(component, -1);
        int sccs = 0;
        while (!s.isEmpty()) {
            int v = s.pop();

            if (component[v] == -1) {
                dfs2(v, adjListTranspose, component, sccs);
                sccs++;
            }
        }

        var size = new int[sccs];
        for (int i = 0; i < n; i++) size[component[i]]++;

        var neighbor = new ArrayList<HashSet<Integer>>(sccs);
        for (int i = 0; i < sccs; i++) neighbor.add(new HashSet<>());

        var degree = new int[sccs];
        for (int v = 0; v < n; v++)
            for (var u : adjListRegular[v])
                if (component[v] != component[(int) u])
                    if (!neighbor.get(component[v]).contains(component[(int) u])) {
                        neighbor.get(component[v]).add(component[(int) u]);
                        degree[component[(int) u]]++;
                    }

        int solo = 0, net = 0;
        for (int i = 0; i < sccs; i++)
            if (degree[i] == 0) {
                if (size[i] == 1) solo++;
                else net++;
            }
        pw.println(solo + " " + net);
        pw.flush();
    }

    static void dfs1(int v, boolean[] visited, ArrayList<Integer>[] adjListRegular, Stack<Integer> s) {
        visited[v] = true;
        for (var u : adjListRegular[v])
            if (!visited[u]) dfs1(u, visited, adjListRegular, s);

        s.push(v);
    }

    static void dfs2(int v, ArrayList<Integer>[] adjListTranspose, int[] component, int sccs) {
        component[v] = sccs;
        for (var u : adjListTranspose[v])
            if (component[u] == -1) dfs2(u, adjListTranspose, component, sccs);
    }
}
