import java.util.*;

public class coconut {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        var playerList = new ArrayList<Player>();
        int s = sc.nextInt(), n = sc.nextInt();

        int i = 1, j = 0;
        while (n-- > 0)
            playerList.add(new Player("folded", i++));

        while (playerList.size() > 1) {
            j = (j + s - 1) % playerList.size();
            if (playerList.get(j).hands.equals("folded")) {
                playerList.get(j).hands = "fist";
                playerList.add(j, new Player("fist", playerList.get(j).number));
            } else if (playerList.get(j).hands.equals("fist"))
                playerList.get(j++).hands = "palm";
            else playerList.remove(j);
        }

        System.out.println(playerList.get(0).number);
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
