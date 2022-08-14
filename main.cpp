#include <iostream>
#include<thread>
#include <chrono>
#include <ctime>
#include<cstdarg>
#include"logger.h"
#include<filesystem>    

using namespace std;

int main()
{
    zjpLog::setPrefix("[GIN]");
    zjpLog::setLogINFlag(zjpLog::LOG_TIME);
    zjpLog::setLogOutFlag(zjpLog::LOG_OUT_FILE | zjpLog::LOG_CONSOLE);
    zjpLog::setOutputFile("../test/log.log");
    int i = 124;
    zjpLog::debug("hello%d", i);
    zjpLog::info("hellossd%d,%s", i + 32, "fdaasf");
    zjpLog::warn("fdsafsa%d", 323);
    zjpLog::error("mytest:%d", i * i);
    zjpLog::debug("sdaf", "fdafs");
    zjpLog::fatal("safa:%d", i + 23 + i);
    return 0;
}
