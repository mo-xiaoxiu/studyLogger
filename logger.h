#ifndef ZJPLOGGER_H_
#define ZJPLOGGER_H_

#include <stdio.h>
#include <stdarg.h>

namespace zjpLog
{

    //文件输出内容
    enum LOG_IN_FLAG
    {
        LOG_DATA = 1 << 0,      //年月日
        LOG_TIME = 1 << 1, //小时-分钟-秒
        LOG_IN_FILE = 1 << 2, //文件名
        LOG_LINE = 1 << 3, //行号
        LOG_FUNC = 1 << 4, //函数
        //默认的标准内容格式
        LOG_IN_DEF = LOG_DATA | LOG_TIME | LOG_IN_FILE | LOG_LINE
    };

    //文件输出方式：文件 or 控制台
    enum LOG_OUTPUT
    {
        LOG_OUT_FILE = 1 << 0,
        LOG_CONSOLE = 1 << 1,
        LOG_OUT_DEF = LOG_CONSOLE
    };

    //日志等级
    enum LOG_LEVEL
    {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_FATAL,
        LEVEL_COUNT
    };


#define debug(format, ...) \
    Logger::instance().logDebug(__FILE__, __LINE__, format, ##__VA_ARGS__)

#define info(format, ...) \
    Logger::instance().logInfo(__FILE__, __LINE__, format, ##__VA_ARGS__)

#define warn(format, ...) \
    Logger::instance().logWarn(__FILE__, __LINE__, format, ##__VA_ARGS__)

#define error(format, ...) \
    Logger::instance().logError(__FILE__, __LINE__, format, ##__VA_ARGS__)

#define fatal(format, ...) \
    Logger::instance().logFatal(__FILE__, __LINE__, format, ##__VA_ARGS__)


    void setPrefix(const char *prefix);       //设置前缀
    void setLogINFlag(LOG_IN_FLAG logflag);   //设置日志输出内容标志
    void setLogOutFlag(int logflag);   //设置日志输出方式
    void setOutputFile(const char *filename); //设置日志输出文件

    class Logger
    {
    private:
        Logger();

    public:
        ~Logger();

        static Logger &instance(); //单例模式：默认构造函数设置为私有的（懒汉式）

        void init();  //初始化日志
        void open();  //打开日志系统（文件）
        void close(); //关闭日志系统（文件）
        void logDebug(const char *file, int line, const char *format, ...);
        void logInfo(const char *file, int line, const char *format, ...);
        void logWarn(const char *file, int line, const char *format, ...);
        void logError(const char *file, int line, const char *format, ...);
        void logFatal(const char *file, int line, const char *format, ...);

        void log(FILE *dest, /*输出方式*/ LOG_LEVEL level, const char *file, int line, const char *format, va_list args);

    private:
        FILE *m_fp;             //文件指针
        const char *m_filename; //文件名
        const char *m_prefix;   //前缀
        //文件输出内容（有哪些）
        LOG_IN_FLAG m_log_in_flag;
        //文件输出标志：输出到终端控制台还是文件
        LOG_OUTPUT m_log_output;

        //打印致命错误
        static void error_print(const char *info);
    };

}

#endif