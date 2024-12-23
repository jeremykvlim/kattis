import java.io.*;
import java.util.*;

public class bst {
    public static void main(String args[]) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        
        int n = Integer.parseInt(br.readLine());
        int[] seq = new int[n], l = new int[n + 2], r = new int[n + 2], pref = new int[n + 2];
              
        for (int i = 0; i < n; i++) seq[i] = Integer.parseInt(br.readLine());
        for (int i = n - 1; i >= 0; i--) {
            int x = seq[i];
            l[x] = r[x] = x;
            
            if (x > 1 && l[x - 1] != 0) l[x] = l[x - 1];
            if (x + 1 <= n && r[x + 1] != 0) r[x] = r[x + 1];
            
            r[l[x]] = r[x];
            l[r[x]] = l[x];
            
            pref[l[x]]++;
            pref[r[x] + 1]--;
        }
        
        for (int i = 2; i <= n; i++) pref[i] += pref[i - 1];
        
        long total = 0;
        for (int x : seq) {
            total += pref[x] - 1;
            pw.println(total);
        }
        pw.flush();
    }
}
