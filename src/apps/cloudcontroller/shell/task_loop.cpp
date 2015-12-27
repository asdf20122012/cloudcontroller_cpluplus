#include "task_loop.h"
#include <csignal>

#include "container/global.h"
#include "container/metaserver.h"
#include "container/upgrademgr.h"

#include "application.h"

namespace cloudcontroller{
namespace shell{

using GlobalContainer = cloudcontroller::container::Global;
using MetaServerContainer = cloudcontroller::container::MetaServer;
using UpgradeMgrContainer = cloudcontroller::container::UpgradeMgr;

void TaskLoop::initCommandContainer()
{
   m_taskContainerPool.insert("Global", new GlobalContainer(*this));
   m_taskContainerPool.insert("MetaServer", new MetaServerContainer(*this));
   m_taskContainerPool.insert("UpgradeMgr", new UpgradeMgrContainer(*this));
}

bool TaskLoop::isNeedRestartSelectCall()
{
   return false;
}

}//shell
}//cloudcontroller