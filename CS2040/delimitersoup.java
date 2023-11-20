import java.io.*;
import java.util.*;

public class delimitersoup {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int l = Integer.parseInt(br.readLine());
        var arr = br.readLine().toCharArray();
        var s = new Stack<Character>();
        for (int i = 0; i < l; i++) {
            var c = arr[i];
            if (c == '(' || c == '[' || c == '{') s.push(c);
            else if (c != ' ') {
                if (s.isEmpty() || s.peek() == ')' || s.peek() == ']' || s.peek() == '}' ||
                        s.peek() != '(' && c == ')' || s.peek() != '[' && c == ']' || s.peek() != '{' && c == '}' ) {
                    System.out.println(c + " " + i);
                    return;
                }
                else s.pop();
            }
        }
        System.out.println("ok so far");
    }
}
