#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/file.h>
#include <errno.h>

int lock(char *);
void unlock(int fd);
void sighandler(int sig);
void errexit(void);

static int end  = 0;

void errexit()
{
  printf("fcntl failure errno=%d\n", errno);
  perror("fail");
  exit(EXIT_FAILURE);
}

void unlock(int fd)
{
  close(fd);
}

int lock(char *lockfile)
{
  int lockfd, res;

  if ((lockfd = open(lockfile, O_WRONLY|O_CREAT, 0600)) == -1) {
    puts("open failed");
    exit(EXIT_FAILURE);
  }

  do {
    if((res = flock(lockfd, LOCK_EX)) == -1) {
      switch(errno) {
        case EINTR:
          printf("Retry get lock.\n");
          sleep(1);
          break;
        default:
          errexit();
      }
    }
  } while(end == 0 && res == -1);

  printf("Lock success.\n");
  return (lockfd);  
}

void sighandler(int sig)
{
  switch(sig) {
    case SIGINT:
      puts("get SIGINT");
      end = 1;
      break;
    case SIGUSR1:
      puts("get SIGUSR1");
      break;
  }
}

int main(void)
{
  int fd;
  struct sigaction act, oact;
  act.sa_handler = sighandler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
#ifdef SA_INTERRUPT
  act.sa_flags |= SA_INTERRUPT;
#elif SA_RESTART
  act.sa_flags |= SA_RESTART;
#endif
  if(sigaction(SIGINT, &act, &oact) < 0) {
    errexit();
  }

  if(sigaction(SIGUSR1, &act, &oact) < 0) {
    errexit();
  }

  fd = lock("/tmp/rock.lock");
  printf("lock fd = %d\n", fd);
  while(end==0) {
    puts("main loop..");
    sleep(1);
  }
  unlock(fd);
  printf("Close fd\n");
  return 0;
}
