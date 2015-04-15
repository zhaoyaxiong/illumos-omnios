/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Copyright 2015 Joyent, Inc. All rights reserved.
 */

#ifndef _LX_BRAND_H
#define	_LX_BRAND_H

#ifndef _ASM
#include <sys/types.h>
#include <sys/cpuvar.h>
#include <sys/zone.h>
#include <sys/ksocket.h>
#endif

#ifdef	__cplusplus
extern "C" {
#endif

#define	LX_BRANDNAME	"lx"

/*
 * Brand uname info
 */
#define	LX_UNAME_SYSNAME	"Linux"
#define	LX_UNAME_RELEASE_2_6	"2.6.18"
#define	LX_UNAME_RELEASE_2_4	"2.4.21"
#define	LX_UNAME_VERSION	"BrandZ virtual linux"
#define	LX_UNAME_MACHINE32	"i686"
#define	LX_UNAME_MACHINE64	"x86_64"

#define	LX_LIB_PATH32	"/native/usr/lib/lx_brand.so.1"
#define	LX_LIB_PATH64	"/native/usr/lib/amd64/lx_brand.so.1"

#if defined(_LP64)
#define	LX_LIB_PATH		LX_LIB_PATH64
#define	LX_UNAME_MACHINE	LX_UNAME_MACHINE64
#else
#define	LX_LIB_PATH		LX_LIB_PATH32
#define	LX_UNAME_MACHINE	LX_UNAME_MACHINE32
#endif

/*
 * This must be large enough for both the 32-bit table and 64-bit table.
 */
#define	LX_NSYSCALLS		358

/*
 * brand(2) subcommands
 *
 * Everything >= 128 is a brand-specific subcommand.
 * > 192 is reserved for in-kernel emulated system calls.
 */
#define	B_LPID_TO_SPAIR		128
#define	B_GET_CURRENT_CONTEXT	129
#define	B_EMULATION_DONE	130
#define	B_PTRACE_KERNEL		131
#define	B_SET_AFFINITY_MASK	132
#define	B_GET_AFFINITY_MASK	133
#define	B_PTRACE_CLONE_BEGIN	134
#define	B_PTRACE_STOP_FOR_OPT	135
#define	B_UNSUPPORTED		136
#define	B_STORE_ARGS		137
#define	B_GETPID		138
#define	B_JUMP_TO_LINUX		139
#define	B_SET_THUNK_PID		140
#define	B_EXIT_AS_SIG		141
#define	B_HELPER_WAITID		142
#define	B_HELPER_CLONE		143
#define	B_HELPER_SETGROUPS	144
#define	B_HELPER_SIGQUEUE	145
#define	B_HELPER_TGSIGQUEUE	146
#define	B_SET_NATIVE_STACK	147

#ifndef _ASM
/*
 * Support for Linux PTRACE_SETOPTIONS handling.
 */
typedef enum lx_ptrace_options {
	LX_PTRACE_O_TRACESYSGOOD =	0x0001,
	LX_PTRACE_O_TRACEFORK =		0x0002,
	LX_PTRACE_O_TRACEVFORK =	0x0004,
	LX_PTRACE_O_TRACECLONE =	0x0008,
	LX_PTRACE_O_TRACEEXEC =		0x0010,
	LX_PTRACE_O_TRACEVFORKDONE =	0x0020,
	LX_PTRACE_O_TRACEEXIT =		0x0040,
	LX_PTRACE_O_TRACESECCOMP =	0x0080
} lx_ptrace_options_t;

#define	LX_PTRACE_O_ALL							\
	(LX_PTRACE_O_TRACESYSGOOD | LX_PTRACE_O_TRACEFORK | 		\
	LX_PTRACE_O_TRACEVFORK | LX_PTRACE_O_TRACECLONE | 		\
	LX_PTRACE_O_TRACEEXEC | LX_PTRACE_O_TRACEVFORKDONE |		\
	LX_PTRACE_O_TRACEEXIT | LX_PTRACE_O_TRACESECCOMP)
#endif /* !_ASM */

/* siginfo si_status for traced events */
#define	LX_PTRACE_EVENT_FORK		0x100
#define	LX_PTRACE_EVENT_VFORK		0x200
#define	LX_PTRACE_EVENT_CLONE		0x300
#define	LX_PTRACE_EVENT_EXEC		0x400
#define	LX_PTRACE_EVENT_VFORK_DONE	0x500
#define	LX_PTRACE_EVENT_EXIT		0x600
#define	LX_PTRACE_EVENT_SECCOMP		0x700

/*
 * Brand-private values for the "pr_what" member of lwpstatus, for use with the
 * PR_BRAND stop reason.  These reasons are validated in lx_stop_notify();
 * update it if you add new reasons here.
 */
#define	LX_PR_SYSENTRY		1
#define	LX_PR_SYSEXIT		2
#define	LX_PR_SIGNALLED		3
#define	LX_PR_EVENT		4


#define	LX_VERSION_1		1
#define	LX_VERSION		LX_VERSION_1

#define	LX_KERN_VERSION_NUM	ZONE_ATTR_BRAND_ATTRS

/*
 * Aux vector containing phdr of Linux executable and ehdr of interpreter
 * (if any), both of which are used by lx_librtld_db to ascertain r_debug.
 * We repurpose the 3rd brand-specific aux vector slot for the Linux
 * AT_SYSINFO_EHDR entry (we modify the a_type in the brand library).
 */
#define	AT_SUN_BRAND_LX_PHDR	AT_SUN_BRAND_AUX1
#define	AT_SUN_BRAND_LX_INTERP	AT_SUN_BRAND_AUX2
#define	AT_SUN_BRAND_LX_SYSINFO_EHDR	AT_SUN_BRAND_AUX3

/* Aux vector containing hz value */
#define	AT_CLKTCK	17
/* Aux vector containing vDSO addr */
#define	AT_SYSINFO_EHDR	33

/*
 * Usermode emulation routines are run on an alternate stack allocated by
 * the brand library.  Every LWP in a process will incur this overhead beyond
 * the regular thread stack:
 */
#define	LX_NATIVE_STACK_PAGE_COUNT	64

/*
 * When returning in a new child process created with vfork(2) (or CLONE_VFORK)
 * we discard some of the native stack to prevent corruption of the parent
 * emulation state.
 */
#define	LX_NATIVE_STACK_VFORK_GAP	0x3000

#ifndef	_ASM

extern struct brand lx_brand;

typedef struct lx_brand_registration {
	uint_t lxbr_version;		/* version number */
	void *lxbr_handler;		/* base address of handler */
	uint32_t lxbr_flags;		/* LX_PROC_* registration flags */
} lx_brand_registration_t;

typedef struct lx_brand_registration32 {
	uint_t lxbr_version;		/* version number */
	uint32_t lxbr_handler;		/* base address of handler */
	uint32_t lxbr_flags;		/* LX_PROC_* registration flags */
} lx_brand_registration32_t;

#ifdef __amd64

typedef struct lx_regs {
	long lxr_fs;
	long lxr_rdi;
	long lxr_rsi;
	long lxr_rbp;
	long lxr_rsp;
	long lxr_rbx;
	long lxr_rdx;
	long lxr_rcx;
	long lxr_rax;
	long lxr_r8;
	long lxr_r9;
	long lxr_r10;
	long lxr_r11;
	long lxr_r12;
	long lxr_r13;
	long lxr_r14;
	long lxr_r15;
	long lxr_rip;

	long lxr_orig_rax;
} lx_regs_t;

typedef struct lx_regs32 {
	uint32_t lxr_gs;
	uint32_t lxr_edi;
	uint32_t lxr_esi;
	uint32_t lxr_ebp;
	uint32_t lxr_esp;
	uint32_t lxr_ebx;
	uint32_t lxr_edx;
	uint32_t lxr_ecx;
	uint32_t lxr_eax;
	uint32_t lxr_eip;

	uint32_t lxr_orig_eax;
} lx_regs32_t;

#else /* ! __amd64 */

typedef struct lx_regs {
	long lxr_gs;
	long lxr_edi;
	long lxr_esi;
	long lxr_ebp;
	long lxr_esp;
	long lxr_ebx;
	long lxr_edx;
	long lxr_ecx;
	long lxr_eax;
	long lxr_eip;

	long lxr_orig_eax;
} lx_regs_t;

#endif /* __amd64 */

#ifdef __amd64
/*
 * The 64-bit native "user_regs_struct" Linux structure.
 */
typedef struct lx_user_regs {
	long lxur_r15;
	long lxur_r14;
	long lxur_r13;
	long lxur_r12;
	long lxur_rbp;
	long lxur_rbx;
	long lxur_r11;
	long lxur_r10;
	long lxur_r9;
	long lxur_r8;
	long lxur_rax;
	long lxur_rcx;
	long lxur_rdx;
	long lxur_rsi;
	long lxur_rdi;
	long lxur_orig_rax;
	long lxur_rip;
	long lxur_xcs;
	long lxur_rflags;
	long lxur_rsp;
	long lxur_xss;
	long lxur_xfs_base;
	long lxur_xgs_base;
	long lxur_xds;
	long lxur_xes;
	long lxur_xfs;
	long lxur_xgs;
} lx_user_regs_t;

#if defined(_KERNEL) && defined(_SYSCALL32_IMPL)
/*
 * 64-bit kernel view of the 32-bit "user_regs_struct" Linux structure.
 */
typedef struct lx_user_regs32 {
	int32_t lxur_ebx;
	int32_t lxur_ecx;
	int32_t lxur_edx;
	int32_t lxur_esi;
	int32_t lxur_edi;
	int32_t lxur_ebp;
	int32_t lxur_eax;
	int32_t lxur_xds;
	int32_t lxur_xes;
	int32_t lxur_xfs;
	int32_t lxur_xgs;
	int32_t lxur_orig_eax;
	int32_t lxur_eip;
	int32_t lxur_xcs;
	int32_t lxur_eflags;
	int32_t lxur_esp;
	int32_t lxur_xss;
} lx_user_regs32_t;
#endif /* defined(_KERNEL) && defined(_SYSCALL32_IMPL) */

#else /* !__amd64 */
/*
 * The 32-bit native "user_regs_struct" Linux structure.
 */
typedef struct lx_user_regs {
	long lxur_ebx;
	long lxur_ecx;
	long lxur_edx;
	long lxur_esi;
	long lxur_edi;
	long lxur_ebp;
	long lxur_eax;
	long lxur_xds;
	long lxur_xes;
	long lxur_xfs;
	long lxur_xgs;
	long lxur_orig_eax;
	long lxur_eip;
	long lxur_xcs;
	long lxur_eflags;
	long lxur_esp;
	long lxur_xss;
} lx_user_regs_t;
#endif /* __amd64 */

#endif /* _ASM */

/*
 * GDT usage
 */
#define	GDT_TLSMIN	(GDT_BRANDMIN)
#define	GDT_TLSMAX	(GDT_TLSMIN + 2)
#define	LX_TLSNUM	(GDT_TLSMAX - GDT_TLSMIN)

#ifndef _ASM

/*
 * Stores information needed by the lx linker to launch the main
 * lx executable.
 */
typedef struct lx_elf_data64 {
	uintptr_t	ed_phdr;
	uintptr_t	ed_phent;
	uintptr_t	ed_phnum;
	uintptr_t	ed_entry;
	uintptr_t	ed_base;
	uintptr_t	ed_ldentry;
} lx_elf_data64_t;

typedef struct lx_elf_data32 {
	uint32_t	ed_phdr;
	uint32_t	ed_phent;
	uint32_t	ed_phnum;
	uint32_t	ed_entry;
	uint32_t	ed_base;
	uint32_t	ed_ldentry;
} lx_elf_data32_t;

#if defined(_LP64)
typedef lx_elf_data64_t lx_elf_data_t;
#else
typedef lx_elf_data32_t lx_elf_data_t;
#endif

typedef enum lx_proc_flags {
	LX_PROC_INSTALL_MODE = 0x01,
	LX_PROC_STRICT_MODE = 0x02
} lx_proc_flags_t;

#define	LX_PROC_ALL	(LX_PROC_INSTALL_MODE | LX_PROC_STRICT_MODE)

#ifdef	_KERNEL

#define	LX_RLFAKE_LOCKS		0
#define	LX_RLFAKE_NICE		1
#define	LX_RLFAKE_RTPRIO	2
#define	LX_RLFAKE_RTTIME	3

#define	LX_RLFAKE_NLIMITS	4

#define	LX_RLIM64_INFINITY	(~0ULL)

typedef struct {
	uint64_t	rlim_cur;
	uint64_t	rlim_max;
} lx_rlimit64_t;

typedef struct lx_proc_data {
	uintptr_t l_handler;	/* address of user-space handler */
	pid_t l_ppid;		/* pid of originating parent proc */
	uint64_t l_ptrace;	/* process being observed with ptrace */
	lx_elf_data_t l_elf_data; /* ELF data for linux executable */
	int l_signal;		/* signal to deliver to parent when this */
				/* thread group dies */
	lx_proc_flags_t l_flags;

	lx_rlimit64_t l_fake_limits[LX_RLFAKE_NLIMITS];
} lx_proc_data_t;

#endif	/* _KERNEL */

/*
 * A data type big enough to bitmap all Linux possible cpus.
 * The bitmap size is defined as 1024 cpus in the Linux 2.4 and 2.6 man pages
 * for sched_getaffinity() and sched_getaffinity().
 */
#define	LX_NCPU		(1024)
#define	LX_AFF_ULONGS	(LX_NCPU / (8 * sizeof (ulong_t)))
typedef ulong_t lx_affmask_t[LX_AFF_ULONGS];

/* Max. length of kernel version string */
#define	LX_VERS_MAX	16

/*
 * Flag values for uc_brand_data[0] in the ucontext_t:
 */
#define	LX_UC_STACK_NATIVE	0x00001
#define	LX_UC_STACK_BRAND	0x00002
#define	LX_UC_RESTORE_NATIVE_SP	0x00010
#define	LX_UC_FRAME_IS_SYSCALL	0x00100
#define	LX_UC_RESTART_SYSCALL	0x01000
#define	LX_UC_IGNORE_LINK	0x10000

#ifdef	_KERNEL

typedef struct lx_lwp_data lx_lwp_data_t;

/*
 * Flag values for "lxpa_flags" on a ptrace(2) accord.
 */
typedef enum lx_accord_flags {
	LX_ACC_TOMBSTONE = 0x01
} lx_accord_flags_t;

/*
 * Flags values for "br_ptrace_flags" in the LWP-specific data.
 */
typedef enum lx_ptrace_state {
	LX_PTRACE_SYSCALL = 0x01,
	LX_PTRACE_EXITING = 0x02,
	LX_PTRACE_STOPPING = 0x04,
	LX_PTRACE_INHERIT = 0x08,
	LX_PTRACE_STOPPED = 0x10,
	LX_PTRACE_PARENT_WAIT = 0x20,
	LX_PTRACE_CLDPEND = 0x40,
	LX_PTRACE_CLONING = 0x80,
	LX_PTRACE_WAITPEND = 0x100
} lx_ptrace_state_t;

/*
 * A ptrace(2) accord represents the relationship between a tracer LWP and the
 * set of LWPs that it is tracing: the tracees.  This data structure belongs
 * primarily to the tracer, but is reference counted so that it may be freed by
 * whoever references it last.
 */
typedef struct lx_ptrace_accord {
	kmutex_t		lxpa_lock;
	uint_t			lxpa_refcnt;
	lx_accord_flags_t	lxpa_flags;

	/*
	 * The tracer must hold "pidlock" while clearing these fields for
	 * exclusion of waitid(), etc.
	 */
	lx_lwp_data_t		*lxpa_tracer;
	kcondvar_t		*lxpa_cvp;

	/*
	 * The "lxpa_tracees_lock" mutex protects the tracee list.
	 */
	kmutex_t		lxpa_tracees_lock;
	list_t			lxpa_tracees;
} lx_ptrace_accord_t;

/*
 * These values are stored in the per-LWP data for a tracee when it is attached
 * to a tracer.  They record the method that was used to attach.
 */
typedef enum lx_ptrace_attach {
	LX_PTA_NONE = 0x00,		/* not attached */
	LX_PTA_ATTACH = 0x01,		/* due to tracer using PTRACE_ATTACH */
	LX_PTA_TRACEME = 0x02,		/* due to child using PTRACE_TRACEME */
	LX_PTA_INHERIT_CLONE = 0x04,	/* due to PTRACE_CLONE clone(2) flag */
	LX_PTA_INHERIT_OPTIONS = 0x08	/* due to PTRACE_SETOPTIONS options */
} lx_ptrace_attach_t;

typedef enum lx_stack_mode {
	LX_STACK_MODE_PREINIT = 0,
	LX_STACK_MODE_INIT,
	LX_STACK_MODE_NATIVE,
	LX_STACK_MODE_BRAND
} lx_stack_mode_t;

/*
 * lx-specific data in the klwp_t
 */
struct lx_lwp_data {
	uint_t	br_lwp_flags;		/* misc. flags */
	klwp_t	*br_lwp;		/* back pointer to container lwp */
	int	br_signal;		/* signal to send to parent when */
					/* clone()'ed child terminates */
	int	br_exitwhy;		/* reason for thread (process) exit */
	int	br_exitwhat;		/* exit code / killing signal */
	lx_affmask_t br_affinitymask;	/* bitmask of CPU sched affinities */
	struct user_desc br_tls[LX_TLSNUM];
			/* descriptors used by libc for TLS */
	ulong_t	br_lx_fsbase;		/* lx fsbase for 64-bit thread ptr */
	ulong_t	br_ntv_fsbase;		/* native fsbase 64-bit thread ptr */
	pid_t	br_pid;			/* converted pid for this thread */
	pid_t	br_tgid;		/* thread group ID for this thread */
	pid_t	br_ppid;		/* parent pid for this thread */
	id_t	br_ptid;		/* parent tid for this thread */
	void	*br_clear_ctidp;	/* clone thread id ptr */
	void	*br_set_ctidp;		/* clone thread id ptr */

	/*
	 * The following struct is used by some system calls to pass extra
	 * flags into the kernel without impinging on the namespace for
	 * illumos.
	 */
	void	*br_scall_args;
	int	br_args_size; /* size in bytes of br_scall_args */

	boolean_t br_waitid_emulate;
	int br_waitid_flags;

	lx_ptrace_state_t br_ptrace_flags; /* ptrace state for this LWP */
	lx_ptrace_options_t br_ptrace_options; /* PTRACE_SETOPTIONS options */
	lx_ptrace_options_t br_ptrace_clone_option; /* current clone(2) type */

	lx_ptrace_attach_t br_ptrace_attach; /* how did we get attached */
	lx_ptrace_accord_t *br_ptrace_accord; /* accord for this tracer LWP */
	lx_ptrace_accord_t *br_ptrace_tracer; /* accord tracing this LWP */
	list_node_t br_ptrace_linkage;	/* linkage for lxpa_tracees list */

	ushort_t br_ptrace_whystop; 	/* stop reason, 0 for no stop */
	ushort_t br_ptrace_whatstop;	/* stop sub-reason */

	int32_t br_ptrace_stopsig;	/* stop signal, 0 for no signal */
	uintptr_t br_ptrace_stopucp;	/* usermode ucontext_t pointer */

	uint_t	br_ptrace_event;
	ulong_t	br_ptrace_eventmsg;

	int	br_syscall_num;		/* current system call number */
	boolean_t br_syscall_restart;	/* should restart on EINTR */

	/*
	 * Store the LX_STACK_MODE for this LWP, and the current extent of the
	 * native (emulation) stack.  This is similar, in principle, to the
	 * sigaltstack mechanism for signal handling.  We also use this mode
	 * flag to determine how to process system calls from this LWP.
	 */
	lx_stack_mode_t	br_stack_mode;
	uintptr_t br_ntv_stack;
	uintptr_t br_ntv_stack_current;

	/*
	 * If this pid is set, we return it with getpid().  This allows the
	 * thunking server to interpose on the pid returned to the Linux
	 * syslog software.
	 */
	pid_t	br_lx_thunk_pid;

	/*
	 * If strict mode is enabled (via LX_STRICT in the environment), any
	 * call to lx_unsupported() will set this boolean to B_TRUE.  This will
	 * cause us to drop SIGSYS on the LWP as it attempts to return to
	 * usermode.
	 */
	boolean_t br_strict_failure;
};

/*
 * Upper limit on br_args_size, low because this value can persist until
 * overridden with another value, and the size is given from userland.
 */
#define	LX_BR_ARGS_SIZE_MAX	(1024)

/* brand specific data */
typedef struct lx_zone_data {
	char lxzd_kernel_version[LX_VERS_MAX];
	ksocket_t lxzd_ioctl_sock;
} lx_zone_data_t;

#define	BR_CPU_BOUND	0x0001

#define	ttolxlwp(t)	((struct lx_lwp_data *)ttolwpbrand(t))
#define	lwptolxlwp(l)	((struct lx_lwp_data *)lwptolwpbrand(l))
#define	ttolxproc(t)	\
	(((t)->t_procp->p_brand == &lx_brand) ? \
	(struct lx_proc_data *)(t)->t_procp->p_brand_data : NULL)
#define	ptolxproc(p)	\
	(((p)->p_brand == &lx_brand) ? \
	(struct lx_proc_data *)(p)->p_brand_data : NULL)
#define	ztolxzd(z)		\
	(((z)->zone_brand == &lx_brand) ?  \
	(lx_zone_data_t *)(z)->zone_brand_data : NULL)

/* Macro for converting to system call arguments. */
#define	LX_ARGS(scall) ((struct lx_##scall##_args *)\
	(ttolxlwp(curthread)->br_scall_args))

/*
 * Determine the upper bound on the system call number:
 */
#if defined(_LP64)
#define	LX_MAX_SYSCALL(lwp)						\
	((lwp_getdatamodel(lwp) == DATAMODEL_NATIVE) ?			\
	    lx_nsysent64 : lx_nsysent32)
#else
#define	LX_MAX_SYSCALL(lwp)	lx_nsysent32
#endif

extern char *lx_get_zone_kern_version(zone_t *);

extern void lx_lwp_set_native_stack_current(lx_lwp_data_t *, uintptr_t);
extern void lx_divert(klwp_t *, uintptr_t);
extern int lx_runexe(klwp_t *, void *);
extern void lx_switch_to_native(klwp_t *);
extern int lx_regs_to_userregs(lx_lwp_data_t *, void *);
extern int lx_uc_to_userregs(lx_lwp_data_t *, void *, void *);
extern int lx_userregs_to_regs(lx_lwp_data_t *lwpd, void *);
extern int lx_userregs_to_uc(lx_lwp_data_t *lwpd, void *, void *);

extern int lx_syscall_enter(void);
extern int lx_syscall_return(klwp_t *, int, long);

extern void lx_trace_sysenter(int, uintptr_t *);
extern void lx_trace_sysreturn(int, long);

extern void lx_emulate_user(klwp_t *, int, uintptr_t *);
#if defined(_SYSCALL32_IMPL)
extern void lx_emulate_user32(klwp_t *, int, uintptr_t *);
#endif

extern int lx_debug;
#define	lx_print	if (lx_debug) printf

/*
 * In-Kernel Linux System Call Description.
 */
typedef struct lx_sysent {
	char	*sy_name;
	long	(*sy_callc)();
	char	sy_flags;
	char	sy_narg;
} lx_sysent_t;

#if defined(_LP64)
extern lx_sysent_t lx_sysent64[LX_NSYSCALLS + 1];
extern int lx_nsysent64;
#endif
extern lx_sysent_t lx_sysent32[LX_NSYSCALLS + 1];
extern int lx_nsysent32;

#endif	/* _KERNEL */
#endif /* _ASM */

#ifdef	__cplusplus
}
#endif

#endif	/* _LX_BRAND_H */