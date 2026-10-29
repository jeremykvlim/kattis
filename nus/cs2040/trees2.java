import java.io.*;
import java.util.*;

public class trees2 {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int q = Integer.parseInt(br.readLine());
        var pos = new ArrayList<Integer>();
        var queries = new ArrayList<Triple<Character, Integer, Integer>>(q);
        for (int i = 0; i < q; i++) {
            var line = br.readLine().split(" ");
            var op = line[0].charAt(0);
            int x = Integer.parseInt(line[1]), y = Integer.parseInt(line[2]);
            queries.add(new Triple<>(op, x, y));
            if (op == 'u') pos.add(x);
        }
        pos.add(0);
        Collections.sort(pos);

        int n = 0;
        for (int i = 0; i < pos.size(); i++) {
            int p = pos.get(i);
            if (n == 0 || pos.get(n - 1) != p) pos.set(n++, p);
        }
        pos.subList(n, pos.size()).clear();

        var st = new PURQSegmentTree(n);
        for (var query : queries) {
            char op = query.first;
            int x = query.second, y = query.third, l = Collections.binarySearch(pos, x);
            if (l < 0) l = -l - 1;
            if (op == 'u') {
                pw.println(st.ST[l + n].value);
                st.point_update(l, y);
            } else {
                int r = Collections.binarySearch(pos, y);
                if (r < 0) r = -r - 2;
                pw.println(st.range_query(l, r + 1).value);
            }
        }
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

    static class PURQSegmentTree {
        static class Monoid {
            int value;

            Monoid() {
                value = 0;
            }

            Monoid(int value) {
                this.value = value;
            }

            Monoid add(Monoid other) {
                return new Monoid(Math.max(value, other.value));
            }
        }

        int n;
        Monoid[] ST;

        PURQSegmentTree(int n) {
            this.n = n;
            ST = new Monoid[2 * n];
            for (int i = 0; i < 2 * n; i++) ST[i] = new Monoid();
        }

        void pull(int i) {
            ST[i] = ST[i << 1].add(ST[i << 1 | 1]);
        }

        void build() {
            for (int i = n - 1; i > 0; i--) pull(i);
        }

        void point_update(int i, int v) {
            for (ST[i += n] = new Monoid(v); i > 1; i >>= 1) pull(i >> 1);
        }

        Monoid range_query(int l, int r) {
            Monoid ml = new Monoid(), mr = new Monoid();
            for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
                if ((l & 1) == 1) ml = ml.add(ST[l++]);
                if ((r & 1) == 1) mr = ST[--r].add(mr);
            }

            return ml.add(mr);
        }
    }
}
