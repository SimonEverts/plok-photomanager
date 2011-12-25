#include "miscutils.h"

#include <QProcessEnvironment>

MiscUtils::MiscUtils()
{
}

QString MiscUtils::plokpmDir (void)
{
    QString result = "~/.plok-pm/";

    QProcessEnvironment processEnv = QProcessEnvironment::systemEnvironment();

    if (processEnv.contains("HOME"))
        result = processEnv.value("HOME") + "/.plok-pm/";

    return result;
}
