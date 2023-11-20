import java.io.*;

public class wheresmyinternet {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

        int[] sets = new int[n + 1], rank = new int[n + 1];
        for (int i = 0; i < n; i++) sets[i] = i;

        while (m-- > 0) {
            var line = br.readLine().split(" ");
            int a = Integer.parseInt(line[0]), b = Integer.parseInt(line[1]), aSet = find(a, sets), bSet = find(b, sets);
            if (aSet != bSet) {
                if (rank[aSet] > rank[bSet]) sets[bSet] = aSet;
                else {
                    sets[aSet] = bSet;
                    if (rank[aSet] == rank[bSet]) rank[bSet]++;
                }
            }
        }

        var connected = true;
        for (int i = 2, root = find(1, sets); i < n + 1; i++)
            if (root != find(i, sets)) {
                pw.println(i);
                connected = false;
            }

        if (connected) pw.println("Connected");
        pw.flush();
    }

    static int find(int p, int[] sets) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
    }
}
