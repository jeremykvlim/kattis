# kattis

My solutions for Kattis problems, not all of them have been archived here yet because I haven't bothered scraping through all my submissions from 2022 till now. Solutions are named `"problemname".<languagefilename>`. To find the problem statement, please go to:

`https://nus.kattis.com/problems/"problemname"` or `https://open.kattis.com/problems/"problemname"`

---

Problems are grouped by [NUS course](https://nus.kattis.com/courses) categorizations and a separate open category for non-NUS course problems. Some courses share several problems, you might be able to find them under any one of the categories. I generally use C++ unless the NUS course requires otherwise (language requirements found below). I switch to Python if I require big integer operations or if the solution is much shorter and simpler implementation-wise.

* `NUS`:

  * Data Structures & Algorithms: `CS2040` (Java), `CS2040C` (C++), `CS2040S` (Java), `IT5003` (Python)

  * Design and Analysis of Algorithms: `CS3230` (C++/Java/Python)

  * Competitive Programming: `CS3233` (C++/Java/Python)

  * Optimization Algorithms: `CS4234` (C++/Java/Python)

---

Kattis compiles using `C++ 17` for [C++](https://open.kattis.com/languages/cpp), `Java 17` for [Java](https://open.kattis.com/languages/java) and `Python 3.10` for [Python](https://open.kattis.com/languages/python3).

Problems have to be solved using base programming language libraries or self-coded structures. External libraries, such as `Boost` for C++, `Guava` for Java, `NumPy` for Python 3 and others are not available. However, you are allowed to attach multiple files in one submission, although I never bothered.

---

My code isn't particularly optimized. I do try to come up with efficient solutions to the problems that don't just scrape by the time or memory limit (I also do not like to overallocate memory just for speed), but I've been prioritising coding style as I want my code to be pleasant to read (with some exceptions because I do get lazy sometimes).

As such, I will try to find a compromise between both efficiency and preference. I hope you can tolerate some habits of mine:

* using `std::vector` instead of arrays

* using `std::unordered_map` or `__gnu_pbds::gp_hash_table` instead of `std::map` if ordering is unnecessary

* using postfix `i++` instead of prefix `++i` for iterator incrementing

* using `constexpr` instead of `const` for compile-time initialization

* avoiding global variables as much as possible

* avoiding any macros (even pre-defined constraints) or typedefs

Occasionally when I learn new techniques, I do revisit some old solutions of mine to see if I can improve them, so I do update my old code (provided I do remember that problem). If you have any questions about a particular solution, please contact me at j_lim@u.nus.edu.
