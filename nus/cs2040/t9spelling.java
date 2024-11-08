import java.util.*;

public class t9spelling {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);

        int n = Integer.parseInt(sc.nextLine());
        String[] keypad = {"+ ", "", " abc", " def", " ghi", " jkl", " mno", " pqrs", " tuv", " wxyz"};
        for (int x = 1; x <= n; x++) {
            var s = new StringBuilder("Case #" + x + ": ");
            for (char c : sc.nextLine().toCharArray())
                for (int i = 0; i < keypad.length; i++)
                    if (keypad[i].indexOf(c) != -1) {
                        if (s.length() > 0 && s.charAt(s.length() - 1) - '0' == i) s.append(' ');
                        s.append(String.valueOf(i).repeat(keypad[i].indexOf(c)));
                        break;
                    }
            System.out.println(s);
        }
    }
}
