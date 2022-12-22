#define ELPP_NO_DEFAULT_LOG_FILE

#include <QApplication>
#include "mainwidget.h"
#include "login.h"
#include "adminpanelwidget.h"
#include "include/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%level %datetime %msg");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Filename, "mac.log");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Enabled, "true");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "true");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToStandardOutput, "false");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::MaxLogFileSize, "5242880");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::LogFlushThreshold, "2");
    QApplication a(argc, argv);
    Login *l = new Login();
    AdminPanelWidget ad;
    MainWidget* q = new MainWidget(nullptr);
    QObject::connect(l, &Login::usernameSignal, q, &MainWidget::getUsername);
    l->openDB();
    int login_result = l->exec();

    if (login_result == 4) {
        LOG(INFO) << "Admin logged in";
        delete l;
        delete q;
        ad.show();
    }
    else if (login_result < 4 && login_result > 0){
        delete l;
        q->openDB();
        q->show();
    }
    else {
        LOG(INFO) << "Logging closed";
        delete q;
        return 0;
    }
    return a.exec();
}
