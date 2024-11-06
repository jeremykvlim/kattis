import java.io.*;
import java.util.*;
import java.util.stream.*;

public class integerlists {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int t = Integer.parseInt(br.readLine());
        next:
        while (t-- > 0) {
            var p = br.readLine();
            int n = Integer.parseInt(br.readLine());

            if (n == 0) {
                var empty = br.readLine();
                pw.println(p.contains("D") ? "error" : empty);
                continue;
            }

            var dq = Arrays.stream(br.readLine().replaceAll("\\[","").replaceAll("]","").split(",")).map(Integer::parseInt).collect(Collectors.toCollection(ArrayDeque::new));

            var reversed = false;
            for (var c : p.toCharArray())
                if (c == 'R') reversed = !reversed;
                else {
                    if (!dq.isEmpty()) {
                        if (!reversed) dq.pop();
                        else dq.pollLast();
                    }
                    else {
                        pw.println("error");
                        continue next;
                    }
                }

            pw.print("[");
            while (!dq.isEmpty()) {
                pw.print(!reversed ? dq.pop() : dq.pollLast());
                if (!dq.isEmpty()) pw.print(",");
            }
            pw.println("]");
        }
        pw.flush();
    }
}
