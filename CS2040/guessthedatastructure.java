import java.io.*;
import java.util.*;

public class guessthedatastructure {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        for (var line = br.readLine(); line != null; line = br.readLine()) {
            int n = Integer.parseInt(line), count = 0;
            int[] command = new int[n], x = new int[n];

            for (int i = 0; i < n; i++) {
                var input = br.readLine().split(" ");
                command[i] = Integer.parseInt(input[0]);
                x[i] = Integer.parseInt(input[1]);
            }

            var ans = "impossible";

            var s = new Stack<Integer>();
            for (int i = 0; i < command.length; i++) {
                if (command[i] == 1) s.push(x[i]);
                else if (s.isEmpty() || s.pop() != x[i]) break;
                if (i == command.length - 1) {
                    ans = "stack";
                    count++;
                }
            }

            var q = new ArrayDeque<Integer>();
            for (int i = 0; i < command.length; i++) {
                if (command[i] == 1) q.addLast(x[i]);
                else if (q.isEmpty() || q.removeFirst() != x[i]) break;
                if (i == command.length - 1) {
                    ans = "queue";
                    count++;
                }
            }

            var pq = new PriorityQueue<Integer>(Collections.reverseOrder());
            for (int i = 0; i < command.length; i++) {
                if (command[i] == 1) pq.add(x[i]);
                else if (pq.isEmpty() || pq.poll() != x[i]) break;
                if (i == command.length - 1) {
                    ans = "priority queue";
                    count++;
                }
            }

            pw.println(count > 1 ? "not sure" : ans);
        }

        pw.close();
    }
}
