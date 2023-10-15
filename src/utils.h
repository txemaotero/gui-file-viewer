#define ASSERT(x)                                                                                                      \
    if (!(x))                                                                                                          \
        __builtin_trap();

#define LOG_ERROR(format, ...)                                                                                         \
    {                                                                                                                  \
        fprintf(stderr, "ERROR: " format "\n", ##__VA_ARGS__);                                                         \
        ASSERT(false);                                                                                                 \
    }

#define LOG_WARN(format, ...)                                                                                          \
    {                                                                                                                  \
        fprintf(stderr, "WARN: " format "\n", ##__VA_ARGS__);                                                          \
    }

#define LOG_INFO(format, ...)                                                                                          \
    {                                                                                                                  \
        fprintf(stderr, "INFO: " format "\n", ##__VA_ARGS__);                                                          \
    }
