import java.io.*;

public class ladice {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), l = Integer.parseInt(input[1]);
        int[] sets = new int[l + 1], size = new int[l + 1], count = new int[l + 1];
        for (int i = 0; i <= l; i++) {
            sets[i] = i;
            size[i] = 1;
        }

        while (n-- > 0) {
            var pair = br.readLine().split(" ");
            int a = Integer.parseInt(pair[0]), b = Integer.parseInt(pair[1]), a_set = find(a, sets), b_set = find(b, sets);

            if (a_set != b_set) {
                sets[b_set] = a_set;
                size[a_set] += size[b_set];
                count[a_set] += count[b_set];
            }

            if (count[a_set] == size[a_set]) pw.println("SMECE");
            else {
                count[a_set]++;
                pw.println("LADICA");
            }
        }

        pw.flush();
    }

    static int find(int p, int[] sets) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
    }
}
