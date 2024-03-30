import java.io.*;
import java.util.*;

public class boatparts {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        var input = br.readLine().split(" ");
        int p = Integer.parseInt(input[0]), n = Integer.parseInt(input[1]), day = 1;
        
        var parts = new HashSet<>();
        for (int i = 0; i < n; i++, day++) {
            parts.add(br.readLine());
            if (parts.size() == p) break;
        }
        
        if (day > n) System.out.println("paradox avoided");
        else System.out.println(day);
    }
}
