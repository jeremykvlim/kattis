import java.io.*;
import java.util.*;

public class workstations {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);
        
        int[] arrive = new int[n], leave = new int[n];
        for (int i = 0; i < n; i++) {
            var times = br.readLine().split(" ");
            arrive[i] = Integer.parseInt(times[0]);
            leave[i] = Integer.parseInt(times[1]) + arrive[i];
        }
        
        Arrays.sort(arrive);
        Arrays.sort(leave);
        int unlocks = 0;
        for (int i = 0, j = 0; i < n && j < n; i++) {
            while (arrive[i] > leave[j] + m) j++;
            if (arrive[i] >= leave[j] && arrive[i] <= leave[j] + m) {
                unlocks++;
                j++;
            }
        }
        
        System.out.println(unlocks);
    }
}
