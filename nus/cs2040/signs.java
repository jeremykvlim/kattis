import java.io.*;
import java.util.*;

public class signs {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var signs = new ArrayList<Triple<String, Integer, String>>(n);
        for (int i = 0; i < n; i++) {
            var sign = br.readLine();
            signs.add(((sign.length() & 1) == 0) ? new Triple<>(sign.substring(sign.length() / 2 - 1, sign.length() / 2 + 1), i, sign)
                                                 : new Triple<>(String.valueOf(sign.charAt(sign.length() / 2)), i, sign));
        }
        Collections.sort(signs);

        for (var sign : signs) pw.println(sign.third);
        pw.flush();
    }

    record Triple<T extends Comparable<T>, U extends Comparable<U>, V extends Comparable<V>>(T first, U second, V third) implements Comparable<Triple<T, U, V>> {
        @Override
        public int compareTo(Triple<T, U, V> t) {
            int cmp = first.compareTo(t.first);
            if (cmp != 0) return cmp;
            cmp = second.compareTo(t.second);
            return (cmp == 0) ? third.compareTo(t.third) : cmp;
        }
    }
}
