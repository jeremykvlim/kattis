import java.io.*;
import java.util.*;

public class sumsets {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int n = Integer.parseInt(br.readLine());
        var s = new int[n];
        for (int i = 0; i < n; i++) s[i] = Integer.parseInt(br.readLine());

        Arrays.sort(s);
        var sums = new HashMap<Integer, Pair>();
        for (int a = 0; a < n; a++)
            for (int b = a + 1; b < n; b++)
                sums.put(s[a] + s[b], new Pair(a, b));

        for (int d = n - 1; d >= 0; d--) {
            for (int c = 0; c < n; c++) {
                if (d == c) continue;
                var p = sums.get(s[d] - s[c]);
                if (p != null && p.first != d && p.second != d && p.first != c && p.second != c) {
                    System.out.println(s[d]);
                    return;
                }
            }
        }
        System.out.println("no solution");
    }

    static class Pair {
        int first, second;

        Pair(int f, int s) {
            first = f;
            second = s;
        }
    }
}
