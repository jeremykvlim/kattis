import java.io.*;
import java.util.*;

public class jetpack {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var field = new char[10][n];
        for (int i = 0; i < field.length; i++) field[i] = br.readLine().toCharArray();

        var moves = new ArrayList<Integer>();
        dfs(9, 0, n, field, moves);
        Collections.reverse(moves);

        pw.println(moves.size());
        moves.forEach(t -> pw.println(t + " 1"));
        pw.flush();
    }

    static boolean dfs(int i, int j, int n, char[][] field, ArrayList<Integer> moves) {
        if (field[i][j] == 'X') return false;
        if (j == n - 1 || dfs(Math.min(9, i + 1), j + 1, n, field, moves)) return true;
        field[i][j] = 'X';
        if (dfs(Math.max(0, i - 1), j + 1, n, field, moves)) {
            moves.add(j);
            return true;
        }
        return false;
    }
}
