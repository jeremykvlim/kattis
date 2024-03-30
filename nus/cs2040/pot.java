import java.io.*;

public class pot {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        int n = Integer.parseInt(br.readLine()), x = 0;
        while (n-- > 0) {
            int p = Integer.parseInt(br.readLine()), num = p / 10, pow = p % 10;
            x += Math.pow(num, pow);
        }
        System.out.println(x);
    }
}
