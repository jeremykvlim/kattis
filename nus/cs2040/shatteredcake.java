import java.io.*;

public class Main {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        int W = Integer.parseInt(br.readLine()), n = Integer.parseInt(br.readLine()), area = 0;
        while (n-- > 0) {
            var line = br.readLine().split(" ");
            area += Integer.parseInt(line[0]) * Integer.parseInt(line[1]);
        }
        System.out.println(area / W);
    }
}
