import java.io.*;
import java.util.*;

public class cardtrading {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var inputs = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int t = inputs[1], k = inputs[2];

        int[] cardCount = new int[t], cards = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray(), maintenance = new int[t];
        for (int i : cards) cardCount[i - 1]++;

        var cardPrices = new Card[t];

        for (int i = 0; i < t; i++) {
            var input = br.readLine().split(" ");
            cardPrices[i] = new Card(Integer.parseInt(input[0]), Integer.parseInt(input[1]));
        }

        long profit = 0;
        for (int i = 0; i < cardCount.length; i++) profit += cardCount[i] * cardPrices[i].sell;

        for (int i = 0; i < t; i++) {
            if (cardCount[i] == 0) maintenance[i] = cardPrices[i].buy * 2;
            else if (cardCount[i] == 1) maintenance[i] = cardPrices[i].buy + cardPrices[i].sell;
            else maintenance[i] = cardPrices[i].sell * 2;
        }

        Arrays.sort(maintenance);
        for (int i = 0; i < k; i++) profit -= maintenance[i];

        System.out.println(profit);
    }

    static class Card {
        int buy, sell;

        Card(int b, int s) {
            buy = b;
            sell = s;
        }
    }
}
