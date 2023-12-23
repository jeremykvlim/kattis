import java.io.*;
import java.util.*;

public class delimitersoup {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int len = Integer.parseInt(br.readLine());
        var l = br.readLine().toCharArray();
        
        var s = new Stack<Character>();
        for (int i = 0; i < len; i++) {
            var c = l[i];
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
