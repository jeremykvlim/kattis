import java.io.*;
import java.util.*;

public class longwait {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int q = input[0], k = input[1];

        var l = new Deque<String>(q);
        var r = new Deque<String>(q);
        while (q-- > 0) {
            var op = br.readLine().split(" ");
            switch (op[0]) {
                case "vip" -> l.pushFront(op[1]);
                case "queue" -> r.pushBack(op[1]);
                case "member" -> {
                    while (k < l.size() && !l.isEmpty()) r.pushFront(l.popBack());
                    while (k > l.size() && !r.isEmpty()) l.pushBack(r.popFront());
                    l.pushBack(op[1]);
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

    static class Deque<T> {
        private T[] dq;
        private int head, tail;

        Deque() {
            head = tail = 0;
            dq = (T[]) new Object[8];
        }

        Deque(int n) {
            head = tail = 0;
            if (n >= 8) {
                n |= (n >>> 1);
                n |= (n >>> 2);
                n |= (n >>> 4);
                n |= (n >>> 8);
                n |= (n >>> 16);
                n++;

                if (n < 0) n >>>= 1;
            }
            dq = (T[]) new Object[n];
        }

        private void resize() {
            int n = dq.length;
            var a = new Object[n << 1];
            System.arraycopy(dq, head, a, 0, n - head);
            System.arraycopy(dq, 0, a, n - head, head);
            dq = (T[]) a;
            head = 0;
            tail = n;
        }

        void pushFront(T v) {
            dq[head = (head - 1) & (dq.length - 1)] = v;
            if (head == tail) resize();
        }

        void pushBack(T v) {
            dq[tail] = v;
            if ((tail = (tail + 1) & (dq.length - 1)) == head) resize();
        }

        T popFront() {
            T v = dq[head];
            dq[head] = null;
            head = (head + 1) & (dq.length - 1);
            return v;
        }

        T popBack() {
            T v = dq[(tail - 1) & (dq.length - 1)];
            dq[(tail - 1) & (dq.length - 1)] = null;
            tail = (tail - 1) & (dq.length - 1);
            return v;
        }

        T get(int i) {
            return dq[(head + i) & (dq.length - 1)];
        }

        T front() {
            return dq[head];
        }

        T back() {
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
            int i = head, t = tail, mask = dq.length - 1;
            head = tail = 0;
            do {
                dq[i] = null;
                i = (i + 1) & mask;
            } while (i != t);
        }
    }
}
