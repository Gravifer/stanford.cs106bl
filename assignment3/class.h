//CRTP

#include <type_traits>
#include <utility>

template <typename T>
class Counter {
protected:
    // objects should never be removed through pointers of this type
    ~Counter() {
        --objects_alive;
    }
public:
    static inline int objects_created = 0;
    static inline int objects_alive = 0;

    Counter() {
        ++objects_created;
        ++objects_alive;
    }
    
    Counter(const Counter&) {
        ++objects_created;
        ++objects_alive;
    }
};

class X : public Counter<X> {
    // ...
};

class Y : public Counter<Y> {
    // ...
};

#include "class.cpp"