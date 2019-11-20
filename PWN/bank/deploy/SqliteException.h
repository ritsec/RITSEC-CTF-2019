#ifndef SQLITE_EXCEPTION_H__
#define SQLITE_EXCEPTION_H__

#include <exception>
#include <string>

class SqliteException : public std::exception {
    std::string m_msg;
    public:
        SqliteException();
        SqliteException(std::string const& msg);
        SqliteException(std::string const& msg, int rc);
        virtual const char* what() const noexcept;
};

#endif
