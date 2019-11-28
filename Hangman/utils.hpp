// Macros para colorir o terminal/console e limpa-lo

#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <ctime>

#include <unistd.h>

#define CRESET  std::cout << "\033[0m"

#define CPINK   std::cout << "\033[38;5;201m"
#define CBLUE   std::cout << "\033[38;5;33m"
#define CYELLOW std::cout << "\033[38;5;227m"
#define CRED    std::cout << "\033[38;5;196m"
#define CGOLD   std::cout << "\033[38;5;220m"
#define CPURPLE std::cout << "\033[38;5;99m"
#define CGREEN  std::cout << "\033[38;5;114m"

#define CLEAR   system("clear")

#define SLEEP(x) usleep((x) * 1000)


#endif /* end of include guard: _UTILS_H_ */
