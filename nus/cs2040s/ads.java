import java.io.*;
import java.util.*;

public class ads {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = br.readLine().split(" ");
        int h = Integer.parseInt(input[0]), w = Integer.parseInt(input[1]);

        var page = new char[h][];
        for (int i = 0; i < h; i++) page[i] = br.readLine().toCharArray();

        var visited = new boolean[h][w];
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                if (!visited[i][j] && page[i][j] == '+') remove(i, j, page, visited);

        for (var l : page) pw.println(new String(l));
        pw.flush();
    }

    static void remove(int r, int c, char[][] page, boolean[][] visited) {
        int h = r, w = c;
        while (h < page.length - 1 && page[h + 1][c] == '+') h++;
        while (w < page[0].length - 1 && page[r][w + 1] == '+') w++;

        for (int i = r + 1; i < h; i++)
            for (int j = c + 1; j < w; j++)
                if (page[i][j] == '+') remove(i, j, page, visited);

        for (int i = r + 1; i < h; i++)
            for (int j = c + 1; j < w; j++) {
                visited[i][j] = true;
                if (Character.isLetter(page[i][j]) || "+?!,. ".indexOf(page[i][j]) >= 0) continue;
                for (int k = r; k <= h; k++) {
                    Arrays.fill(page[k], c, w + 1, ' ');
                    Arrays.fill(visited[k], c, w + 1, true);
                }
                break;
            }
    }
}
