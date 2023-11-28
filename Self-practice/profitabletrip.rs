use std::{
    io::{stdin, BufRead},
    collections::VecDeque,
};

fn main() {
    let mut lines = stdin().lock().lines();
    let input: Vec<i32> = lines.next().unwrap().unwrap().split_whitespace().map(|num| num.parse().unwrap()).collect();
    let (n, m, w) = (input[0], input[1], input[2]);

    let mut adj_list: Vec<Vec<(usize, i32)>> = vec![vec![]; n as usize + 1];
    let mut profit: Vec<i32> = vec![i32::MIN; n as usize + 1];
    profit[1] = 0;

    for _ in 0..m {
        let road: Vec<i32> = lines.next().unwrap().unwrap().split_whitespace().map(|num| num.parse().unwrap()).collect();
        adj_list[road[0] as usize].push((road[1] as usize, road[2]));
    }

    let mut dfs: VecDeque<usize> = VecDeque::new();
    dfs.push_back(1);

    while let Some(v) = dfs.pop_front() {
        for &u in &adj_list[v] {
            if profit[u.0] < w.min(u.1 + profit[v]) {
                profit[u.0] = w.min(u.1 + profit[v]);
                dfs.push_back(u.0);
            }
        }
    }

    println!("{}", profit[n as usize]);
}
