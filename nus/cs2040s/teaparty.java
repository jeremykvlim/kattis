import java.io.*;
import java.util.*;

public class teaparty {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        br.readLine();

        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int a = input[0], b = input[1], m = input[2], q = input[3];

        var intervals = new TreeMap<Integer, Integer>();
        var rabbits = new HashMap<Integer, Integer>();
        while (q-- > 0) {
            input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            int x = input[1];
            if (input[0] == 1) {
                int chair = (int) (((long) a * x + b) % m);

                var e = intervals.floorEntry(chair);
                if (e != null && chair <= e.getValue()) {
                    chair = e.getValue() + 1;
                    if (chair == m) {
                        e = intervals.firstEntry();
                        chair = e.getKey() == 0 ? e.getValue() + 1 : 0;
                    }
                }

                int l = chair, r = chair;
                e = intervals.lowerEntry(chair);
                if (e != null && chair == e.getValue() + 1) {
                    l = e.getKey();
                    intervals.remove(l);
                }
                e = intervals.higherEntry(chair);
                if (e != null && chair == e.getKey() - 1) {
                    r = e.getValue();
                    intervals.remove(e.getKey());
                }
                intervals.put(l, r);
                rabbits.put(x, chair);
                pw.println(chair);
            } else {
                int chair = rabbits.remove(x);
                var e = intervals.floorEntry(chair);
                int l = e.getKey(), r = e.getValue();
                intervals.remove(l);
                if (l < chair) intervals.put(l, chair - 1);
                if (chair < r) intervals.put(chair + 1, r);
            }
        }
        pw.flush();
    }
}