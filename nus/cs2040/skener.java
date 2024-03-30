import java.io.*;
import java.util.*;

public class skener {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();

        for (int i = 0; i < input[0]; i++) {
            var sb = new StringBuilder();
            for (char ch: br.readLine().toCharArray())
                sb.append(String.valueOf(ch).repeat(Math.max(0, input[3])));
            for (int j = 0; j < input[2]; j++) System.out.println(sb);
        }
    }
}
