import java.io.*;
import java.util.*;

public class nesteddolls {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int t = Integer.parseInt(br.readLine());

        while (t-- > 0) {
            int m = Integer.parseInt(br.readLine());

            var dolls = new ArrayList<Pair<Integer, Integer>>(m);
            var input = br.readLine().split(" ");
            for (int i = 0; i < m; i++) dolls.add(new Pair<>(Integer.parseInt(input[2 * i]), -Integer.parseInt(input[2 * i + 1])));
            Collections.sort(dolls);

            var lnds = new int[m];
            lnds[0] = -dolls.get(0).second;
            int len = 1;
            for (int i = 1; i < m; i++) {
                int height = -dolls.get(i).second;
                if (lnds[0] < height) lnds[0] = height;
                else if (lnds[len - 1] >= height) lnds[len++] = height;
                else {
                    int l = -1, r = len, mid;
                    while (l + 1 < r) {
                        mid = l + (r - l) / 2;

                        if (lnds[mid] < height) r = mid;
                        else l = mid;
                    }
                    lnds[r] = height;
                }
            }
            pw.println(len);
        }
        pw.flush();
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
