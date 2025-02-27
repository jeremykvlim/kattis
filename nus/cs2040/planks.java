import java.io.*;
import java.util.*;

public class planks {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int q = Integer.parseInt(br.readLine());

        var planks = new TreeMap<Integer, TreeMap<Integer, Integer>>();
        while (q-- > 0) {
            var parts = br.readLine().split(" ");
            char op = parts[0].charAt(0);

            if (op == 'a') {
                int wp = Integer.parseInt(parts[1]), lp = Integer.parseInt(parts[2]);
                planks.computeIfAbsent(lp, k -> new TreeMap<>()).merge(wp, 1, Integer::sum);
            } else if (op == 'c') {
                int x = Integer.parseInt(parts[1]);

                var a = plank(planks, x, true);
                var b = plank(planks, x, false);
                pw.println((1L + Math.abs(a.first - b.first)) * (1L + a.second + b.second));
            }
        }
        pw.flush();
    }

    static Pair<Integer, Integer> plank(TreeMap<Integer, TreeMap<Integer, Integer>> planks, int x, boolean a) {
        int l = a ? planks.floorKey(x) : planks.ceilingKey(x);
        var weights = planks.get(l);
        int w = a ? weights.firstKey() : weights.lastKey();

        if (weights.get(w) == 1) {
            weights.remove(w);

            if (weights.isEmpty()) planks.remove(l);
        } else weights.put(w, weights.get(w) - 1);

        return new Pair<>(l, w);
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
