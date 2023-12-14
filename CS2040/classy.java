import java.io.*;
import java.util.*;

public class classy {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            int n = Integer.parseInt(br.readLine());
            var people = new Person[n];

            for (int i = 0; n > 0; n--, i++) {
                var middle = new String[11];
                Arrays.fill(middle, "middle");
                var input = br.readLine().split(" ");
                var name = input[0].replace(":", "");
                var classes = input[1].split("-");
                for (int j = classes.length - 1, k = 10; j >= 0; j--, k--) middle[k] = classes[j];
                people[i] = new Person(name, middle);
            }
            
            Arrays.sort(people);
            for (var p : people) System.out.println(p.name);
            for (int i = 0; i < 30; i++) System.out.print("=");
            System.out.println();
        }
    }
    
    static class Person implements Comparable<Person> {
        String name;
        String[] classes;

        Person(String n, String[] c) {
            name = n;
            classes = c;
        }

        @Override
        public int compareTo(Person p) {
            for (int i = 10; i >= 0; i--) {
                if (!p.classes[i].equals(classes[i]))
                    return p.classes[i].charAt(0) - classes[i].charAt(0);
            } return name.compareTo(p.name); 
        }
    }
}
