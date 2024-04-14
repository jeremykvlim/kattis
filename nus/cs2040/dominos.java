public class dominos {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            var inputs = br.readLine().split(" ");
            int n = Integer.parseInt(inputs[0]), m = Integer.parseInt(inputs[1]);

            var adjList = new ArrayList[n + 1];
            for (int i = 1; i <= n; i++) adjList[i] = new ArrayList<Integer>();

            var indegree = new int[n + 1];
            while (m-- > 0) {
                var vertices = br.readLine().split(" ");
                int x = Integer.parseInt(vertices[0]), y = Integer.parseInt(vertices[1]);

                adjList[x].add(y);
                indegree[y]++;
            }

            int count = 0;
            var visited = new boolean[n + 1];
            for (int i = 1; i <= n; i++)
                if (!visited[i] && indegree[i] == 0) {
                    count++;
                    dfs(i, visited, adjList);
                }

            for (int i = 1; i <= n; i++)
                if (!visited[i])
                    for (var j : adjList[i]) adjList[(int) j].add(i);

            for (int i = 1; i <= n; i++)
                if (!visited[i]) {
                    count++;
                    dfs(i, visited, adjList);
                }

            System.out.println(count);
        }
    }

    static void dfs(int v, boolean[] visited, ArrayList[] adjList) {
        visited[v] = true;

        for (var u : adjList[v])
            if (!visited[(int) u]) dfs((int) u, visited, adjList);
    }
}
