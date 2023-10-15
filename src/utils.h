#define ASSERT(x, ...)                                                                                                 \
    if (!(x))                                                                                                          \
    {                                                                                                                  \
        LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                               \
        __builtin_trap();                                                                                              \
    }


#define BIT(x) (1 << x)
