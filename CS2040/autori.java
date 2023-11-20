import java.io.*;
import java.util.*;

public class autori {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split("-");
        for (var s : input) System.out.print(s.charAt(0));
    }
}
