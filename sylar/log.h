#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>

namespace sylar{

//日志事件
class LogEvent {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent();
private:
	const char* m_file = nullptr;	//文件名
	int32_t m_threadId = 0;			//行号
	uint32_t m_elapse = 0;			//程序启动开始到现在的毫秒数
	uint32_t m_threadId = 0;		//线程id
	uint32_t m_fiberId = 0;			//协程id
	uint64_t m_time;				//时间戳
	std::string m_content;
};

//日志级别
class LogLevel {
public:	
	enum {
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};
};

//日志输出地
class LogAppender {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	virtual ~LogAppender(){}

	void log(LogLevel::Level level, LogEvent::ptr event);
private:
	LogLevel::Level m_level;
};

//日志格式器
class LogFormatter {
public:	
	typedef std::shared_ptr<LogEvent> ptr;

	std::string format(LogEvent::ptr event);
private:
};

class Logger {
public:
	typedef std::shared_ptr<LogEvent> ptr;

	Logger(const std::string& name = "root");
	void log(LevelLevel, LogEvent::ptr event);

	void debug(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);

	void addAppender(LogAppender::ptr appender);
	void delAppender(LogAppender::ptr appender);
	LogLevel::Level getLevel() const { return m_level;}
	void setLevel(LogLevel::Level val) { m_level = val;}

private:
	std::string m_name;
	LogLevel::Level m_level;
	std::list<LogAppender::ptr> m_appenders;
};
// 输出到控制台
class StdoutLogAppender : public LogAppender {
	
};
// 输出到文件
class FileLogAppender : public LogAppender {
	
};


}
#endif
