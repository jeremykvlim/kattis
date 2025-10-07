import java.io.*;
import java.util.*;

public class bracketmatching {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        var n = Integer.parseInt(br.readLine());
        var st = new Stack<Character>();
        for (char c : br.readLine().toCharArray()) {
            if (c == '(') st.push(')');
            else if (c == '{') st.push('}');
            else if (c == '[') st.push(']');
            else if (st.isEmpty() || st.pop() != c) {
                System.out.println("Invalid");
                System.exit(0);
            }
        }
        System.out.println(st.isEmpty() ? "Valid" : "Invalid");
    }
}
