#ifndef LOGGER_H
#define LOGGER_H

#include "datatypes.h"
#include <QString>
#include <QMutex>
#include "logdb.h"
class Logger
{
public:
    enum APPSETUPOPTION
    {
        ASO_MAINFACPANE,
        ASO_BEACONPANE,
        ASO_DIESELPANE,
        ASO_DISPPANE,
        ASO_RTSPANE,
        ASO_Ust_comport
    };

    Logger(LogDb &logDb);


    void logAppRun();
    void logAppQuit();
    void logStateDiff(unsigned char rtsNumber, const RTEState &prevState, const RTEState &newState,QString bprm);
    void logRme400Management(unsigned char rtsNumber, const RTECommand &prevInfo, const RTECommand &newInfo, QString bprm);

    //! pk - какой ПК
    void logPKManagement(unsigned char pk, bool check, QString bprm);

    //! mrm - какой MRM
    void logMRMManagement(unsigned char mrm, bool check, QString bprm);

    void logDEAManagement(bool check, QString bprm);
    void logRRManagement(bool check, QString bprm);

    void logRme400ManagementOff(StateNum, const RTECommand &prevInfo, const RTECommand &newInfo, QString bprm);
    void logConnectionFail(unsigned char rtsNumber, unsigned char connectionFail, QString bprm);
    void logConfigure(unsigned char rtsNumber, const RTEState &rtsState, QString bprm);

    void logLogin(const QString &userName, bool login);
    void logAppendAccount(const QString &name);
    void logModifyAccount(const QString &name);
    void logRemoveAccount(const QString &name);
    void logSoundEnable(bool enable);

    void logAppSetup(unsigned char rtsNumber, APPSETUPOPTION aso, bool enable, QString bprm);
    void logFails(unsigned int stateListSize, unsigned char rtsNumber, const RTEState &prevState, const RTEState &newState, QString bprm);

private:
    QMutex dataAccessMutex;
    LogDb &logDb;

    void logCommon(LogDbRecord *rec);
    void logCommon_command();
};
#endif // LOGGER_H
