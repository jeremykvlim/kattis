import java.io.*;
import java.util.*;

public class teque {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());

        var l = new Deque<String>(n);
        var r = new Deque<String>(n);
        while (n-- > 0) {
            var command = br.readLine().split(" ");
            switch (command[0]) {
                case "push_front" -> l.pushFront(command[1]);
                case "push_back" -> r.pushBack(command[1]);
                case "push_middle" -> {
                    int m = (l.size() + r.size() + 1) / 2;
                    while (m < l.size()) r.pushFront(l.popBack());
                    while (m > l.size()) l.pushBack(r.popFront());
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

        Deque(int n) {
            head = tail = 0;
            int size = 8;
            if (n >= size) {
                size = n;
                size |= (size >>> 1);
                size |= (size >>> 2);
                size |= (size >>> 4);
                size |= (size >>> 8);
                size |= (size >>> 16);
                size++;

                if (size < 0) size >>>= 1;
            }
            dq = (T[]) new Object[size];
        }

        private void resize() {
            int p = head, n = dq.length, r = n - p, size = n << 1;
            if (size < 0) throw new IllegalStateException();
            Object[] a = new Object[size];
            System.arraycopy(dq, p, a, 0, r);
            System.arraycopy(dq, 0, a, r, p);
            dq = (T[]) a;
            head = 0;
            tail = n;
        }

        void pushFront(T e) {
            if (e == null) throw new NullPointerException();
            dq[head = (head - 1) & (dq.length - 1)] = e;
            if (head == tail) resize();
        }

        void pushBack(T e) {
            if (e == null) throw new NullPointerException();
            dq[tail] = e;
            if ((tail = (tail + 1) & (dq.length - 1)) == head) resize();
        }

        T popFront() {
            int h = head;
            T x = dq[h];
            if (x == null) throw new NoSuchElementException();
            dq[h] = null;
            head = (h + 1) & (dq.length - 1);
            return x;
        }

        T popBack() {
            int t = (tail - 1) & (dq.length - 1);
            T x = dq[t];
            if (x == null) throw new NoSuchElementException();
            dq[t] = null;
            tail = t;
            return x;
        }

        T get(int i) {
            T x = dq[(head + i) & (dq.length - 1)];
            if (x == null) throw new NoSuchElementException();
            return x;
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
            int h = head, t = tail;
            if (h != t) {
                head = tail = 0;
                int i = h, mask = dq.length - 1;
                do {
                    dq[i] = null;
                    i = (i + 1) & mask;
                } while (i != t);
            }
        }
    }
}
