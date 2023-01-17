// ANSI codes on linux and macos, empty on windows
#if defined (__linux__) || defined (__APPLE__)
    #define RESET "\033[0m"
    #define RED "\033[31m"
    #define GREEN "\033[32m"
    #define YELLOW "\033[33m"
    #define BLUE "\033[34m"
#else
    #define RESET ""
    #define RED ""
    #define GREEN ""
    #define YELLOW ""
    #define BLUE ""
#endif