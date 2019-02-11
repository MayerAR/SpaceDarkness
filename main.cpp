#include <QApplication>
#include "mainController/mainController.h"


using namespace std;

int main(int argc, char *argv[])
{
//    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    QApplication a(argc, argv);

    MainController m;
    m.createStartDialog();
    a.exec();
    return 0;
}


//    QTranslator translator;
//    translator.load(QLocale::system().name(), ":/lang/");
//    a.installTranslator(&translator);
