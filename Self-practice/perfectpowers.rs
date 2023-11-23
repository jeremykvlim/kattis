use std::io::{self, stdin, stdout, BufWriter, Write};

fn main() {
    let stdin = stdin();
    let stdout = stdout();
    let mut writer = BufWriter::new(stdout.lock());
    for line in stdin.lock().lines() {
        let mut x: i64 = line.unwrap().trim().parse().unwrap();
        if x == 0 { break; }
        let mut p: i64 = 1;
        for b in 2.. {
            if b * b > x.abs() { break; }
            let mut a: i64 = b * b;
            p = 2;
            while a < x.abs() {
                a *= b;
                p += 1;
            }
            if a == x.abs() && (x > 0 || p % 2 == 1) { break; }
            else { p = 1; }
        }
        writeln!(writer, "{}", p);
    }
}
