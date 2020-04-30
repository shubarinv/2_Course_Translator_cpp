//
// Created by vhundef on 27.04.2020.
//
#include <stdexcept>

class NotImplementedException : public std::logic_error
{
private:

    std::string _text;

    NotImplementedException(const char* message, const char* function): std::logic_error("Not Implemented")
    {
        _text = message;
        _text += " : ";
        _text += function;
    };

public:
    NotImplementedException(): NotImplementedException("Not Implemented", __FUNCTION__){
    }
    explicit NotImplementedException(const char* message): NotImplementedException(message, __FUNCTION__){}

    virtual const char *what() const noexcept
    {
        return _text.c_str();
    }
};

class ParsingError: public std::exception {
private:
    std::string message_;
public:
    ParsingError(const std::string& message) : message_(message) {}
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};
