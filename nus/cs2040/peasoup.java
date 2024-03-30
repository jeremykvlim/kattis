import java.io.*;
import java.util.*;

public class peasoup {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        int n = Integer.parseInt(br.readLine());
        while (n-- > 0) {
            int k = Integer.parseInt(br.readLine());
            boolean pancakes = false, peasoup = false;
            var name = br.readLine();
            while (k-- > 0) {
                var item = br.readLine();
                if (item.equals("pea soup")) peasoup = true;
                else if (item.equals("pancakes")) pancakes = true;

                if (pancakes && peasoup) {
                    System.out.println(name);
                    return;
                }
            }
        }
        
        System.out.println("Anywhere is fine I guess");
    }
}
