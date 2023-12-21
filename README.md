# Kattis-Solutions

My solutions for Kattis problems. Solutions are named `"problemname".<languagefilename>`. To find the problem statement, please go to:

`https://nus.kattis.com/problems/"problemname"` or `https://open.kattis.com/problems/"problemname"`

---

Problems are grouped by [NUS course](https://nus.kattis.com/courses) categorizations and a separate self-practice category for non-NUS course problems:

* Data Structures & Algorithms: CS2040 (`Java`), CS2040C (`C++`), CS2040S (`Java`), IT5003 (`Python`)

* Competitive Programming: CS3233 (any language of choice)

* Optimization Algorithms: CS4234 (`C++`/`Java`/`Python`)

* Self-practice: `C++`/`Java`/`Python`

---

Kattis compiles using `g++ 11` for [C++](https://open.kattis.com/languages/cpp), `Java 11` for [Java](https://open.kattis.com/languages/java) and `Python 3.8` for [Python](https://open.kattis.com/languages/python3).

Problems have to be solved using base programming language libraries or self-coded structures. External libraries, such as `Boost` for C++, `Guava` for Java, `NumPy` for Python 3 and others are not available. However, you are allowed to attach multiple files in one submission, although I never bothered.

I generally use `C++` unless the NUS course requires otherwise. I switch to `Python` if I require precision or big integer operations or if the solution is much shorter and simpler implementation-wise.

---

My code isn't particularly optimized. I do try to come up with efficient solutions to the problems that don't just scrape by the time or memory limit, but I've been prioritising coding style as I want my code to be pleasant to read (with some _fun_ exceptions; if the solution is very simple, I try to get the shortest solution with Python). 

As such, I will try to find a compromise between both efficiency and preference. I hope you can tolerate some habits of mine:

* using `std::vector` instead of arrays

* using `unordered_map` or `gp_hash_table` instead of `map` for non-ordered purposes

* using postfix `i++` instead of `++i` for iterator incrementing

* using `constexpr` instead of `const` for compile-time initialization

* avoiding global variables at all costs

* avoiding any macros or typedefs

Occasionally when I learn new techniques, I do revisit some old solutions of mine to see if I can improve them, so I do update my old code (provided I do remember that problem). If you have any questions about a particular solution, please contact me at j_lim@u.nus.edu.
