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
    { //! Exception safety: basic guarantee only (not strong)
      // Attempt to create new resources BEFORE modifying state (where possible)
      bool grow = _capacity < other._capacity;
      std::string temp_name = other._name; //! Rarely throws
      size_t temp_capacity = grow ? other._capacity : _capacity;
      std::unique_ptr<std::string[]> temp_friends; // Keep existing allocation if possible
      if (grow) { //* then 0 < other._capacity
        temp_friends = std::make_unique<std::string[]>(temp_capacity); //! May throw
      } else { // if the `copy()` below throws in this branch, *this is tempered
        temp_friends.reset(_friends); // Take ownership of existing array
        _friends = nullptr;              // Prevent double delete
      }
      // Copy friends data if needed
      if (other._friends && temp_friends) {
        std::copy(other._friends, other._friends + other._size, 
          temp_friends.get()); //! May throw - non-grow case: object left inconsistent
      } else if (!other._friends) {
        temp_friends.reset();
        temp_capacity = 0;
      }
      
      // Commit state changes (some already done above in !grow case)
      _name = std::move(temp_name);      // Commit name change
      if (grow) {
        _capacity = temp_capacity;       // Update capacity for growth case
      }
      _friends = temp_friends.release(); // Restore ownership of array
      _size = other._size;               // Update size
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

/**
 * Custom comparison operator to compare Users by name.
 * @param other The other User to compare to.
 * @return `true` if this User's name is alphabetically less
 *    than the other User's name; `false` otherwise.
 */
bool User::operator<(const User& other) const {
  return _name < other._name;
}

/**
 * Custom compound assignment operator to befriend another User.
 * Adds the other User and this User to each-other's friends list.
 */
User& User::operator+=(User& other) {
  if (this != &other) { // no self-befriending
    this->add_friend(other._name);
    other.add_friend(this->_name);
  }
  return *this;
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
