#include "logger.h"
#include <filesystem>

using namespace zjpLog;

#define COLOR_INFO "\033[0m\033[40;32m"  // green
#define COLOR_DEBUG "\033[0m\033[40;36m" // teal
#define COLOR_WARN "\033[0m\033[40;33m"  // yellow
#define COLOR_ERROR "\033[0m\033[40;31m" // red
#define COLOR_FATAL "\033[0m\033[40;35m" // purple
#define COLOR_END "\033[0;37;40m"        // reset

const char *log_filename = nullptr;
int log_flag = LOG_IN_DEF;
int log_output_flag = LOG_OUT_DEF;
const char *log_prefix = nullptr;

const char *color_level_text[] = {
    COLOR_DEBUG "DEBUG" COLOR_END,
    COLOR_INFO "INFO" COLOR_END,
    COLOR_WARN "WARN" COLOR_END,
    COLOR_ERROR "ERROR" COLOR_END,
    COLOR_FATAL "FATAL" COLOR_END};

const char *plain_level_text[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"};

const char *GET_LEVEL_TEXT(FILE *dest, LOG_LEVEL level)
{
    if (dest == stdout || dest == stderr)
        return color_level_text[level];
    return plain_level_text[level];
}

//设置前缀
void zjpLog::setPrefix(const char *prefix)
{
    log_prefix = prefix;
}

//设置日志输出内容标志
void zjpLog::setLogINFlag(LOG_IN_FLAG logflag)
{
    log_flag = logflag;
}

//设置日志输出方式
void zjpLog::setLogOutFlag(int logflag)
{
    log_output_flag = logflag;
}  

//设置日志输出文件
void zjpLog::setOutputFile(const char *filename)
{
    log_filename = filename;
}

Logger& Logger::instance()
{
    static Logger lg;
    return lg;
}

Logger::Logger()
{
    init();
}

Logger::~Logger()
{
    close();
}

void Logger::init()
{
    open();
    m_log_in_flag = static_cast<LOG_IN_FLAG>(log_flag);
    m_log_output = static_cast<LOG_OUTPUT>(log_output_flag);
    m_prefix = log_prefix;
}

void Logger::open()
{
    //文件名存在
    if (log_filename != nullptr)
    {
        m_filename = log_filename;
        m_fp = fopen(log_filename, "a+");
        if (!m_fp)
        {
            std::string current_path = std::filesystem::current_path();
            fprintf(stderr, "open log file failed: %s, log execute at: %s\n", log_filename, current_path.c_str());
            exit(1);
        }
    }
}

void Logger::close()
{
    if (m_fp)
    {
        fclose(m_fp);
        m_fp = nullptr;
    }
}

void Logger::error_print(const char *info)
{
    fprintf(stderr, "\033[0;40;31m%s\033[0,40,37m", info);
}

void Logger::log(FILE *dest, LOG_LEVEL level, const char *file, int line, const char *format, va_list args)
{
    flockfile(dest);                                //文件加锁
    auto *level_text = GET_LEVEL_TEXT(dest, level); //获取日志等级字符串
    auto tm = std::time(nullptr);
    auto ptm = localtime(&tm);
    char buff[256] = {0};
    //如果有前缀，打印前缀
    if (m_prefix)
    {
        fprintf(dest, "%s ", m_prefix);
    }
    //打印年月日
    if (m_log_in_flag & LOG_DATA)
    {
        strftime(buff, sizeof(buff), "%Y-%m-%d", ptm);
        fprintf(dest, "[%s]", buff);
    }
    fprintf(dest, "%s ", level_text);      //打印日志等级
    fprintf(dest, "[%s:%d] ", file, line); //打印文件名和行号
    vfprintf(dest, format, args);          //打印日志输出内容
    fprintf(dest, "\r\n");
    fflush(dest);
    funlockfile(dest); //解锁
}

void Logger::logDebug(const char *filename, int line, const char *format, ...)
{
    va_list args;
    //输出到文件
    if (m_log_output & LOG_OUT_FILE)
    {
        va_start(args, format);
        if (m_fp)
        {
            if (std::filesystem::exists(m_filename)) //写入的文件还存在
                log(m_fp, LOG_DEBUG, filename, line, format, args);
            else //写入的文件已经删除了
            {
                close(); //关闭文件
                m_fp = nullptr;
                //并打印错误
                error_print("file has been removed!");
            }
        }
        va_end(args);
    }
    //输出到控制台
    if (m_log_output & LOG_CONSOLE)
    {
        va_start(args, format);
        log(stdout, LOG_DEBUG, filename, line, format, args);
        va_end(args);
    }
}

void Logger::logInfo(const char *filename, int line, const char *format, ...)
{
    va_list args;
    //输出到文件
    if (m_log_output & LOG_OUT_FILE)
    {
        va_start(args, format);
        if (m_fp)
        {
            if (std::filesystem::exists(m_filename)) //写入的文件还存在
                log(m_fp, LOG_INFO, filename, line, format, args);
            else //写入的文件已经删除了
            {
                close(); //关闭文件
                m_fp = nullptr;
                //并打印错误
                error_print("file has been removed!");
            }
        }
        va_end(args);
    }
    //输出到控制台
    if (m_log_output & LOG_CONSOLE)
    {
        va_start(args, format);
        log(stdout, LOG_INFO, filename, line, format, args);
        va_end(args);
    }
}

void Logger::logWarn(const char *filename, int line, const char *format, ...)
{
    va_list args;
    //输出到文件
    if (m_log_output & LOG_OUT_FILE)
    {
        va_start(args, format);
        if (m_fp)
        {
            if (std::filesystem::exists(m_filename)) //写入的文件还存在
                log(m_fp, LOG_WARN, filename, line, format, args);
            else //写入的文件已经删除了
            {
                close(); //关闭文件
                m_fp = nullptr;
                //并打印错误
                error_print("file has been removed!");
            }
        }
        va_end(args);
    }
    //输出到控制台
    if (m_log_output & LOG_CONSOLE)
    {
        va_start(args, format);
        log(stdout, LOG_WARN, filename, line, format, args);
        va_end(args);
    }
}

void Logger::logError(const char *filename, int line, const char *format, ...)
{
    va_list args;
    //输出到文件
    if (m_log_output & LOG_OUT_FILE)
    {
        va_start(args, format);
        if (m_fp)
        {
            if (std::filesystem::exists(m_filename)) //写入的文件还存在
                log(m_fp, LOG_ERROR, filename, line, format, args);
            else //写入的文件已经删除了
            {
                close(); //关闭文件
                m_fp = nullptr;
                //并打印错误
                error_print("file has been removed!");
            }
        }
        va_end(args);
    }
    //输出到控制台
    if (m_log_output & LOG_CONSOLE)
    {
        va_start(args, format);
        log(stdout, LOG_ERROR, filename, line, format, args);
        va_end(args);
    }
}

void Logger::logFatal(const char *filename, int line, const char *format, ...)
{
    va_list args;
    //输出到文件
    if (m_log_output & LOG_OUT_FILE)
    {
        va_start(args, format);
        if (m_fp)
        {
            if (std::filesystem::exists(m_filename)) //写入的文件还存在
                log(m_fp, LOG_FATAL, filename, line, format, args);
            else //写入的文件已经删除了
            {
                close(); //关闭文件
                m_fp = nullptr;
                //并打印错误
                error_print("file has been removed!");
            }
        }
        va_end(args);
    }
    //输出到控制台
    if (m_log_output & LOG_CONSOLE)
    {
        va_start(args, format);
        log(stdout, LOG_FATAL, filename, line, format, args);
        va_end(args);
    }
}
