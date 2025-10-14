#include "user.h"

/**
 * Creates a new User with the given name and no friends.
 */
User::User(const std::string& name)
  : _name(name)
  , _friends(nullptr)
  , _size(0)
  , _capacity(0)
{
}

/**
 * Adds a friend to this User's list of friends.
 * @param name The name of the friend to add.
 */
void
User::add_friend(const std::string& name)
{
  if (_size == _capacity) {
    _capacity = 2 * _capacity + 1;
    std::string* newFriends = new std::string[_capacity];
    for (size_t i = 0; i < _size; ++i) {
      newFriends[i] = _friends[i];
    }
    delete[] _friends;
    _friends = newFriends;
  }

  _friends[_size++] = name;
}

/**
 * Returns the name of this User.
 */
std::string
User::get_name() const
{
  return _name;
}

/**
 * Returns the number of friends this User has.
 */
size_t
User::size() const
{
  return _size;
}

/**
 * Sets the friend at the given index to the given name.
 * @param index The index of the friend to set.
 * @param name The name to set the friend to.
 */
void User::set_friend(size_t index, const std::string& name)
{
  _friends[index] = name;
}

/** 
 * STUDENT TODO:
 * The definitions for your custom operators and special member functions will go here!
 */

User::~User() {
  delete[] _friends;
}

User::User(const User& other)
  : _name(other._name)
  , _friends(other._friends ? new std::string[other._capacity] : nullptr)
  , _size(other._size)
  , _capacity(other._capacity) 
{
  if (_friends) {
    std::copy(other._friends, other._friends + _size, _friends);
  }
}

User& User::operator=(const User& other) {
  if (this != &other) { //* notice how much shorter copy-and-swap is!
    // User(other).swap(*this); //* see <https://en.wikibooks.org/wiki/More_C++_Idioms/Copy-and-swap>
    { // Create new resources BEFORE modifying state
      std::string new_name = other._name; //! Rarely throws
      std::string* new_friends = _friends; // Keep existing allocation if possible
      size_t new_capacity = _capacity;
      
      if (_capacity < other._capacity) { //* then 0 < other._capacity
        new_friends = new std::string[other._capacity]; //! May throw
        new_capacity = other._capacity;
      }
      
      // Copy friends data if needed
      if (other._friends && new_friends) {
        std::copy(other._friends, other._friends + other._size, 
          new_friends); //! May throw
      } else if (!other._friends) {
        // Handle case where other has no friends - we should also have none
        new_friends = nullptr;
        new_capacity = 0;
      }
      
      // Only modify state after all allocations succeed
      _name = std::move(new_name);
      if (new_friends != _friends) {
        delete[] _friends;
        _friends = new_friends;
        _capacity = new_capacity;
      }
      _size = other._size;
    }
  }
  return *this;
}

void User::swap(User& other) noexcept {
  using std::swap;
  swap(_name, other._name);
  swap(_friends, other._friends);
  swap(_size, other._size);
  swap(_capacity, other._capacity);
}
inline void swap(User& first, User& second) noexcept {
  first.swap(second);
}

/** Overloaded output stream operator for User
 * @example std::cout << user; // User(name=Alice, friends=[Bob, Charlie])
 */
std::ostream& operator<<(std::ostream& os, const User& user) {
  os << "User(name=" << user._name << ", friends=[";
  for (size_t i = 0; i < user._size; ++i) {
    os << user._friends[i];
    if (i < user._size - 1) {
      os << ", ";
    }
  }
  os << "])";
  return os;
}
