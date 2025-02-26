import java.io.*;

public class teque {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());

        var l = new Deque<String>();
        var r = new Deque<String>();
        while (n-- > 0) {
            var command = br.readLine().split(" ");
            switch (command[0]) {
                case "push_front" -> l.pushFront(command[1]);
                case "push_back" -> r.pushBack(command[1]);
                case "push_middle" -> {
                    int m = (l.size() + r.size() + 1) / 2;
                    while (m < l.size() && !l.isEmpty()) r.pushFront(l.popBack());
                    while (m > l.size() && !r.isEmpty()) l.pushBack(r.popFront());
                    l.pushBack(command[1]);
                }
                default -> {
                    int i = Integer.parseInt(command[1]);
                    pw.println(i < l.size() ? l.get(i) : r.get(i - l.size()));
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
