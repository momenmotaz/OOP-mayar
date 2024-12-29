#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class InvalidEmailException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid email format";
    }
};

class InvalidPasswordException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid password format";
    }
};

class UserNotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "User not found";
    }
};

class AuthenticationException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Authentication failed";
    }
};

class PostNotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Post not found";
    }
};

class CommentNotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Comment not found";
    }
};

class InvalidAccessException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Access denied: insufficient permissions";
    }
};

class FileOperationException : public std::exception {
public:
    const char* what() const noexcept override {
        return "File operation failed";
    }
};

class InvalidDataFormatException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid data format in file";
    }
};

#endif
