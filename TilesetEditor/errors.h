#ifndef ERRORS_H
#define ERRORS_H

#include <QString>

class ContextError : public std::exception
{
private:
    QString const msg;
public:
    ContextError(QString msg) : msg(msg) { }
};

#endif // ERRORS_H
