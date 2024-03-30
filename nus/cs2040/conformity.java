import java.io.*;
import java.util.*;

public class conformity {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        int n = Integer.parseInt(br.readLine()), max = 0;
        var combo = new HashMap<Combination, Integer>();
        while (n-- > 0) {
            var courses = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            Arrays.sort(courses);
            var combo = new Combination(courses);
            combo.put(combo, combo.getOrDefault(combo, 0) + 1);
            max = Math.max(max, combo.get(combo));
        }

        int total = 0;
        for (var set : combo.entrySet())
            if (set.getValue() == max) total += set.getValue();

        System.out.println(total);
    }

    static class Combination {
        int[] courses;

        Combination(int[] c) {
            courses = c;
        }

        @Override
        public int hashCode() {
            return Arrays.hashCode(courses);
        }

        @Override
        public boolean equals(Object o) {
            if (o == null) return false;
            if (o == this) return true;
            if (o instanceof Combination) {
                Combination object = (Combination) o;
                return Arrays.equals(courses, object.courses);
            }
            return false;
        }
    }
}
