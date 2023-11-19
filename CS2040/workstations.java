import java.util.*;
import java.io.*;

public class workstations {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]), ans = 0;
        int[] arrive = new int[n], leave = new int[n];
        for (int i = 0; i < n; i++) {
            var times = br.readLine().split(" ");
            arrive[i] = Integer.parseInt(times[0]);
            leave[i] = Integer.parseInt(times[1]) + arrive[i];
        }
        Arrays.sort(arrive);
        Arrays.sort(leave);
        for (int i = 0, j = 0; i < n && j < n; i++) {
            while (arrive[i] > leave[j] + m) j++;
            if (arrive[i] >= leave[j] && arrive[i] <= leave[j] + m) {
                ans++;
                j++;
            }
        }
        System.out.println(ans);
    }
}
