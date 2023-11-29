import java.io.*;
import java.util.*;

public class jetpack {
    static boolean dfs(int x, int y, int n, char[][] field, ArrayList<Integer> moves) {
        if (field[x][y] == 'X') return false;
        if (y == n - 1 || dfs(Math.min(9, x + 1), y + 1, n, field, moves)) return true;
        field[x][y] = 'X';
        if (dfs(Math.max(0, x - 1), y + 1, n, field, moves)) {
            moves.add(y);
            return true;
        }
        return false;
    }

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        char[][] field = new char[10][n];
        for (int i = 0; i < 10; i++) field[i] = br.readLine().toCharArray();

        var moves = new ArrayList<Integer>();
        dfs(9, 0, n, field, moves);
        Collections.reverse(moves);

        pw.println(moves.size());
        moves.forEach(t -> pw.println(t + " 1"));
        pw.flush();
    }
}
