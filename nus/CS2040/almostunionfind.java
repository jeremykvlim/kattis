import java.io.*;
import java.util.*;

public class almostunionfind {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        for (var line = br.readLine(); line != null; line = br.readLine()) {
            var input = line.split(" ");
            int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

            var nodes = new HashSet[n + 1];
            var sets = new int[n + 1];
            var sum = new long[n + 1];
            for (int i = 1; i <= n; i++) {
                nodes[i] = new HashSet<Integer>();
                nodes[i].add(i);
                sets[i] = i;
                sum[i] = i;
            }

            while (m-- > 0) {
                var command = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
                int o = command[0], p = command[1];
                if (o == 1) {
                    int q = command[2];
                    if (sets[p] != sets[q]) {
                        if (nodes[sets[p]].size() < nodes[sets[q]].size()) {
                            var temp = p;
                            p = q;
                            q = temp;
                        }

                        sum[sets[p]] += sum[sets[q]];

                        for (var t : nodes[sets[q]]) {
                            sets[(int) t] = sets[p];
                            nodes[sets[p]].add(t);
                        }
                    }
                } else if (o == 2) {
                    int q = command[2];
                    if (sets[p] != sets[q]) {
                        nodes[sets[p]].remove(p);
                        nodes[sets[q]].add(p);
                        sum[sets[p]] -= p;
                        sum[sets[q]] += p;
                        sets[p] = sets[q];
                    }
                } else pw.println(nodes[sets[p]].size() + " " + sum[sets[p]]);
            }
        }

        pw.flush();
    }
}
