#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <stringstreams>
#include <fasteam>

namespace sylar{

class Logger;
//日志事件
class LogEvent {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent();

	const char* getfile() const{ return m_file;}
	int32_t getLine() const { return m_line;}
	uint32_t getElapse() const { return m_elapse;}
	uint32_t getThreadId() const { return m_fiberId;}
	uint32_t getTime() const { return m_time};
	const std::string& getContent() const { return m_content;}
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
		UNKNOW = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};

	static const char* ToString(LogLevel::Level level);
};

//日志输出地
class LogAppender {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	virtual ~LogAppender(){}

	virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
	
	void setFormatter(LogFormatter::ptr val) { m_formatter = val;}
	LogFormatter::ptr getFormatter() const { return m_formatter; }
private:
	LogLevel::Level m_level;
	LogFrmatter::ptr m_formatter;
};

//日志格式器
class LogFormatter {
public:	
	typedef std::shared_ptr<LogEvent> ptr;
	LogFormatter(const std::string& pattern);

	std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
private:
	class FormatItem {
	public: 
		typedef std::shared_ptr<FormatItem> ptr;
		FormatItem(const std::string& fmt = "") {};
		virtual ~FormatItem() {}
		virtual void std::string format(std::ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
	};
	void init();

private:
	std::string m_pattern;
	std::vector<FormatItem::ptr> m_item;

protected:
	LogLevel::Level m_level;
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
	
	const std::string& getName() const { return m_name};
private:
	std::string m_name;
	LogLevel::Level m_level;
	std::list<LogAppender::ptr> m_appenders;
};
// 输出到控制台
class StdoutLogAppender : public LogAppender {
	typedef std::shared_ptr<StdoutLogAppender> ptr;
	void log(LogLevel::Level level, LogEvent::ptr event) overide;
};
// 输出到文件
class FileLogAppender : public LogAppender {
public:	
	typedef std::shared_ptr<FileLogAppender> ptr;
	FileLogAppender(const std::string& filename);
	void log(LogLevel::Level level, LogEvent::ptr event) overide;

	void reopen();
private:
	std::string m_name;
	std::ofstream m_filestream;
};


}
#endif
