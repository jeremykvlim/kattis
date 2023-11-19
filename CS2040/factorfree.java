import java.util.*;
import java.io.*;

public class factorfree {
    static int[] elements, factor, pos, prev, next, parent;
    static ArrayList<Integer> primes;

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int n = Integer.parseInt(br.readLine());
        elements = new int[n + 1];
        var st = new StringTokenizer(br.readLine());
        int max = 0;
        for (int i = 1; i <= n; i++) {
            elements[i] = Integer.parseInt(st.nextToken());
            max = Math.max(max, elements[i]);
        }
        factor = new int[max + 1];
        primes = new ArrayList<>();
        primes.add(0);
        sieve(max);

        int biggest = -1;
        for (int i : factor)
            biggest = Math.max(biggest, i);

        pos = new int[biggest + 1];
        prev = new int[n + 1];
        next = new int[n + 1];
        parent = new int[n + 1];

        Arrays.fill(pos, 1, primes.size(), -1);
        for (int i = 1; i <= n; i++) {
            prev[i] = 1;
            for (int j = elements[i]; j > 1; j /= primes.get(factor[j]))
                prev[i] = Math.max(prev[i], pos[factor[j]] + 1);
            for (int j = elements[i]; j > 1; j /= primes.get(factor[j]))
                pos[factor[j]] = i;
        }
        Arrays.fill(pos, 1, primes.size(), n + 1);
        for (int i = n; i > 0; i--) {
            next[i] = n;
            for (int j = elements[i]; j > 1; j /= primes.get(factor[j]))
                next[i] = Math.min(next[i], pos[factor[j]] - 1);
            for (int j = elements[i]; j > 1; j /= primes.get(factor[j]))
                pos[factor[j]] = i;
        }
        if (dfs(1, n, 0)) {
            for (int i = 1; i <= n; i++) pw.print(parent[i] + " ");
            pw.println();
        } else pw.println("impossible");
        pw.flush();
    }

    static void sieve(int max) {
        var prime = new BitSet(max + 1);
        prime.set(2, max + 1);
        for (int i = 2; i <= max; i++) {
            if (prime.get(i)) {
                primes.add(i);
                factor[i] = primes.size() - 1;
            }
            for (int j = 1, k; j < primes.size() && (k = i * primes.get(j)) <= max; j++) {
                factor[k] = factor[i];
                prime.clear(k);
                if (i % primes.get(j) == 0) break;
            }
        }
    }

    static boolean dfs(int l, int r, int p) {
        if (l > r) return true;
        for (int left = l, right = r; left <= right; left++, right--) {
            if (l >= prev[left] && r <= next[left]) {
                parent[left] = p;
                return dfs(l, left - 1, left) && dfs(left + 1, r, left);
            }
            if (l >= prev[right] && r <= next[right]) {
                parent[right] = p;
                return dfs(l, right - 1, right) && dfs(right + 1, r, right);
            }
        }
        return false;
    }
}
