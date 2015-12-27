#include <QCoreApplication>

#include "corelib/io/terminal.h"

#include "global.h"
#include "global/common_funcs.h"

#include "cclib/shell/task_meta.h"
#include "cclib/shell/abstract_task.h"
#include "cclib/shell/task_meta.h"

#include "task/metaserver/entry.h"
#include "task/upgrademgr/entry.h"

namespace cloudcontroller{
namespace container{

using sn::corelib::Terminal;
using sn::corelib::TerminalColor;
using cclib::shell::TaskMeta;
using cclib::shell::AbstractTask;

using MetaServerTask = cloudcontroller::task::metaserver::EntryTask;
using UpgradeMgrEntryTask = cloudcontroller::task::upgrademgr::EntryTask;

Global::Global(shell::TaskLoop &loop)
   : AbstractTaskContainer("Global", loop)
{
   m_containerPs = "cloudcontroller >> ";
   initUsage();
   initRouter();
   initTaskPool();
}

void Global::initUsage()
{
   addUsageText("you can use commands as fowllow:\n", TerminalColor::LightGreen);
   addUsageText("version    show version info\n");
   addUsageText("help       get online usage doc\n");
   addUsageText("quit       exit application\n");
   addUsageText("metaserver connect --host=<host>         <host> specify the host server address\n");
}

void Global::initRouter()
{
   addTaskRoute("version", "version", 1, {
                   {"category", "Global"},
                   {"name", "Version"}
                });
   addTaskRoute("help", "help", 1, {
                   {"category", "Global"},
                   {"name", "Help"}
                });
   addTaskRoute("quit", "quit", 1, {
                   {"category", "Global"},
                   {"name", "Quit"}
                });
   addTaskRoute("metaserver", "metaserver connect --host=", 1, {
                   {"category", "Global"},
                   {"name", "MetaServer"}
                });
   addTaskRoute("upgrademgr", "upgrademgr connect --host=", 1, {
                   {"category", "Global"},
                   {"name", "UpgradeMgr"},
                   {"port", "7777"}
                });
}

void Global::initTaskPool()
{
   m_taskRegisterPool.insert("Global_Global_MetaServer", [](AbstractTaskContainer& container, const TaskMeta& meta)->AbstractTask*{
      MetaServerTask* task = new MetaServerTask(container, meta);
      return task;
   });
   m_taskRegisterPool.insert("Global_Global_UpgradeMgr", [](AbstractTaskContainer& container, const TaskMeta& meta)->AbstractTask*{
      UpgradeMgrEntryTask* task = new UpgradeMgrEntryTask(container, meta);
      return task;
   });
}


void Global::runTask(const TaskMeta& meta)
{
   if(!dispatchBuildInTask(meta)){
      AbstractTaskContainer::runTask(meta);
   }
}

bool Global::dispatchBuildInTask(const TaskMeta& meta)
{
   QString key(meta.getCategory()+'_'+meta.getName());
   if(key == "Global_Version"){
      showVersionTask(meta);
      return true;
   }else if(key == "Global_Help"){
      showHelpTask(meta);
      return true;
   }else if(key == "Global_Quit"){
      quitTask(meta);
      return true;
   }
   return false;
}


void Global::showVersionTask(const TaskMeta&)
{
   Terminal::writeText(QString("v%1\n").arg(CLOUD_CONTROLLER_VERSION).toLocal8Bit(), TerminalColor::Cyan);
}

void Global::showHelpTask(const TaskMeta&)
{
   printUsage();
}

void Global::quitTask(const TaskMeta&)
{
   m_taskLoop.exitRequest();
}

}//container
}//cloudcontroller