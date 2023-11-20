import java.io.*;

public class swaptosort {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), k = Integer.parseInt(input[1]);

        int[] sets = new int[n + 1], size = new int[n + 1];
        for (int i = 0; i < n + 1; i++) {
            sets[i] = i;
            size[i] = 1;
        }

        for (int i = 0; i < k; i++) {
            var pair = br.readLine().split(" ");
            int a = Integer.parseInt(pair[0]), b = Integer.parseInt(pair[1]);
            int aSet = find(a, sets), bSet = find(b, sets);
            if (size[aSet] > size[bSet]) {
                size[aSet] += size[bSet];
                sets[bSet] = aSet;
            } else {
                size[bSet] += size[aSet];
                sets[aSet] = bSet;
            }
        }

        for (int i = 1, j = n; i <= n / 2; i++, j--) {
            if (find(i, sets) != find(j, sets)) {
                System.out.println("No");
                return;
            }
        }
        System.out.println("Yes");
    }

    static int find(int p, int[] sets) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
    }
}
