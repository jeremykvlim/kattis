import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

public class integerlists {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int t = Integer.parseInt(br.readLine());
        next:
        while (t-- > 0) {
            var program = br.readLine();
            int elements = Integer.parseInt(br.readLine());

            if (elements == 0) {
                var empty = br.readLine();
                pw.println(program.contains("D") ? "error" : empty);
                continue;
            }

            var dq = Arrays.stream(br.readLine().replaceAll("\\[","").replaceAll("]","").split(","))
                     .map(Integer::parseInt).collect(Collectors.toCollection(ArrayDeque::new));

            var reversed = false;
            for (int i = 0; i < program.length(); i++)
                if (program.charAt(i) == 'R') reversed = !reversed;
                else {
                    if (!dq.isEmpty()) {
                        if (!reversed) dq.removeFirst();
                        else dq.removeLast();
                    }
                    else {
                        pw.println("error");
                        continue next;
                    }
                }

            pw.print("[");
            while (!dq.isEmpty())
                if (dq.size() > 1) pw.print((!reversed ? dq.removeFirst() : dq.removeLast()) + ",");
                else pw.print(!reversed ? dq.removeFirst() : dq.removeLast());
            pw.println("]");
        }
        
        pw.flush();
    }
}
