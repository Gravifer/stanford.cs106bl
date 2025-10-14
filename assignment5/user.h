/*
 * CS106L Assignment 5: TreeBook
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

#include <iostream>
#include <string>

class User
{
  /**
   * Invariants:
   * - `_name` is a valid `std::string`
   * - `_friends` is either `nullptr` or to a valid array of `std::strings`
   * - `_size` is the number of friends currently stored in `_friends`
   * - `_capacity` is the size of the allocated array backing `_friends`
   * - `_size <= _capacity`
   * - If `_friends` is `nullptr`, then both `_size` and `_capacity` are 0
   * - Whenever `_capacity` is 0, `_friends` is `nullptr`
   */
public:
  User(const std::string& name);
  void add_friend(const std::string& name);
  std::string get_name() const;
  size_t size() const;
  void set_friend(size_t index, const std::string& name);

  /** 
   * STUDENT TODO:
   * Your custom operators and special member functions will go here!
   */
  ~User(); // destructor
  User(const User& other); // copy constructor
  User& operator=(const User& other); // copy assignment
  friend std::ostream& operator<<(std::ostream& os, const User& user);
  void swap(User& other) noexcept; // swap function
  friend inline void swap(User& first, User& second) noexcept;

private:
  std::string _name;
  std::string* _friends;
  size_t _size;
  size_t _capacity;
};