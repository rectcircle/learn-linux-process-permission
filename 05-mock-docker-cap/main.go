// sudo go run ./05-mock-docker-cap/
// the kernal support 41 caps: chown, dac_override, dac_read_search, fowner, fsetid, kill, setgid, setuid, setpcap, linux_immutable, net_bind_service, net_broadcast, net_admin, net_raw, ipc_lock, ipc_owner, sys_module, sys_rawio, sys_chroot, sys_ptrace, sys_pacct, sys_admin, sys_boot, sys_nice, sys_resource, sys_time, sys_tty_config, mknod, lease, audit_write, audit_control, setfcap, mac_override, mac_admin, syslog, wake_alarm, block_suspend, audit_read, perfmon, bpf, checkpoint_restore,
// the origin root proc caps:  { effective="full" permitted="full" inheritable="empty" bounding="full" }
// the root proc new caps:  { effective="chown, dac_override, fowner, fsetid, kill, setgid, setuid, setpcap, net_bind_service, net_raw, sys_chroot, mknod, audit_write, setfcap" permitted="chown, dac_override, fowner, fsetid, kill, setgid, setuid, setpcap, net_bind_service, net_raw, sys_chroot, mknod, audit_write, setfcap" inheritable="chown, dac_override, fowner, fsetid, kill, setgid, setuid, setpcap, net_bind_service, net_raw, sys_chroot, mknod, audit_write, setfcap" bounding="chown, dac_override, fowner, fsetid, kill, setgid, setuid, setpcap, net_bind_service, net_raw, sys_chroot, mknod, audit_write, setfcap" }
// the nonroot proc new caps:  { effective="empty" permitted="empty" inheritable="chown, dac_override, fowner, fsetid, kill, setgid, setuid, setpcap, net_bind_service, net_raw, sys_chroot, mknod, audit_write, setfcap" bounding="chown, dac_override, fowner, fsetid, kill, setgid, setuid, setpcap, net_bind_service, net_raw, sys_chroot, mknod, audit_write, setfcap" }
package main

import (
	"fmt"
	"syscall"

	"github.com/syndtr/gocapability/capability"
)

func main() {
	fmt.Printf("the kernal support %d caps: ", len(capability.List()))
	for _, c := range capability.List() {
		fmt.Printf("%s, ", c)
	}
	fmt.Println()

	caps, err := capability.NewPid2(0)
	if err != nil {
		panic(err)
	}
	err = caps.Load()
	if err != nil {
		panic(err)
	}
	fmt.Println("the origin root proc caps: ", caps.String())
	// 设置当前进程的 caps
	caps.Clear(capability.EFFECTIVE | capability.PERMITTED | capability.BOUNDING | capability.INHERITABLE | capability.AMBIENT)
	caps.Set(capability.EFFECTIVE|capability.PERMITTED|capability.BOUNDING|capability.INHERITABLE,
		capability.CAP_CHOWN,
		capability.CAP_DAC_OVERRIDE,
		capability.CAP_FSETID,
		capability.CAP_FOWNER,
		capability.CAP_MKNOD,
		capability.CAP_NET_RAW,
		capability.CAP_SETGID,
		capability.CAP_SETUID,
		capability.CAP_SETFCAP,
		capability.CAP_SETPCAP,
		capability.CAP_NET_BIND_SERVICE,
		capability.CAP_SYS_CHROOT,
		capability.CAP_KILL,
		capability.CAP_AUDIT_WRITE,
	)
	err = caps.Apply(capability.EFFECTIVE | capability.PERMITTED | capability.BOUNDING | capability.INHERITABLE | capability.AMBIENT)
	if err != nil {
		panic(err)
	}
	fmt.Println("the root proc new caps: ", caps.String())
	// 切换到普通用户
	syscall.Setuid(1000)
	err = caps.Load() // 重新加载
	if err != nil {
		panic(err)
	}
	fmt.Println("the nonroot proc new caps: ", caps.String())
}
