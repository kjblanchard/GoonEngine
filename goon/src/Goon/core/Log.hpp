#pragma once
#include <spdlog/logger.h>
#include <spdlog/fmt/ostr.h>

namespace goon
{
    class Log
    {
    public:
        /**
         * @brief Initializes the Logger
         */
        static void Init();
        /**
         * @brief Gets the core logger
         *
         * @return Shared ptr to a core logger used for the engine
         */
        inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
        /**
         * @brief Gets the Client (application) logger
         *
         * @return Shared ptr to a client logger
         */
        inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    private:
        /**
         * @brief Shared ptr to a spdlog logger
         */
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        /**
         * @brief Shared ptr to a client logger
         */
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

//Macros so that we can strip this in distribution builds easily.
//
//Core log metrics
#define GN_CORE_ERROR(...) ::goon::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GN_CORE_WARN(...) ::goon::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GN_CORE_INFO(...) ::goon::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GN_CORE_TRACE(...) ::goon::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GN_CORE_FATAL(...) ::goon::Log::GetCoreLogger()->critical(__VA_ARGS__)
//Client log metrics, this is what we should use in mono
#define GN_ERROR(...) ::goon::Log::GetClientLogger()->error(__VA_ARGS__)
#define GN_WARN(...) ::goon::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GN_INFO(...) ::goon::Log::GetClientLogger()->info(__VA_ARGS__)
#define GN_TRACE(...) ::goon::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GN_FATAL(...) ::goon::Log::GetClientLogger()->critical(__VA_ARGS__)