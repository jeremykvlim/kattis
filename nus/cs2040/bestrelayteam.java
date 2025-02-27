import java.io.*;
import java.util.*;

public class bestrelayteam {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var runners = new ArrayList<Triple<Double, Double, String>>(n);
        for (int i = 0; i < n; i++) {
            var line = br.readLine().split(" ");
            runners.add(new Triple<>(Double.parseDouble(line[2]), Double.parseDouble(line[1]), line[0]));
        }
        Collections.sort(runners);

        var time = Double.POSITIVE_INFINITY;
        var team = new ArrayList<String>();
        for (var r1 : runners) {
            var t = r1.second;
            var r = new ArrayList<String>();
            r.add(r1.third);

            for (var r2 : runners)
                if (r1 != r2) {
                    t += r2.first;
                    r.add(r2.third);
                    if (r.size() == 4) break;
                }

            if (time > t) {
                time = t;
                team = r;
            }
        }

        pw.println(time);
        for (var r : team) pw.println(r);
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
