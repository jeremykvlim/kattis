import java.io.*;
import java.util.*;

public class almostunionfind {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        for (var line = br.readLine(); line != null; line = br.readLine()) {
            var input = line.split(" ");
            int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

            var members = new HashSet[n + 1];
            var id = new int[n + 1];
            var sum = new long[n + 1];
            for (int i = 1; i <= n; i++) {
                members[i] = new HashSet<Integer>();
                members[i].add(i);
                id[i] = i;
                sum[i] = i;
            }

            while (m-- > 0) {
                var command = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
                int o = command[0], p = command[1];
                if (o == 1) {
                    int q = command[2];
                    if (id[p] != id[q]) {
                        if (members[id[p]].size() < members[id[q]].size()) {
                            var temp = p;
                            p = q;
                            q = temp;
                        }

                        sum[id[p]] += sum[id[q]];

                        for (var t : members[id[q]]) {
                            id[(int) t] = id[p];
                            members[id[p]].add(t);
                        }
                    }
                } else if (o == 2) {
                    int q = command[2];
                    if (id[p] != id[q]) {
                        members[id[p]].remove(p);
                        members[id[q]].add(p);
                        sum[id[p]] -= p;
                        sum[id[q]] += p;
                        id[p] = id[q];
                    }
                } else pw.println(members[id[p]].size() + " " + sum[id[p]]);
            }
        }
        
        pw.flush();
    }
}
