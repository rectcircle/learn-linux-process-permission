// gcc 02-set-uid-gid.c && sudo ./a.out
// before: 
// ruid: 0, euid: 0, suid: 0
// rgid: 0, egid: 0, sgid: 0
// supplementary group ids: 0

// after: 
// ruid: 1000, euid: 1000, suid: 1000
// rgid: 1000, egid: 1000, sgid: 1000
// supplementary group ids: 24, 25, 29, 30, 44, 46, 109, 112, 1000

#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <grp.h>

void printTheProccessIdentifiers() {
    gid_t supplementaryGids[32]; // sysconf(_SC_NGROUPS_MAX)
    uid_t ruid, euid, suid;
    gid_t rgid, egid, sgid;

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

int main()
{
    printf("before: \n");
    printTheProccessIdentifiers();

    // 子进程 fork 完成后，配置身份标识
    setgid(1000);
    initgroups("rectcircle", 1000); // 非 POSIX.1 标准，读取 /etc/group，并调用 setgroups 系统调用。
    setuid(1000); // uid 要最后设置，否则前面两个讲没有权限。
    printf("after: \n");
    printTheProccessIdentifiers();
    // 开始执行 exec
    // ...
    return 0;
}
