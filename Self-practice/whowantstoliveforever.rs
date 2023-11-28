use std::io::{stdin, stdout, BufRead, BufWriter, Write};

fn main() {
    let mut writer = BufWriter::new(stdout().lock());
    let mut lines = stdin().lock().lines();

    let t: i32 = lines.next().unwrap().unwrap().parse().unwrap();
    for _ in 0..t {
        let s: String = lines.next().unwrap().unwrap();
        let mut s: Vec<char> = s.chars().collect();
        s.push('0');
        let n = ((s.len() ^ (s.len() - 1)) + 1) >> 1;
        let mut lives = false;
        for i in 1..s.len() / n {
            for j in 0..n - 1 {
                if s[i * n + j] != s[i * n - j - 2] {
                    lives = true;
                    break;
                }
                if lives { break; }
            }
            if s[i * n - 1] != '0' {
                lives = true;
                break;
            }
            if lives { break; }
        }
        writeln!(writer, "{}", if lives { "LIVES" } else { "DIES" });
    }
}
