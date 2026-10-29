import java.io.*;
import java.util.*;

public class stockprices {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            int n = Integer.parseInt(br.readLine());

            var ask = new TreeMap<Integer, Integer>();
            var bid = new TreeMap<Integer, Integer>();
            int stock = -1;
            while (n-- > 0) {
                var line = br.readLine().split(" ");
                var type = line[0];
                int x = Integer.parseInt(line[1]), y = Integer.parseInt(line[4]);
                var orders = type.equals("buy") ? bid : ask;
                orders.merge(y, x, Integer::sum);
                while (!ask.isEmpty() && !bid.isEmpty() && ask.firstKey() <= bid.lastKey()) {
                    int a = ask.firstKey(), b = bid.lastKey(), traded = Math.min(ask.get(a), bid.get(b));
                    stock = a;
                    ask.put(a, ask.get(a) - traded);
                    if (ask.get(a) == 0) ask.remove(a);
                    bid.put(b, bid.get(b) - traded);
                    if (bid.get(b) == 0) bid.remove(b);
                }

                pw.println((ask.isEmpty() ? "-" : ask.firstKey()) + " " + (bid.isEmpty() ? "-" : bid.lastKey()) + " " + (stock == -1 ? "-" : stock));
            }
        }
        pw.flush();
    }
}
