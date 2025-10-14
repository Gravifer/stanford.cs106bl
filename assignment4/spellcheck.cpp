#include "spellcheck.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

Corpus tokenize(std::string& source) { // * see <https://en.cppreference.com/w/cpp/string/byte/isspace>
  auto spaces = find_all(source.begin(), source.end(), // ::isspace); 
    [](unsigned char c){ return std::isspace(c); }); // static_cast<int(*)(int)>(std::isspace));
  Corpus tokens;
  auto last = source.begin();
  for (auto it : spaces) {
    tokens.emplace(last, it);
    last = it + 1;
  }
  tokens.emplace(last, source.end());
  return tokens;
}

std::set<Misspelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  /* TODO: Implement this method */
  return std::set<Misspelling>();
};

/* Helper methods */

#include "utils.cpp"