#ifndef ERRORS_H
#define ERRORS_H

#include <QString>

class ProjectError : public std::exception
{
private:
    QString const _msg;
public:
    ProjectError(QString msg) : _msg(msg) { }
    QString const & msg() const { return _msg; }
};

#endif // ERRORS_H
