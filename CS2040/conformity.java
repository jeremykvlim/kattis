import java.io.*;
import java.util.*;

public class conformity {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var map = new HashMap<Combination, Integer>();
        int n = Integer.parseInt(br.readLine()), max = 0, ans = 0;
        while (n-- > 0) {
            int[] courses = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            Arrays.sort(courses);
            var combo = new Combination(courses);
            map.put(combo, map.getOrDefault(combo, 0) + 1);
            max = Math.max(max, map.get(combo));
        }
        for (var set : map.entrySet())
            if (set.getValue() == max) ans += set.getValue();

        System.out.println(ans);
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
            if (o instanceof Combination) { //Java 11 has no pattern matching for instanceof
                Combination object = (Combination) o;
                return Arrays.equals(courses, object.courses);
            }
            return false;
        }
    }
}
