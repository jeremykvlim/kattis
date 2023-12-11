import java.io.*;
import java.util.*;

public class cheatingluck {
     public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int d = input[0], g = input[1], n = input[2], k = input[3], r = n - k, total = d + g;
        var coins = new long[d + g][n + 1][n + 1];
        var bet = new long[d + g][n + 1][n + 1];
        System.out.println(play(d, n, r, total, bet, coins));
    }
    
    static long play(int d, int n, int r, long total, long[][][] bets, long[][][] coins) {
        if (d >= total) return total;
        if (d == 0 || n == 0 || r == 0) return Math.min(total, d * (1L << n));
        if (coins[d][n][r] != 0) return coins[d][n][r];

        coins[d][n][r] = d;
        play(d - 1, n, r, total, bets, coins);
        int least = (int) bets[d - 1][n][r], most = Math.min(d, (int) (total - d));
        least = Math.min(least, most);
        bets[d][n][r] = least;
        for (int bet = least; bet <= most; bet++) {
            long max = Math.min(play(d - bet, n - 1, r - 1, total, bets, coins), play(d + bet, n - 1, r, total, bets, coins));
            if (max < coins[d][n][r]) break;
            bets[d][n][r] = bet;
            coins[d][n][r] = max;
        }
        return coins[d][n][r];
    }
}
