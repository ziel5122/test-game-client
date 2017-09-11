#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string>

#ifdef WIN32

#else
  #include <netinet/in.h>
  #include <sys/socket.h>
#endif

std::chrono::high_resolution_clock::time_point t1, t2, t_diff;

double duration;

float message[] = { 1.2, 3.4, 5.6, 7.8 };

int fd;
int message_len = sizeof(message);
const unsigned int SERVER_PORT = 8892;

int main(int argc, char* argv[]) {
  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("cannot create socket");
    return 0;
  }

  struct sockaddr_in my_addr = { AF_INET, htonl(INADDR_ANY), htons(0) };

  if (bind(fd, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
    perror("bind failed");
    return 0;
  }

  struct sockaddr_in server_addr = {
    AF_INET,
    htonl(INADDR_ANY),
    htons(SERVER_PORT)
  };

  t1 = std::chrono::high_resolution_clock::now();

  while(1) {
    while(1) {
      t2 = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration<double, std::milli>(t2 - t1).count();
      if (duration > 15.625) {
        std::cout << duration << std::endl;
        sendto(fd, (const char*)message, sizeof(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        break;
      }
    }
    t1 = std::chrono::high_resolution_clock::now();
  }
}
