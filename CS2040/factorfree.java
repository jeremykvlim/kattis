import java.util.*;
import java.io.*;

public class factorfree {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var elements = new int[n + 1];
        var st = new StringTokenizer(br.readLine());
        int max = 0;
        for (int i = 1; i <= n; i++) {
            elements[i] = Integer.parseInt(st.nextToken());
            max = Math.max(max, elements[i]);
        }
        var factor = new int[max + 1];
        var primes = new ArrayList<Integer>();
        primes.add(0);
        sieve(factor, primes, max);

        int biggest = -1;
        for (int i : factor)
            biggest = Math.max(biggest, i);

        int[] pos = new int[biggest + 1], prev = new int[n + 1], next = new int[n + 1], parent = new int[n + 1];

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
        if (dfs(1, n, 0, prev, next, parent)) {
            for (int i = 1; i <= n; i++) pw.print(parent[i] + " ");
            pw.println();
        } else pw.println("impossible");
        pw.flush();
    }

    static void sieve(int[] factor, ArrayList<Integer> primes, int max_element) {
        var prime = new BitSet(max_element + 1);
        prime.set(2, max_element + 1);
        for (int i = 2; i <= max_element; i++) {
            if (prime.get(i)) {
                primes.add(i);
                factor[i] = primes.size() - 1;
            }
            for (int j = 1, k; j < primes.size() && (k = i * primes.get(j)) <= max_element; j++) {
                factor[k] = factor[i];
                prime.clear(k);
                if (i % primes.get(j) == 0) break;
            }
        }
    }

    static boolean dfs(int l, int r, int p, int[] prev, int[] next, int[] parent) {
        var s = new Stack<Integer>();
        s.push(l);
        s.push(r);
        s.push(p);

        while (!s.isEmpty()) {
            p = s.pop();
            r = s.pop();
            l = s.pop();

            if (l > r) continue;
            
            var found = false;
            for (int left = l, right = r; left <= right; left++, right--) 
                if ((l >= prev[left] && r <= next[left]) || (l >= prev[right] && r <= next[right])) {
                    int mid = (l >= prev[left] && r <= next[left]) ? left : right;
                    parent[mid] = p;

                    s.push(mid + 1);
                    s.push(r);
                    s.push(mid);

                    s.push(l);
                    s.push(mid - 1);
                    s.push(mid);

                    found = true;
                    break;
                }
            
            if (!found) return false;
        }

        return true;
    }
}
