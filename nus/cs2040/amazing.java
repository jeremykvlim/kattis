import java.io.*;

public class amazing {
    static BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    static PrintWriter pw = new PrintWriter(System.out);

    public static void main(String[] args) throws IOException {
        var visited = new boolean[201][201];
        if (!dfs(100, 100, visited)) {
            pw.println("no way out");
            pw.flush();
        }
    }
    
    static boolean dfs(int x, int y, boolean[][] visited) throws IOException {
        visited[x][y] = true;

        int[] dx = {1, 0, -1, 0}, dy = {0, 1, 0, -1};
        String[] dir = {"right", "down", "left", "up"};
        for (int i = 0; i < 4; i++) {
            if (!visited[x + dx[i]][y + dy[i]]) {
                pw.println(dir[i]);
                pw.flush();

                var response = br.readLine();
                if (response.equals("solved")) return true;
                else if (response.equals("ok")) {
                    if (dfs(x + dx[i], y + dy[i], visited)) return true;
                    else {
                        pw.println(dir[(i + 2) % 4]);
                        pw.flush();
                        br.readLine();
                    }
                }
            }
        }
        return false;
    }
}
