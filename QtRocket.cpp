
/// \cond
// C headers
// C++ headers
#include <thread>

// 3rd party headers
#include <QApplication>
#include <QLocale>
#include <QTranslator>
/// \endcond

// qtrocket headers
#include "QtRocket.h"
#include "gui/MainWindow.h"

// Initialize static member data
QtRocket* QtRocket::instance = nullptr;
std::mutex QtRocket::mtx;
bool QtRocket::initialized = false;


// The gui worker thread
void guiWorker(int argc, char* argv[], int& ret)
{
   QApplication a(argc, argv);
   a.setWindowIcon(QIcon(":/qtrocket.png"));

   // Start translation component.
   // TODO: Only support US English at the moment. Anyone want to help translate?
   QTranslator translator;
   const QStringList uiLanguages = QLocale::system().uiLanguages();
   for (const QString &locale : uiLanguages)
   {
      const QString baseName = "qtrocket_" + QLocale(locale).name();
      if (translator.load(":/i18n/" + baseName))
      {
         a.installTranslator(&translator);
         break;
      }
   }

   // Go!
   MainWindow w(QtRocket::getInstance());
   w.show();
   ret = a.exec();

}

QtRocket* QtRocket::getInstance()
{
   if(!initialized)
   {
      init();
   }
   return instance;
}

void QtRocket::init()
{
   std::lock_guard<std::mutex> lck(mtx);
   if(!initialized)
   {
      instance = new QtRocket();
      initialized = true;
   }
}

QtRocket::QtRocket()
{
   logger = utils::Logger::getInstance();
   running = false;

   // Need to set some sane defaults for Simulation Options
   // The default constructor for SimulationOptions will do that for us, so just use that
   setSimulationOptions(std::make_shared<sim::SimulationOptions>());

   rocket =
      std::make_shared<Rocket>();

}

int QtRocket::run(int argc, char* argv[])
{
   // Really should only start this thread once
   if(!running)
   {
      running = true;
      int ret = 0;
      std::thread guiThread(guiWorker, argc, argv, std::ref(ret));
      guiThread.join();
      return ret;
   }
   return 0;
}

void QtRocket::addMotorModels(std::vector<model::MotorModel>& m)
{
   for(const auto& i : m)
   {
      motorModels.push_back(i);
   }
   // TODO: Now clear any duplicates?
}
