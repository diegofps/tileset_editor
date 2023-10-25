#ifndef ERRORS_H
#define ERRORS_H

#include <QString>

class ContextError : public std::exception
{
private:
    QString const _msg;
public:
    ContextError(QString msg) : _msg(msg) { }
    QString const & msg() const { return _msg; }
};

#endif // ERRORS_H
