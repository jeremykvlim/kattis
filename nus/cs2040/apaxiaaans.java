import java.io.*;

public class apaxiaaans {
    public static void main(String[] args) throws Exception {
        var dis = new DataInputStream(System.in);
        
        var sb = new StringBuilder();
        byte b = ' ', prev;
        do {
            prev = b;
            b = dis.readByte();
        } while (b <= ' ');
        while (b > ' ') {
            if (b != prev) sb.appendCodePoint(b);
            prev = b;
            b = dis.readByte();
        }
        System.out.println(sb);
    }
}
