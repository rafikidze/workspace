#include "logger.h"
#include <QByteArray>
#include <application.h>
#include "qdebug.h"
Logger::Logger(LogDb &logDb) :
logDb( logDb )
{

}

void Logger::logAppRun()
{
    logCommon( new AppRunRecord() );
}

void Logger::logAppQuit()
{
    dataAccessMutex.lock();
    logCommon( new AppQuitRecord() );
    dataAccessMutex.unlock();
}

void Logger::logConfigure(unsigned char rtsNumber, const RTEState &rtsState, QString bprm )
{


    dataAccessMutex.lock();
    unsigned char data[5] = {0, 0, 0, 0, 0};
        data[0] |= (rtsState._1k          == 1 ? 1 : 0) << 7;
        data[0] |= (rtsState._2k          == 1 ? 1 : 0) << 6;
        data[0] |= (rtsState._P200        == 1 ? 1 : 0) << 5;
        data[0] |= (rtsState._PT          == 1 ? 1 : 0) << 4;
        data[0] |= (rtsState._DES         == 1 ? 1 : 0) << 3;
        data[0] |= (rtsState._Rez_PAR     == 1 ? 1 : 0) << 2;
        data[0] |= (rtsState._AV_PAR      == 1 ? 1 : 0) << 1;
        data[0] |= (rtsState._Rab_DES     == 1 ? 1 : 0) << 0;

        data[1] |= (rtsState._AV_DES      == 1 ? 1 : 0) << 7;
        data[1] |= (rtsState._Zapr_puska  == 1 ? 1 : 0) << 6;
        data[1] |= (rtsState._MRM1        == 1 ? 1 : 0) << 5;
        data[1] |= (rtsState._MRM2        == 1 ? 1 : 0) << 4;
        data[1] |= ((rtsState._P200_MRM1 || rtsState._P200_MRM2) ? 1 : 0)  << 5;
        data[1] |= (rtsState._Rez_MRM     == 1 ? 1 : 0) << 2;
        data[1] |= (rtsState._AV_MRM      == 1 ? 1 : 0) << 1;

        data[2]  =  rtsState._AV;
        data[3]  =  rtsState._AV_PD1;
        data[4]  =  rtsState._AV_PD2;


    QByteArray data_ar( reinterpret_cast<char *>( data ), sizeof( data ) );

    Application::app->retLogDb().appendRec( new ConfigureRecord( rtsNumber, data_ar, bprm ) );
    logFails( 1, rtsNumber, RTEState(), rtsState, bprm );
    dataAccessMutex.unlock();
}

void Logger::logStateDiff(unsigned char rtsNumber, const RTEState &prevState, const RTEState &newState,QString bprm)
{

    dataAccessMutex.lock();
    QList<LogDbRecord *> list;

    if(prevState._1k         != newState._1k        )  list.append( new StateRecord( rtsNumber, StateRecord::F_1k,         newState._1k,bprm         ) );
    if(prevState._2k         != newState._2k        )   list.append( new StateRecord( rtsNumber, StateRecord::F_2k,         newState._2k,bprm         ) );
    if(prevState._P200       != newState._P200      )   list.append( new StateRecord( rtsNumber, StateRecord::F_P200,       newState._P200,bprm       ) );
    if(prevState._PT         != newState._PT        )   list.append( new StateRecord( rtsNumber, StateRecord::F_PT,         newState._PT,bprm         ) );
    if(prevState._Rez_PAR    != newState._Rez_PAR   )   list.append( new StateRecord( rtsNumber, StateRecord::F_Rez_PAR,    newState._Rez_PAR,bprm    ) );
    if(prevState._Rab_DES    != newState._Rab_DES   )   list.append( new StateRecord( rtsNumber, StateRecord::F_Rab_DES,    newState._Rab_DES,bprm    ) );
    if(prevState._MRM1       != newState._MRM1      )   list.append( new StateRecord( rtsNumber, StateRecord::F_MRM1,       newState._MRM1,bprm       ) );
    if(prevState._MRM2       != newState._MRM2      )   list.append( new StateRecord( rtsNumber, StateRecord::F_MRM2,       newState._MRM2,bprm       ) );
    if((prevState._P200_MRM1 != newState._P200_MRM1) ||
          (prevState._P200_MRM2 != newState._P200_MRM2))
    {
        unsigned char freq_mrm;
        freq_mrm = ((newState._P200_MRM1&0x02) | (newState._P200_MRM2&0x02) ) ? 1 : 0;
        list.append( new StateRecord( rtsNumber, StateRecord::F_P200_MRM, freq_mrm, bprm  ) );
    }
    if(prevState._Rez_MRM    != newState._Rez_MRM   )   list.append( new StateRecord( rtsNumber, StateRecord::F_Rez_MRM,    newState._Rez_MRM,bprm    ) );
    if(prevState._Zapr_puska != newState._Zapr_puska)   list.append( new StateRecord( rtsNumber, StateRecord::F_Zapr_puska, newState._Zapr_puska,bprm ) );


    for(unsigned char i = 0; i < list.size(); i++)
         Application::app->retLogDb().appendRec( list[i] );

    logFails(list.size(), rtsNumber, prevState, newState,bprm );
    dataAccessMutex.unlock();
}

void Logger::logRme400Management(unsigned char rtsNumber, const RTECommand &prevInfo, const RTECommand &newInfo, QString bprm)
{
    dataAccessMutex.lock();

    LogDbRecord *rec = 0;
    LogDbRecord *rec_izl = 0;




    // мы либо вкл/выкл прд, либо мрм, либо ДЭА, либо РР
    // дополнительно должно отображаться только излучение!

    if(newInfo._SbrA == 1)  {
        rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_SbrA, 1, bprm );
        Application::app->retLogDb().appendRec(rec);
    }


    logCommon_command();
    dataAccessMutex.unlock();

}
void Logger::logPKManagement(unsigned char pk, bool check, QString bprm){

    unsigned char rtsNumber = 0x00;
    dataAccessMutex.lock();
    LogDbRecord *rec = 0;


    if(check){ // включаем ПК и излучение

        switch (pk) {
        case 1:
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_1k, 1, bprm );
            Application::app->retLogDb().appendRec(rec);
            break;
        case 2:
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_2k, 1, bprm );
            Application::app->retLogDb().appendRec(rec);
            break;

        }
        rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_P200, 1, bprm );
        Application::app->retLogDb().appendRec(rec);

    }
    else {

        switch (pk) {
        case 1:
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_1k, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
            break;
        case 2:
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_2k, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
            break;

        }
        rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_P200, 0, bprm );
        Application::app->retLogDb().appendRec(rec);

    }

    logCommon_command();
    dataAccessMutex.unlock();

}
//! mrm - какой MRM
void Logger::logMRMManagement(unsigned char mrm, bool check, QString bprm){
    unsigned char rtsNumber = 0x00;
    dataAccessMutex.lock();
    LogDbRecord *rec = 0;


    if(check){ // включаем ПК и излучение

        switch (mrm) {
        case 1:
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_MRM1, 1, bprm );
            Application::app->retLogDb().appendRec(rec);
            break;
        case 2:
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_MRM2, 1, bprm );
            Application::app->retLogDb().appendRec(rec);
            break;

        }
        rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_P200_MRM, 1, bprm );
        Application::app->retLogDb().appendRec(rec);

    }
    else {

        switch (mrm) {
        case 1:
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_MRM1, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
            break;
        case 2:
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_MRM2, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
            break;

        }
        rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_P200_MRM, 0, bprm );
        Application::app->retLogDb().appendRec(rec);

    }

    logCommon_command();
    dataAccessMutex.unlock();
}


void Logger::logDEAManagement(bool check, QString bprm)
{
    unsigned char rtsNumber = 0x00;
    dataAccessMutex.lock();
    LogDbRecord *rec = 0;

    if(check){
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_Zapr_puska, 1, bprm );
            Application::app->retLogDb().appendRec(rec);
        }
    else {
        rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_Zapr_puska, 0, bprm );
        Application::app->retLogDb().appendRec(rec);

    }

    logCommon_command();
    dataAccessMutex.unlock();
}

void Logger::logRRManagement(bool check, QString bprm)
{
    unsigned char rtsNumber = 0x00;
    dataAccessMutex.lock();
    LogDbRecord *rec = 0;

    if(check){
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_PT, 1, bprm );
            Application::app->retLogDb().appendRec(rec);
        }
    else {
        rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_PT, 0, bprm );
        Application::app->retLogDb().appendRec(rec);

    }

    logCommon_command();
    dataAccessMutex.unlock();
}

void Logger::logRme400ManagementOff(StateNum state, const RTECommand &prevInfo, const RTECommand &newInfo, QString bprm)
{
    unsigned char rtsNumber = 0x00;
    dataAccessMutex.lock();
    LogDbRecord *rec = 0;


    if(state == PRD)
    {
        if(newInfo._1k == 0 && prevInfo._1k == 1) {
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_1k, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
        }
        else if(newInfo._2k == 0 && prevInfo._2k == 1){
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_2k, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
        }
        else if(newInfo._2k == 0 && prevInfo._2k == 0){
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_K, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
        }

        rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_P200, 0, bprm );
        Application::app->retLogDb().appendRec(rec);


    }
    else if (state == MRM){

        if((newInfo._MRM1&0x01) == 0 && (prevInfo._MRM1&0x01) == 1){
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_MRM1, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
        }
        else if((newInfo._MRM2&0x01) == 0 && (prevInfo._MRM2&0x01) == 1){
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_MRM2, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
        }
        else if((newInfo._MRM2&0x01) == 0 && (prevInfo._MRM2&0x01) == 0){
            rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_MRM, 0, bprm );
            Application::app->retLogDb().appendRec(rec);
        }

        rec = new Rme400ManagementRecord( rtsNumber, Rme400ManagementRecord::F_P200_MRM, 0, bprm );
        Application::app->retLogDb().appendRec(rec);


        }

    logCommon_command();
    dataAccessMutex.unlock();


}
void Logger::logConnectionFail(unsigned char rtsNumber, unsigned char connectionFail, QString bprm)
{
    dataAccessMutex.lock();
    logCommon( new ConnectionFailRecord(rtsNumber, connectionFail, bprm ) );
    dataAccessMutex.unlock();
}

void Logger::logLogin(const QString &userName, bool login)
{
    dataAccessMutex.lock();
    logCommon( new LoginRecord( userName, login ? 1 : 0) );
    dataAccessMutex.unlock();
}

void Logger::logAppendAccount(const QString &name)
{
    dataAccessMutex.lock();
    logCommon( new AccountManagementRecord( name, AccountManagementRecord::A_APPEND, "----" ) );
    dataAccessMutex.unlock();
}

void Logger::logModifyAccount(const QString &name)
{
    dataAccessMutex.lock();
    logCommon( new AccountManagementRecord( name, AccountManagementRecord::A_MODIFY, "----" ) );
    dataAccessMutex.unlock();
}

void Logger::logRemoveAccount(const QString &name)
{
    dataAccessMutex.lock();
    logCommon( new AccountManagementRecord( name, AccountManagementRecord::A_REMOVE, "----" ) );
    dataAccessMutex.unlock();
}

void Logger::logSoundEnable(bool enable)
{
    dataAccessMutex.lock();
    logCommon( new SoundRecord( enable ? 1 : 0 ) );
    dataAccessMutex.unlock();
}

void Logger::logAppSetup(unsigned char rtsNumber, APPSETUPOPTION aso, bool enable, QString bprm)
{
    dataAccessMutex.lock();
    AppSetupRecord::OPTION o = AppSetupRecord::O_UNDEFINED;
    switch(aso)
    {
    case ASO_MAINFACPANE: o = AppSetupRecord::O_MAINFACPANE; break;
    case ASO_BEACONPANE:  o = AppSetupRecord::O_BEACONPANE;  break;
    case ASO_DIESELPANE:  o = AppSetupRecord::O_DIESELPANE;  break;
    case ASO_DISPPANE:    o = AppSetupRecord::O_DISPPANE;    break;
    case ASO_RTSPANE:     o = AppSetupRecord::O_RTSPANE;     break;
    case ASO_Ust_comport: o = AppSetupRecord::O_Ust_comport; break;

    }

    logCommon( new AppSetupRecord( rtsNumber, o, enable ? 1 : 0, bprm ) );
    dataAccessMutex.unlock();
}

void Logger::logFails(unsigned int stateListSize, unsigned char rtsNumber, const RTEState &prevState, const RTEState &newState, QString bprm)
{
    QList<LogDbRecord *> list;

    if(prevState._AV_PAR  != newState._AV_PAR ) list.append( new StateRecord( rtsNumber, StateRecord::F_AV_PAR,  newState._AV_PAR,bprm                ) );
    if(prevState._AV_DES  != newState._AV_DES ) list.append( new StateRecord( rtsNumber, StateRecord::F_AV_DES,  newState._AV_DES,bprm                ) );
    if(prevState._AV_MRM  != newState._AV_MRM ) list.append( new StateRecord( rtsNumber, StateRecord::F_AV_MRM,  newState._AV_MRM,bprm                ) );
    if(prevState._AV      != newState._AV     )
    {
        if(prevState._AV == 0x03 || prevState._AV == 0x04 || prevState._AV == 0x09)
            list.append( new StateRecord( rtsNumber, StateRecord::F_AV,     prevState._AV & 0x0F,bprm             ) );

        if(newState._AV == 0x03 || newState._AV == 0x04 || newState._AV == 0x09)
            list.append( new StateRecord( rtsNumber, StateRecord::F_AV,      newState._AV & 0x0F | 0x10,bprm      ) );

    }
    if(prevState._AV_PD1  != newState._AV_PD1 )
    {
        if(prevState._AV_PD1 != 0x00 && prevState._AV_PD1 != 0x0E && prevState._AV_PD1 != 0x0F)
            list.append( new StateRecord( rtsNumber, StateRecord::F_AV_PD1, prevState._AV_PD1 & 0x0F,bprm         ) );

        if(newState._AV_PD1 != 0x00 && newState._AV_PD1 != 0x0E && newState._AV_PD1 != 0x0F)
            list.append( new StateRecord( rtsNumber, StateRecord::F_AV_PD1,  newState._AV_PD1 & 0x0F | 0x10,bprm  ) );

    }
    if(prevState._AV_PD2  != newState._AV_PD2 )
    {
        if(prevState._AV_PD2 != 0x00 && prevState._AV_PD2 != 0x0E && prevState._AV_PD2 != 0x0F)
            list.append( new StateRecord( rtsNumber, StateRecord::F_AV_PD2, prevState._AV_PD2 & 0x0F,bprm         ) );

        if(newState._AV_PD2 != 0x00 && newState._AV_PD2 != 0x0E && newState._AV_PD2 != 0x0F)
            list.append( new StateRecord( rtsNumber, StateRecord::F_AV_PD2,  newState._AV_PD2 & 0x0F | 0x10,bprm  ) );
    }

    if(stateListSize == 0 && list.size() == 0)
        return;

    for(unsigned char i = 0; i < list.size(); i++)
        Application::app->retLogDb().appendRec( list[i] );


     Application::app->retAllForm().retLogTableView().updateView();
     Application::app->ret_file_settings().saveLogFile();

}

void Logger::logCommon(LogDbRecord *rec)
{
     Application::app->retLogDb().appendRec(rec);
     Application::app->retAllForm().retLogTableView().updateView();
     Application::app->ret_file_settings().saveLogFile();
}
void Logger::logCommon_command()
{

     Application::app->retAllForm().retLogTableView().updateView();
     Application::app->ret_file_settings().saveLogFile();
}
