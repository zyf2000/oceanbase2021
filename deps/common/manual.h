#define COLOR_CODE
#ifdef COLOR_CODE
#define COLOR_RED     "\33[1;31m"
#define COLOR_GREEN   "\33[1;32m"
#define COLOR_YELLOW  "\33[1;33m"
#define COLOR_BLUE    "\33[1;34m"
#define COLOR_PURPLE  "\33[1;35m"
#define COLOR_CYAN    "\33[1;36m"
#define COLOR_WHITE   "\33[1;37m"
#define COLOR_NONE    "\33[0m"
#else
#define COLOR_RED     ""
#define COLOR_GREEN   ""
#define COLOR_YELLOW  ""
#define COLOR_BLUE    ""
#define COLOR_PURPLE  ""
#define COLOR_CYAN    ""
#define COLOR_WHITE   ""
#define COLOR_NONE    ""
#endif // COLOR_CODE
