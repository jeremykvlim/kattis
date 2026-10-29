import java.io.*;
import java.util.*;

public class mountainpeeker {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        int n = Integer.parseInt(br.readLine()), h = Integer.parseInt(br.readLine());
        var coords = new ArrayList<Pair<Integer, Integer>>(n);
        for (int i = 0; i < n; i++) {
            var line = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            coords.add(new Pair<>(line[0], line[1]));
        }
        coords.sort(Comparator.comparingInt(p -> Math.abs(p.first)));

        int count = 0;
        double[] slope = {-1e9, -1e9};
        for (var c : coords) {
            int x = c.first, y = c.second;
            if (x == 0) {
                if (y >= h) {
                    System.out.println(1);
                    System.exit(0);
                }
                count++;
                continue;
            }

            int side = x > 0 ? 1 : 0;
            var s = (double) (y - h) / Math.abs(x);
            if (slope[side] < s) {
                slope[side] = s;
                count++;
            }
        }
        System.out.println(count);
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
