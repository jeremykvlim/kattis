import java.io.*;
import java.util.*;

public class whatdoesthefoxsay {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int t = Integer.parseInt(br.readLine());
        
        var words = new HashSet<String>();
        for (int i = 0; i < t; i++) {
            var r = br.readLine().split(" ");
            var line = br.readLine().split(" ");
            while (!line[0].equals("what") ) {
                words.add(line[2]);
                line = br.readLine().split(" ");
            }

            var sounds = new StringBuilder();
            for (var w : r)
                if (!words.contains(w)) sounds.append(w).append(" ");
            System.out.println(sounds);
        }
    }
}
