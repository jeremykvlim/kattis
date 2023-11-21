import java.io.*;
import java.util.*;

public class almostunionfind {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        for (var line = br.readLine(); line != null; line = br.readLine()) {
            var input = line.split(" ");
            int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);
            
            int[] sets = new int[2*n + 1], size = new int[2*n + 1];
            long[] sum = new long[2*n + 1];
            for (int i = 1, j = n + 1; i <= n; i++, j++) {
                sets[i] = sets[j] = j;
                sum[j] = i;
                size[j] = 1;
            }
            
            while (m-- > 0) {
                var command = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
                if (command[0] == 1) {
                    int p = command[1], q = command[2], pSet = find(p, sets), qSet = find(q, sets);
                    if (pSet != qSet) {
                        size[pSet] += size[qSet];
                        sum[pSet] += sum[qSet];
                        sets[qSet] = pSet;
                    }
                } else if (command[0] == 2) {
                    int p = command[1], q = command[2], pSet = find(p, sets), qSet = find(q, sets);
                    if (pSet != qSet) {
                        size[pSet]--;
                        sum[pSet] -= p;
                        size[qSet]++;
                        sum[qSet] += p;
                        sets[p] = qSet;
                    }
                } else {
                    int p = command[1], pSet = find(p, sets);
                    pw.println(size[pSet] + " " + sum[pSet]);
                }
            }
        }
        pw.flush();
    }

    static int find(int p, int[] sets) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
    }
}
