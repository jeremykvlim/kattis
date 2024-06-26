import java.io.*;

public class ballotboxes {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        for (var line = br.readLine(); !line.equals("-1 -1"); line = br.readLine()) {
            var input = line.split(" ");
            int n = Integer.parseInt(input[0]), b = Integer.parseInt(input[1]);

            var cities = new int[n];
            int l = 1, r = -1, m;
            for (int i = 0; i < n; i++) {
                cities[i] = Integer.parseInt(br.readLine());
                r = Math.max(cities[i], r);
            }

            while (l + 1 < r) {
                m = l + (r - l) / 2;
                int boxes = 0;
                for (int c : cities) boxes += (c - 1) / m + 1;
                
                if (boxes <= b) r = m;
                else l = m;
            }
            
            pw.println(r);
            br.readLine();
        }
        pw.flush();
    }
}
