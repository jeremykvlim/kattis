import java.io.*;

public class ballotboxes {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        for (;;) {
            var line = br.readLine().split(" ");
            int n = Integer.parseInt(line[0]), b = Integer.parseInt(line[1]); 

            if (n == -1 && b == -1) {
                pw.flush();
                break;
            }

            var cities = new int[n];
            int r = 0, l = 1, m;
            for (int i = 0; i < n; i++) {
                cities[i] = Integer.parseInt(br.readLine()) - 1;
                r = Math.max(cities[i] + 1, r);
            }

            while (l < r) {
                m = (r - l) / 2 + l;
                int boxes = 0;
                for (int i = 0; i < n; i++) boxes += (cities[i] + m) / m;
                if (boxes > b) l = m + 1;
                else r = m;
            }
            pw.println(l);
            br.readLine();
        }
    }
}
