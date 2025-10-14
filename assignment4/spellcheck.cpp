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
  // auto tokenStart = source.begin();
  // for (auto space : spaces) {
  //   if (tokenStart < space)  // deal with consecutive spaces
  //       tokens.emplace(source, tokenStart, space);
  //   tokenStart = space + 1;
  // }
  // tokens.emplace(source, tokenStart, source.end());
  /* auto _ = */ std::transform(
    spaces.begin(), spaces.end() - 1, spaces.begin() + 1,
    std::inserter(tokens, tokens.end()),
    [&source](auto space, auto nextSpace) {
      return Token{source, space + 1, nextSpace};
    }
  );
  /* auto _ = */ std::erase_if(tokens, [](const Token& t){ return t.content.empty(); });
  return tokens;
}

std::set<Misspelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  /* TODO: Implement this method */
  return std::set<Misspelling>();
};

/* Helper methods */

#include "utils.cpp"