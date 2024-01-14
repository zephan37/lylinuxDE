#include "execlinuxcmd.h"
#include <QProcess>

void exec_linux_cmd(QString strcmd, QString &cmdresult, QString &cmderror)
{
    QProcess process;
    process.start("bash", QStringList() << "-c" << strcmd);
    process.waitForFinished();

    cmdresult = process.readAllStandardOutput();
    cmderror = process.readAllStandardError();

    cmdresult = cmdresult.left(cmdresult.indexOf("."));

}
