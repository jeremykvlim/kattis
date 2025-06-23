import java.io.*;
import java.util.*;

public class longwait {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int q = input[0], k = input[1];

        var l = new ArrayDeque<String>();
        var r = new ArrayDeque<String>();
        while (q-- > 0) {
            var op = br.readLine().split(" ");
            switch (op[0]) {
                case "vip" -> l.addFirst(op[1]);
                case "queue" -> r.addLast(op[1]);
                case "member" -> {
                    while (k < l.size() && !l.isEmpty()) r.addFirst(l.removeLast());
                    while (k > l.size() && !r.isEmpty()) l.addLast(r.removeFirst());
                    l.addLast(op[1]);
                }
                case "slower" -> k++;
                case "faster" -> k--;
                default -> {
                    int pos = Integer.parseInt(op[1]) - 1;
                    pw.println(pos < l.size() ? l.get(pos) : r.get(pos - l.size()));
                }
            }
        }
        pw.flush();
    }

    static class ArrayDeque<T> {
        private T[] dq;
        private int head, tail;

        ArrayDeque() {
            head = tail = 0;
            dq = (T[]) new Object[8];
        }

        ArrayDeque(int n) {
            head = tail = 0;
            dq = (T[]) new Object[Math.max(8, Integer.highestOneBit(n - 1) << 1)];
        }

        private void enlarge() {
            int n = dq.length;
            var a = new Object[n << 1];
            System.arraycopy(dq, head, a, 0, n - head);
            System.arraycopy(dq, 0, a, n - head, head);
            dq = (T[]) a;
            head = 0;
            tail = n;
        }

        void addFirst(T v) {
            dq[head = (head - 1) & (dq.length - 1)] = v;
            if (head == tail) enlarge();
        }

        void addLast(T v) {
            dq[tail] = v;
            if ((tail = (tail + 1) & (dq.length - 1)) == head) enlarge();
        }

        T removeFirst() {
            T v = dq[head];
            dq[head] = null;
            head = (head + 1) & (dq.length - 1);
            return v;
        }

        T removeLast() {
            T v = dq[(tail - 1) & (dq.length - 1)];
            dq[tail = (tail - 1) & (dq.length - 1)] = null;
            return v;
        }

        T get(int i) {
            return dq[(head + i) & (dq.length - 1)];
        }

        void set(int i, T v) {
            dq[(head + i) & (dq.length - 1)] = v;
        }

        T first() {
            return dq[head];
        }

        T last() {
            return dq[(tail - 1) & (dq.length - 1)];
        }

        int size() {
            return (tail - head) & (dq.length - 1);
        }

        boolean isEmpty() {
            return head == tail;
        }

        void clear() {
            if (head == tail) return;
            int h = head, t = tail, mask = dq.length - 1;
            head = tail = 0;
            do {
                dq[h] = null;
                h = (h + 1) & mask;
            } while (h != t);
        }
    }
}
