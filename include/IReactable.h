#ifndef IREACTABLE_H
#define IREACTABLE_H

class User;  // Forward declaration

class IReactable {
public:
    virtual void addLike(User* user) = 0;
    virtual void removeLike(User* user) = 0;
    virtual ~IReactable() = default;
};

#endif
