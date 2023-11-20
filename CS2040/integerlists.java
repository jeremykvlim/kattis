import java.io.*;
import java.util.*;

public class integerlists {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int tests = Integer.parseInt(br.readLine());
        while (tests-- > 0) {
            var program = br.readLine();
            int elements = Integer.parseInt(br.readLine());
            
            if (elements == 0) {
                var empty = br.readLine();
                var error = program.contains("D");
                if (!error) pw.println(empty);
                else pw.println("error");
                continue;
            }
            
            var list = br.readLine().replaceAll("\\[","").replaceAll("]","").split(",");
            var deque = new ArrayDeque<Integer>();
            
            for (var s : list) {
                int element = Integer.parseInt(s);
                deque.add(element);
            }
            
            boolean reversed = false, empty = false;
            
            for (int i = 0; i < program.length(); i++) {
                if (program.charAt(i) == 'R') reversed = !reversed;
                else {
                    if (!deque.isEmpty()) {
                        if (!reversed) deque.removeFirst();
                        else deque.removeLast();
                    }
                    else {
                        pw.println("error");
                        empty = true;
                        break;
                    }
                }
            }
            
            if (empty) continue;
            int size = deque.size();
            pw.print("[");
            
            if (!reversed)
                for (int i = 0; i < size; i++) {
                    if (deque.size() != 1) pw.print(deque.removeFirst()+",");
                    else pw.print(deque.removeFirst());
                }
            else
                for (int i = 0; i < size; i++) {
                    if (deque.size() != 1) pw.print(deque.removeLast() + ",");
                    else pw.print(deque.removeLast());
                }
                
            pw.println("]");
        }
        pw.flush();
    }
}
