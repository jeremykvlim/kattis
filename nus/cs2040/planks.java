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

                int[] a = plank(planks, x, true), b = plank(planks, x, false);
                pw.println((1L + a[1] + b[1]) * (1L + Math.abs(a[0] - b[0])));
            }
        }
        pw.flush();
    }

    static int[] plank(TreeMap<Integer, TreeMap<Integer, Integer>> planks, int x, boolean a) {
        int l = a ? planks.floorKey(x) : planks.ceilingKey(x);
        var weights = planks.get(l);
        int w = a ? weights.firstKey() : weights.lastKey();

        if (weights.get(w) == 1) {
            weights.remove(w);

            if (weights.isEmpty()) planks.remove(l);
        } else weights.put(w, weights.get(w) - 1);

        return new int[]{l, w};
    }
}
