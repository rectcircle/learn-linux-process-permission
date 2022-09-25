// gcc 01-get-uid-gid.c && ./a.out && sudo ./a.out
// ruid: 1000, euid: 1000, suid: 1000
// rgid: 1000, egid: 1000, sgid: 1000
// supplementary group ids: 24, 25, 29, 30, 44, 46, 109, 112, 1000

// ruid: 0, euid: 0, suid: 0
// rgid: 0, egid: 0, sgid: 0
// supplementary group ids: 0

#define _GNU_SOURCE
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

void printTheProccessIdentifiers() {
    gid_t supplementaryGids[32]; // sysconf(_SC_NGROUPS_MAX)
    uid_t ruid, euid, suid;
    gid_t rgid, egid, sgid;

    // uid_t ruid = getuid();   // 获取真实用户 ID
    // uid_t euid = geteuid();  // 获取有效用户 ID
    // gid_t rgid = getgid();   // 获取真实组 ID
    // gid_t egid = getegid();  // 获取有效组 ID
    getresuid(&ruid, &euid, &suid);                               // 非 POSIX.1 标准
    getresgid(&rgid, &egid, &sgid);                               // 非  POSIX.1 标准
    int supplementaryGidsSize = getgroups(32, supplementaryGids); // 获取附属组 ID 列表

    printf("ruid: %d, euid: %d, suid: %d\n", ruid, euid, suid);
    printf("rgid: %d, egid: %d, sgid: %d\n", rgid, egid, sgid);
    printf("supplementary group ids: ");
    for (int i = 0; i < supplementaryGidsSize; i++)
    {
        printf("%d", supplementaryGids[i]);
        if (i != supplementaryGidsSize - 1)
        {
            printf(", ");
        }
    }
    printf("\n\n");
}


int main() {
    printTheProccessIdentifiers();
    return 0;
}
