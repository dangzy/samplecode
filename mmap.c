#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void mmapcopy(int fd, int size)
{
    char *bufp;
    bufp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (bufp == (void *)-1)
        fprintf(stderr, "mmap: %s\n", strerror(errno));

    memcpy(bufp, "Linuxdd", 7);

    write(1, bufp, size);
    getchar();
    munmap(bufp, size);
    return;
}
int main(int argc, char **argv)
{
    struct stat stat;
    if (argc != 2)
    {
        printf("error.\n");
        exit(0);
    }
    int fd = open(argv[1], O_RDWR, 0);  // O_RDWR 才能被读写。
    if (fd < 0)
        fprintf(stderr, "open: %s\n", strerror(errno));  // 使用异常检查是个好习惯， 他可以帮助程序员迅速定位出错的地方！
    fstat(fd, &stat);
    mmapcopy(fd, stat.st_size);
    close(fd);
    return 0;
}
