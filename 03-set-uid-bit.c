// sudo gcc 03-set-uid-bit.c
// sudo chown root:root a.out && sudo chmod u+s a.out && ./a.out
    // origin
    // ruid: 1000, euid: 0, suid: 0

    // setuid(origin ruid = 1000)
    // ruid: 1000, euid: 1000, suid: 1000

    // setuid(origin suid = 0)
    // ruid: 1000, euid: 1000, suid: 1000

// sudo chown 1001:1001 a.out && sudo chmod u+s a.out && ./a.out
    // origin
    // ruid: 1000, euid: 1001, suid: 1001

    // setuid(origin ruid = 1000)
    // ruid: 1000, euid: 1000, suid: 1001

    // setuid(origin suid = 1001)
    // ruid: 1000, euid: 1001, suid: 1001

#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <grp.h>

int main() {
    uid_t oruid, oeuid, osuid;
    getresuid(&oruid, &oeuid, &osuid); // 非 POSIX.1 标准
    uid_t ruid, euid, suid;

    printf("origin\n");
    getresuid(&ruid, &euid, &suid); // 非 POSIX.1 标准
    printf("ruid: %d, euid: %d, suid: %d\n", ruid, euid, suid);
    printf("\n");

    printf("setuid(origin ruid = %d)\n", oruid);
    setuid(oruid);
    getresuid(&ruid, &euid, &suid); // 非 POSIX.1 标准
    printf("ruid: %d, euid: %d, suid: %d\n", ruid, euid, suid);
    printf("\n");

    printf("setuid(origin suid = %d)\n", osuid);
    setuid(osuid);
    getresuid(&ruid, &euid, &suid); // 非 POSIX.1 标准
    printf("ruid: %d, euid: %d, suid: %d\n", ruid, euid, suid);
    return 0;
}
