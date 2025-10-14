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
  if (dictionary.empty() || source.empty()) {
    return {};
  }
  namespace rv = std::ranges::views;
  auto view = source
    | rv::filter([&dictionary](const Token& t) { return
        !dictionary.contains(t.content);
      })
    | rv::transform([&dictionary](const Token& t) {
        auto view = dictionary
          | rv::filter([&t](const std::string& word) { return
              levenshtein(t.content, word) <= 1;
            });
        std::set<std::string> suggestions(view.begin(), view.end());
        return Misspelling {t, suggestions};
      })
    | rv::filter([](const Misspelling& m) { return
        !m.suggestions.empty();
      });
    // | std::ranges::to<std::set<Misspelling>>(); // C++23
  std::set<Misspelling> misspellings(view.begin(), view.end());
  return misspellings;
};

/* Helper methods */

#include "utils.cpp"