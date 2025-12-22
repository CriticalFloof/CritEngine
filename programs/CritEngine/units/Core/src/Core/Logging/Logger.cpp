#include "Logger.h"

const std::string Debug::GetCurrentTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();

	auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration) - hours;
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration) - hours - minutes;
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) - hours - minutes - seconds;

	std::time_t time_now = std::chrono::system_clock::to_time_t(now);

	// This is considered deprecated because it's not thread-safe, but the alternatives are not cross-platform and
	// I don't think we need multithreading for this, so we can ignore the warning
	std::tm local_time;
	localtime_s(&local_time, &time_now);

	/* Regardless, this is how it would be done safely in Windows:
	std::tm local_time;
	localtime_s(&local_time, &time_now);

	And for POSIX:
	std::tm local_time;
	localtime_r(&time_now, &local_time);

	*/
	std::ostringstream timestamp;
	timestamp << std::put_time(&local_time, "%H:%M:%S") << ":"
		<< std::setfill('0') << std::setw(3) << milliseconds.count();

	return timestamp.str();
}

const void LogInfo(const std::string& label, const std::string& message)
{
	Debug::Log("[" + label + "]", message);
}

const void LogWarning(const std::string& label, const std::string& message)
{
	Debug::LogWarning("[" + label + "]", message);
}

const void LogError(const std::string& label, const std::string& message)
{
	Debug::LogError("[" + label + "]", message);
}

