import java.io.*;
import java.util.*;

public class hyacinth {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int n = Integer.parseInt(br.readLine());
        var adjList = new ArrayList[n + 1];
        for (int i = 1; i <= n; i++) adjList[i] = new ArrayList<Integer>();
        
        for (int m = 0; m < n - 1; m++) {
            var line = br.readLine().split(" ");
            adjList[Integer.parseInt(line[0])].add(Integer.parseInt(line[1]));
            adjList[Integer.parseInt(line[1])].add(Integer.parseInt(line[0]));
        }
        
        var q = new ArrayDeque<Integer>();
        for (int i = 1; i <= n; i++) {
            if (adjList[i].size() == 1) {
                q.add(i);
                break;
            }
        }
        
        var freq = new int[n + 1][2];
        for (int i = 1; i <= n; i++) Arrays.fill(freq[i], 1);
        
        var visited = new boolean[n + 1];
        int f = 1;
        while (!q.isEmpty()) {
            int v = q.poll();
            visited[v] = true;
            freq[v][1] = f++;
            
            for (Object u : adjList[v]) {
                if (!visited[(int) u]) {
                    q.add((Integer) u);
                    freq[(int) u][0] = freq[v][1];
                }
            }
        }
        
        if (n == 2) freq[1][0] = n;
        for (int i = 1; i <= n; i++) pw.println(freq[i][0] + " " + freq[i][1]);
        pw.flush();
    }
}
