#include "SqliteException.h"
#include <sstream>

SqliteException::SqliteException() {
    m_msg = "Sqlite Exception";
}
SqliteException::SqliteException(std::string const& msg){
    m_msg = msg;
}
SqliteException::SqliteException(std::string const& msg, int rc){
    std::ostringstream oss;
    m_msg = msg;
    if (rc != 0){
        oss << m_msg << " RC = " << rc;
        m_msg = oss.str();
    }
}
const char* SqliteException::what() const noexcept{
    return m_msg.c_str();
}
