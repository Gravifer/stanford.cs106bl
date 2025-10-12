/*
 * CS106L Assignment 2: Marriage Pact
 * Created by Haven Whitney with modifications by Fabio Ibanez & Jacob Roberts-Baca.
 *
 * Welcome to Assignment 2 of CS106L! Please complete each STUDENT TODO
 * in this file. You do not need to modify any other files.
 *
 */

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>

std::string kYourName = "Jane Doe"; // Don't forget to change this!

/**
 * Takes in a file name and returns a set containing all of the applicant names as a set.
 *
 * @param filename  The name of the file to read.
 *                  Each line of the file will be a single applicant's name.
 * @returns         A set of all applicant names read from the file.
 *
 * @remark Feel free to change the return type of this function (and the function
 * below it) to use a `std::unordered_set` instead. If you do so, make sure
 * to also change the corresponding functions in `utils.h`.
 */
std::set<std::string> get_applicants(std::string filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << '\n';
    std::exit(1);
  }
  std::set<std::string> applicants;
  std::string applicantName;
  while (std::getline(file, applicantName)) {
    if (applicantName.empty()) {
      continue;
    }
    auto [_, success] = applicants.insert(applicantName);
    if (!success) {
      std::cerr << "Warning: Duplicate applicant name found: " << applicantName << '\n';
    }
  }
  return applicants;
}

/**
 * Takes in a set of student names by reference and returns a queue of names
 * that match the given student name.
 *
 * @param name      The returned queue of names should have the same initials as this name.
 * @param students  The set of student names.
 * @return          A queue containing pointers to each matching name.
 */
std::queue<const std::string*> find_matches(std::string name, std::set<std::string>& students) {
  if (name.empty()) {
    std::cerr << "Error: Target name provided to `find_matches` is empty.\n";
    std::exit(1); // return {};
  }
  if (students.empty()) {
    std::cerr << "Warning: Student set provided to `find_matches` is empty.\n";
    return {};
  }
  auto initials_of = [](const std::string& name) {
    std::string initials;
    bool start_of_word = true;
    for (char ch : name) {
      if (std::isalpha(ch)) {
        if (start_of_word) {
          initials += ch;
          start_of_word = false;
        }
      } else if (std::isspace(ch)) {
        start_of_word = true;
      }
    }
    return initials;
  };
  auto target_initials = initials_of(name);
  auto is_match = [&](const std::string& student) {
    if (student.empty()) {
      return false;
    }
    auto student_initials = initials_of(student);
    return student_initials == target_initials;
  };
  std::queue<const std::string*> matches;
  for (const auto& student : students) {
    if (is_match(student)) {
      matches.push(&student);
    }
  }
  return matches;
}

/**
 * Takes in a queue of pointers to possible matches and determines the one true match!
 *
 * You can implement this function however you'd like, but try to do something a bit
 * more complicated than a simple `pop()`.
 *
 * @param matches The queue of possible matches.
 * @return        Your magical one true love.
 *                Will return "NO MATCHES FOUND." if `matches` is empty.
 */
std::string get_match(std::queue<const std::string*>& matches) {
  if (matches.empty()) {
    return "NO MATCHES FOUND.";
  }
  const std::string* best_match = matches.front();
  matches.pop();
  
  auto is_better_match = [](const std::string& candidate, const std::string& current_best) {
    auto count_vowels = [](const std::string& str) {
      int count = 0;
      for (char ch : str) {
        ch = std::tolower(ch);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
          count++;
        }
      }
      return count;
    };
    int candidate_vowels = count_vowels(candidate);
    int current_best_vowels = count_vowels(current_best);
    if (candidate_vowels > current_best_vowels) {
      return true;
    } else if (candidate_vowels == current_best_vowels) {
      return candidate < current_best; // Lexicographically smaller
    }
    return false;
  };

  while (!matches.empty()) {
    const std::string* next = matches.front();
    matches.pop();
    // If the next match is better, update the best match
    if (is_better_match(*next, *best_match)) {
      best_match = next;
    }
  }
  return *best_match;
}

/* #### Please don't remove this line! #### */
#include "autograder/utils.hpp"
