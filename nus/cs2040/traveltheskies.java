import java.io.*;
import java.util.*;

public class traveltheskies {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        var input = br.readLine().split(" ");
        int k = Integer.parseInt(input[0]), n = Integer.parseInt(input[1]), m = Integer.parseInt(input[2]);

        int[] u = new int[m], v = new int[m], z = new int[m];
        var days = new ArrayList<Pair<Integer, Integer>>(m);
        for (int i = 0; i < m; i++) {
            var line = br.readLine().split(" ");
            u[i] = Integer.parseInt(line[0]);
            v[i] = Integer.parseInt(line[1]);
            days.add(new Pair<>(Integer.parseInt(line[2]), i));
            z[i] = Integer.parseInt(line[3]);
        }
        Collections.sort(days);

        var adjMatrix = new int[k + 1][n + 1];
        for (int i = 0; i < k * n; i++) {
            var line = br.readLine().split(" ");
            int a = Integer.parseInt(line[0]), b = Integer.parseInt(line[1]), c = Integer.parseInt(line[2]);
            adjMatrix[a][b] += c;
        }

        var capacity = new int[k + 1];
        for (int i = 0, prev = 1; i < m; i++) {
            int d = days.get(i).first, j = days.get(i).second;
            if (prev != d) {
                prev = d;
                for (int l = 1; l <= k; l++) adjMatrix[l][prev] += adjMatrix[l][prev - 1] + capacity[l];
                Arrays.fill(capacity, 0);
            }

            if (adjMatrix[u[j]][d] < z[j]) {
                System.out.println("suboptimal");
                System.exit(0);
            }

            capacity[v[j]] += z[j];
            adjMatrix[u[j]][d] -= z[j];
        }
        System.out.println("optimal");
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
