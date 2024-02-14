import java.util.*;

public class coconut {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        var players = new ArrayList<Player>();
        int s = sc.nextInt(), n = sc.nextInt();

        int i = 1, j = 0;
        while (n-- > 0) players.add(new Player("folded", i++));

        while (players.size() > 1) {
            j = (j + s - 1) % players.size();
            if (players.get(j).hands.equals("folded")) {
                players.get(j).hands = "fist";
                players.add(j, new Player("fist", players.get(j).number));
            } else if (players.get(j).hands.equals("fist"))
                players.get(j++).hands = "palm";
            else players.remove(j);
        }

        System.out.println(players.get(0).number);
    }

    static class Player {
        String hands;
        int number;

        Player(String h, int n) {
            hands = h;
            number = n;
        }
    }
}
