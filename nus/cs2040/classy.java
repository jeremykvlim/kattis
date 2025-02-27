import java.io.*;
import java.util.*;

public class classy {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            int n = Integer.parseInt(br.readLine());

            var people = new ArrayList<Pair<String, String>>(n);
            for (int i = 0; i < n; i++) {
                var input = br.readLine().split(" ");
                var classes = input[1].split("-");
                var levels = new char[10];
                Arrays.fill(levels, 'b');

                for (int j = classes.length - 1, k = 0; j >= 0; j--, k++) {
                    if (classes[j].charAt(0) == 'u') levels[k] = 'a';
                    else if (classes[j].charAt(0) == 'l') levels[k] = 'c';
                }
                people.add(new Pair<>(new String(levels), input[0].substring(0, input[0].length() - 1)));
            }
            Collections.sort(people);

            for (var p : people) pw.println(p.second);
            for (int i = 0; i < 30; i++) pw.print('=');
            pw.println();
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
