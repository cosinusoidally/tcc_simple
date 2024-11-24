typedef unsigned int size_t;
typedef int ssize_t;
typedef long int wchar_t;
typedef int ptrdiff_t;
typedef int intptr_t;
typedef unsigned int uintptr_t;
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
void *alloca(size_t size);
union wait
  {
    int w_status;
    struct
      {
 unsigned int __w_termsig:7;
 unsigned int __w_coredump:1;
 unsigned int __w_retcode:8;
 unsigned int:16;
      } __wait_terminated;
    struct
      {
 unsigned int __w_stopval:8;
 unsigned int __w_stopsig:8;
 unsigned int:16;
      } __wait_stopped;
  };
typedef union
  {
    union wait *__uptr;
    int *__iptr;
  } __WAIT_STATUS __attribute__ ((__transparent_union__));
typedef struct
  {
    int quot;
    int rem;
  } div_t;
typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;
__extension__ typedef struct
  {
    long long int quot;
    long long int rem;
  } lldiv_t;
extern size_t __ctype_get_mb_cur_max (void) ;
extern double atof (__const char *__nptr) __attribute__ ((__pure__));
extern int atoi (__const char *__nptr) __attribute__ ((__pure__));
extern long int atol (__const char *__nptr) __attribute__ ((__pure__));
__extension__ extern long long int atoll (__const char *__nptr)
     __attribute__ ((__pure__));
extern double strtod (__const char *__restrict __nptr,
        char **__restrict __endptr) ;
extern float strtof (__const char *__restrict __nptr,
       char **__restrict __endptr) ;
extern long double strtold (__const char *__restrict __nptr,
       char **__restrict __endptr) ;
extern long int strtol (__const char *__restrict __nptr,
   char **__restrict __endptr, int __base) ;
extern unsigned long int strtoul (__const char *__restrict __nptr,
      char **__restrict __endptr, int __base)
     ;
__extension__
extern long long int strtoq (__const char *__restrict __nptr,
        char **__restrict __endptr, int __base) ;
__extension__
extern unsigned long long int strtouq (__const char *__restrict __nptr,
           char **__restrict __endptr, int __base)
     ;
__extension__
extern long long int strtoll (__const char *__restrict __nptr,
         char **__restrict __endptr, int __base) ;
__extension__
extern unsigned long long int strtoull (__const char *__restrict __nptr,
     char **__restrict __endptr, int __base)
     ;
typedef struct __locale_struct
{
  struct locale_data *__locales[13];
  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;
} *__locale_t;
extern long int __strtol_l (__const char *__restrict __nptr,
       char **__restrict __endptr, int __base,
       __locale_t __loc) ;
extern unsigned long int __strtoul_l (__const char *__restrict __nptr,
          char **__restrict __endptr,
          int __base, __locale_t __loc) ;
__extension__
extern long long int __strtoll_l (__const char *__restrict __nptr,
      char **__restrict __endptr, int __base,
      __locale_t __loc) ;
__extension__
extern unsigned long long int __strtoull_l (__const char *__restrict __nptr,
         char **__restrict __endptr,
         int __base, __locale_t __loc)
     ;
extern double __strtod_l (__const char *__restrict __nptr,
     char **__restrict __endptr, __locale_t __loc)
     ;
extern float __strtof_l (__const char *__restrict __nptr,
    char **__restrict __endptr, __locale_t __loc) ;
extern long double __strtold_l (__const char *__restrict __nptr,
    char **__restrict __endptr,
    __locale_t __loc) ;
extern double __strtod_internal (__const char *__restrict __nptr,
     char **__restrict __endptr, int __group)
     ;
extern float __strtof_internal (__const char *__restrict __nptr,
    char **__restrict __endptr, int __group)
     ;
extern long double __strtold_internal (__const char *__restrict __nptr,
           char **__restrict __endptr,
           int __group) ;
extern long int __strtol_internal (__const char *__restrict __nptr,
       char **__restrict __endptr,
       int __base, int __group) ;
extern unsigned long int __strtoul_internal (__const char *__restrict __nptr,
          char **__restrict __endptr,
          int __base, int __group) ;
__extension__
extern long long int __strtoll_internal (__const char *__restrict __nptr,
      char **__restrict __endptr,
      int __base, int __group) ;
__extension__
extern unsigned long long int __strtoull_internal (__const char *
         __restrict __nptr,
         char **__restrict __endptr,
         int __base, int __group)
     ;
extern char *l64a (long int __n) ;
extern long int a64l (__const char *__s) __attribute__ ((__pure__));
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long __u_long;
__extension__ typedef unsigned long long int __u_quad_t;
__extension__ typedef long long int __quad_t;
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;
typedef __quad_t *__qaddr_t;
typedef __u_quad_t __dev_t;
typedef __u_int __uid_t;
typedef __u_int __gid_t;
typedef __u_long __ino_t;
typedef __u_int __mode_t;
typedef __u_int __nlink_t;
typedef long int __off_t;
typedef __quad_t __loff_t;
typedef int __pid_t;
typedef int __ssize_t;
typedef __u_long __rlim_t;
typedef __u_quad_t __rlim64_t;
typedef __u_int __id_t;
typedef struct
  {
    int __val[2];
  } __fsid_t;
typedef int __daddr_t;
typedef char *__caddr_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef long int __swblk_t;
typedef long int __clock_t;
typedef int __clockid_t;
typedef int __timer_t;
typedef int __key_t;
typedef unsigned short int __ipc_pid_t;
typedef long int __blksize_t;
typedef long int __blkcnt_t;
typedef __quad_t __blkcnt64_t;
typedef __u_long __fsblkcnt_t;
typedef __u_quad_t __fsblkcnt64_t;
typedef __u_long __fsfilcnt_t;
typedef __u_quad_t __fsfilcnt64_t;
typedef __u_quad_t __ino64_t;
typedef __loff_t __off64_t;
typedef long int __t_scalar_t;
typedef unsigned long int __t_uscalar_t;
typedef int __intptr_t;
typedef unsigned int __socklen_t;
struct __sched_param
  {
    int __sched_priority;
  };
typedef int __atomic_lock_t;
struct _pthread_fastlock
{
  long int __status;
  __atomic_lock_t __spinlock;
};
typedef struct _pthread_descr_struct *_pthread_descr;
typedef struct __pthread_attr_s
{
  int __detachstate;
  int __schedpolicy;
  struct __sched_param __schedparam;
  int __inheritsched;
  int __scope;
  size_t __guardsize;
  int __stackaddr_set;
  void *__stackaddr;
  size_t __stacksize;
} pthread_attr_t;
typedef struct
{
  struct _pthread_fastlock __c_lock;
  _pthread_descr __c_waiting;
} pthread_cond_t;
typedef struct
{
  int __dummy;
} pthread_condattr_t;
typedef unsigned int pthread_key_t;
typedef struct
{
  int __m_reserved;
  int __m_count;
  _pthread_descr __m_owner;
  int __m_kind;
  struct _pthread_fastlock __m_lock;
} pthread_mutex_t;
typedef struct
{
  int __mutexkind;
} pthread_mutexattr_t;
typedef int pthread_once_t;
typedef struct _pthread_rwlock_t
{
  struct _pthread_fastlock __rw_lock;
  int __rw_readers;
  _pthread_descr __rw_writer;
  _pthread_descr __rw_read_waiting;
  _pthread_descr __rw_write_waiting;
  int __rw_kind;
  int __rw_pshared;
} pthread_rwlock_t;
typedef struct
{
  int __lockkind;
  int __pshared;
} pthread_rwlockattr_t;
typedef volatile int pthread_spinlock_t;
typedef struct {
  struct _pthread_fastlock __ba_lock;
  int __ba_required;
  int __ba_present;
  _pthread_descr __ba_waiting;
} pthread_barrier_t;
typedef struct {
  int __pshared;
} pthread_barrierattr_t;
typedef unsigned long int pthread_t;
typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;
typedef __loff_t loff_t;
typedef __ino_t ino_t;
typedef __ino64_t ino64_t;
typedef __dev_t dev_t;
typedef __gid_t gid_t;
typedef __mode_t mode_t;
typedef __nlink_t nlink_t;
typedef __uid_t uid_t;
typedef __off_t off_t;
typedef __off64_t off64_t;
typedef __pid_t pid_t;
typedef __id_t id_t;
typedef __ssize_t ssize_t;
typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;
typedef __key_t key_t;
typedef __clock_t clock_t;
typedef __time_t time_t;
typedef __clockid_t clockid_t;
typedef __timer_t timer_t;
typedef __useconds_t useconds_t;
typedef __suseconds_t suseconds_t;
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
typedef unsigned int u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int u_int64_t __attribute__ ((__mode__ (__DI__)));
typedef int register_t __attribute__ ((__mode__ (__word__)));
typedef int __sig_atomic_t;
typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
  } __sigset_t;
typedef __sigset_t sigset_t;
struct timespec
  {
    __time_t tv_sec;
    long int tv_nsec;
  };
struct timeval
  {
    __time_t tv_sec;
    __suseconds_t tv_usec;
  };
typedef long int __fd_mask;
typedef struct
  {
    __fd_mask fds_bits[1024 / (8 * sizeof (__fd_mask))];
  } fd_set;
typedef __fd_mask fd_mask;
extern int select (int __nfds, fd_set *__restrict __readfds,
     fd_set *__restrict __writefds,
     fd_set *__restrict __exceptfds,
     struct timeval *__restrict __timeout) ;
extern int pselect (int __nfds, fd_set *__restrict __readfds,
      fd_set *__restrict __writefds,
      fd_set *__restrict __exceptfds,
      const struct timespec *__restrict __timeout,
      const __sigset_t *__restrict __sigmask) ;
typedef __blksize_t blksize_t;
typedef __blkcnt_t blkcnt_t;
typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;
typedef __blkcnt64_t blkcnt64_t;
typedef __fsblkcnt64_t fsblkcnt64_t;
typedef __fsfilcnt64_t fsfilcnt64_t;
extern long int random (void) ;
extern void srandom (unsigned int __seed) ;
extern char *initstate (unsigned int __seed, char *__statebuf,
   size_t __statelen) ;
extern char *setstate (char *__statebuf) ;
struct random_data
  {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
  };
extern int random_r (struct random_data *__restrict __buf,
       int32_t *__restrict __result) ;
extern int srandom_r (unsigned int __seed, struct random_data *__buf) ;
extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
   size_t __statelen,
   struct random_data *__restrict __buf) ;
extern int setstate_r (char *__restrict __statebuf,
         struct random_data *__restrict __buf) ;
extern int rand (void) ;
extern void srand (unsigned int __seed) ;
extern int rand_r (unsigned int *__seed) ;
extern double drand48 (void) ;
extern double erand48 (unsigned short int __xsubi[3]) ;
extern long int lrand48 (void) ;
extern long int nrand48 (unsigned short int __xsubi[3]) ;
extern long int mrand48 (void) ;
extern long int jrand48 (unsigned short int __xsubi[3]) ;
extern void srand48 (long int __seedval) ;
extern unsigned short int *seed48 (unsigned short int __seed16v[3]) ;
extern void lcong48 (unsigned short int __param[7]) ;
struct drand48_data
  {
    unsigned short int __x[3];
    unsigned short int __old_x[3];
    unsigned short int __c;
    unsigned short int __init;
    unsigned long long int __a;
  };
extern int drand48_r (struct drand48_data *__restrict __buffer,
        double *__restrict __result) ;
extern int erand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        double *__restrict __result) ;
extern int lrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result) ;
extern int nrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result) ;
extern int mrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result) ;
extern int jrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result) ;
extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
     ;
extern int seed48_r (unsigned short int __seed16v[3],
       struct drand48_data *__buffer) ;
extern int lcong48_r (unsigned short int __param[7],
        struct drand48_data *__buffer) ;
extern void *malloc (size_t __size) __attribute__ ((__malloc__));
extern void *calloc (size_t __nmemb, size_t __size)
     __attribute__ ((__malloc__));
extern void *realloc (void *__ptr, size_t __size) __attribute__ ((__malloc__));
extern void free (void *__ptr) ;
extern void cfree (void *__ptr) ;
extern void *alloca (size_t __size) ;
extern void *valloc (size_t __size) __attribute__ ((__malloc__));
extern int posix_memalign (void **__memptr, size_t __alignment, size_t __size)
     __attribute__ ((__malloc__));
extern void abort (void) __attribute__ ((__noreturn__));
extern int atexit (void (*__func) (void)) ;
extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
     ;
extern void exit (int __status) __attribute__ ((__noreturn__));
extern void _Exit (int __status) __attribute__ ((__noreturn__));
extern char *getenv (__const char *__name) ;
extern char *__secure_getenv (__const char *__name) ;
extern int putenv (char *__string) ;
extern int setenv (__const char *__name, __const char *__value, int __replace)
     ;
extern int unsetenv (__const char *__name) ;
extern int clearenv (void) ;
extern char *mktemp (char *__template) ;
extern int mkstemp (char *__template) ;
extern int mkstemp64 (char *__template) ;
extern char *mkdtemp (char *__template) ;
extern int system (__const char *__command) ;
extern char *canonicalize_file_name (__const char *__name) ;
extern char *realpath (__const char *__restrict __name,
         char *__restrict __resolved) ;
typedef int (*__compar_fn_t) (__const void *, __const void *);
typedef __compar_fn_t comparison_fn_t;
extern void *bsearch (__const void *__key, __const void *__base,
        size_t __nmemb, size_t __size, __compar_fn_t __compar);
extern void qsort (void *__base, size_t __nmemb, size_t __size,
     __compar_fn_t __compar);
extern int abs (int __x) __attribute__ ((__const__));
extern long int labs (long int __x) __attribute__ ((__const__));
__extension__ extern long long int llabs (long long int __x)
     __attribute__ ((__const__));
extern div_t div (int __numer, int __denom)
     __attribute__ ((__const__));
extern ldiv_t ldiv (long int __numer, long int __denom)
     __attribute__ ((__const__));
__extension__ extern lldiv_t lldiv (long long int __numer,
        long long int __denom)
     __attribute__ ((__const__));
extern char *ecvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) ;
extern char *fcvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) ;
extern char *gcvt (double __value, int __ndigit, char *__buf) ;
extern char *qecvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign) ;
extern char *qfcvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign) ;
extern char *qgcvt (long double __value, int __ndigit, char *__buf) ;
extern int ecvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) ;
extern int fcvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) ;
extern int qecvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len) ;
extern int qfcvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len) ;
extern int mblen (__const char *__s, size_t __n) ;
extern int mbtowc (wchar_t *__restrict __pwc,
     __const char *__restrict __s, size_t __n) ;
extern int wctomb (char *__s, wchar_t __wchar) ;
extern size_t mbstowcs (wchar_t *__restrict __pwcs,
   __const char *__restrict __s, size_t __n) ;
extern size_t wcstombs (char *__restrict __s,
   __const wchar_t *__restrict __pwcs, size_t __n)
     ;
extern int rpmatch (__const char *__response) ;
extern int getsubopt (char **__restrict __optionp,
        char *__const *__restrict __tokens,
        char **__restrict __valuep) ;
extern void setkey (__const char *__key) ;
extern int posix_openpt (int __oflag) ;
extern int grantpt (int __fd) ;
extern int unlockpt (int __fd) ;
extern char *ptsname (int __fd) ;
extern int ptsname_r (int __fd, char *__buf, size_t __buflen) ;
extern int getpt (void) ;
extern int getloadavg (double __loadavg[], int __nelem) ;
typedef struct _IO_FILE FILE;
typedef struct _IO_FILE __FILE;
typedef unsigned int wint_t;
typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;
typedef struct
{
  __off_t __pos;
  __mbstate_t __state;
} _G_fpos_t;
typedef struct
{
  __off64_t __pos;
  __mbstate_t __state;
} _G_fpos64_t;
enum
{
  __GCONV_OK = 0,
  __GCONV_NOCONV,
  __GCONV_NODB,
  __GCONV_NOMEM,
  __GCONV_EMPTY_INPUT,
  __GCONV_FULL_OUTPUT,
  __GCONV_ILLEGAL_INPUT,
  __GCONV_INCOMPLETE_INPUT,
  __GCONV_ILLEGAL_DESCRIPTOR,
  __GCONV_INTERNAL_ERROR
};
enum
{
  __GCONV_IS_LAST = 0x0001,
  __GCONV_IGNORE_ERRORS = 0x0002
};
struct __gconv_step;
struct __gconv_step_data;
struct __gconv_loaded_object;
struct __gconv_trans_data;
typedef int (*__gconv_fct) (struct __gconv_step *, struct __gconv_step_data *,
       __const unsigned char **, __const unsigned char *,
       unsigned char **, size_t *, int, int);
typedef int (*__gconv_init_fct) (struct __gconv_step *);
typedef void (*__gconv_end_fct) (struct __gconv_step *);
typedef int (*__gconv_trans_fct) (struct __gconv_step *,
      struct __gconv_step_data *, void *,
      __const unsigned char *,
      __const unsigned char **,
      __const unsigned char *, unsigned char **,
      size_t *);
typedef int (*__gconv_trans_context_fct) (void *, __const unsigned char *,
       __const unsigned char *,
       unsigned char *, unsigned char *);
typedef int (*__gconv_trans_query_fct) (__const char *, __const char ***,
     size_t *);
typedef int (*__gconv_trans_init_fct) (void **, const char *);
typedef void (*__gconv_trans_end_fct) (void *);
struct __gconv_trans_data
{
  __gconv_trans_fct __trans_fct;
  __gconv_trans_context_fct __trans_context_fct;
  __gconv_trans_end_fct __trans_end_fct;
  void *__data;
  struct __gconv_trans_data *__next;
};
struct __gconv_step
{
  struct __gconv_loaded_object *__shlib_handle;
  __const char *__modname;
  int __counter;
  char *__from_name;
  char *__to_name;
  __gconv_fct __fct;
  __gconv_init_fct __init_fct;
  __gconv_end_fct __end_fct;
  int __min_needed_from;
  int __max_needed_from;
  int __min_needed_to;
  int __max_needed_to;
  int __stateful;
  void *__data;
};
struct __gconv_step_data
{
  unsigned char *__outbuf;
  unsigned char *__outbufend;
  int __flags;
  int __invocation_counter;
  int __internal_use;
  __mbstate_t *__statep;
  __mbstate_t __state;
  struct __gconv_trans_data *__trans;
};
typedef struct __gconv_info
{
  size_t __nsteps;
  struct __gconv_step *__steps;
  __extension__ struct __gconv_step_data __data [];
} *__gconv_t;
typedef union
{
  struct __gconv_info __cd;
  struct
  {
    struct __gconv_info __cd;
    struct __gconv_step_data __data;
  } __combined;
} _G_iconv_t;
typedef int _G_int16_t __attribute__ ((__mode__ (__HI__)));
typedef int _G_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int _G_uint16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int _G_uint32_t __attribute__ ((__mode__ (__SI__)));
typedef char *va_list;
typedef va_list __gnuc_va_list;
struct _IO_jump_t; struct _IO_FILE;
typedef void _IO_lock_t;
struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;
  int _pos;
};
enum __codecvt_result
{
  __codecvt_ok,
  __codecvt_partial,
  __codecvt_error,
  __codecvt_noconv
};
struct _IO_FILE {
  int _flags;
  char* _IO_read_ptr;
  char* _IO_read_end;
  char* _IO_read_base;
  char* _IO_write_base;
  char* _IO_write_ptr;
  char* _IO_write_end;
  char* _IO_buf_base;
  char* _IO_buf_end;
  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;
  struct _IO_marker *_markers;
  struct _IO_FILE *_chain;
  int _fileno;
  int _blksize;
  __off_t _old_offset;
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];
  _IO_lock_t *_lock;
  __off64_t _offset;
  void *__pad1;
  void *__pad2;
  int _mode;
  char _unused2[15 * sizeof (int) - 2 * sizeof (void *)];
};
typedef struct _IO_FILE _IO_FILE;
struct _IO_FILE_plus;
extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
typedef __ssize_t __io_read_fn (void *__cookie, char *__buf, size_t __nbytes);
typedef __ssize_t __io_write_fn (void *__cookie, __const char *__buf,
     size_t __n);
typedef int __io_seek_fn (void *__cookie, __off64_t *__pos, int __w);
typedef int __io_close_fn (void *__cookie);
typedef __io_read_fn cookie_read_function_t;
typedef __io_write_fn cookie_write_function_t;
typedef __io_seek_fn cookie_seek_function_t;
typedef __io_close_fn cookie_close_function_t;
typedef struct
{
  __io_read_fn *read;
  __io_write_fn *write;
  __io_seek_fn *seek;
  __io_close_fn *close;
} _IO_cookie_io_functions_t;
typedef _IO_cookie_io_functions_t cookie_io_functions_t;
struct _IO_cookie_file;
extern void _IO_cookie_init (struct _IO_cookie_file *__cfile, int __read_write,
        void *__cookie, _IO_cookie_io_functions_t __fns);
extern int __underflow (_IO_FILE *) ;
extern int __uflow (_IO_FILE *) ;
extern int __overflow (_IO_FILE *, int) ;
extern wint_t __wunderflow (_IO_FILE *) ;
extern wint_t __wuflow (_IO_FILE *) ;
extern wint_t __woverflow (_IO_FILE *, wint_t) ;
extern int _IO_getc (_IO_FILE *__fp) ;
extern int _IO_putc (int __c, _IO_FILE *__fp) ;
extern int _IO_feof (_IO_FILE *__fp) ;
extern int _IO_ferror (_IO_FILE *__fp) ;
extern int _IO_peekc_locked (_IO_FILE *__fp) ;
extern void _IO_flockfile (_IO_FILE *) ;
extern void _IO_funlockfile (_IO_FILE *) ;
extern int _IO_ftrylockfile (_IO_FILE *) ;
extern int _IO_vfscanf (_IO_FILE * __restrict, const char * __restrict,
   __gnuc_va_list, int *__restrict) ;
extern int _IO_vfprintf (_IO_FILE *__restrict, const char *__restrict,
    __gnuc_va_list) ;
extern __ssize_t _IO_padn (_IO_FILE *, int, __ssize_t) ;
extern size_t _IO_sgetn (_IO_FILE *, void *, size_t) ;
extern __off64_t _IO_seekoff (_IO_FILE *, __off64_t, int, int) ;
extern __off64_t _IO_seekpos (_IO_FILE *, __off64_t, int) ;
extern void _IO_free_backup_area (_IO_FILE *) ;
typedef _G_fpos_t fpos_t;
typedef _G_fpos64_t fpos64_t;
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
extern int remove (__const char *__filename) ;
extern int rename (__const char *__old, __const char *__new) ;
extern FILE *tmpfile (void) ;
extern FILE *tmpfile64 (void) ;
extern char *tmpnam (char *__s) ;
extern char *tmpnam_r (char *__s) ;
extern char *tempnam (__const char *__dir, __const char *__pfx)
     __attribute__ ((__malloc__));
extern int fclose (FILE *__stream) ;
extern int fflush (FILE *__stream) ;
extern int fflush_unlocked (FILE *__stream) ;
extern int fcloseall (void) ;
extern FILE *fopen (__const char *__restrict __filename,
      __const char *__restrict __modes) ;
extern FILE *freopen (__const char *__restrict __filename,
        __const char *__restrict __modes,
        FILE *__restrict __stream) ;
extern FILE *fopen64 (__const char *__restrict __filename,
        __const char *__restrict __modes) ;
extern FILE *freopen64 (__const char *__restrict __filename,
   __const char *__restrict __modes,
   FILE *__restrict __stream) ;
extern FILE *fdopen (int __fd, __const char *__modes) ;
extern FILE *fopencookie (void *__restrict __magic_cookie,
     __const char *__restrict __modes,
     _IO_cookie_io_functions_t __io_funcs) ;
extern FILE *fmemopen (void *__s, size_t __len, __const char *__modes) ;
extern FILE *open_memstream (char **__restrict __bufloc,
        size_t *__restrict __sizeloc) ;
extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) ;
extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
      int __modes, size_t __n) ;
extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
         size_t __size) ;
extern void setlinebuf (FILE *__stream) ;
extern int fprintf (FILE *__restrict __stream,
      __const char *__restrict __format, ...) ;
extern int printf (__const char *__restrict __format, ...) ;
extern int sprintf (char *__restrict __s,
      __const char *__restrict __format, ...) ;
extern int vfprintf (FILE *__restrict __s, __const char *__restrict __format,
       __gnuc_va_list __arg) ;
extern int vprintf (__const char *__restrict __format, __gnuc_va_list __arg)
     ;
extern int vsprintf (char *__restrict __s, __const char *__restrict __format,
       __gnuc_va_list __arg) ;
extern int snprintf (char *__restrict __s, size_t __maxlen,
       __const char *__restrict __format, ...)
     __attribute__ ((__format__ (__printf__, 3, 4)));
extern int vsnprintf (char *__restrict __s, size_t __maxlen,
        __const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 3, 0)));
extern int vasprintf (char **__restrict __ptr, __const char *__restrict __f,
        __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 2, 0)));
extern int __asprintf (char **__restrict __ptr,
         __const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));
extern int asprintf (char **__restrict __ptr,
       __const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));
extern int vdprintf (int __fd, __const char *__restrict __fmt,
       __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 2, 0)));
extern int dprintf (int __fd, __const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));
extern int fscanf (FILE *__restrict __stream,
     __const char *__restrict __format, ...) ;
extern int scanf (__const char *__restrict __format, ...) ;
extern int sscanf (__const char *__restrict __s,
     __const char *__restrict __format, ...) ;
extern int vfscanf (FILE *__restrict __s, __const char *__restrict __format,
      __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 2, 0)));
extern int vscanf (__const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 1, 0)));
extern int vsscanf (__const char *__restrict __s,
      __const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 2, 0)));
extern int fgetc (FILE *__stream) ;
extern int getc (FILE *__stream) ;
extern int getchar (void) ;
extern int getc_unlocked (FILE *__stream) ;
extern int getchar_unlocked (void) ;
extern int fgetc_unlocked (FILE *__stream) ;
extern int fputc (int __c, FILE *__stream) ;
extern int putc (int __c, FILE *__stream) ;
extern int putchar (int __c) ;
extern int fputc_unlocked (int __c, FILE *__stream) ;
extern int putc_unlocked (int __c, FILE *__stream) ;
extern int putchar_unlocked (int __c) ;
extern int getw (FILE *__stream) ;
extern int putw (int __w, FILE *__stream) ;
extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     ;
extern char *fgets_unlocked (char *__restrict __s, int __n,
        FILE *__restrict __stream) ;
extern char *gets (char *__s) ;
extern __ssize_t __getdelim (char **__restrict __lineptr,
          size_t *__restrict __n, int __delimiter,
          FILE *__restrict __stream) ;
extern __ssize_t getdelim (char **__restrict __lineptr,
        size_t *__restrict __n, int __delimiter,
        FILE *__restrict __stream) ;
extern __ssize_t getline (char **__restrict __lineptr,
       size_t *__restrict __n,
       FILE *__restrict __stream) ;
extern int fputs (__const char *__restrict __s, FILE *__restrict __stream)
     ;
extern int fputs_unlocked (__const char *__restrict __s,
      FILE *__restrict __stream) ;
extern int puts (__const char *__s) ;
extern int ungetc (int __c, FILE *__stream) ;
extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream) ;
extern size_t fwrite (__const void *__restrict __ptr, size_t __size,
        size_t __n, FILE *__restrict __s) ;
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
         size_t __n, FILE *__restrict __stream) ;
extern size_t fwrite_unlocked (__const void *__restrict __ptr, size_t __size,
          size_t __n, FILE *__restrict __stream) ;
extern int fseek (FILE *__stream, long int __off, int __whence) ;
extern long int ftell (FILE *__stream) ;
extern void rewind (FILE *__stream) ;
extern int fseeko (FILE *__stream, __off_t __off, int __whence) ;
extern __off_t ftello (FILE *__stream) ;
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos)
     ;
extern int fsetpos (FILE *__stream, __const fpos_t *__pos) ;
extern int fseeko64 (FILE *__stream, __off64_t __off, int __whence) ;
extern __off64_t ftello64 (FILE *__stream) ;
extern int fgetpos64 (FILE *__restrict __stream, fpos64_t *__restrict __pos)
     ;
extern int fsetpos64 (FILE *__stream, __const fpos64_t *__pos) ;
extern void clearerr (FILE *__stream) ;
extern int feof (FILE *__stream) ;
extern int ferror (FILE *__stream) ;
extern void clearerr_unlocked (FILE *__stream) ;
extern int feof_unlocked (FILE *__stream) ;
extern int ferror_unlocked (FILE *__stream) ;
extern void perror (__const char *__s) ;
extern int sys_nerr;
extern __const char *__const sys_errlist[];
extern int _sys_nerr;
extern __const char *__const _sys_errlist[];
extern int fileno (FILE *__stream) ;
extern int fileno_unlocked (FILE *__stream) ;
extern FILE *popen (__const char *__command, __const char *__modes) ;
extern int pclose (FILE *__stream) ;
extern char *ctermid (char *__s) ;
extern char *cuserid (char *__s) ;
struct obstack;
extern int obstack_printf (struct obstack *__restrict __obstack,
      __const char *__restrict __format, ...) ;
extern int obstack_vprintf (struct obstack *__restrict __obstack,
       __const char *__restrict __format,
       __gnuc_va_list __args) ;
extern void flockfile (FILE *__stream) ;
extern int ftrylockfile (FILE *__stream) ;
extern void funlockfile (FILE *__stream) ;
extern void *memcpy (void *__restrict __dest,
       __const void *__restrict __src, size_t __n) ;
extern void *memmove (void *__dest, __const void *__src, size_t __n)
     ;
extern void *memccpy (void *__restrict __dest, __const void *__restrict __src,
        int __c, size_t __n)
     ;
extern void *memset (void *__s, int __c, size_t __n) ;
extern int memcmp (__const void *__s1, __const void *__s2, size_t __n)
     __attribute__ ((__pure__));
extern void *memchr (__const void *__s, int __c, size_t __n)
      __attribute__ ((__pure__));
extern void *rawmemchr (__const void *__s, int __c) __attribute__ ((__pure__));
extern void *memrchr (__const void *__s, int __c, size_t __n)
      __attribute__ ((__pure__));
extern char *strcpy (char *__restrict __dest, __const char *__restrict __src)
     ;
extern char *strncpy (char *__restrict __dest,
        __const char *__restrict __src, size_t __n) ;
extern char *strcat (char *__restrict __dest, __const char *__restrict __src)
     ;
extern char *strncat (char *__restrict __dest, __const char *__restrict __src,
        size_t __n) ;
extern int strcmp (__const char *__s1, __const char *__s2)
     __attribute__ ((__pure__));
extern int strncmp (__const char *__s1, __const char *__s2, size_t __n)
     __attribute__ ((__pure__));
extern int strcoll (__const char *__s1, __const char *__s2)
     __attribute__ ((__pure__));
extern size_t strxfrm (char *__restrict __dest,
         __const char *__restrict __src, size_t __n) ;
extern int __strcoll_l (__const char *__s1, __const char *__s2, __locale_t __l)
     __attribute__ ((__pure__));
extern size_t __strxfrm_l (char *__dest, __const char *__src, size_t __n,
      __locale_t __l) ;
extern char *strdup (__const char *__s) __attribute__ ((__malloc__));
extern char *strndup (__const char *__string, size_t __n)
     __attribute__ ((__malloc__));
extern char *strchr (__const char *__s, int __c) __attribute__ ((__pure__));
extern char *strrchr (__const char *__s, int __c) __attribute__ ((__pure__));
extern char *strchrnul (__const char *__s, int __c) __attribute__ ((__pure__));
extern size_t strcspn (__const char *__s, __const char *__reject)
     __attribute__ ((__pure__));
extern size_t strspn (__const char *__s, __const char *__accept)
     __attribute__ ((__pure__));
extern char *strpbrk (__const char *__s, __const char *__accept)
     __attribute__ ((__pure__));
extern char *strstr (__const char *__haystack, __const char *__needle)
     __attribute__ ((__pure__));
extern char *strcasestr (__const char *__haystack, __const char *__needle)
     __attribute__ ((__pure__));
extern char *strtok (char *__restrict __s, __const char *__restrict __delim)
     ;
extern char *__strtok_r (char *__restrict __s,
    __const char *__restrict __delim,
    char **__restrict __save_ptr) ;
extern char *strtok_r (char *__restrict __s, __const char *__restrict __delim,
         char **__restrict __save_ptr) ;
extern void *memmem (__const void *__haystack, size_t __haystacklen,
       __const void *__needle, size_t __needlelen)
     __attribute__ ((__pure__));
extern void *__mempcpy (void *__restrict __dest,
   __const void *__restrict __src, size_t __n) ;
extern void *mempcpy (void *__restrict __dest,
        __const void *__restrict __src, size_t __n) ;
extern size_t strlen (__const char *__s) __attribute__ ((__pure__));
extern size_t strnlen (__const char *__string, size_t __maxlen)
     __attribute__ ((__pure__));
extern char *strerror (int __errnum) ;
extern char *strerror_r (int __errnum, char *__buf, size_t __buflen) ;
extern void __bzero (void *__s, size_t __n) ;
extern void bcopy (__const void *__src, void *__dest, size_t __n) ;
extern void bzero (void *__s, size_t __n) ;
extern int bcmp (__const void *__s1, __const void *__s2, size_t __n)
     __attribute__ ((__pure__));
extern char *index (__const char *__s, int __c) __attribute__ ((__pure__));
extern char *rindex (__const char *__s, int __c) __attribute__ ((__pure__));
extern int ffs (int __i) __attribute__ ((__const__));
extern int ffsl (long int __l) __attribute__ ((__const__));
__extension__ extern int ffsll (long long int __ll)
     __attribute__ ((__const__));
extern int strcasecmp (__const char *__s1, __const char *__s2)
     __attribute__ ((__pure__));
extern int strncasecmp (__const char *__s1, __const char *__s2, size_t __n)
     __attribute__ ((__pure__));
extern int __strcasecmp_l (__const char *__s1, __const char *__s2,
      __locale_t __loc) __attribute__ ((__pure__));
extern int __strncasecmp_l (__const char *__s1, __const char *__s2,
       size_t __n, __locale_t __loc)
     __attribute__ ((__pure__));
extern char *strsep (char **__restrict __stringp,
       __const char *__restrict __delim) ;
extern int strverscmp (__const char *__s1, __const char *__s2)
     __attribute__ ((__pure__));
extern char *strsignal (int __sig) ;
extern char *__stpcpy (char *__restrict __dest, __const char *__restrict __src)
     ;
extern char *stpcpy (char *__restrict __dest, __const char *__restrict __src)
     ;
extern char *__stpncpy (char *__restrict __dest,
   __const char *__restrict __src, size_t __n) ;
extern char *stpncpy (char *__restrict __dest,
        __const char *__restrict __src, size_t __n) ;
extern char *strfry (char *__string) ;
extern void *memfrob (void *__s, size_t __n) ;
extern char *basename (__const char *__filename) ;
extern int errno;
extern int *__errno_location (void) __attribute__ ((__const__));
extern char *program_invocation_name, *program_invocation_short_name;
typedef int error_t;
typedef long double float_t;
typedef long double double_t;
extern double acos (double __x) ; extern double __acos (double __x) ;
extern double asin (double __x) ; extern double __asin (double __x) ;
extern double atan (double __x) ; extern double __atan (double __x) ;
extern double atan2 (double __y, double __x) ; extern double __atan2 (double __y, double __x) ;
extern double cos (double __x) ; extern double __cos (double __x) ;
extern double sin (double __x) ; extern double __sin (double __x) ;
extern double tan (double __x) ; extern double __tan (double __x) ;
extern void sincos (double __x, double *__sinx, double *__cosx) ; extern void __sincos (double __x, double *__sinx, double *__cosx)
                                                           ;
extern double cosh (double __x) ; extern double __cosh (double __x) ;
extern double sinh (double __x) ; extern double __sinh (double __x) ;
extern double tanh (double __x) ; extern double __tanh (double __x) ;
extern double acosh (double __x) ; extern double __acosh (double __x) ;
extern double asinh (double __x) ; extern double __asinh (double __x) ;
extern double atanh (double __x) ; extern double __atanh (double __x) ;
extern double exp (double __x) ; extern double __exp (double __x) ;
extern double exp10 (double __x) ; extern double __exp10 (double __x) ;
extern double pow10 (double __x) ; extern double __pow10 (double __x) ;
extern double frexp (double __x, int *__exponent) ; extern double __frexp (double __x, int *__exponent) ;
extern double ldexp (double __x, int __exponent) ; extern double __ldexp (double __x, int __exponent) ;
extern double log (double __x) ; extern double __log (double __x) ;
extern double log10 (double __x) ; extern double __log10 (double __x) ;
extern double modf (double __x, double *__iptr) ; extern double __modf (double __x, double *__iptr) ;
extern double expm1 (double __x) ; extern double __expm1 (double __x) ;
extern double log1p (double __x) ; extern double __log1p (double __x) ;
extern double logb (double __x) ; extern double __logb (double __x) ;
extern double exp2 (double __x) ; extern double __exp2 (double __x) ;
extern double log2 (double __x) ; extern double __log2 (double __x) ;
extern double pow (double __x, double __y) ; extern double __pow (double __x, double __y) ;
extern double sqrt (double __x) ; extern double __sqrt (double __x) ;
extern double hypot (double __x, double __y) ; extern double __hypot (double __x, double __y) ;
extern double cbrt (double __x) ; extern double __cbrt (double __x) ;
extern double ceil (double __x) ; extern double __ceil (double __x) ;
extern double fabs (double __x) __attribute__ ((__const__)); extern double __fabs (double __x) __attribute__ ((__const__));
extern double floor (double __x) ; extern double __floor (double __x) ;
extern double fmod (double __x, double __y) ; extern double __fmod (double __x, double __y) ;
extern int __isinf (double __value) __attribute__ ((__const__));
extern int __finite (double __value) __attribute__ ((__const__));
extern int isinf (double __value) __attribute__ ((__const__));
extern int finite (double __value) __attribute__ ((__const__));
extern double drem (double __x, double __y) ; extern double __drem (double __x, double __y) ;
extern double significand (double __x) ; extern double __significand (double __x) ;
extern double copysign (double __x, double __y) __attribute__ ((__const__)); extern double __copysign (double __x, double __y) __attribute__ ((__const__));
extern double nan (__const char *__tagb) __attribute__ ((__const__)); extern double __nan (__const char *__tagb) __attribute__ ((__const__));
extern int __isnan (double __value) __attribute__ ((__const__));
extern int isnan (double __value) __attribute__ ((__const__));
extern double j0 (double) ; extern double __j0 (double) ;
extern double j1 (double) ; extern double __j1 (double) ;
extern double jn (int, double) ; extern double __jn (int, double) ;
extern double y0 (double) ; extern double __y0 (double) ;
extern double y1 (double) ; extern double __y1 (double) ;
extern double yn (int, double) ; extern double __yn (int, double) ;
extern double erf (double) ; extern double __erf (double) ;
extern double erfc (double) ; extern double __erfc (double) ;
extern double lgamma (double) ; extern double __lgamma (double) ;
extern double tgamma (double) ; extern double __tgamma (double) ;
extern double gamma (double) ; extern double __gamma (double) ;
extern double lgamma_r (double, int *__signgamp) ; extern double __lgamma_r (double, int *__signgamp) ;
extern double rint (double __x) ; extern double __rint (double __x) ;
extern double nextafter (double __x, double __y) __attribute__ ((__const__)); extern double __nextafter (double __x, double __y) __attribute__ ((__const__));
extern double nexttoward (double __x, long double __y) __attribute__ ((__const__)); extern double __nexttoward (double __x, long double __y) __attribute__ ((__const__));
extern double remainder (double __x, double __y) ; extern double __remainder (double __x, double __y) ;
extern double scalb (double __x, double __n) ; extern double __scalb (double __x, double __n) ;
extern double scalbn (double __x, int __n) ; extern double __scalbn (double __x, int __n) ;
extern int ilogb (double __x) ; extern int __ilogb (double __x) ;
extern double scalbln (double __x, long int __n) ; extern double __scalbln (double __x, long int __n) ;
extern double nearbyint (double __x) ; extern double __nearbyint (double __x) ;
extern double round (double __x) ; extern double __round (double __x) ;
extern double trunc (double __x) __attribute__ ((__const__)); extern double __trunc (double __x) __attribute__ ((__const__));
extern double remquo (double __x, double __y, int *__quo) ; extern double __remquo (double __x, double __y, int *__quo) ;
extern long int lrint (double __x) ; extern long int __lrint (double __x) ;
extern long long int llrint (double __x) ; extern long long int __llrint (double __x) ;
extern long int lround (double __x) ; extern long int __lround (double __x) ;
extern long long int llround (double __x) ; extern long long int __llround (double __x) ;
extern double fdim (double __x, double __y) ; extern double __fdim (double __x, double __y) ;
extern double fmax (double __x, double __y) ; extern double __fmax (double __x, double __y) ;
extern double fmin (double __x, double __y) ; extern double __fmin (double __x, double __y) ;
extern int __fpclassify (double __value)
     __attribute__ ((__const__));
extern int __signbit (double __value)
     __attribute__ ((__const__));
extern double fma (double __x, double __y, double __z) ; extern double __fma (double __x, double __y, double __z) ;
extern float acosf (float __x) ; extern float __acosf (float __x) ;
extern float asinf (float __x) ; extern float __asinf (float __x) ;
extern float atanf (float __x) ; extern float __atanf (float __x) ;
extern float atan2f (float __y, float __x) ; extern float __atan2f (float __y, float __x) ;
extern float cosf (float __x) ; extern float __cosf (float __x) ;
extern float sinf (float __x) ; extern float __sinf (float __x) ;
extern float tanf (float __x) ; extern float __tanf (float __x) ;
extern void sincosf (float __x, float *__sinx, float *__cosx) ; extern void __sincosf (float __x, float *__sinx, float *__cosx)
                                                           ;
extern float coshf (float __x) ; extern float __coshf (float __x) ;
extern float sinhf (float __x) ; extern float __sinhf (float __x) ;
extern float tanhf (float __x) ; extern float __tanhf (float __x) ;
extern float acoshf (float __x) ; extern float __acoshf (float __x) ;
extern float asinhf (float __x) ; extern float __asinhf (float __x) ;
extern float atanhf (float __x) ; extern float __atanhf (float __x) ;
extern float expf (float __x) ; extern float __expf (float __x) ;
extern float exp10f (float __x) ; extern float __exp10f (float __x) ;
extern float pow10f (float __x) ; extern float __pow10f (float __x) ;
extern float frexpf (float __x, int *__exponent) ; extern float __frexpf (float __x, int *__exponent) ;
extern float ldexpf (float __x, int __exponent) ; extern float __ldexpf (float __x, int __exponent) ;
extern float logf (float __x) ; extern float __logf (float __x) ;
extern float log10f (float __x) ; extern float __log10f (float __x) ;
extern float modff (float __x, float *__iptr) ; extern float __modff (float __x, float *__iptr) ;
extern float expm1f (float __x) ; extern float __expm1f (float __x) ;
extern float log1pf (float __x) ; extern float __log1pf (float __x) ;
extern float logbf (float __x) ; extern float __logbf (float __x) ;
extern float exp2f (float __x) ; extern float __exp2f (float __x) ;
extern float log2f (float __x) ; extern float __log2f (float __x) ;
extern float powf (float __x, float __y) ; extern float __powf (float __x, float __y) ;
extern float sqrtf (float __x) ; extern float __sqrtf (float __x) ;
extern float hypotf (float __x, float __y) ; extern float __hypotf (float __x, float __y) ;
extern float cbrtf (float __x) ; extern float __cbrtf (float __x) ;
extern float ceilf (float __x) ; extern float __ceilf (float __x) ;
extern float fabsf (float __x) __attribute__ ((__const__)); extern float __fabsf (float __x) __attribute__ ((__const__));
extern float floorf (float __x) ; extern float __floorf (float __x) ;
extern float fmodf (float __x, float __y) ; extern float __fmodf (float __x, float __y) ;
extern int __isinff (float __value) __attribute__ ((__const__));
extern int __finitef (float __value) __attribute__ ((__const__));
extern int isinff (float __value) __attribute__ ((__const__));
extern int finitef (float __value) __attribute__ ((__const__));
extern float dremf (float __x, float __y) ; extern float __dremf (float __x, float __y) ;
extern float significandf (float __x) ; extern float __significandf (float __x) ;
extern float copysignf (float __x, float __y) __attribute__ ((__const__)); extern float __copysignf (float __x, float __y) __attribute__ ((__const__));
extern float nanf (__const char *__tagb) __attribute__ ((__const__)); extern float __nanf (__const char *__tagb) __attribute__ ((__const__));
extern int __isnanf (float __value) __attribute__ ((__const__));
extern int isnanf (float __value) __attribute__ ((__const__));
extern float j0f (float) ; extern float __j0f (float) ;
extern float j1f (float) ; extern float __j1f (float) ;
extern float jnf (int, float) ; extern float __jnf (int, float) ;
extern float y0f (float) ; extern float __y0f (float) ;
extern float y1f (float) ; extern float __y1f (float) ;
extern float ynf (int, float) ; extern float __ynf (int, float) ;
extern float erff (float) ; extern float __erff (float) ;
extern float erfcf (float) ; extern float __erfcf (float) ;
extern float lgammaf (float) ; extern float __lgammaf (float) ;
extern float tgammaf (float) ; extern float __tgammaf (float) ;
extern float gammaf (float) ; extern float __gammaf (float) ;
extern float lgammaf_r (float, int *__signgamp) ; extern float __lgammaf_r (float, int *__signgamp) ;
extern float rintf (float __x) ; extern float __rintf (float __x) ;
extern float nextafterf (float __x, float __y) __attribute__ ((__const__)); extern float __nextafterf (float __x, float __y) __attribute__ ((__const__));
extern float nexttowardf (float __x, long double __y) __attribute__ ((__const__)); extern float __nexttowardf (float __x, long double __y) __attribute__ ((__const__));
extern float remainderf (float __x, float __y) ; extern float __remainderf (float __x, float __y) ;
extern float scalbf (float __x, float __n) ; extern float __scalbf (float __x, float __n) ;
extern float scalbnf (float __x, int __n) ; extern float __scalbnf (float __x, int __n) ;
extern int ilogbf (float __x) ; extern int __ilogbf (float __x) ;
extern float scalblnf (float __x, long int __n) ; extern float __scalblnf (float __x, long int __n) ;
extern float nearbyintf (float __x) ; extern float __nearbyintf (float __x) ;
extern float roundf (float __x) ; extern float __roundf (float __x) ;
extern float truncf (float __x) __attribute__ ((__const__)); extern float __truncf (float __x) __attribute__ ((__const__));
extern float remquof (float __x, float __y, int *__quo) ; extern float __remquof (float __x, float __y, int *__quo) ;
extern long int lrintf (float __x) ; extern long int __lrintf (float __x) ;
extern long long int llrintf (float __x) ; extern long long int __llrintf (float __x) ;
extern long int lroundf (float __x) ; extern long int __lroundf (float __x) ;
extern long long int llroundf (float __x) ; extern long long int __llroundf (float __x) ;
extern float fdimf (float __x, float __y) ; extern float __fdimf (float __x, float __y) ;
extern float fmaxf (float __x, float __y) ; extern float __fmaxf (float __x, float __y) ;
extern float fminf (float __x, float __y) ; extern float __fminf (float __x, float __y) ;
extern int __fpclassifyf (float __value)
     __attribute__ ((__const__));
extern int __signbitf (float __value)
     __attribute__ ((__const__));
extern float fmaf (float __x, float __y, float __z) ; extern float __fmaf (float __x, float __y, float __z) ;
extern long double acosl (long double __x) ; extern long double __acosl (long double __x) ;
extern long double asinl (long double __x) ; extern long double __asinl (long double __x) ;
extern long double atanl (long double __x) ; extern long double __atanl (long double __x) ;
extern long double atan2l (long double __y, long double __x) ; extern long double __atan2l (long double __y, long double __x) ;
extern long double cosl (long double __x) ; extern long double __cosl (long double __x) ;
extern long double sinl (long double __x) ; extern long double __sinl (long double __x) ;
extern long double tanl (long double __x) ; extern long double __tanl (long double __x) ;
extern void sincosl (long double __x, long double *__sinx, long double *__cosx) ; extern void __sincosl (long double __x, long double *__sinx, long double *__cosx)
                                                           ;
extern long double coshl (long double __x) ; extern long double __coshl (long double __x) ;
extern long double sinhl (long double __x) ; extern long double __sinhl (long double __x) ;
extern long double tanhl (long double __x) ; extern long double __tanhl (long double __x) ;
extern long double acoshl (long double __x) ; extern long double __acoshl (long double __x) ;
extern long double asinhl (long double __x) ; extern long double __asinhl (long double __x) ;
extern long double atanhl (long double __x) ; extern long double __atanhl (long double __x) ;
extern long double expl (long double __x) ; extern long double __expl (long double __x) ;
extern long double exp10l (long double __x) ; extern long double __exp10l (long double __x) ;
extern long double pow10l (long double __x) ; extern long double __pow10l (long double __x) ;
extern long double frexpl (long double __x, int *__exponent) ; extern long double __frexpl (long double __x, int *__exponent) ;
extern long double ldexpl (long double __x, int __exponent) ; extern long double __ldexpl (long double __x, int __exponent) ;
extern long double logl (long double __x) ; extern long double __logl (long double __x) ;
extern long double log10l (long double __x) ; extern long double __log10l (long double __x) ;
extern long double modfl (long double __x, long double *__iptr) ; extern long double __modfl (long double __x, long double *__iptr) ;
extern long double expm1l (long double __x) ; extern long double __expm1l (long double __x) ;
extern long double log1pl (long double __x) ; extern long double __log1pl (long double __x) ;
extern long double logbl (long double __x) ; extern long double __logbl (long double __x) ;
extern long double exp2l (long double __x) ; extern long double __exp2l (long double __x) ;
extern long double log2l (long double __x) ; extern long double __log2l (long double __x) ;
extern long double powl (long double __x, long double __y) ; extern long double __powl (long double __x, long double __y) ;
extern long double sqrtl (long double __x) ; extern long double __sqrtl (long double __x) ;
extern long double hypotl (long double __x, long double __y) ; extern long double __hypotl (long double __x, long double __y) ;
extern long double cbrtl (long double __x) ; extern long double __cbrtl (long double __x) ;
extern long double ceill (long double __x) ; extern long double __ceill (long double __x) ;
extern long double fabsl (long double __x) __attribute__ ((__const__)); extern long double __fabsl (long double __x) __attribute__ ((__const__));
extern long double floorl (long double __x) ; extern long double __floorl (long double __x) ;
extern long double fmodl (long double __x, long double __y) ; extern long double __fmodl (long double __x, long double __y) ;
extern int __isinfl (long double __value) __attribute__ ((__const__));
extern int __finitel (long double __value) __attribute__ ((__const__));
extern int isinfl (long double __value) __attribute__ ((__const__));
extern int finitel (long double __value) __attribute__ ((__const__));
extern long double dreml (long double __x, long double __y) ; extern long double __dreml (long double __x, long double __y) ;
extern long double significandl (long double __x) ; extern long double __significandl (long double __x) ;
extern long double copysignl (long double __x, long double __y) __attribute__ ((__const__)); extern long double __copysignl (long double __x, long double __y) __attribute__ ((__const__));
extern long double nanl (__const char *__tagb) __attribute__ ((__const__)); extern long double __nanl (__const char *__tagb) __attribute__ ((__const__));
extern int __isnanl (long double __value) __attribute__ ((__const__));
extern int isnanl (long double __value) __attribute__ ((__const__));
extern long double j0l (long double) ; extern long double __j0l (long double) ;
extern long double j1l (long double) ; extern long double __j1l (long double) ;
extern long double jnl (int, long double) ; extern long double __jnl (int, long double) ;
extern long double y0l (long double) ; extern long double __y0l (long double) ;
extern long double y1l (long double) ; extern long double __y1l (long double) ;
extern long double ynl (int, long double) ; extern long double __ynl (int, long double) ;
extern long double erfl (long double) ; extern long double __erfl (long double) ;
extern long double erfcl (long double) ; extern long double __erfcl (long double) ;
extern long double lgammal (long double) ; extern long double __lgammal (long double) ;
extern long double tgammal (long double) ; extern long double __tgammal (long double) ;
extern long double gammal (long double) ; extern long double __gammal (long double) ;
extern long double lgammal_r (long double, int *__signgamp) ; extern long double __lgammal_r (long double, int *__signgamp) ;
extern long double rintl (long double __x) ; extern long double __rintl (long double __x) ;
extern long double nextafterl (long double __x, long double __y) __attribute__ ((__const__)); extern long double __nextafterl (long double __x, long double __y) __attribute__ ((__const__));
extern long double nexttowardl (long double __x, long double __y) __attribute__ ((__const__)); extern long double __nexttowardl (long double __x, long double __y) __attribute__ ((__const__));
extern long double remainderl (long double __x, long double __y) ; extern long double __remainderl (long double __x, long double __y) ;
extern long double scalbl (long double __x, long double __n) ; extern long double __scalbl (long double __x, long double __n) ;
extern long double scalbnl (long double __x, int __n) ; extern long double __scalbnl (long double __x, int __n) ;
extern int ilogbl (long double __x) ; extern int __ilogbl (long double __x) ;
extern long double scalblnl (long double __x, long int __n) ; extern long double __scalblnl (long double __x, long int __n) ;
extern long double nearbyintl (long double __x) ; extern long double __nearbyintl (long double __x) ;
extern long double roundl (long double __x) ; extern long double __roundl (long double __x) ;
extern long double truncl (long double __x) __attribute__ ((__const__)); extern long double __truncl (long double __x) __attribute__ ((__const__));
extern long double remquol (long double __x, long double __y, int *__quo) ; extern long double __remquol (long double __x, long double __y, int *__quo) ;
extern long int lrintl (long double __x) ; extern long int __lrintl (long double __x) ;
extern long long int llrintl (long double __x) ; extern long long int __llrintl (long double __x) ;
extern long int lroundl (long double __x) ; extern long int __lroundl (long double __x) ;
extern long long int llroundl (long double __x) ; extern long long int __llroundl (long double __x) ;
extern long double fdiml (long double __x, long double __y) ; extern long double __fdiml (long double __x, long double __y) ;
extern long double fmaxl (long double __x, long double __y) ; extern long double __fmaxl (long double __x, long double __y) ;
extern long double fminl (long double __x, long double __y) ; extern long double __fminl (long double __x, long double __y) ;
extern int __fpclassifyl (long double __value)
     __attribute__ ((__const__));
extern int __signbitl (long double __value)
     __attribute__ ((__const__));
extern long double fmal (long double __x, long double __y, long double __z) ; extern long double __fmal (long double __x, long double __y, long double __z) ;
extern int signgam;
enum
  {
    FP_NAN,
    FP_INFINITE,
    FP_ZERO,
    FP_SUBNORMAL,
    FP_NORMAL
  };
typedef enum
{
  _IEEE_ = -1,
  _SVID_,
  _XOPEN_,
  _POSIX_,
  _ISOC_
} _LIB_VERSION_TYPE;
extern _LIB_VERSION_TYPE _LIB_VERSION;
struct exception
  {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
  };
extern int matherr (struct exception *__exc);
struct flock
  {
    short int l_type;
    short int l_whence;
    __off_t l_start;
    __off_t l_len;
    __pid_t l_pid;
  };
struct flock64
  {
    short int l_type;
    short int l_whence;
    __off64_t l_start;
    __off64_t l_len;
    __pid_t l_pid;
  };
struct stat
  {
    __dev_t st_dev;
    unsigned short int __pad1;
    __ino_t st_ino;
    __mode_t st_mode;
    __nlink_t st_nlink;
    __uid_t st_uid;
    __gid_t st_gid;
    __dev_t st_rdev;
    unsigned short int __pad2;
    __off_t st_size;
    __blksize_t st_blksize;
    __blkcnt_t st_blocks;
    __time_t st_atime;
    unsigned long int __unused1;
    __time_t st_mtime;
    unsigned long int __unused2;
    __time_t st_ctime;
    unsigned long int __unused3;
    unsigned long int __unused4;
    unsigned long int __unused5;
  };
struct stat64
  {
    __dev_t st_dev;
    unsigned int __pad1;
    __ino_t __st_ino;
    __mode_t st_mode;
    __nlink_t st_nlink;
    __uid_t st_uid;
    __gid_t st_gid;
    __dev_t st_rdev;
    unsigned int __pad2;
    __off64_t st_size;
    __blksize_t st_blksize;
    __blkcnt64_t st_blocks;
    __time_t st_atime;
    unsigned long int __unused1;
    __time_t st_mtime;
    unsigned long int __unused2;
    __time_t st_ctime;
    unsigned long int __unused3;
    __ino64_t st_ino;
  };
extern int stat (__const char *__restrict __file,
   struct stat *__restrict __buf) ;
extern int fstat (int __fd, struct stat *__buf) ;
extern int stat64 (__const char *__restrict __file,
     struct stat64 *__restrict __buf) ;
extern int fstat64 (int __fd, struct stat64 *__buf) ;
extern int lstat (__const char *__restrict __file,
    struct stat *__restrict __buf) ;
extern int lstat64 (__const char *__restrict __file,
      struct stat64 *__restrict __buf) ;
extern int chmod (__const char *__file, __mode_t __mode) ;
extern int fchmod (int __fd, __mode_t __mode) ;
extern __mode_t umask (__mode_t __mask) ;
extern __mode_t getumask (void) ;
extern int mkdir (__const char *__path, __mode_t __mode) ;
extern int mknod (__const char *__path, __mode_t __mode, __dev_t __dev)
     ;
extern int mkfifo (__const char *__path, __mode_t __mode) ;
extern int __fxstat (int __ver, int __fildes, struct stat *__stat_buf) ;
extern int __xstat (int __ver, __const char *__filename,
      struct stat *__stat_buf) ;
extern int __lxstat (int __ver, __const char *__filename,
       struct stat *__stat_buf) ;
extern int __fxstat64 (int __ver, int __fildes, struct stat64 *__stat_buf)
     ;
extern int __xstat64 (int __ver, __const char *__filename,
        struct stat64 *__stat_buf) ;
extern int __lxstat64 (int __ver, __const char *__filename,
         struct stat64 *__stat_buf) ;
extern int __xmknod (int __ver, __const char *__path, __mode_t __mode,
       __dev_t *__dev) ;
extern __inline__ int stat (__const char *__path,
       struct stat *__statbuf)
{
  return __xstat (3, __path, __statbuf);
}
extern __inline__ int lstat (__const char *__path,
        struct stat *__statbuf)
{
  return __lxstat (3, __path, __statbuf);
}
extern __inline__ int fstat (int __fd, struct stat *__statbuf)
{
  return __fxstat (3, __fd, __statbuf);
}
extern __inline__ int mknod (__const char *__path, __mode_t __mode,
        __dev_t __dev)
{
  return __xmknod (1, __path, __mode, &__dev);
}
extern __inline__ int stat64 (__const char *__path,
         struct stat64 *__statbuf)
{
  return __xstat64 (3, __path, __statbuf);
}
extern __inline__ int lstat64 (__const char *__path,
          struct stat64 *__statbuf)
{
  return __lxstat64 (3, __path, __statbuf);
}
extern __inline__ int fstat64 (int __fd, struct stat64 *__statbuf)
{
  return __fxstat64 (3, __fd, __statbuf);
}
extern int fcntl (int __fd, int __cmd, ...) ;
extern int open (__const char *__file, int __oflag, ...) ;
extern int open64 (__const char *__file, int __oflag, ...) ;
extern int creat (__const char *__file, __mode_t __mode) ;
extern int creat64 (__const char *__file, __mode_t __mode) ;
extern int lockf (int __fd, int __cmd, __off_t __len) ;
extern int lockf64 (int __fd, int __cmd, __off64_t __len) ;
extern int posix_fadvise (int __fd, __off_t __offset, size_t __len,
     int __advise) ;
extern int posix_fadvise64 (int __fd, __off64_t __offset, size_t __len,
       int __advise) ;
extern int posix_fallocate (int __fd, __off_t __offset, size_t __len) ;
extern int posix_fallocate64 (int __fd, __off64_t __offset, size_t __len)
     ;
typedef int __jmp_buf[6];
typedef struct __jmp_buf_tag
  {
    __jmp_buf __jmpbuf;
    int __mask_was_saved;
    __sigset_t __saved_mask;
  } jmp_buf[1];
extern int setjmp (jmp_buf __env) ;
extern int _setjmp (jmp_buf __env) ;
extern int __sigsetjmp (jmp_buf __env, int __savemask) ;
extern void longjmp (jmp_buf __env, int __val)
     __attribute__ ((__noreturn__));
extern void _longjmp (jmp_buf __env, int __val)
     __attribute__ ((__noreturn__));
typedef jmp_buf sigjmp_buf;
extern void siglongjmp (sigjmp_buf __env, int __val)
     __attribute__ ((__noreturn__));
struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
  long int tm_gmtoff;
  __const char *tm_zone;
};
struct itimerspec
  {
    struct timespec it_interval;
    struct timespec it_value;
  };
struct sigevent;
extern clock_t clock (void) ;
extern time_t time (time_t *__timer) ;
extern double difftime (time_t __time1, time_t __time0)
     __attribute__ ((__const__));
extern time_t mktime (struct tm *__tp) ;
extern size_t strftime (char *__restrict __s, size_t __maxsize,
   __const char *__restrict __format,
   __const struct tm *__restrict __tp) ;
extern char *strptime (__const char *__restrict __s,
         __const char *__restrict __fmt, struct tm *__tp)
     ;
extern struct tm *gmtime (__const time_t *__timer) ;
extern struct tm *localtime (__const time_t *__timer) ;
extern struct tm *gmtime_r (__const time_t *__restrict __timer,
       struct tm *__restrict __tp) ;
extern struct tm *localtime_r (__const time_t *__restrict __timer,
          struct tm *__restrict __tp) ;
extern char *asctime (__const struct tm *__tp) ;
extern char *ctime (__const time_t *__timer) ;
extern char *asctime_r (__const struct tm *__restrict __tp,
   char *__restrict __buf) ;
extern char *ctime_r (__const time_t *__restrict __timer,
        char *__restrict __buf) ;
extern char *__tzname[2];
extern int __daylight;
extern long int __timezone;
extern char *tzname[2];
extern void tzset (void) ;
extern int daylight;
extern long int timezone;
extern int stime (__const time_t *__when) ;
extern time_t timegm (struct tm *__tp) ;
extern time_t timelocal (struct tm *__tp) ;
extern int dysize (int __year) __attribute__ ((__const__));
extern int nanosleep (__const struct timespec *__requested_time,
        struct timespec *__remaining) ;
extern int clock_getres (clockid_t __clock_id, struct timespec *__res) ;
extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp) ;
extern int clock_settime (clockid_t __clock_id, __const struct timespec *__tp)
     ;
extern int clock_nanosleep (clockid_t __clock_id, int __flags,
       __const struct timespec *__req,
       struct timespec *__rem) ;
extern int clock_getcpuclockid (pid_t __pid, clockid_t *__clock_id) ;
extern int timer_create (clockid_t __clock_id,
    struct sigevent *__restrict __evp,
    timer_t *__restrict __timerid) ;
extern int timer_delete (timer_t __timerid) ;
extern int timer_settime (timer_t __timerid, int __flags,
     __const struct itimerspec *__restrict __value,
     struct itimerspec *__restrict __ovalue) ;
extern int timer_gettime (timer_t __timerid, struct itimerspec *__value)
     ;
extern int timer_getoverrun (timer_t __timerid) ;
extern int getdate_err;
extern struct tm *getdate (__const char *__string) ;
extern int getdate_r (__const char *__restrict __string,
        struct tm *__restrict __resbufp) ;
typedef __intptr_t intptr_t;
typedef __socklen_t socklen_t;
extern int access (__const char *__name, int __type) ;
extern int euidaccess (__const char *__name, int __type) ;
extern __off_t lseek (int __fd, __off_t __offset, int __whence) ;
extern __off64_t lseek64 (int __fd, __off64_t __offset, int __whence) ;
extern int close (int __fd) ;
extern ssize_t read (int __fd, void *__buf, size_t __nbytes) ;
extern ssize_t write (int __fd, __const void *__buf, size_t __n) ;
extern ssize_t pread (int __fd, void *__buf, size_t __nbytes, __off_t __offset)
     ;
extern ssize_t pwrite (int __fd, __const void *__buf, size_t __n,
         __off_t __offset) ;
extern ssize_t pread64 (int __fd, void *__buf, size_t __nbytes,
   __off64_t __offset) ;
extern ssize_t pwrite64 (int __fd, __const void *__buf, size_t __n,
    __off64_t __offset) ;
extern int pipe (int __pipedes[2]) ;
extern unsigned int alarm (unsigned int __seconds) ;
extern unsigned int sleep (unsigned int __seconds) ;
extern __useconds_t ualarm (__useconds_t __value, __useconds_t __interval)
     ;
extern int usleep (__useconds_t __useconds) ;
extern int pause (void) ;
extern int chown (__const char *__file, __uid_t __owner, __gid_t __group)
     ;
extern int fchown (int __fd, __uid_t __owner, __gid_t __group) ;
extern int lchown (__const char *__file, __uid_t __owner, __gid_t __group)
     ;
extern int chdir (__const char *__path) ;
extern int fchdir (int __fd) ;
extern char *getcwd (char *__buf, size_t __size) ;
extern char *get_current_dir_name (void) ;
extern char *getwd (char *__buf) ;
extern int dup (int __fd) ;
extern int dup2 (int __fd, int __fd2) ;
extern char **__environ;
extern char **environ;
extern int execve (__const char *__path, char *__const __argv[],
     char *__const __envp[]) ;
extern int fexecve (int __fd, char *__const __argv[], char *__const __envp[])
     ;
extern int execv (__const char *__path, char *__const __argv[]) ;
extern int execle (__const char *__path, __const char *__arg, ...) ;
extern int execl (__const char *__path, __const char *__arg, ...) ;
extern int execvp (__const char *__file, char *__const __argv[]) ;
extern int execlp (__const char *__file, __const char *__arg, ...) ;
extern int nice (int __inc) ;
extern void _exit (int __status) __attribute__ ((__noreturn__));
enum
  {
    _PC_LINK_MAX,
    _PC_MAX_CANON,
    _PC_MAX_INPUT,
    _PC_NAME_MAX,
    _PC_PATH_MAX,
    _PC_PIPE_BUF,
    _PC_CHOWN_RESTRICTED,
    _PC_NO_TRUNC,
    _PC_VDISABLE,
    _PC_SYNC_IO,
    _PC_ASYNC_IO,
    _PC_PRIO_IO,
    _PC_SOCK_MAXBUF,
    _PC_FILESIZEBITS,
    _PC_REC_INCR_XFER_SIZE,
    _PC_REC_MAX_XFER_SIZE,
    _PC_REC_MIN_XFER_SIZE,
    _PC_REC_XFER_ALIGN,
    _PC_ALLOC_SIZE_MIN,
    _PC_SYMLINK_MAX
  };
enum
  {
    _SC_ARG_MAX,
    _SC_CHILD_MAX,
    _SC_CLK_TCK,
    _SC_NGROUPS_MAX,
    _SC_OPEN_MAX,
    _SC_STREAM_MAX,
    _SC_TZNAME_MAX,
    _SC_JOB_CONTROL,
    _SC_SAVED_IDS,
    _SC_REALTIME_SIGNALS,
    _SC_PRIORITY_SCHEDULING,
    _SC_TIMERS,
    _SC_ASYNCHRONOUS_IO,
    _SC_PRIORITIZED_IO,
    _SC_SYNCHRONIZED_IO,
    _SC_FSYNC,
    _SC_MAPPED_FILES,
    _SC_MEMLOCK,
    _SC_MEMLOCK_RANGE,
    _SC_MEMORY_PROTECTION,
    _SC_MESSAGE_PASSING,
    _SC_SEMAPHORES,
    _SC_SHARED_MEMORY_OBJECTS,
    _SC_AIO_LISTIO_MAX,
    _SC_AIO_MAX,
    _SC_AIO_PRIO_DELTA_MAX,
    _SC_DELAYTIMER_MAX,
    _SC_MQ_OPEN_MAX,
    _SC_MQ_PRIO_MAX,
    _SC_VERSION,
    _SC_PAGESIZE,
    _SC_RTSIG_MAX,
    _SC_SEM_NSEMS_MAX,
    _SC_SEM_VALUE_MAX,
    _SC_SIGQUEUE_MAX,
    _SC_TIMER_MAX,
    _SC_BC_BASE_MAX,
    _SC_BC_DIM_MAX,
    _SC_BC_SCALE_MAX,
    _SC_BC_STRING_MAX,
    _SC_COLL_WEIGHTS_MAX,
    _SC_EQUIV_CLASS_MAX,
    _SC_EXPR_NEST_MAX,
    _SC_LINE_MAX,
    _SC_RE_DUP_MAX,
    _SC_CHARCLASS_NAME_MAX,
    _SC_2_VERSION,
    _SC_2_C_BIND,
    _SC_2_C_DEV,
    _SC_2_FORT_DEV,
    _SC_2_FORT_RUN,
    _SC_2_SW_DEV,
    _SC_2_LOCALEDEF,
    _SC_PII,
    _SC_PII_XTI,
    _SC_PII_SOCKET,
    _SC_PII_INTERNET,
    _SC_PII_OSI,
    _SC_POLL,
    _SC_SELECT,
    _SC_UIO_MAXIOV,
    _SC_IOV_MAX = _SC_UIO_MAXIOV,
    _SC_PII_INTERNET_STREAM,
    _SC_PII_INTERNET_DGRAM,
    _SC_PII_OSI_COTS,
    _SC_PII_OSI_CLTS,
    _SC_PII_OSI_M,
    _SC_T_IOV_MAX,
    _SC_THREADS,
    _SC_THREAD_SAFE_FUNCTIONS,
    _SC_GETGR_R_SIZE_MAX,
    _SC_GETPW_R_SIZE_MAX,
    _SC_LOGIN_NAME_MAX,
    _SC_TTY_NAME_MAX,
    _SC_THREAD_DESTRUCTOR_ITERATIONS,
    _SC_THREAD_KEYS_MAX,
    _SC_THREAD_STACK_MIN,
    _SC_THREAD_THREADS_MAX,
    _SC_THREAD_ATTR_STACKADDR,
    _SC_THREAD_ATTR_STACKSIZE,
    _SC_THREAD_PRIORITY_SCHEDULING,
    _SC_THREAD_PRIO_INHERIT,
    _SC_THREAD_PRIO_PROTECT,
    _SC_THREAD_PROCESS_SHARED,
    _SC_NPROCESSORS_CONF,
    _SC_NPROCESSORS_ONLN,
    _SC_PHYS_PAGES,
    _SC_AVPHYS_PAGES,
    _SC_ATEXIT_MAX,
    _SC_PASS_MAX,
    _SC_XOPEN_VERSION,
    _SC_XOPEN_XCU_VERSION,
    _SC_XOPEN_UNIX,
    _SC_XOPEN_CRYPT,
    _SC_XOPEN_ENH_I18N,
    _SC_XOPEN_SHM,
    _SC_2_CHAR_TERM,
    _SC_2_C_VERSION,
    _SC_2_UPE,
    _SC_XOPEN_XPG2,
    _SC_XOPEN_XPG3,
    _SC_XOPEN_XPG4,
    _SC_CHAR_BIT,
    _SC_CHAR_MAX,
    _SC_CHAR_MIN,
    _SC_INT_MAX,
    _SC_INT_MIN,
    _SC_LONG_BIT,
    _SC_WORD_BIT,
    _SC_MB_LEN_MAX,
    _SC_NZERO,
    _SC_SSIZE_MAX,
    _SC_SCHAR_MAX,
    _SC_SCHAR_MIN,
    _SC_SHRT_MAX,
    _SC_SHRT_MIN,
    _SC_UCHAR_MAX,
    _SC_UINT_MAX,
    _SC_ULONG_MAX,
    _SC_USHRT_MAX,
    _SC_NL_ARGMAX,
    _SC_NL_LANGMAX,
    _SC_NL_MSGMAX,
    _SC_NL_NMAX,
    _SC_NL_SETMAX,
    _SC_NL_TEXTMAX,
    _SC_XBS5_ILP32_OFF32,
    _SC_XBS5_ILP32_OFFBIG,
    _SC_XBS5_LP64_OFF64,
    _SC_XBS5_LPBIG_OFFBIG,
    _SC_XOPEN_LEGACY,
    _SC_XOPEN_REALTIME,
    _SC_XOPEN_REALTIME_THREADS,
    _SC_ADVISORY_INFO,
    _SC_BARRIERS,
    _SC_BASE,
    _SC_C_LANG_SUPPORT,
    _SC_C_LANG_SUPPORT_R,
    _SC_CLOCK_SELECTION,
    _SC_CPUTIME,
    _SC_THREAD_CPUTIME,
    _SC_DEVICE_IO,
    _SC_DEVICE_SPECIFIC,
    _SC_DEVICE_SPECIFIC_R,
    _SC_FD_MGMT,
    _SC_FIFO,
    _SC_PIPE,
    _SC_FILE_ATTRIBUTES,
    _SC_FILE_LOCKING,
    _SC_FILE_SYSTEM,
    _SC_MONOTONIC_CLOCK,
    _SC_MULTI_PROCESS,
    _SC_SINGLE_PROCESS,
    _SC_NETWORKING,
    _SC_READER_WRITER_LOCKS,
    _SC_SPIN_LOCKS,
    _SC_REGEXP,
    _SC_REGEX_VERSION,
    _SC_SHELL,
    _SC_SIGNALS,
    _SC_SPAWN,
    _SC_SPORADIC_SERVER,
    _SC_THREAD_SPORADIC_SERVER,
    _SC_SYSTEM_DATABASE,
    _SC_SYSTEM_DATABASE_R,
    _SC_TIMEOUTS,
    _SC_TYPED_MEMORY_OBJECTS,
    _SC_USER_GROUPS,
    _SC_USER_GROUPS_R,
    _SC_2_PBS,
    _SC_2_PBS_ACCOUNTING,
    _SC_2_PBS_LOCATE,
    _SC_2_PBS_MESSAGE,
    _SC_2_PBS_TRACK,
    _SC_SYMLOOP_MAX,
    _SC_STREAMS,
    _SC_2_PBS_CHECKPOINT,
    _SC_V6_ILP32_OFF32,
    _SC_V6_ILP32_OFFBIG,
    _SC_V6_LP64_OFF64,
    _SC_V6_LPBIG_OFFBIG,
    _SC_HOST_NAME_MAX,
    _SC_TRACE,
    _SC_TRACE_EVENT_FILTER,
    _SC_TRACE_INHERIT,
    _SC_TRACE_LOG
  };
enum
  {
    _CS_PATH,
    _CS_LFS_CFLAGS = 1000,
    _CS_LFS_LDFLAGS,
    _CS_LFS_LIBS,
    _CS_LFS_LINTFLAGS,
    _CS_LFS64_CFLAGS,
    _CS_LFS64_LDFLAGS,
    _CS_LFS64_LIBS,
    _CS_LFS64_LINTFLAGS,
    _CS_XBS5_ILP32_OFF32_CFLAGS = 1100,
    _CS_XBS5_ILP32_OFF32_LDFLAGS,
    _CS_XBS5_ILP32_OFF32_LIBS,
    _CS_XBS5_ILP32_OFF32_LINTFLAGS,
    _CS_XBS5_ILP32_OFFBIG_CFLAGS,
    _CS_XBS5_ILP32_OFFBIG_LDFLAGS,
    _CS_XBS5_ILP32_OFFBIG_LIBS,
    _CS_XBS5_ILP32_OFFBIG_LINTFLAGS,
    _CS_XBS5_LP64_OFF64_CFLAGS,
    _CS_XBS5_LP64_OFF64_LDFLAGS,
    _CS_XBS5_LP64_OFF64_LIBS,
    _CS_XBS5_LP64_OFF64_LINTFLAGS,
    _CS_XBS5_LPBIG_OFFBIG_CFLAGS,
    _CS_XBS5_LPBIG_OFFBIG_LDFLAGS,
    _CS_XBS5_LPBIG_OFFBIG_LIBS,
    _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS,
    _CS_POSIX_V6_ILP32_OFF32_CFLAGS,
    _CS_POSIX_V6_ILP32_OFF32_LDFLAGS,
    _CS_POSIX_V6_ILP32_OFF32_LIBS,
    _CS_POSIX_V6_ILP32_OFF32_LINTFLAGS,
    _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS,
    _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS,
    _CS_POSIX_V6_ILP32_OFFBIG_LIBS,
    _CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS,
    _CS_POSIX_V6_LP64_OFF64_CFLAGS,
    _CS_POSIX_V6_LP64_OFF64_LDFLAGS,
    _CS_POSIX_V6_LP64_OFF64_LIBS,
    _CS_POSIX_V6_LP64_OFF64_LINTFLAGS,
    _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS,
    _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS,
    _CS_POSIX_V6_LPBIG_OFFBIG_LIBS,
    _CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS,
    _CS_V6_WIDTH_RESTRICTED_ENVS
  };
extern long int pathconf (__const char *__path, int __name) ;
extern long int fpathconf (int __fd, int __name) ;
extern long int sysconf (int __name) __attribute__ ((__const__));
extern size_t confstr (int __name, char *__buf, size_t __len) ;
extern __pid_t getpid (void) ;
extern __pid_t getppid (void) ;
extern __pid_t getpgrp (void) ;
extern __pid_t __getpgid (__pid_t __pid) ;
extern __pid_t getpgid (__pid_t __pid) ;
extern int setpgid (__pid_t __pid, __pid_t __pgid) ;
extern int setpgrp (void) ;
extern __pid_t setsid (void) ;
extern __pid_t getsid (__pid_t __pid) ;
extern __uid_t getuid (void) ;
extern __uid_t geteuid (void) ;
extern __gid_t getgid (void) ;
extern __gid_t getegid (void) ;
extern int getgroups (int __size, __gid_t __list[]) ;
extern int group_member (__gid_t __gid) ;
extern int setuid (__uid_t __uid) ;
extern int setreuid (__uid_t __ruid, __uid_t __euid) ;
extern int seteuid (__uid_t __uid) ;
extern int setgid (__gid_t __gid) ;
extern int setregid (__gid_t __rgid, __gid_t __egid) ;
extern int setegid (__gid_t __gid) ;
extern __pid_t fork (void) ;
extern __pid_t vfork (void) ;
extern char *ttyname (int __fd) ;
extern int ttyname_r (int __fd, char *__buf, size_t __buflen) ;
extern int isatty (int __fd) ;
extern int ttyslot (void) ;
extern int link (__const char *__from, __const char *__to) ;
extern int symlink (__const char *__from, __const char *__to) ;
extern int readlink (__const char *__restrict __path, char *__restrict __buf,
       size_t __len) ;
extern int unlink (__const char *__name) ;
extern int rmdir (__const char *__path) ;
extern __pid_t tcgetpgrp (int __fd) ;
extern int tcsetpgrp (int __fd, __pid_t __pgrp_id) ;
extern char *getlogin (void) ;
extern int getlogin_r (char *__name, size_t __name_len) ;
extern int setlogin (__const char *__name) ;
extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;
extern int getopt (int ___argc, char *const *___argv, const char *__shortopts);
extern int gethostname (char *__name, size_t __len) ;
extern int sethostname (__const char *__name, size_t __len) ;
extern int sethostid (long int __id) ;
extern int getdomainname (char *__name, size_t __len) ;
extern int setdomainname (__const char *__name, size_t __len) ;
extern int vhangup (void) ;
extern int revoke (__const char *__file) ;
extern int profil (unsigned short int *__sample_buffer, size_t __size,
     size_t __offset, unsigned int __scale) ;
extern int acct (__const char *__name) ;
extern char *getusershell (void) ;
extern void endusershell (void) ;
extern void setusershell (void) ;
extern int daemon (int __nochdir, int __noclose) ;
extern int chroot (__const char *__path) ;
extern char *getpass (__const char *__prompt) ;
extern int fsync (int __fd) ;
extern long int gethostid (void) ;
extern void sync (void) ;
extern int getpagesize (void) __attribute__ ((__const__));
extern int truncate (__const char *__file, __off_t __length) ;
extern int truncate64 (__const char *__file, __off64_t __length) ;
extern int ftruncate (int __fd, __off_t __length) ;
extern int ftruncate64 (int __fd, __off64_t __length) ;
extern int getdtablesize (void) ;
extern int brk (void *__addr) ;
extern void *sbrk (intptr_t __delta) ;
extern long int syscall (long int __sysno, ...) ;
extern int fdatasync (int __fildes) ;
extern char *crypt (__const char *__key, __const char *__salt) ;
extern void encrypt (char *__block, int __edflag) ;
extern void swab (__const void *__restrict __from, void *__restrict __to,
    ssize_t __n) ;
extern char *ctermid (char *__s) ;
extern int pthread_atfork (void (*__prepare) (void),
      void (*__parent) (void),
      void (*__child) (void)) ;
struct timezone
  {
    int tz_minuteswest;
    int tz_dsttime;
  };
typedef struct timezone *__restrict __timezone_ptr_t;
extern int gettimeofday (struct timeval *__restrict __tv,
    __timezone_ptr_t __tz) ;
extern int settimeofday (__const struct timeval *__tv,
    __const struct timezone *__tz) ;
extern int adjtime (__const struct timeval *__delta,
      struct timeval *__olddelta) ;
enum __itimer_which
  {
    ITIMER_REAL = 0,
    ITIMER_VIRTUAL = 1,
    ITIMER_PROF = 2
  };
struct itimerval
  {
    struct timeval it_interval;
    struct timeval it_value;
  };
typedef enum __itimer_which __itimer_which_t;
extern int getitimer (__itimer_which_t __which,
        struct itimerval *__value) ;
extern int setitimer (__itimer_which_t __which,
        __const struct itimerval *__restrict __new,
        struct itimerval *__restrict __old) ;
extern int utimes (__const char *__file, __const struct timeval __tvp[2])
     ;
extern void _dl_mcount_wrapper_check (void *__selfpc) ;
extern void *dlopen (__const char *__file, int __mode) ;
extern int dlclose (void *__handle) ;
extern void *dlsym (void *__restrict __handle,
      __const char *__restrict __name) ;
extern void *dlvsym (void *__restrict __handle,
       __const char *__restrict __name,
       __const char *__restrict __version) ;
extern char *dlerror (void) ;
typedef struct
{
  __const char *dli_fname;
  void *dli_fbase;
  __const char *dli_sname;
  void *dli_saddr;
} Dl_info;
extern int dladdr (__const void *__address, Dl_info *__info) ;
extern float strtof (const char *__nptr, char **__endptr);
extern long double strtold (const char *__nptr, char **__endptr);
struct TCCState;
typedef struct TCCState TCCState;
 TCCState *tcc_new(void);
 void tcc_delete(TCCState *s);
 int tcc_add_file(TCCState *s, const char *filename);
 int tcc_set_output_type(TCCState *s, int output_type);
 int tcc_output_file(TCCState *s, const char *filename);
 void *tcc_get_symbol(TCCState *s, const char *name);
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
__extension__
typedef unsigned long long int uint64_t;
typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
__extension__
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
__extension__
typedef unsigned long long int uint_least64_t;
typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
__extension__
typedef long long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
__extension__
typedef unsigned long long int uint_fast64_t;
typedef unsigned int uintptr_t;
__extension__
typedef long long int intmax_t;
__extension__
typedef unsigned long long int uintmax_t;
typedef long int __gwchar_t;
typedef struct
  {
    long long int quot;
    long long int rem;
  } imaxdiv_t;
extern intmax_t imaxabs (intmax_t __n) __attribute__ ((__const__));
extern imaxdiv_t imaxdiv (intmax_t __numer, intmax_t __denom)
      __attribute__ ((__const__));
extern intmax_t strtoimax (__const char *__restrict __nptr,
      char **__restrict __endptr, int __base) ;
extern uintmax_t strtoumax (__const char *__restrict __nptr,
       char ** __restrict __endptr, int __base) ;
extern intmax_t wcstoimax (__const __gwchar_t *__restrict __nptr,
      __gwchar_t **__restrict __endptr, int __base)
     ;
extern uintmax_t wcstoumax (__const __gwchar_t *__restrict __nptr,
       __gwchar_t ** __restrict __endptr, int __base)
     ;
typedef uint16_t Elf32_Half;
typedef uint16_t Elf64_Half;
typedef uint32_t Elf32_Word;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf64_Word;
typedef int32_t Elf64_Sword;
typedef uint64_t Elf32_Xword;
typedef int64_t Elf32_Sxword;
typedef uint64_t Elf64_Xword;
typedef int64_t Elf64_Sxword;
typedef uint32_t Elf32_Addr;
typedef uint64_t Elf64_Addr;
typedef uint32_t Elf32_Off;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf32_Section;
typedef uint16_t Elf64_Section;
typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;
typedef struct
{
  unsigned char e_ident[(16)];
  Elf32_Half e_type;
  Elf32_Half e_machine;
  Elf32_Word e_version;
  Elf32_Addr e_entry;
  Elf32_Off e_phoff;
  Elf32_Off e_shoff;
  Elf32_Word e_flags;
  Elf32_Half e_ehsize;
  Elf32_Half e_phentsize;
  Elf32_Half e_phnum;
  Elf32_Half e_shentsize;
  Elf32_Half e_shnum;
  Elf32_Half e_shstrndx;
} Elf32_Ehdr;
typedef struct
{
  unsigned char e_ident[(16)];
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;
  Elf64_Off e_phoff;
  Elf64_Off e_shoff;
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
} Elf64_Ehdr;
typedef struct
{
  Elf32_Word sh_name;
  Elf32_Word sh_type;
  Elf32_Word sh_flags;
  Elf32_Addr sh_addr;
  Elf32_Off sh_offset;
  Elf32_Word sh_size;
  Elf32_Word sh_link;
  Elf32_Word sh_info;
  Elf32_Word sh_addralign;
  Elf32_Word sh_entsize;
} Elf32_Shdr;
typedef struct
{
  Elf64_Word sh_name;
  Elf64_Word sh_type;
  Elf64_Xword sh_flags;
  Elf64_Addr sh_addr;
  Elf64_Off sh_offset;
  Elf64_Xword sh_size;
  Elf64_Word sh_link;
  Elf64_Word sh_info;
  Elf64_Xword sh_addralign;
  Elf64_Xword sh_entsize;
} Elf64_Shdr;
typedef struct
{
  Elf32_Word st_name;
  Elf32_Addr st_value;
  Elf32_Word st_size;
  unsigned char st_info;
  unsigned char st_other;
  Elf32_Section st_shndx;
} Elf32_Sym;
typedef struct
{
  Elf64_Word st_name;
  unsigned char st_info;
  unsigned char st_other;
  Elf64_Section st_shndx;
  Elf64_Addr st_value;
  Elf64_Xword st_size;
} Elf64_Sym;
typedef struct
{
  Elf32_Half si_boundto;
  Elf32_Half si_flags;
} Elf32_Syminfo;
typedef struct
{
  Elf64_Half si_boundto;
  Elf64_Half si_flags;
} Elf64_Syminfo;
typedef struct
{
  Elf32_Addr r_offset;
  Elf32_Word r_info;
} Elf32_Rel;
typedef struct
{
  Elf64_Addr r_offset;
  Elf64_Xword r_info;
} Elf64_Rel;
typedef struct
{
  Elf32_Addr r_offset;
  Elf32_Word r_info;
  Elf32_Sword r_addend;
} Elf32_Rela;
typedef struct
{
  Elf64_Addr r_offset;
  Elf64_Xword r_info;
  Elf64_Sxword r_addend;
} Elf64_Rela;
typedef struct
{
  Elf32_Word p_type;
  Elf32_Off p_offset;
  Elf32_Addr p_vaddr;
  Elf32_Addr p_paddr;
  Elf32_Word p_filesz;
  Elf32_Word p_memsz;
  Elf32_Word p_flags;
  Elf32_Word p_align;
} Elf32_Phdr;
typedef struct
{
  Elf64_Word p_type;
  Elf64_Word p_flags;
  Elf64_Off p_offset;
  Elf64_Addr p_vaddr;
  Elf64_Addr p_paddr;
  Elf64_Xword p_filesz;
  Elf64_Xword p_memsz;
  Elf64_Xword p_align;
} Elf64_Phdr;
typedef struct
{
  Elf32_Sword d_tag;
  union
    {
      Elf32_Word d_val;
      Elf32_Addr d_ptr;
    } d_un;
} Elf32_Dyn;
typedef struct
{
  Elf64_Sxword d_tag;
  union
    {
      Elf64_Xword d_val;
      Elf64_Addr d_ptr;
    } d_un;
} Elf64_Dyn;
typedef struct
{
  Elf32_Half vd_version;
  Elf32_Half vd_flags;
  Elf32_Half vd_ndx;
  Elf32_Half vd_cnt;
  Elf32_Word vd_hash;
  Elf32_Word vd_aux;
  Elf32_Word vd_next;
} Elf32_Verdef;
typedef struct
{
  Elf64_Half vd_version;
  Elf64_Half vd_flags;
  Elf64_Half vd_ndx;
  Elf64_Half vd_cnt;
  Elf64_Word vd_hash;
  Elf64_Word vd_aux;
  Elf64_Word vd_next;
} Elf64_Verdef;
typedef struct
{
  Elf32_Word vda_name;
  Elf32_Word vda_next;
} Elf32_Verdaux;
typedef struct
{
  Elf64_Word vda_name;
  Elf64_Word vda_next;
} Elf64_Verdaux;
typedef struct
{
  Elf32_Half vn_version;
  Elf32_Half vn_cnt;
  Elf32_Word vn_file;
  Elf32_Word vn_aux;
  Elf32_Word vn_next;
} Elf32_Verneed;
typedef struct
{
  Elf64_Half vn_version;
  Elf64_Half vn_cnt;
  Elf64_Word vn_file;
  Elf64_Word vn_aux;
  Elf64_Word vn_next;
} Elf64_Verneed;
typedef struct
{
  Elf32_Word vna_hash;
  Elf32_Half vna_flags;
  Elf32_Half vna_other;
  Elf32_Word vna_name;
  Elf32_Word vna_next;
} Elf32_Vernaux;
typedef struct
{
  Elf64_Word vna_hash;
  Elf64_Half vna_flags;
  Elf64_Half vna_other;
  Elf64_Word vna_name;
  Elf64_Word vna_next;
} Elf64_Vernaux;
typedef struct
{
  uint32_t a_type;
  union
    {
      uint32_t a_val;
    } a_un;
} Elf32_auxv_t;
typedef struct
{
  uint64_t a_type;
  union
    {
      uint64_t a_val;
    } a_un;
} Elf64_auxv_t;
typedef struct
{
  Elf32_Word n_namesz;
  Elf32_Word n_descsz;
  Elf32_Word n_type;
} Elf32_Nhdr;
typedef struct
{
  Elf64_Word n_namesz;
  Elf64_Word n_descsz;
  Elf64_Word n_type;
} Elf64_Nhdr;
typedef struct
{
  Elf32_Xword m_value;
  Elf32_Word m_info;
  Elf32_Word m_poffset;
  Elf32_Half m_repeat;
  Elf32_Half m_stride;
} Elf32_Move;
typedef struct
{
  Elf64_Xword m_value;
  Elf64_Xword m_info;
  Elf64_Xword m_poffset;
  Elf64_Half m_repeat;
  Elf64_Half m_stride;
} Elf64_Move;
typedef union
{
  struct
    {
      Elf32_Word gt_current_g_value;
      Elf32_Word gt_unused;
    } gt_header;
  struct
    {
      Elf32_Word gt_g_value;
      Elf32_Word gt_bytes;
    } gt_entry;
} Elf32_gptab;
typedef struct
{
  Elf32_Word ri_gprmask;
  Elf32_Word ri_cprmask[4];
  Elf32_Sword ri_gp_value;
} Elf32_RegInfo;
typedef struct
{
  unsigned char kind;
  unsigned char size;
  Elf32_Section section;
  Elf32_Word info;
} Elf_Options;
typedef struct
{
  Elf32_Word hwp_flags1;
  Elf32_Word hwp_flags2;
} Elf_Options_Hw;
typedef struct
{
  Elf32_Word l_name;
  Elf32_Word l_time_stamp;
  Elf32_Word l_checksum;
  Elf32_Word l_version;
  Elf32_Word l_flags;
} Elf32_Lib;
typedef struct
{
  Elf64_Word l_name;
  Elf64_Word l_time_stamp;
  Elf64_Word l_checksum;
  Elf64_Word l_version;
  Elf64_Word l_flags;
} Elf64_Lib;
typedef Elf32_Addr Elf32_Conflict;
enum __stab_debug_code
{
N_GSYM=0x20,
N_FNAME=0x22,
N_FUN=0x24,
N_STSYM=0x26,
N_LCSYM=0x28,
N_MAIN=0x2a,
N_PC=0x30,
N_NSYMS=0x32,
N_NOMAP=0x34,
N_OBJ=0x38,
N_OPT=0x3c,
N_RSYM=0x40,
N_M2C=0x42,
N_SLINE=0x44,
N_DSLINE=0x46,
N_BSLINE=0x48,
N_BROWS=0x48,
N_DEFD=0x4a,
N_EHDECL=0x50,
N_MOD2=0x50,
N_CATCH=0x54,
N_SSYM=0x60,
N_SO=0x64,
N_LSYM=0x80,
N_BINCL=0x82,
N_SOL=0x84,
N_PSYM=0xa0,
N_EINCL=0xa2,
N_ENTRY=0xa4,
N_LBRAC=0xc0,
N_EXCL=0xc2,
N_SCOPE=0xc4,
N_RBRAC=0xe0,
N_BCOMM=0xe2,
N_ECOMM=0xe4,
N_ECOML=0xe8,
N_NBTEXT=0xF0,
N_NBDATA=0xF2,
N_NBBSS=0xF4,
N_NBSTS=0xF6,
N_NBLCS=0xF8,
N_LENG=0xfe,
LAST_UNUSED_STAB_CODE
};
enum {
    TREG_EAX = 0,
    TREG_ECX,
    TREG_EDX,
    TREG_EBX,
    TREG_ST0,
    TREG_ESP = 4
};
typedef struct TokenSym {
    struct TokenSym *hash_next;
    struct Sym *sym_define;
    struct Sym *sym_label;
    struct Sym *sym_struct;
    struct Sym *sym_identifier;
    int tok;
    int len;
    char str[1];
} TokenSym;
typedef int nwchar_t;
typedef struct CString {
    int size;
    void *data;
    int size_allocated;
} CString;
typedef struct CType {
    int t;
    struct Sym *ref;
} CType;
typedef union CValue {
    long double ld;
    double d;
    float f;
    uint64_t i;
    struct {
        int size;
        const void *data;
    } str;
    int tab[12/4];
} CValue;
typedef struct SValue {
    CType type;
    unsigned short r;
    unsigned short r2;
    CValue c;
    struct Sym *sym;
} SValue;
struct SymAttr {
    unsigned short
    aligned : 5,
    packed : 1,
    weak : 1,
    visibility : 2,
    dllexport : 1,
    dllimport : 1,
    unused : 5;
};
struct FuncAttr {
    unsigned
    func_call : 3,
    func_type : 2,
    func_args : 8;
};
typedef struct AttributeDef {
    struct SymAttr a;
    struct FuncAttr f;
    struct Section *section;
    int alias_target;
    int asm_label;
    char attr_mode;
} AttributeDef;
typedef struct Sym {
    int v;
    unsigned short r;
    struct SymAttr a;
    union {
        struct {
            int c;
            union {
                int sym_scope;
                int jnext;
                struct FuncAttr f;
                int auxtype;
            };
        };
        long long enum_val;
        int *d;
    };
    CType type;
    union {
        struct Sym *next;
        int asm_label;
    };
    struct Sym *prev;
    struct Sym *prev_tok;
} Sym;
typedef struct Section {
    unsigned long data_offset;
    unsigned char *data;
    unsigned long data_allocated;
    int sh_name;
    int sh_num;
    int sh_type;
    int sh_flags;
    int sh_info;
    int sh_addralign;
    int sh_entsize;
    unsigned long sh_size;
    Elf32_Addr sh_addr;
    unsigned long sh_offset;
    int nb_hashed_syms;
    struct Section *link;
    struct Section *reloc;
    struct Section *hash;
    struct Section *prev;
    char name[1];
} Section;
typedef struct DLLReference {
    int level;
    void *handle;
    char name[1];
} DLLReference;
typedef struct BufferedFile {
    uint8_t *buf_ptr;
    uint8_t *buf_end;
    int fd;
    struct BufferedFile *prev;
    int line_num;
    int line_ref;
    int ifndef_macro;
    int ifndef_macro_saved;
    int *ifdef_stack_ptr;
    int include_next_index;
    char filename[1024];
    char *true_filename;
    unsigned char unget[4];
    unsigned char buffer[1];
} BufferedFile;
typedef struct TokenString {
    int *str;
    int len;
    int lastlen;
    int allocated_len;
    int last_line_num;
    int save_line_num;
    struct TokenString *prev;
    const int *prev_ptr;
    char alloc;
} TokenString;
typedef struct InlineFunc {
    TokenString *func_str;
    Sym *sym;
    char filename[1];
} InlineFunc;
typedef struct CachedInclude {
    int ifndef_macro;
    int once;
    int hash_next;
    char filename[1];
} CachedInclude;
typedef struct ExprValue {
    uint64_t v;
    Sym *sym;
    int pcrel;
} ExprValue;
typedef struct ASMOperand {
    int id;
    char *constraint;
    char asm_str[16];
    SValue *vt;
    int ref_index;
    int input_index;
    int priority;
    int reg;
    int is_llong;
    int is_memory;
    int is_rw;
} ASMOperand;
struct sym_attr {
    unsigned got_offset;
    unsigned plt_offset;
    int plt_sym;
    int dyn_index;
};
struct TCCState {
    int verbose;
    int nostdinc;
    int nostdlib;
    int nocommon;
    int static_link;
    int rdynamic;
    int symbolic;
    int alacarte_link;
    char *tcc_lib_path;
    char *soname;
    char *rpath;
    int enable_new_dtags;
    int output_type;
    int output_format;
    int char_is_unsigned;
    int leading_underscore;
    int ms_extensions;
    int dollars_in_identifiers;
    int ms_bitfields;
    int warn_write_strings;
    int warn_unsupported;
    int warn_error;
    int warn_none;
    int warn_implicit_function_declaration;
    int warn_gcc_compat;
    int do_debug;
    int do_bounds_check;
    int run_test;
    Elf32_Addr text_addr;
    int has_text_addr;
    unsigned section_align;
    char *init_symbol;
    char *fini_symbol;
    int seg_size;
    DLLReference **loaded_dlls;
    int nb_loaded_dlls;
    char **include_paths;
    int nb_include_paths;
    char **sysinclude_paths;
    int nb_sysinclude_paths;
    char **library_paths;
    int nb_library_paths;
    char **crt_paths;
    int nb_crt_paths;
    char **cmd_include_files;
    int nb_cmd_include_files;
    void *error_opaque;
    void (*error_func)(void *opaque, const char *msg);
    int error_set_jmp_enabled;
    jmp_buf error_jmp_buf;
    int nb_errors;
    FILE *ppfp;
    enum {
 LINE_MACRO_OUTPUT_FORMAT_GCC,
 LINE_MACRO_OUTPUT_FORMAT_NONE,
 LINE_MACRO_OUTPUT_FORMAT_STD,
    LINE_MACRO_OUTPUT_FORMAT_P10 = 11
    } Pflag;
    char dflag;
    char **target_deps;
    int nb_target_deps;
    BufferedFile *include_stack[32];
    BufferedFile **include_stack_ptr;
    int ifdef_stack[64];
    int *ifdef_stack_ptr;
    int cached_includes_hash[32];
    CachedInclude **cached_includes;
    int nb_cached_includes;
    int pack_stack[8];
    int *pack_stack_ptr;
    char **pragma_libs;
    int nb_pragma_libs;
    struct InlineFunc **inline_fns;
    int nb_inline_fns;
    Section **sections;
    int nb_sections;
    Section **priv_sections;
    int nb_priv_sections;
    Section *got;
    Section *plt;
    Section *dynsymtab_section;
    Section *dynsym;
    Section *symtab;
    struct sym_attr *sym_attrs;
    int nb_sym_attrs;
    const char *runtime_main;
    void **runtime_mem;
    int nb_runtime_mem;
    struct filespec **files;
    int nb_files;
    int nb_libraries;
    int filetype;
    char *outfile;
    int option_r;
    int do_bench;
    int gen_deps;
    char *deps_outfile;
    int option_pthread;
    int argc;
    char **argv;
};
struct filespec {
    char type;
    char alacarte;
    char name[1];
};
enum tcc_token {
    TOK_LAST = 256 - 1
     ,TOK_INT
     ,TOK_VOID
     ,TOK_CHAR
     ,TOK_IF
     ,TOK_ELSE
     ,TOK_WHILE
     ,TOK_BREAK
     ,TOK_RETURN
     ,TOK_FOR
     ,TOK_EXTERN
     ,TOK_STATIC
     ,TOK_UNSIGNED
     ,TOK_GOTO
     ,TOK_DO
     ,TOK_CONTINUE
     ,TOK_SWITCH
     ,TOK_CASE
     ,TOK_CONST1
     ,TOK_CONST2
     ,TOK_CONST3
     ,TOK_VOLATILE1
     ,TOK_VOLATILE2
     ,TOK_VOLATILE3
     ,TOK_LONG
     ,TOK_REGISTER
     ,TOK_SIGNED1
     ,TOK_SIGNED2
     ,TOK_SIGNED3
     ,TOK_AUTO
     ,TOK_INLINE1
     ,TOK_INLINE2
     ,TOK_INLINE3
     ,TOK_RESTRICT1
     ,TOK_RESTRICT2
     ,TOK_RESTRICT3
     ,TOK_EXTENSION
     ,TOK_GENERIC
     ,TOK_FLOAT
     ,TOK_DOUBLE
     ,TOK_BOOL
     ,TOK_SHORT
     ,TOK_STRUCT
     ,TOK_UNION
     ,TOK_TYPEDEF
     ,TOK_DEFAULT
     ,TOK_ENUM
     ,TOK_SIZEOF
     ,TOK_ATTRIBUTE1
     ,TOK_ATTRIBUTE2
     ,TOK_ALIGNOF1
     ,TOK_ALIGNOF2
     ,TOK_TYPEOF1
     ,TOK_TYPEOF2
     ,TOK_TYPEOF3
     ,TOK_LABEL
     ,TOK_ASM1
     ,TOK_ASM2
     ,TOK_ASM3
     ,TOK_DEFINE
     ,TOK_INCLUDE
     ,TOK_INCLUDE_NEXT
     ,TOK_IFDEF
     ,TOK_IFNDEF
     ,TOK_ELIF
     ,TOK_ENDIF
     ,TOK_DEFINED
     ,TOK_UNDEF
     ,TOK_ERROR
     ,TOK_WARNING
     ,TOK_LINE
     ,TOK_PRAGMA
     ,TOK___LINE__
     ,TOK___FILE__
     ,TOK___DATE__
     ,TOK___TIME__
     ,TOK___FUNCTION__
     ,TOK___VA_ARGS__
     ,TOK___COUNTER__
     ,TOK___FUNC__
     ,TOK___NAN__
     ,TOK___SNAN__
     ,TOK___INF__
     ,TOK_SECTION1
     ,TOK_SECTION2
     ,TOK_ALIGNED1
     ,TOK_ALIGNED2
     ,TOK_PACKED1
     ,TOK_PACKED2
     ,TOK_WEAK1
     ,TOK_WEAK2
     ,TOK_ALIAS1
     ,TOK_ALIAS2
     ,TOK_UNUSED1
     ,TOK_UNUSED2
     ,TOK_CDECL1
     ,TOK_CDECL2
     ,TOK_CDECL3
     ,TOK_STDCALL1
     ,TOK_STDCALL2
     ,TOK_STDCALL3
     ,TOK_FASTCALL1
     ,TOK_FASTCALL2
     ,TOK_FASTCALL3
     ,TOK_REGPARM1
     ,TOK_REGPARM2
     ,TOK_MODE
     ,TOK_MODE_QI
     ,TOK_MODE_DI
     ,TOK_MODE_HI
     ,TOK_MODE_SI
     ,TOK_MODE_word
     ,TOK_DLLEXPORT
     ,TOK_DLLIMPORT
     ,TOK_NORETURN1
     ,TOK_NORETURN2
     ,TOK_VISIBILITY1
     ,TOK_VISIBILITY2
     ,TOK_builtin_types_compatible_p
     ,TOK_builtin_choose_expr
     ,TOK_builtin_constant_p
     ,TOK_builtin_frame_address
     ,TOK_builtin_return_address
     ,TOK_builtin_expect
     ,TOK_pack
     ,TOK_comment
     ,TOK_lib
     ,TOK_push_macro
     ,TOK_pop_macro
     ,TOK_once
     ,TOK_option
     ,TOK_memcpy
     ,TOK_memmove
     ,TOK_memset
     ,TOK___divdi3
     ,TOK___moddi3
     ,TOK___udivdi3
     ,TOK___umoddi3
     ,TOK___ashrdi3
     ,TOK___lshrdi3
     ,TOK___ashldi3
     ,TOK___floatundisf
     ,TOK___floatundidf
     ,TOK___floatundixf
     ,TOK___fixunsxfdi
     ,TOK___fixunssfdi
     ,TOK___fixunsdfdi
     ,TOK___fixsfdi
     ,TOK___fixdfdi
     ,TOK___fixxfdi
     ,TOK_alloca
     ,TOK___bound_ptr_add
     ,TOK___bound_ptr_indir1
     ,TOK___bound_ptr_indir2
     ,TOK___bound_ptr_indir4
     ,TOK___bound_ptr_indir8
     ,TOK___bound_ptr_indir12
     ,TOK___bound_ptr_indir16
     ,TOK___bound_main_arg
     ,TOK___bound_local_new
     ,TOK___bound_local_delete
     ,TOK_strlen
     ,TOK_strcpy
 ,TOK_ASMDIR_byte
 ,TOK_ASMDIR_word
 ,TOK_ASMDIR_align
 ,TOK_ASMDIR_balign
 ,TOK_ASMDIR_p2align
 ,TOK_ASMDIR_set
 ,TOK_ASMDIR_skip
 ,TOK_ASMDIR_space
 ,TOK_ASMDIR_string
 ,TOK_ASMDIR_asciz
 ,TOK_ASMDIR_ascii
 ,TOK_ASMDIR_file
 ,TOK_ASMDIR_globl
 ,TOK_ASMDIR_global
 ,TOK_ASMDIR_weak
 ,TOK_ASMDIR_hidden
 ,TOK_ASMDIR_ident
 ,TOK_ASMDIR_size
 ,TOK_ASMDIR_type
 ,TOK_ASMDIR_text
 ,TOK_ASMDIR_data
 ,TOK_ASMDIR_bss
 ,TOK_ASMDIR_previous
 ,TOK_ASMDIR_pushsection
 ,TOK_ASMDIR_popsection
 ,TOK_ASMDIR_fill
 ,TOK_ASMDIR_rept
 ,TOK_ASMDIR_endr
 ,TOK_ASMDIR_org
 ,TOK_ASMDIR_quad
 ,TOK_ASMDIR_code16
 ,TOK_ASMDIR_code32
 ,TOK_ASMDIR_short
 ,TOK_ASMDIR_long
 ,TOK_ASMDIR_int
 ,TOK_ASMDIR_section
 ,TOK_ASM_al
 ,TOK_ASM_cl
 ,TOK_ASM_dl
 ,TOK_ASM_bl
 ,TOK_ASM_ah
 ,TOK_ASM_ch
 ,TOK_ASM_dh
 ,TOK_ASM_bh
 ,TOK_ASM_ax
 ,TOK_ASM_cx
 ,TOK_ASM_dx
 ,TOK_ASM_bx
 ,TOK_ASM_sp
 ,TOK_ASM_bp
 ,TOK_ASM_si
 ,TOK_ASM_di
 ,TOK_ASM_eax
 ,TOK_ASM_ecx
 ,TOK_ASM_edx
 ,TOK_ASM_ebx
 ,TOK_ASM_esp
 ,TOK_ASM_ebp
 ,TOK_ASM_esi
 ,TOK_ASM_edi
 ,TOK_ASM_mm0
 ,TOK_ASM_mm1
 ,TOK_ASM_mm2
 ,TOK_ASM_mm3
 ,TOK_ASM_mm4
 ,TOK_ASM_mm5
 ,TOK_ASM_mm6
 ,TOK_ASM_mm7
 ,TOK_ASM_xmm0
 ,TOK_ASM_xmm1
 ,TOK_ASM_xmm2
 ,TOK_ASM_xmm3
 ,TOK_ASM_xmm4
 ,TOK_ASM_xmm5
 ,TOK_ASM_xmm6
 ,TOK_ASM_xmm7
 ,TOK_ASM_cr0
 ,TOK_ASM_cr1
 ,TOK_ASM_cr2
 ,TOK_ASM_cr3
 ,TOK_ASM_cr4
 ,TOK_ASM_cr5
 ,TOK_ASM_cr6
 ,TOK_ASM_cr7
 ,TOK_ASM_tr0
 ,TOK_ASM_tr1
 ,TOK_ASM_tr2
 ,TOK_ASM_tr3
 ,TOK_ASM_tr4
 ,TOK_ASM_tr5
 ,TOK_ASM_tr6
 ,TOK_ASM_tr7
 ,TOK_ASM_db0
 ,TOK_ASM_db1
 ,TOK_ASM_db2
 ,TOK_ASM_db3
 ,TOK_ASM_db4
 ,TOK_ASM_db5
 ,TOK_ASM_db6
 ,TOK_ASM_db7
 ,TOK_ASM_dr0
 ,TOK_ASM_dr1
 ,TOK_ASM_dr2
 ,TOK_ASM_dr3
 ,TOK_ASM_dr4
 ,TOK_ASM_dr5
 ,TOK_ASM_dr6
 ,TOK_ASM_dr7
 ,TOK_ASM_es
 ,TOK_ASM_cs
 ,TOK_ASM_ss
 ,TOK_ASM_ds
 ,TOK_ASM_fs
 ,TOK_ASM_gs
 ,TOK_ASM_st
 ,TOK_ASM_rip
 ,TOK_ASM_movb ,TOK_ASM_movw ,TOK_ASM_movl ,TOK_ASM_mov
 ,TOK_ASM_addb ,TOK_ASM_addw ,TOK_ASM_addl ,TOK_ASM_add
 ,TOK_ASM_orb ,TOK_ASM_orw ,TOK_ASM_orl ,TOK_ASM_or
 ,TOK_ASM_adcb ,TOK_ASM_adcw ,TOK_ASM_adcl ,TOK_ASM_adc
 ,TOK_ASM_sbbb ,TOK_ASM_sbbw ,TOK_ASM_sbbl ,TOK_ASM_sbb
 ,TOK_ASM_andb ,TOK_ASM_andw ,TOK_ASM_andl ,TOK_ASM_and
 ,TOK_ASM_subb ,TOK_ASM_subw ,TOK_ASM_subl ,TOK_ASM_sub
 ,TOK_ASM_xorb ,TOK_ASM_xorw ,TOK_ASM_xorl ,TOK_ASM_xor
 ,TOK_ASM_cmpb ,TOK_ASM_cmpw ,TOK_ASM_cmpl ,TOK_ASM_cmp
 ,TOK_ASM_incb ,TOK_ASM_incw ,TOK_ASM_incl ,TOK_ASM_inc
 ,TOK_ASM_decb ,TOK_ASM_decw ,TOK_ASM_decl ,TOK_ASM_dec
 ,TOK_ASM_notb ,TOK_ASM_notw ,TOK_ASM_notl ,TOK_ASM_not
 ,TOK_ASM_negb ,TOK_ASM_negw ,TOK_ASM_negl ,TOK_ASM_neg
 ,TOK_ASM_mulb ,TOK_ASM_mulw ,TOK_ASM_mull ,TOK_ASM_mul
 ,TOK_ASM_imulb ,TOK_ASM_imulw ,TOK_ASM_imull ,TOK_ASM_imul
 ,TOK_ASM_divb ,TOK_ASM_divw ,TOK_ASM_divl ,TOK_ASM_div
 ,TOK_ASM_idivb ,TOK_ASM_idivw ,TOK_ASM_idivl ,TOK_ASM_idiv
 ,TOK_ASM_xchgb ,TOK_ASM_xchgw ,TOK_ASM_xchgl ,TOK_ASM_xchg
 ,TOK_ASM_testb ,TOK_ASM_testw ,TOK_ASM_testl ,TOK_ASM_test
 ,TOK_ASM_rolb ,TOK_ASM_rolw ,TOK_ASM_roll ,TOK_ASM_rol
 ,TOK_ASM_rorb ,TOK_ASM_rorw ,TOK_ASM_rorl ,TOK_ASM_ror
 ,TOK_ASM_rclb ,TOK_ASM_rclw ,TOK_ASM_rcll ,TOK_ASM_rcl
 ,TOK_ASM_rcrb ,TOK_ASM_rcrw ,TOK_ASM_rcrl ,TOK_ASM_rcr
 ,TOK_ASM_shlb ,TOK_ASM_shlw ,TOK_ASM_shll ,TOK_ASM_shl
 ,TOK_ASM_shrb ,TOK_ASM_shrw ,TOK_ASM_shrl ,TOK_ASM_shr
 ,TOK_ASM_sarb ,TOK_ASM_sarw ,TOK_ASM_sarl ,TOK_ASM_sar
 ,TOK_ASM_shldw ,TOK_ASM_shldl ,TOK_ASM_shld
 ,TOK_ASM_shrdw ,TOK_ASM_shrdl ,TOK_ASM_shrd
 ,TOK_ASM_pushw
 ,TOK_ASM_pushl
 ,TOK_ASM_push
 ,TOK_ASM_popw
 ,TOK_ASM_popl
 ,TOK_ASM_pop
 ,TOK_ASM_inb ,TOK_ASM_inw ,TOK_ASM_inl ,TOK_ASM_in
 ,TOK_ASM_outb ,TOK_ASM_outw ,TOK_ASM_outl ,TOK_ASM_out
 ,TOK_ASM_movzbw ,TOK_ASM_movzbl ,TOK_ASM_movzb
 ,TOK_ASM_movzwl
 ,TOK_ASM_movsbw
 ,TOK_ASM_movsbl
 ,TOK_ASM_movswl
 ,TOK_ASM_leaw ,TOK_ASM_leal ,TOK_ASM_lea
 ,TOK_ASM_les
 ,TOK_ASM_lds
 ,TOK_ASM_lss
 ,TOK_ASM_lfs
 ,TOK_ASM_lgs
 ,TOK_ASM_call
 ,TOK_ASM_jmp
 ,TOK_ASM_lcall
 ,TOK_ASM_ljmp
 ,TOK_ASM_jo ,TOK_ASM_jno ,TOK_ASM_jb ,TOK_ASM_jc ,TOK_ASM_jnae ,TOK_ASM_jnb ,TOK_ASM_jnc ,TOK_ASM_jae ,TOK_ASM_je ,TOK_ASM_jz ,TOK_ASM_jne ,TOK_ASM_jnz ,TOK_ASM_jbe ,TOK_ASM_jna ,TOK_ASM_jnbe ,TOK_ASM_ja ,TOK_ASM_js ,TOK_ASM_jns ,TOK_ASM_jp ,TOK_ASM_jpe ,TOK_ASM_jnp ,TOK_ASM_jpo ,TOK_ASM_jl ,TOK_ASM_jnge ,TOK_ASM_jnl ,TOK_ASM_jge ,TOK_ASM_jle ,TOK_ASM_jng ,TOK_ASM_jnle ,TOK_ASM_jg
 ,TOK_ASM_seto ,TOK_ASM_setno ,TOK_ASM_setb ,TOK_ASM_setc ,TOK_ASM_setnae ,TOK_ASM_setnb ,TOK_ASM_setnc ,TOK_ASM_setae ,TOK_ASM_sete ,TOK_ASM_setz ,TOK_ASM_setne ,TOK_ASM_setnz ,TOK_ASM_setbe ,TOK_ASM_setna ,TOK_ASM_setnbe ,TOK_ASM_seta ,TOK_ASM_sets ,TOK_ASM_setns ,TOK_ASM_setp ,TOK_ASM_setpe ,TOK_ASM_setnp ,TOK_ASM_setpo ,TOK_ASM_setl ,TOK_ASM_setnge ,TOK_ASM_setnl ,TOK_ASM_setge ,TOK_ASM_setle ,TOK_ASM_setng ,TOK_ASM_setnle ,TOK_ASM_setg
 ,TOK_ASM_setob ,TOK_ASM_setnob ,TOK_ASM_setbb ,TOK_ASM_setcb ,TOK_ASM_setnaeb ,TOK_ASM_setnbb ,TOK_ASM_setncb ,TOK_ASM_setaeb ,TOK_ASM_seteb ,TOK_ASM_setzb ,TOK_ASM_setneb ,TOK_ASM_setnzb ,TOK_ASM_setbeb ,TOK_ASM_setnab ,TOK_ASM_setnbeb ,TOK_ASM_setab ,TOK_ASM_setsb ,TOK_ASM_setnsb ,TOK_ASM_setpb ,TOK_ASM_setpeb ,TOK_ASM_setnpb ,TOK_ASM_setpob ,TOK_ASM_setlb ,TOK_ASM_setngeb ,TOK_ASM_setnlb ,TOK_ASM_setgeb ,TOK_ASM_setleb ,TOK_ASM_setngb ,TOK_ASM_setnleb ,TOK_ASM_setgb
 ,TOK_ASM_cmovo ,TOK_ASM_cmovno ,TOK_ASM_cmovb ,TOK_ASM_cmovc ,TOK_ASM_cmovnae ,TOK_ASM_cmovnb ,TOK_ASM_cmovnc ,TOK_ASM_cmovae ,TOK_ASM_cmove ,TOK_ASM_cmovz ,TOK_ASM_cmovne ,TOK_ASM_cmovnz ,TOK_ASM_cmovbe ,TOK_ASM_cmovna ,TOK_ASM_cmovnbe ,TOK_ASM_cmova ,TOK_ASM_cmovs ,TOK_ASM_cmovns ,TOK_ASM_cmovp ,TOK_ASM_cmovpe ,TOK_ASM_cmovnp ,TOK_ASM_cmovpo ,TOK_ASM_cmovl ,TOK_ASM_cmovnge ,TOK_ASM_cmovnl ,TOK_ASM_cmovge ,TOK_ASM_cmovle ,TOK_ASM_cmovng ,TOK_ASM_cmovnle ,TOK_ASM_cmovg
 ,TOK_ASM_bsfw ,TOK_ASM_bsfl ,TOK_ASM_bsf
 ,TOK_ASM_bsrw ,TOK_ASM_bsrl ,TOK_ASM_bsr
 ,TOK_ASM_btw ,TOK_ASM_btl ,TOK_ASM_bt
 ,TOK_ASM_btsw ,TOK_ASM_btsl ,TOK_ASM_bts
 ,TOK_ASM_btrw ,TOK_ASM_btrl ,TOK_ASM_btr
 ,TOK_ASM_btcw ,TOK_ASM_btcl ,TOK_ASM_btc
 ,TOK_ASM_larw ,TOK_ASM_larl ,TOK_ASM_lar
 ,TOK_ASM_lslw ,TOK_ASM_lsll ,TOK_ASM_lsl
 ,TOK_ASM_fadd ,TOK_ASM_faddp ,TOK_ASM_fadds ,TOK_ASM_fiaddl ,TOK_ASM_faddl ,TOK_ASM_fiadds
 ,TOK_ASM_fmul ,TOK_ASM_fmulp ,TOK_ASM_fmuls ,TOK_ASM_fimull ,TOK_ASM_fmull ,TOK_ASM_fimuls
 ,TOK_ASM_fcom
 ,TOK_ASM_fcom_1
 ,TOK_ASM_fcoms ,TOK_ASM_ficoml ,TOK_ASM_fcoml ,TOK_ASM_ficoms
 ,TOK_ASM_fcomp ,TOK_ASM_fcompp ,TOK_ASM_fcomps ,TOK_ASM_ficompl ,TOK_ASM_fcompl ,TOK_ASM_ficomps
 ,TOK_ASM_fsub ,TOK_ASM_fsubp ,TOK_ASM_fsubs ,TOK_ASM_fisubl ,TOK_ASM_fsubl ,TOK_ASM_fisubs
 ,TOK_ASM_fsubr ,TOK_ASM_fsubrp ,TOK_ASM_fsubrs ,TOK_ASM_fisubrl ,TOK_ASM_fsubrl ,TOK_ASM_fisubrs
 ,TOK_ASM_fdiv ,TOK_ASM_fdivp ,TOK_ASM_fdivs ,TOK_ASM_fidivl ,TOK_ASM_fdivl ,TOK_ASM_fidivs
 ,TOK_ASM_fdivr ,TOK_ASM_fdivrp ,TOK_ASM_fdivrs ,TOK_ASM_fidivrl ,TOK_ASM_fdivrl ,TOK_ASM_fidivrs
 ,TOK_ASM_xaddb ,TOK_ASM_xaddw ,TOK_ASM_xaddl ,TOK_ASM_xadd
 ,TOK_ASM_cmpxchgb ,TOK_ASM_cmpxchgw ,TOK_ASM_cmpxchgl ,TOK_ASM_cmpxchg
 ,TOK_ASM_cmpsb ,TOK_ASM_cmpsw ,TOK_ASM_cmpsl ,TOK_ASM_cmps
 ,TOK_ASM_scmpb ,TOK_ASM_scmpw ,TOK_ASM_scmpl ,TOK_ASM_scmp
 ,TOK_ASM_insb ,TOK_ASM_insw ,TOK_ASM_insl ,TOK_ASM_ins
 ,TOK_ASM_outsb ,TOK_ASM_outsw ,TOK_ASM_outsl ,TOK_ASM_outs
 ,TOK_ASM_lodsb ,TOK_ASM_lodsw ,TOK_ASM_lodsl ,TOK_ASM_lods
 ,TOK_ASM_slodb ,TOK_ASM_slodw ,TOK_ASM_slodl ,TOK_ASM_slod
 ,TOK_ASM_movsb ,TOK_ASM_movsw ,TOK_ASM_movsl ,TOK_ASM_movs
 ,TOK_ASM_smovb ,TOK_ASM_smovw ,TOK_ASM_smovl ,TOK_ASM_smov
 ,TOK_ASM_scasb ,TOK_ASM_scasw ,TOK_ASM_scasl ,TOK_ASM_scas
 ,TOK_ASM_sscab ,TOK_ASM_sscaw ,TOK_ASM_sscal ,TOK_ASM_ssca
 ,TOK_ASM_stosb ,TOK_ASM_stosw ,TOK_ASM_stosl ,TOK_ASM_stos
 ,TOK_ASM_sstob ,TOK_ASM_sstow ,TOK_ASM_sstol ,TOK_ASM_ssto
     ,TOK_ASM_clc
     ,TOK_ASM_cld
     ,TOK_ASM_cli
     ,TOK_ASM_clts
     ,TOK_ASM_cmc
     ,TOK_ASM_lahf
     ,TOK_ASM_sahf
     ,TOK_ASM_pusha
     ,TOK_ASM_popa
     ,TOK_ASM_pushfl
     ,TOK_ASM_popfl
     ,TOK_ASM_pushf
     ,TOK_ASM_popf
     ,TOK_ASM_stc
     ,TOK_ASM_std
     ,TOK_ASM_sti
     ,TOK_ASM_aaa
     ,TOK_ASM_aas
     ,TOK_ASM_daa
     ,TOK_ASM_das
     ,TOK_ASM_aad
     ,TOK_ASM_aam
     ,TOK_ASM_cbw
     ,TOK_ASM_cwd
     ,TOK_ASM_cwde
     ,TOK_ASM_cdq
     ,TOK_ASM_cbtw
     ,TOK_ASM_cwtl
     ,TOK_ASM_cwtd
     ,TOK_ASM_cltd
     ,TOK_ASM_int3
     ,TOK_ASM_into
     ,TOK_ASM_iret
     ,TOK_ASM_rsm
     ,TOK_ASM_hlt
     ,TOK_ASM_nop
     ,TOK_ASM_pause
     ,TOK_ASM_xlat
     ,TOK_ASM_wait
     ,TOK_ASM_fwait
     ,TOK_ASM_aword
     ,TOK_ASM_addr16
     ,TOK_ASM_data16
     ,TOK_ASM_lock
     ,TOK_ASM_rep
     ,TOK_ASM_repe
     ,TOK_ASM_repz
     ,TOK_ASM_repne
     ,TOK_ASM_repnz
     ,TOK_ASM_invd
     ,TOK_ASM_wbinvd
     ,TOK_ASM_cpuid
     ,TOK_ASM_wrmsr
     ,TOK_ASM_rdtsc
     ,TOK_ASM_rdmsr
     ,TOK_ASM_rdpmc
     ,TOK_ASM_ud2
    ,TOK_ASM_leave
    ,TOK_ASM_ret
    ,TOK_ASM_retl
    ,TOK_ASM_lret
     ,TOK_ASM_fucompp
     ,TOK_ASM_ftst
     ,TOK_ASM_fxam
     ,TOK_ASM_fld1
     ,TOK_ASM_fldl2t
     ,TOK_ASM_fldl2e
     ,TOK_ASM_fldpi
     ,TOK_ASM_fldlg2
     ,TOK_ASM_fldln2
     ,TOK_ASM_fldz
     ,TOK_ASM_f2xm1
     ,TOK_ASM_fyl2x
     ,TOK_ASM_fptan
     ,TOK_ASM_fpatan
     ,TOK_ASM_fxtract
     ,TOK_ASM_fprem1
     ,TOK_ASM_fdecstp
     ,TOK_ASM_fincstp
     ,TOK_ASM_fprem
     ,TOK_ASM_fyl2xp1
     ,TOK_ASM_fsqrt
     ,TOK_ASM_fsincos
     ,TOK_ASM_frndint
     ,TOK_ASM_fscale
     ,TOK_ASM_fsin
     ,TOK_ASM_fcos
     ,TOK_ASM_fchs
     ,TOK_ASM_fabs
     ,TOK_ASM_fninit
     ,TOK_ASM_fnclex
     ,TOK_ASM_fnop
    ,TOK_ASM_fxch
    ,TOK_ASM_fnstsw
    ,TOK_ASM_emms
    ,TOK_ASM_enter
    ,TOK_ASM_loopne
    ,TOK_ASM_loopnz
    ,TOK_ASM_loope
    ,TOK_ASM_loopz
    ,TOK_ASM_loop
    ,TOK_ASM_jecxz
    ,TOK_ASM_fld
    ,TOK_ASM_fldl
    ,TOK_ASM_flds
    ,TOK_ASM_fildl
    ,TOK_ASM_fildq
    ,TOK_ASM_fildll
    ,TOK_ASM_fldt
    ,TOK_ASM_fbld
    ,TOK_ASM_fst
    ,TOK_ASM_fstl
    ,TOK_ASM_fsts
    ,TOK_ASM_fstps
    ,TOK_ASM_fstpl
    ,TOK_ASM_fist
    ,TOK_ASM_fistp
    ,TOK_ASM_fistl
    ,TOK_ASM_fistpl
    ,TOK_ASM_fstp
    ,TOK_ASM_fistpq
    ,TOK_ASM_fistpll
    ,TOK_ASM_fstpt
    ,TOK_ASM_fbstp
    ,TOK_ASM_fucom
    ,TOK_ASM_fucomp
    ,TOK_ASM_finit
    ,TOK_ASM_fldcw
    ,TOK_ASM_fnstcw
    ,TOK_ASM_fstcw
    ,TOK_ASM_fstsw
    ,TOK_ASM_fclex
    ,TOK_ASM_fnstenv
    ,TOK_ASM_fstenv
    ,TOK_ASM_fldenv
    ,TOK_ASM_fnsave
    ,TOK_ASM_fsave
    ,TOK_ASM_frstor
    ,TOK_ASM_ffree
    ,TOK_ASM_ffreep
    ,TOK_ASM_fxsave
    ,TOK_ASM_fxrstor
    ,TOK_ASM_arpl
    ,TOK_ASM_lgdt
    ,TOK_ASM_lidt
    ,TOK_ASM_lldt
    ,TOK_ASM_lmsw
    ,TOK_ASM_ltr
    ,TOK_ASM_sgdt
    ,TOK_ASM_sidt
    ,TOK_ASM_sldt
    ,TOK_ASM_smsw
    ,TOK_ASM_str
    ,TOK_ASM_verr
    ,TOK_ASM_verw
    ,TOK_ASM_bswap
    ,TOK_ASM_invlpg
    ,TOK_ASM_boundl
    ,TOK_ASM_boundw
    ,TOK_ASM_cmpxchg8b
    ,TOK_ASM_fcmovb
    ,TOK_ASM_fcmove
    ,TOK_ASM_fcmovbe
    ,TOK_ASM_fcmovu
    ,TOK_ASM_fcmovnb
    ,TOK_ASM_fcmovne
    ,TOK_ASM_fcmovnbe
    ,TOK_ASM_fcmovnu
    ,TOK_ASM_fucomi
    ,TOK_ASM_fcomi
    ,TOK_ASM_fucomip
    ,TOK_ASM_fcomip
    ,TOK_ASM_movd
    ,TOK_ASM_movq
    ,TOK_ASM_packssdw
    ,TOK_ASM_packsswb
    ,TOK_ASM_packuswb
    ,TOK_ASM_paddb
    ,TOK_ASM_paddw
    ,TOK_ASM_paddd
    ,TOK_ASM_paddsb
    ,TOK_ASM_paddsw
    ,TOK_ASM_paddusb
    ,TOK_ASM_paddusw
    ,TOK_ASM_pand
    ,TOK_ASM_pandn
    ,TOK_ASM_pcmpeqb
    ,TOK_ASM_pcmpeqw
    ,TOK_ASM_pcmpeqd
    ,TOK_ASM_pcmpgtb
    ,TOK_ASM_pcmpgtw
    ,TOK_ASM_pcmpgtd
    ,TOK_ASM_pmaddwd
    ,TOK_ASM_pmulhw
    ,TOK_ASM_pmullw
    ,TOK_ASM_por
    ,TOK_ASM_psllw
    ,TOK_ASM_pslld
    ,TOK_ASM_psllq
    ,TOK_ASM_psraw
    ,TOK_ASM_psrad
    ,TOK_ASM_psrlw
    ,TOK_ASM_psrld
    ,TOK_ASM_psrlq
    ,TOK_ASM_psubb
    ,TOK_ASM_psubw
    ,TOK_ASM_psubd
    ,TOK_ASM_psubsb
    ,TOK_ASM_psubsw
    ,TOK_ASM_psubusb
    ,TOK_ASM_psubusw
    ,TOK_ASM_punpckhbw
    ,TOK_ASM_punpckhwd
    ,TOK_ASM_punpckhdq
    ,TOK_ASM_punpcklbw
    ,TOK_ASM_punpcklwd
    ,TOK_ASM_punpckldq
    ,TOK_ASM_pxor
    ,TOK_ASM_movups
    ,TOK_ASM_movaps
    ,TOK_ASM_movhps
    ,TOK_ASM_addps
    ,TOK_ASM_cvtpi2ps
    ,TOK_ASM_cvtps2pi
    ,TOK_ASM_cvttps2pi
    ,TOK_ASM_divps
    ,TOK_ASM_maxps
    ,TOK_ASM_minps
    ,TOK_ASM_mulps
    ,TOK_ASM_pavgb
    ,TOK_ASM_pavgw
    ,TOK_ASM_pmaxsw
    ,TOK_ASM_pmaxub
    ,TOK_ASM_pminsw
    ,TOK_ASM_pminub
    ,TOK_ASM_rcpss
    ,TOK_ASM_rsqrtps
    ,TOK_ASM_sqrtps
    ,TOK_ASM_subps
};
static int gnu_ext;
static int tcc_ext;
static struct TCCState *tcc_state;
static char *pstrcpy(char *buf, int buf_size, const char *s);
static char *pstrncpy(char *out, const char *in, size_t num);
 char *tcc_basename(const char *name);
 char *tcc_fileextension (const char *name);
 void tcc_free(void *ptr);
 void *tcc_malloc(unsigned long size);
 void *tcc_mallocz(unsigned long size);
 void *tcc_realloc(void *ptr, unsigned long size);
 char *tcc_strdup(const char *str);
 void tcc_memcheck(void);
 void tcc_error_noabort(const char *fmt, ...);
 __attribute__((noreturn)) void tcc_error(const char *fmt, ...);
 void tcc_warning(const char *fmt, ...);
static void dynarray_add(void *ptab, int *nb_ptr, void *data);
static void dynarray_reset(void *pp, int *n);
static inline void cstr_ccat(CString *cstr, int ch);
static void cstr_cat(CString *cstr, const char *str, int len);
static void cstr_wccat(CString *cstr, int ch);
static void cstr_new(CString *cstr);
static void cstr_free(CString *cstr);
static void cstr_reset(CString *cstr);
static inline void sym_free(Sym *sym);
static Sym *sym_push2(Sym **ps, int v, int t, int c);
static Sym *sym_find2(Sym *s, int v);
static Sym *sym_push(int v, CType *type, int r, int c);
static void sym_pop(Sym **ptop, Sym *b, int keep);
static inline Sym *struct_find(int v);
static inline Sym *sym_find(int v);
static Sym *global_identifier_push(int v, int t, int c);
static void tcc_open_bf(TCCState *s1, const char *filename, int initlen);
static int tcc_open(TCCState *s1, const char *filename);
static void tcc_close(void);
static int tcc_add_file_internal(TCCState *s1, const char *filename, int flags);
 int tcc_parse_args(TCCState *s, int *argc, char ***argv, int optind);
static struct BufferedFile *file;
static int ch, tok;
static CValue tokc;
static const int *macro_ptr;
static int parse_flags;
static int tok_flags;
static CString tokcstr;
static int total_lines;
static int total_bytes;
static int tok_ident;
static TokenSym **table_ident;
static TokenSym *tok_alloc(const char *str, int len);
static const char *get_tok_str(int v, CValue *cv);
static void begin_macro(TokenString *str, int alloc);
static void end_macro(void);
static int set_idnum(int c, int val);
static inline void tok_str_new(TokenString *s);
static TokenString *tok_str_alloc(void);
static void tok_str_free(TokenString *s);
static void tok_str_free_str(int *str);
static void tok_str_add(TokenString *s, int t);
static void tok_str_add_tok(TokenString *s);
static inline Sym *define_find(int v);
static void free_defines(Sym *b);
static Sym *label_find(int v);
static Sym *label_push(Sym **ptop, int v, int flags);
static void preprocess(int is_bof);
static void next_nomacro(void);
static void next(void);
static inline void unget_tok(int last_tok);
static void preprocess_start(TCCState *s1, int is_asm);
static void preprocess_end(TCCState *s1);
static void tccpp_new(TCCState *s);
static void tccpp_delete(TCCState *s);
static void skip(int c);
static __attribute__((noreturn)) void expect(const char *msg);
static inline int is_space(int ch) {
    return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r';
}
static inline int isid(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
static inline int isnum(int c) {
    return c >= '0' && c <= '9';
}
static inline int isoct(int c) {
    return c >= '0' && c <= '7';
}
static inline int toup(int c) {
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}
static Sym *sym_free_first;
static void **sym_pools;
static int nb_sym_pools;
static Sym *global_stack;
static Sym *local_stack;
static Sym *local_label_stack;
static Sym *global_label_stack;
static Sym *define_stack;
static CType char_pointer_type, func_old_type, int_type, size_type;
static SValue __vstack[1+ 256], *vtop, *pvtop;
static int rsym, anon_sym, ind, loc;
static int const_wanted;
static int nocode_wanted;
static int global_expr;
static CType func_vt;
static int func_var;
static int func_vc;
static int last_line_num, last_ind, func_ind;
static const char *funcname;
static int g_debug;
static void tcc_debug_start(TCCState *s1);
static void tcc_debug_end(TCCState *s1);
static void tcc_debug_funcstart(TCCState *s1, Sym *sym);
static void tcc_debug_funcend(TCCState *s1, int size);
static void tcc_debug_line(TCCState *s1);
static int tccgen_compile(TCCState *s1);
static void check_vstack(void);
static inline int is_float(int t);
static int ieee_finite(double d);
static void test_lvalue(void);
static void vpushi(int v);
static Elf32_Sym *elfsym(Sym *);
static void update_storage(Sym *sym);
static Sym *external_global_sym(int v, CType *type, int r);
static void vset(CType *type, int r, int v);
static void vswap(void);
static void vpush_global_sym(CType *type, int v);
static void vrote(SValue *e, int n);
static void vrott(int n);
static void vrotb(int n);
static void vpushv(SValue *v);
static void save_reg(int r);
static void save_reg_upstack(int r, int n);
static int get_reg(int rc);
static void save_regs(int n);
static void gaddrof(void);
static int gv(int rc);
static void gv2(int rc1, int rc2);
static void vpop(void);
static void gen_op(int op);
static int type_size(CType *type, int *a);
static void mk_pointer(CType *type);
static void vstore(void);
static void inc(int post, int c);
static void parse_mult_str (CString *astr, const char *msg);
static int lvalue_type(int t);
static void indir(void);
static void unary(void);
static void expr_prod(void);
static void expr_sum(void);
static void gexpr(void);
static int expr_const(void);
static Sym *get_sym_ref(CType *type, Section *sec, unsigned long offset, unsigned long size);
typedef struct {
    unsigned int n_strx;
    unsigned char n_type;
    unsigned char n_other;
    unsigned short n_desc;
    unsigned int n_value;
} Stab_Sym;
static Section *text_section, *data_section, *bss_section;
static Section *common_section;
static Section *cur_text_section;
static Section *last_text_section;
static Section *bounds_section;
static Section *lbounds_section;
static Section *symtab_section;
static Section *stab_section, *stabstr_section;
static void tccelf_new(TCCState *s);
static void tccelf_delete(TCCState *s);
static void tccelf_begin_file(TCCState *s1);
static void tccelf_end_file(TCCState *s1);
static Section *new_section(TCCState *s1, const char *name, int sh_type, int sh_flags);
static void section_realloc(Section *sec, unsigned long new_size);
static size_t section_add(Section *sec, Elf32_Addr size, int align);
static void *section_ptr_add(Section *sec, Elf32_Addr size);
static void section_reserve(Section *sec, unsigned long size);
static Section *new_symtab(TCCState *s1, const char *symtab_name, int sh_type, int sh_flags, const char *strtab_name, const char *hash_name, int hash_sh_flags);
static void put_extern_sym2(Sym *sym, int sh_num, Elf32_Addr value, unsigned long size, int can_add_underscore);
static void put_extern_sym(Sym *sym, Section *section, Elf32_Addr value, unsigned long size);
static void greloc(Section *s, Sym *sym, unsigned long offset, int type);
static void greloca(Section *s, Sym *sym, unsigned long offset, int type, Elf32_Addr addend);
static int put_elf_str(Section *s, const char *sym);
static int put_elf_sym(Section *s, Elf32_Addr value, unsigned long size, int info, int other, int shndx, const char *name);
static int set_elf_sym(Section *s, Elf32_Addr value, unsigned long size, int info, int other, int shndx, const char *name);
static int find_elf_sym(Section *s, const char *name);
static void put_elf_reloca(Section *symtab, Section *s, unsigned long offset, int type, int symbol, Elf32_Addr addend);
static int tcc_object_type(int fd, Elf32_Ehdr *h);
static int tcc_load_object_file(TCCState *s1, int fd, unsigned long file_offset);
static int tcc_load_archive(TCCState *s1, int fd);
static struct sym_attr *get_sym_attr(TCCState *s1, int index, int alloc);
static Elf32_Addr get_elf_sym_addr(TCCState *s, const char *name, int err);
static void *tcc_get_symbol_err(TCCState *s, const char *name);
static uint8_t *parse_comment(uint8_t *p);
static void minp(void);
static inline void inp(void);
static int handle_eob(void);
enum gotplt_entry {
    NO_GOTPLT_ENTRY,
    BUILD_GOT_ONLY,
    AUTO_GOTPLT_ENTRY,
    ALWAYS_GOTPLT_ENTRY
};
static const int reg_classes[5];
static void gsym_addr(int t, int a);
static void gsym(int t);
static void load(int r, SValue *sv);
static void store(int r, SValue *v);
static int gfunc_sret(CType *vt, int variadic, CType *ret, int *align, int *regsize);
static void gfunc_call(int nb_args);
static void gfunc_prolog(CType *func_type);
static void gfunc_epilog(void);
static int gjmp(int t);
static void gjmp_addr(int a);
static int gtst(int inv, int t);
static void gtst_addr(int inv, int a);
static void gen_opi(int op);
static void gen_opf(int op);
static void gen_cvt_ftoi(int t);
static void gen_cvt_ftof(int t);
static void ggoto(void);
static void o(unsigned int c);
static void gen_cvt_itof(int t);
static void gen_vla_sp_save(int addr);
static void gen_vla_sp_restore(int addr);
static void gen_vla_alloc(CType *type, int align);
static inline uint16_t read16le(unsigned char *p) {
    return p[0] | (uint16_t)p[1] << 8;
}
static inline void write16le(unsigned char *p, uint16_t x) {
    p[0] = x & 255; p[1] = x >> 8 & 255;
}
static inline uint32_t read32le(unsigned char *p) {
  return read16le(p) | (uint32_t)read16le(p + 2) << 16;
}
static inline void write32le(unsigned char *p, uint32_t x) {
    write16le(p, x); write16le(p + 2, x >> 16);
}
static inline void add32le(unsigned char *p, int32_t x) {
    write32le(p, read32le(p) + x);
}
static inline uint64_t read64le(unsigned char *p) {
  return read32le(p) | (uint64_t)read32le(p + 4) << 32;
}
static inline void write64le(unsigned char *p, uint64_t x) {
    write32le(p, x); write32le(p + 4, x >> 32);
}
static inline void add64le(unsigned char *p, int64_t x) {
    write64le(p, read64le(p) + x);
}
static void g(int c);
static void gen_le16(int c);
static void gen_le32(int c);
static void gen_addr32(int r, Sym *sym, int c);
static void gen_addrpc32(int r, Sym *sym, int c);
static int rt_num_callers;
static const char **rt_bound_error_msg;
static void *rt_prog_main;
static int gnu_ext = 1;
static int tcc_ext = 1;
static struct TCCState *tcc_state;
static int nb_states;
static int tok_flags;
static int parse_flags;
static struct BufferedFile *file;
static int ch, tok;
static CValue tokc;
static const int *macro_ptr;
static CString tokcstr;
static int total_lines;
static int total_bytes;
static int tok_ident;
static TokenSym **table_ident;
static TokenSym *hash_ident[16384];
static char token_buf[1024 + 1];
static CString cstr_buf;
static CString macro_equal_buf;
static TokenString tokstr_buf;
static unsigned char isidnum_table[256 - (-1)];
static int pp_debug_tok, pp_debug_symv;
static int pp_once;
static int pp_expr;
static int pp_counter;
static void tok_print(const char *msg, const int *str);
static struct TinyAlloc *toksym_alloc;
static struct TinyAlloc *tokstr_alloc;
static struct TinyAlloc *cstr_alloc;
static TokenString *macro_stack;
static const char tcc_keywords[] =
     "int" "\0"
     "void" "\0"
     "char" "\0"
     "if" "\0"
     "else" "\0"
     "while" "\0"
     "break" "\0"
     "return" "\0"
     "for" "\0"
     "extern" "\0"
     "static" "\0"
     "unsigned" "\0"
     "goto" "\0"
     "do" "\0"
     "continue" "\0"
     "switch" "\0"
     "case" "\0"
     "const" "\0"
     "__const" "\0"
     "__const__" "\0"
     "volatile" "\0"
     "__volatile" "\0"
     "__volatile__" "\0"
     "long" "\0"
     "register" "\0"
     "signed" "\0"
     "__signed" "\0"
     "__signed__" "\0"
     "auto" "\0"
     "inline" "\0"
     "__inline" "\0"
     "__inline__" "\0"
     "restrict" "\0"
     "__restrict" "\0"
     "__restrict__" "\0"
     "__extension__" "\0"
     "_Generic" "\0"
     "float" "\0"
     "double" "\0"
     "_Bool" "\0"
     "short" "\0"
     "struct" "\0"
     "union" "\0"
     "typedef" "\0"
     "default" "\0"
     "enum" "\0"
     "sizeof" "\0"
     "__attribute" "\0"
     "__attribute__" "\0"
     "__alignof" "\0"
     "__alignof__" "\0"
     "typeof" "\0"
     "__typeof" "\0"
     "__typeof__" "\0"
     "__label__" "\0"
     "asm" "\0"
     "__asm" "\0"
     "__asm__" "\0"
     "define" "\0"
     "include" "\0"
     "include_next" "\0"
     "ifdef" "\0"
     "ifndef" "\0"
     "elif" "\0"
     "endif" "\0"
     "defined" "\0"
     "undef" "\0"
     "error" "\0"
     "warning" "\0"
     "line" "\0"
     "pragma" "\0"
     "__LINE__" "\0"
     "__FILE__" "\0"
     "__DATE__" "\0"
     "__TIME__" "\0"
     "__FUNCTION__" "\0"
     "__VA_ARGS__" "\0"
     "__COUNTER__" "\0"
     "__func__" "\0"
     "__nan__" "\0"
     "__snan__" "\0"
     "__inf__" "\0"
     "section" "\0"
     "__section__" "\0"
     "aligned" "\0"
     "__aligned__" "\0"
     "packed" "\0"
     "__packed__" "\0"
     "weak" "\0"
     "__weak__" "\0"
     "alias" "\0"
     "__alias__" "\0"
     "unused" "\0"
     "__unused__" "\0"
     "cdecl" "\0"
     "__cdecl" "\0"
     "__cdecl__" "\0"
     "stdcall" "\0"
     "__stdcall" "\0"
     "__stdcall__" "\0"
     "fastcall" "\0"
     "__fastcall" "\0"
     "__fastcall__" "\0"
     "regparm" "\0"
     "__regparm__" "\0"
     "__mode__" "\0"
     "__QI__" "\0"
     "__DI__" "\0"
     "__HI__" "\0"
     "__SI__" "\0"
     "__word__" "\0"
     "dllexport" "\0"
     "dllimport" "\0"
     "noreturn" "\0"
     "__noreturn__" "\0"
     "visibility" "\0"
     "__visibility__" "\0"
     "__builtin_types_compatible_p" "\0"
     "__builtin_choose_expr" "\0"
     "__builtin_constant_p" "\0"
     "__builtin_frame_address" "\0"
     "__builtin_return_address" "\0"
     "__builtin_expect" "\0"
     "pack" "\0"
     "comment" "\0"
     "lib" "\0"
     "push_macro" "\0"
     "pop_macro" "\0"
     "once" "\0"
     "option" "\0"
     "memcpy" "\0"
     "memmove" "\0"
     "memset" "\0"
     "__divdi3" "\0"
     "__moddi3" "\0"
     "__udivdi3" "\0"
     "__umoddi3" "\0"
     "__ashrdi3" "\0"
     "__lshrdi3" "\0"
     "__ashldi3" "\0"
     "__floatundisf" "\0"
     "__floatundidf" "\0"
     "__floatundixf" "\0"
     "__fixunsxfdi" "\0"
     "__fixunssfdi" "\0"
     "__fixunsdfdi" "\0"
     "__fixsfdi" "\0"
     "__fixdfdi" "\0"
     "__fixxfdi" "\0"
     "alloca" "\0"
     "__bound_ptr_add" "\0"
     "__bound_ptr_indir1" "\0"
     "__bound_ptr_indir2" "\0"
     "__bound_ptr_indir4" "\0"
     "__bound_ptr_indir8" "\0"
     "__bound_ptr_indir12" "\0"
     "__bound_ptr_indir16" "\0"
     "__bound_main_arg" "\0"
     "__bound_local_new" "\0"
     "__bound_local_delete" "\0"
     "strlen" "\0"
     "strcpy" "\0"
 "." "byte" "\0"
 "." "word" "\0"
 "." "align" "\0"
 "." "balign" "\0"
 "." "p2align" "\0"
 "." "set" "\0"
 "." "skip" "\0"
 "." "space" "\0"
 "." "string" "\0"
 "." "asciz" "\0"
 "." "ascii" "\0"
 "." "file" "\0"
 "." "globl" "\0"
 "." "global" "\0"
 "." "weak" "\0"
 "." "hidden" "\0"
 "." "ident" "\0"
 "." "size" "\0"
 "." "type" "\0"
 "." "text" "\0"
 "." "data" "\0"
 "." "bss" "\0"
 "." "previous" "\0"
 "." "pushsection" "\0"
 "." "popsection" "\0"
 "." "fill" "\0"
 "." "rept" "\0"
 "." "endr" "\0"
 "." "org" "\0"
 "." "quad" "\0"
 "." "code16" "\0"
 "." "code32" "\0"
 "." "short" "\0"
 "." "long" "\0"
 "." "int" "\0"
 "." "section" "\0"
 "al" "\0"
 "cl" "\0"
 "dl" "\0"
 "bl" "\0"
 "ah" "\0"
 "ch" "\0"
 "dh" "\0"
 "bh" "\0"
 "ax" "\0"
 "cx" "\0"
 "dx" "\0"
 "bx" "\0"
 "sp" "\0"
 "bp" "\0"
 "si" "\0"
 "di" "\0"
 "eax" "\0"
 "ecx" "\0"
 "edx" "\0"
 "ebx" "\0"
 "esp" "\0"
 "ebp" "\0"
 "esi" "\0"
 "edi" "\0"
 "mm0" "\0"
 "mm1" "\0"
 "mm2" "\0"
 "mm3" "\0"
 "mm4" "\0"
 "mm5" "\0"
 "mm6" "\0"
 "mm7" "\0"
 "xmm0" "\0"
 "xmm1" "\0"
 "xmm2" "\0"
 "xmm3" "\0"
 "xmm4" "\0"
 "xmm5" "\0"
 "xmm6" "\0"
 "xmm7" "\0"
 "cr0" "\0"
 "cr1" "\0"
 "cr2" "\0"
 "cr3" "\0"
 "cr4" "\0"
 "cr5" "\0"
 "cr6" "\0"
 "cr7" "\0"
 "tr0" "\0"
 "tr1" "\0"
 "tr2" "\0"
 "tr3" "\0"
 "tr4" "\0"
 "tr5" "\0"
 "tr6" "\0"
 "tr7" "\0"
 "db0" "\0"
 "db1" "\0"
 "db2" "\0"
 "db3" "\0"
 "db4" "\0"
 "db5" "\0"
 "db6" "\0"
 "db7" "\0"
 "dr0" "\0"
 "dr1" "\0"
 "dr2" "\0"
 "dr3" "\0"
 "dr4" "\0"
 "dr5" "\0"
 "dr6" "\0"
 "dr7" "\0"
 "es" "\0"
 "cs" "\0"
 "ss" "\0"
 "ds" "\0"
 "fs" "\0"
 "gs" "\0"
 "st" "\0"
 "rip" "\0"
 "mov" "b" "\0" "mov" "w" "\0" "mov" "l" "\0" "mov" "\0"
 "add" "b" "\0" "add" "w" "\0" "add" "l" "\0" "add" "\0"
 "or" "b" "\0" "or" "w" "\0" "or" "l" "\0" "or" "\0"
 "adc" "b" "\0" "adc" "w" "\0" "adc" "l" "\0" "adc" "\0"
 "sbb" "b" "\0" "sbb" "w" "\0" "sbb" "l" "\0" "sbb" "\0"
 "and" "b" "\0" "and" "w" "\0" "and" "l" "\0" "and" "\0"
 "sub" "b" "\0" "sub" "w" "\0" "sub" "l" "\0" "sub" "\0"
 "xor" "b" "\0" "xor" "w" "\0" "xor" "l" "\0" "xor" "\0"
 "cmp" "b" "\0" "cmp" "w" "\0" "cmp" "l" "\0" "cmp" "\0"
 "inc" "b" "\0" "inc" "w" "\0" "inc" "l" "\0" "inc" "\0"
 "dec" "b" "\0" "dec" "w" "\0" "dec" "l" "\0" "dec" "\0"
 "not" "b" "\0" "not" "w" "\0" "not" "l" "\0" "not" "\0"
 "neg" "b" "\0" "neg" "w" "\0" "neg" "l" "\0" "neg" "\0"
 "mul" "b" "\0" "mul" "w" "\0" "mul" "l" "\0" "mul" "\0"
 "imul" "b" "\0" "imul" "w" "\0" "imul" "l" "\0" "imul" "\0"
 "div" "b" "\0" "div" "w" "\0" "div" "l" "\0" "div" "\0"
 "idiv" "b" "\0" "idiv" "w" "\0" "idiv" "l" "\0" "idiv" "\0"
 "xchg" "b" "\0" "xchg" "w" "\0" "xchg" "l" "\0" "xchg" "\0"
 "test" "b" "\0" "test" "w" "\0" "test" "l" "\0" "test" "\0"
 "rol" "b" "\0" "rol" "w" "\0" "rol" "l" "\0" "rol" "\0"
 "ror" "b" "\0" "ror" "w" "\0" "ror" "l" "\0" "ror" "\0"
 "rcl" "b" "\0" "rcl" "w" "\0" "rcl" "l" "\0" "rcl" "\0"
 "rcr" "b" "\0" "rcr" "w" "\0" "rcr" "l" "\0" "rcr" "\0"
 "shl" "b" "\0" "shl" "w" "\0" "shl" "l" "\0" "shl" "\0"
 "shr" "b" "\0" "shr" "w" "\0" "shr" "l" "\0" "shr" "\0"
 "sar" "b" "\0" "sar" "w" "\0" "sar" "l" "\0" "sar" "\0"
 "shld" "w" "\0" "shld" "l" "\0" "shld" "\0"
 "shrd" "w" "\0" "shrd" "l" "\0" "shrd" "\0"
 "pushw" "\0"
 "pushl" "\0"
 "push" "\0"
 "popw" "\0"
 "popl" "\0"
 "pop" "\0"
 "in" "b" "\0" "in" "w" "\0" "in" "l" "\0" "in" "\0"
 "out" "b" "\0" "out" "w" "\0" "out" "l" "\0" "out" "\0"
 "movzb" "w" "\0" "movzb" "l" "\0" "movzb" "\0"
 "movzwl" "\0"
 "movsbw" "\0"
 "movsbl" "\0"
 "movswl" "\0"
 "lea" "w" "\0" "lea" "l" "\0" "lea" "\0"
 "les" "\0"
 "lds" "\0"
 "lss" "\0"
 "lfs" "\0"
 "lgs" "\0"
 "call" "\0"
 "jmp" "\0"
 "lcall" "\0"
 "ljmp" "\0"
 "jo" "\0" "jno" "\0" "jb" "\0" "jc" "\0" "jnae" "\0" "jnb" "\0" "jnc" "\0" "jae" "\0" "je" "\0" "jz" "\0" "jne" "\0" "jnz" "\0" "jbe" "\0" "jna" "\0" "jnbe" "\0" "ja" "\0" "js" "\0" "jns" "\0" "jp" "\0" "jpe" "\0" "jnp" "\0" "jpo" "\0" "jl" "\0" "jnge" "\0" "jnl" "\0" "jge" "\0" "jle" "\0" "jng" "\0" "jnle" "\0" "jg" "\0"
 "seto" "\0" "setno" "\0" "setb" "\0" "setc" "\0" "setnae" "\0" "setnb" "\0" "setnc" "\0" "setae" "\0" "sete" "\0" "setz" "\0" "setne" "\0" "setnz" "\0" "setbe" "\0" "setna" "\0" "setnbe" "\0" "seta" "\0" "sets" "\0" "setns" "\0" "setp" "\0" "setpe" "\0" "setnp" "\0" "setpo" "\0" "setl" "\0" "setnge" "\0" "setnl" "\0" "setge" "\0" "setle" "\0" "setng" "\0" "setnle" "\0" "setg" "\0"
 "setob" "\0" "setnob" "\0" "setbb" "\0" "setcb" "\0" "setnaeb" "\0" "setnbb" "\0" "setncb" "\0" "setaeb" "\0" "seteb" "\0" "setzb" "\0" "setneb" "\0" "setnzb" "\0" "setbeb" "\0" "setnab" "\0" "setnbeb" "\0" "setab" "\0" "setsb" "\0" "setnsb" "\0" "setpb" "\0" "setpeb" "\0" "setnpb" "\0" "setpob" "\0" "setlb" "\0" "setngeb" "\0" "setnlb" "\0" "setgeb" "\0" "setleb" "\0" "setngb" "\0" "setnleb" "\0" "setgb" "\0"
 "cmovo" "\0" "cmovno" "\0" "cmovb" "\0" "cmovc" "\0" "cmovnae" "\0" "cmovnb" "\0" "cmovnc" "\0" "cmovae" "\0" "cmove" "\0" "cmovz" "\0" "cmovne" "\0" "cmovnz" "\0" "cmovbe" "\0" "cmovna" "\0" "cmovnbe" "\0" "cmova" "\0" "cmovs" "\0" "cmovns" "\0" "cmovp" "\0" "cmovpe" "\0" "cmovnp" "\0" "cmovpo" "\0" "cmovl" "\0" "cmovnge" "\0" "cmovnl" "\0" "cmovge" "\0" "cmovle" "\0" "cmovng" "\0" "cmovnle" "\0" "cmovg" "\0"
 "bsf" "w" "\0" "bsf" "l" "\0" "bsf" "\0"
 "bsr" "w" "\0" "bsr" "l" "\0" "bsr" "\0"
 "bt" "w" "\0" "bt" "l" "\0" "bt" "\0"
 "bts" "w" "\0" "bts" "l" "\0" "bts" "\0"
 "btr" "w" "\0" "btr" "l" "\0" "btr" "\0"
 "btc" "w" "\0" "btc" "l" "\0" "btc" "\0"
 "lar" "w" "\0" "lar" "l" "\0" "lar" "\0"
 "lsl" "w" "\0" "lsl" "l" "\0" "lsl" "\0"
 "f" "add" "\0" "f" "add" "p" "\0" "f" "add" "s" "\0" "fi" "add" "l" "\0" "f" "add" "l" "\0" "fi" "add" "s" "\0"
 "f" "mul" "\0" "f" "mul" "p" "\0" "f" "mul" "s" "\0" "fi" "mul" "l" "\0" "f" "mul" "l" "\0" "fi" "mul" "s" "\0"
 "fcom" "\0"
 "fcom_1" "\0"
 "f" "com" "s" "\0" "fi" "com" "l" "\0" "f" "com" "l" "\0" "fi" "com" "s" "\0"
 "f" "comp" "\0" "f" "comp" "p" "\0" "f" "comp" "s" "\0" "fi" "comp" "l" "\0" "f" "comp" "l" "\0" "fi" "comp" "s" "\0"
 "f" "sub" "\0" "f" "sub" "p" "\0" "f" "sub" "s" "\0" "fi" "sub" "l" "\0" "f" "sub" "l" "\0" "fi" "sub" "s" "\0"
 "f" "subr" "\0" "f" "subr" "p" "\0" "f" "subr" "s" "\0" "fi" "subr" "l" "\0" "f" "subr" "l" "\0" "fi" "subr" "s" "\0"
 "f" "div" "\0" "f" "div" "p" "\0" "f" "div" "s" "\0" "fi" "div" "l" "\0" "f" "div" "l" "\0" "fi" "div" "s" "\0"
 "f" "divr" "\0" "f" "divr" "p" "\0" "f" "divr" "s" "\0" "fi" "divr" "l" "\0" "f" "divr" "l" "\0" "fi" "divr" "s" "\0"
 "xadd" "b" "\0" "xadd" "w" "\0" "xadd" "l" "\0" "xadd" "\0"
 "cmpxchg" "b" "\0" "cmpxchg" "w" "\0" "cmpxchg" "l" "\0" "cmpxchg" "\0"
 "cmps" "b" "\0" "cmps" "w" "\0" "cmps" "l" "\0" "cmps" "\0"
 "scmp" "b" "\0" "scmp" "w" "\0" "scmp" "l" "\0" "scmp" "\0"
 "ins" "b" "\0" "ins" "w" "\0" "ins" "l" "\0" "ins" "\0"
 "outs" "b" "\0" "outs" "w" "\0" "outs" "l" "\0" "outs" "\0"
 "lods" "b" "\0" "lods" "w" "\0" "lods" "l" "\0" "lods" "\0"
 "slod" "b" "\0" "slod" "w" "\0" "slod" "l" "\0" "slod" "\0"
 "movs" "b" "\0" "movs" "w" "\0" "movs" "l" "\0" "movs" "\0"
 "smov" "b" "\0" "smov" "w" "\0" "smov" "l" "\0" "smov" "\0"
 "scas" "b" "\0" "scas" "w" "\0" "scas" "l" "\0" "scas" "\0"
 "ssca" "b" "\0" "ssca" "w" "\0" "ssca" "l" "\0" "ssca" "\0"
 "stos" "b" "\0" "stos" "w" "\0" "stos" "l" "\0" "stos" "\0"
 "ssto" "b" "\0" "ssto" "w" "\0" "ssto" "l" "\0" "ssto" "\0"
     "clc" "\0"
     "cld" "\0"
     "cli" "\0"
     "clts" "\0"
     "cmc" "\0"
     "lahf" "\0"
     "sahf" "\0"
     "pusha" "\0"
     "popa" "\0"
     "pushfl" "\0"
     "popfl" "\0"
     "pushf" "\0"
     "popf" "\0"
     "stc" "\0"
     "std" "\0"
     "sti" "\0"
     "aaa" "\0"
     "aas" "\0"
     "daa" "\0"
     "das" "\0"
     "aad" "\0"
     "aam" "\0"
     "cbw" "\0"
     "cwd" "\0"
     "cwde" "\0"
     "cdq" "\0"
     "cbtw" "\0"
     "cwtl" "\0"
     "cwtd" "\0"
     "cltd" "\0"
     "int3" "\0"
     "into" "\0"
     "iret" "\0"
     "rsm" "\0"
     "hlt" "\0"
     "nop" "\0"
     "pause" "\0"
     "xlat" "\0"
     "wait" "\0"
     "fwait" "\0"
     "aword" "\0"
     "addr16" "\0"
     "data16" "\0"
     "lock" "\0"
     "rep" "\0"
     "repe" "\0"
     "repz" "\0"
     "repne" "\0"
     "repnz" "\0"
     "invd" "\0"
     "wbinvd" "\0"
     "cpuid" "\0"
     "wrmsr" "\0"
     "rdtsc" "\0"
     "rdmsr" "\0"
     "rdpmc" "\0"
     "ud2" "\0"
    "leave" "\0"
    "ret" "\0"
    "retl" "\0"
    "lret" "\0"
     "fucompp" "\0"
     "ftst" "\0"
     "fxam" "\0"
     "fld1" "\0"
     "fldl2t" "\0"
     "fldl2e" "\0"
     "fldpi" "\0"
     "fldlg2" "\0"
     "fldln2" "\0"
     "fldz" "\0"
     "f2xm1" "\0"
     "fyl2x" "\0"
     "fptan" "\0"
     "fpatan" "\0"
     "fxtract" "\0"
     "fprem1" "\0"
     "fdecstp" "\0"
     "fincstp" "\0"
     "fprem" "\0"
     "fyl2xp1" "\0"
     "fsqrt" "\0"
     "fsincos" "\0"
     "frndint" "\0"
     "fscale" "\0"
     "fsin" "\0"
     "fcos" "\0"
     "fchs" "\0"
     "fabs" "\0"
     "fninit" "\0"
     "fnclex" "\0"
     "fnop" "\0"
    "fxch" "\0"
    "fnstsw" "\0"
    "emms" "\0"
    "enter" "\0"
    "loopne" "\0"
    "loopnz" "\0"
    "loope" "\0"
    "loopz" "\0"
    "loop" "\0"
    "jecxz" "\0"
    "fld" "\0"
    "fldl" "\0"
    "flds" "\0"
    "fildl" "\0"
    "fildq" "\0"
    "fildll" "\0"
    "fldt" "\0"
    "fbld" "\0"
    "fst" "\0"
    "fstl" "\0"
    "fsts" "\0"
    "fstps" "\0"
    "fstpl" "\0"
    "fist" "\0"
    "fistp" "\0"
    "fistl" "\0"
    "fistpl" "\0"
    "fstp" "\0"
    "fistpq" "\0"
    "fistpll" "\0"
    "fstpt" "\0"
    "fbstp" "\0"
    "fucom" "\0"
    "fucomp" "\0"
    "finit" "\0"
    "fldcw" "\0"
    "fnstcw" "\0"
    "fstcw" "\0"
    "fstsw" "\0"
    "fclex" "\0"
    "fnstenv" "\0"
    "fstenv" "\0"
    "fldenv" "\0"
    "fnsave" "\0"
    "fsave" "\0"
    "frstor" "\0"
    "ffree" "\0"
    "ffreep" "\0"
    "fxsave" "\0"
    "fxrstor" "\0"
    "arpl" "\0"
    "lgdt" "\0"
    "lidt" "\0"
    "lldt" "\0"
    "lmsw" "\0"
    "ltr" "\0"
    "sgdt" "\0"
    "sidt" "\0"
    "sldt" "\0"
    "smsw" "\0"
    "str" "\0"
    "verr" "\0"
    "verw" "\0"
    "bswap" "\0"
    "invlpg" "\0"
    "boundl" "\0"
    "boundw" "\0"
    "cmpxchg8b" "\0"
    "fcmovb" "\0"
    "fcmove" "\0"
    "fcmovbe" "\0"
    "fcmovu" "\0"
    "fcmovnb" "\0"
    "fcmovne" "\0"
    "fcmovnbe" "\0"
    "fcmovnu" "\0"
    "fucomi" "\0"
    "fcomi" "\0"
    "fucomip" "\0"
    "fcomip" "\0"
    "movd" "\0"
    "movq" "\0"
    "packssdw" "\0"
    "packsswb" "\0"
    "packuswb" "\0"
    "paddb" "\0"
    "paddw" "\0"
    "paddd" "\0"
    "paddsb" "\0"
    "paddsw" "\0"
    "paddusb" "\0"
    "paddusw" "\0"
    "pand" "\0"
    "pandn" "\0"
    "pcmpeqb" "\0"
    "pcmpeqw" "\0"
    "pcmpeqd" "\0"
    "pcmpgtb" "\0"
    "pcmpgtw" "\0"
    "pcmpgtd" "\0"
    "pmaddwd" "\0"
    "pmulhw" "\0"
    "pmullw" "\0"
    "por" "\0"
    "psllw" "\0"
    "pslld" "\0"
    "psllq" "\0"
    "psraw" "\0"
    "psrad" "\0"
    "psrlw" "\0"
    "psrld" "\0"
    "psrlq" "\0"
    "psubb" "\0"
    "psubw" "\0"
    "psubd" "\0"
    "psubsb" "\0"
    "psubsw" "\0"
    "psubusb" "\0"
    "psubusw" "\0"
    "punpckhbw" "\0"
    "punpckhwd" "\0"
    "punpckhdq" "\0"
    "punpcklbw" "\0"
    "punpcklwd" "\0"
    "punpckldq" "\0"
    "pxor" "\0"
    "movups" "\0"
    "movaps" "\0"
    "movhps" "\0"
    "addps" "\0"
    "cvtpi2ps" "\0"
    "cvtps2pi" "\0"
    "cvttps2pi" "\0"
    "divps" "\0"
    "maxps" "\0"
    "minps" "\0"
    "mulps" "\0"
    "pavgb" "\0"
    "pavgw" "\0"
    "pmaxsw" "\0"
    "pmaxub" "\0"
    "pminsw" "\0"
    "pminub" "\0"
    "rcpss" "\0"
    "rsqrtps" "\0"
    "sqrtps" "\0"
    "subps" "\0"
;
static const unsigned char tok_two_chars[] =
  {
    '<','=', 0x9e,
    '>','=', 0x9d,
    '!','=', 0x95,
    '&','&', 0xa0,
    '|','|', 0xa1,
    '+','+', 0xa4,
    '-','-', 0xa2,
    '=','=', 0x94,
    '<','<', 0x01,
    '>','>', 0x02,
    '+','=', 0xab,
    '-','=', 0xad,
    '*','=', 0xaa,
    '/','=', 0xaf,
    '%','=', 0xa5,
    '&','=', 0xa6,
    '^','=', 0xde,
    '|','=', 0xfc,
    '-','>', 0xc7,
    '.','.', 0xa8,
    '#','#', 0xca,
    0
};
static void next_nomacro_spc(void);
static void skip(int c)
{
    if (tok != c)
        tcc_error("'%c' expected (got \"%s\")", c, get_tok_str(tok, &tokc));
    next();
}
static void expect(const char *msg)
{
    tcc_error("%s expected", msg);
}
typedef struct TinyAlloc {
    unsigned limit;
    unsigned size;
    uint8_t *buffer;
    uint8_t *p;
    unsigned nb_allocs;
    struct TinyAlloc *next, *top;
} TinyAlloc;
typedef struct tal_header_t {
    unsigned size;
} tal_header_t;
static TinyAlloc *tal_new(TinyAlloc **pal, unsigned limit, unsigned size)
{
    TinyAlloc *al = tcc_mallocz(sizeof(TinyAlloc));
    al->p = al->buffer = tcc_malloc(size);
    al->limit = limit;
    al->size = size;
    if (pal) *pal = al;
    return al;
}
static void tal_delete(TinyAlloc *al)
{
    TinyAlloc *next;
tail_call:
    if (!al)
        return;
    next = al->next;
    tcc_free(al->buffer);
    tcc_free(al);
    al = next;
    goto tail_call;
}
static void tal_free_impl(TinyAlloc *al, void *p )
{
    if (!p)
        return;
tail_call:
    if (al->buffer <= (uint8_t *)p && (uint8_t *)p < al->buffer + al->size) {
        al->nb_allocs--;
        if (!al->nb_allocs)
            al->p = al->buffer;
    } else if (al->next) {
        al = al->next;
        goto tail_call;
    }
    else
        tcc_free(p);
}
static void *tal_realloc_impl(TinyAlloc **pal, void *p, unsigned size )
{
    tal_header_t *header;
    void *ret;
    int is_own;
    unsigned adj_size = (size + 3) & -4;
    TinyAlloc *al = *pal;
tail_call:
    is_own = (al->buffer <= (uint8_t *)p && (uint8_t *)p < al->buffer + al->size);
    if ((!p || is_own) && size <= al->limit) {
        if (al->p + adj_size + sizeof(tal_header_t) < al->buffer + al->size) {
            header = (tal_header_t *)al->p;
            header->size = adj_size;
            ret = al->p + sizeof(tal_header_t);
            al->p += adj_size + sizeof(tal_header_t);
            if (is_own) {
                header = (((tal_header_t *)p) - 1);
                memcpy(ret, p, header->size);
            } else {
                al->nb_allocs++;
            }
            return ret;
        } else if (is_own) {
            al->nb_allocs--;
            ret = tal_realloc_impl(&*pal, 0, size);
            header = (((tal_header_t *)p) - 1);
            memcpy(ret, p, header->size);
            return ret;
        }
        if (al->next) {
            al = al->next;
        } else {
            TinyAlloc *bottom = al, *next = al->top ? al->top : al;
            al = tal_new(pal, next->limit, next->size * 2);
            al->next = next;
            bottom->top = al;
        }
        goto tail_call;
    }
    if (is_own) {
        al->nb_allocs--;
        ret = tcc_malloc(size);
        header = (((tal_header_t *)p) - 1);
        memcpy(ret, p, header->size);
    } else if (al->next) {
        al = al->next;
        goto tail_call;
    } else
        ret = tcc_realloc(p, size);
    return ret;
}
static void cstr_realloc(CString *cstr, int new_size)
{
    int size;
    size = cstr->size_allocated;
    if (size < 8)
        size = 8;
    while (size < new_size)
        size = size * 2;
    cstr->data = tal_realloc_impl(&cstr_alloc, cstr->data, size);
    cstr->size_allocated = size;
}
static inline void cstr_ccat(CString *cstr, int ch)
{
    int size;
    size = cstr->size + 1;
    if (size > cstr->size_allocated)
        cstr_realloc(cstr, size);
    ((unsigned char *)cstr->data)[size - 1] = ch;
    cstr->size = size;
}
static void cstr_cat(CString *cstr, const char *str, int len)
{
    int size;
    if (len <= 0)
        len = strlen(str) + 1 + len;
    size = cstr->size + len;
    if (size > cstr->size_allocated)
        cstr_realloc(cstr, size);
    memmove(((unsigned char *)cstr->data) + cstr->size, str, len);
    cstr->size = size;
}
static void cstr_wccat(CString *cstr, int ch)
{
    int size;
    size = cstr->size + sizeof(nwchar_t);
    if (size > cstr->size_allocated)
        cstr_realloc(cstr, size);
    *(nwchar_t *)(((unsigned char *)cstr->data) + size - sizeof(nwchar_t)) = ch;
    cstr->size = size;
}
static void cstr_new(CString *cstr)
{
    memset(cstr, 0, sizeof(CString));
}
static void cstr_free(CString *cstr)
{
    tal_free_impl(cstr_alloc, cstr->data);
    cstr_new(cstr);
}
static void cstr_reset(CString *cstr)
{
    cstr->size = 0;
}
static void add_char(CString *cstr, int c)
{
    if (c == '\'' || c == '\"' || c == '\\') {
        cstr_ccat(cstr, '\\');
    }
    if (c >= 32 && c <= 126) {
        cstr_ccat(cstr, c);
    } else {
        cstr_ccat(cstr, '\\');
        if (c == '\n') {
            cstr_ccat(cstr, 'n');
        } else {
            cstr_ccat(cstr, '0' + ((c >> 6) & 7));
            cstr_ccat(cstr, '0' + ((c >> 3) & 7));
            cstr_ccat(cstr, '0' + (c & 7));
        }
    }
}
static TokenSym *tok_alloc_new(TokenSym **pts, const char *str, int len)
{
    TokenSym *ts, **ptable;
    int i;
    if (tok_ident >= 0x10000000)
        tcc_error("memory full (symbols)");
    i = tok_ident - 256;
    if ((i % 512) == 0) {
        ptable = tcc_realloc(table_ident, (i + 512) * sizeof(TokenSym *));
        table_ident = ptable;
    }
    ts = tal_realloc_impl(&toksym_alloc, 0, sizeof(TokenSym) + len);
    table_ident[i] = ts;
    ts->tok = tok_ident++;
    ts->sym_define = ((void*)0);
    ts->sym_label = ((void*)0);
    ts->sym_struct = ((void*)0);
    ts->sym_identifier = ((void*)0);
    ts->len = len;
    ts->hash_next = ((void*)0);
    memcpy(ts->str, str, len);
    ts->str[len] = '\0';
    *pts = ts;
    return ts;
}
static TokenSym *tok_alloc(const char *str, int len)
{
    TokenSym *ts, **pts;
    int i;
    unsigned int h;
    h = 1;
    for(i=0;i<len;i++)
        h = ((h) + ((h) << 5) + ((h) >> 27) + (((unsigned char *)str)[i]));
    h &= (16384 - 1);
    pts = &hash_ident[h];
    for(;;) {
        ts = *pts;
        if (!ts)
            break;
        if (ts->len == len && !memcmp(ts->str, str, len))
            return ts;
        pts = &(ts->hash_next);
    }
    return tok_alloc_new(pts, str, len);
}
static const char *get_tok_str(int v, CValue *cv)
{
    char *p;
    int i, len;
    cstr_reset(&cstr_buf);
    p = cstr_buf.data;
    switch(v) {
    case 0xb5:
    case 0xb6:
    case 0xce:
    case 0xcf:
    case 0xb7:
    case 0xb8:
        sprintf(p, "%llu", (unsigned long long)cv->i);
        break;
    case 0xb4:
        cstr_ccat(&cstr_buf, 'L');
    case 0xb3:
        cstr_ccat(&cstr_buf, '\'');
        add_char(&cstr_buf, cv->i);
        cstr_ccat(&cstr_buf, '\'');
        cstr_ccat(&cstr_buf, '\0');
        break;
    case 0xbe:
    case 0xbf:
        return (char*)cv->str.data;
    case 0xba:
        cstr_ccat(&cstr_buf, 'L');
    case 0xb9:
        cstr_ccat(&cstr_buf, '\"');
        if (v == 0xb9) {
            len = cv->str.size - 1;
            for(i=0;i<len;i++)
                add_char(&cstr_buf, ((unsigned char *)cv->str.data)[i]);
        } else {
            len = (cv->str.size / sizeof(nwchar_t)) - 1;
            for(i=0;i<len;i++)
                add_char(&cstr_buf, ((nwchar_t *)cv->str.data)[i]);
        }
        cstr_ccat(&cstr_buf, '\"');
        cstr_ccat(&cstr_buf, '\0');
        break;
    case 0xbb:
        cstr_cat(&cstr_buf, "<float>", 0);
        break;
    case 0xbc:
 cstr_cat(&cstr_buf, "<double>", 0);
 break;
    case 0xbd:
 cstr_cat(&cstr_buf, "<long double>", 0);
 break;
    case 0xc0:
 cstr_cat(&cstr_buf, "<linenumber>", 0);
 break;
    case 0x9c:
        v = '<';
        goto addv;
    case 0x9f:
        v = '>';
        goto addv;
    case 0xc8:
        return strcpy(p, "...");
    case 0x81:
        return strcpy(p, "<<=");
    case 0x82:
        return strcpy(p, ">>=");
    case (-1):
        return strcpy(p, "<eof>");
    default:
        if (v < 256) {
            const unsigned char *q = tok_two_chars;
            while (*q) {
                if (q[2] == v) {
                    *p++ = q[0];
                    *p++ = q[1];
                    *p = '\0';
                    return cstr_buf.data;
                }
                q += 3;
            }
        if (v >= 127) {
            sprintf(cstr_buf.data, "<%02x>", v);
            return cstr_buf.data;
        }
        addv:
            *p++ = v;
            *p = '\0';
        } else if (v < tok_ident) {
            return table_ident[v - 256]->str;
        } else if (v >= 0x10000000) {
            sprintf(p, "L.%u", v - 0x10000000);
        } else {
            return ((void*)0);
        }
        break;
    }
    return cstr_buf.data;
}
static int handle_eob(void)
{
    BufferedFile *bf = file;
    int len;
    if (bf->buf_ptr >= bf->buf_end) {
        if (bf->fd >= 0) {
            len = 8192;
            len = read(bf->fd, bf->buffer, len);
            if (len < 0)
                len = 0;
        } else {
            len = 0;
        }
        total_bytes += len;
        bf->buf_ptr = bf->buffer;
        bf->buf_end = bf->buffer + len;
        *bf->buf_end = '\\';
    }
    if (bf->buf_ptr < bf->buf_end) {
        return bf->buf_ptr[0];
    } else {
        bf->buf_ptr = bf->buf_end;
        return (-1);
    }
}
static inline void inp(void)
{
    ch = *(++(file->buf_ptr));
    if (ch == '\\')
        ch = handle_eob();
}
static int handle_stray_noerror(void)
{
    while (ch == '\\') {
        inp();
        if (ch == '\n') {
            file->line_num++;
            inp();
        } else if (ch == '\r') {
            inp();
            if (ch != '\n')
                goto fail;
            file->line_num++;
            inp();
        } else {
        fail:
            return 1;
        }
    }
    return 0;
}
static void handle_stray(void)
{
    if (handle_stray_noerror())
        tcc_error("stray '\\' in program");
}
static int handle_stray1(uint8_t *p)
{
    int c;
    file->buf_ptr = p;
    if (p >= file->buf_end) {
        c = handle_eob();
        if (c != '\\')
            return c;
        p = file->buf_ptr;
    }
    ch = *p;
    if (handle_stray_noerror()) {
        if (!(parse_flags & 0x0020))
            tcc_error("stray '\\' in program");
        *--file->buf_ptr = '\\';
    }
    p = file->buf_ptr;
    c = *p;
    return c;
}
static void minp(void)
{
    inp();
    if (ch == '\\')
        handle_stray();
}
static uint8_t *parse_line_comment(uint8_t *p)
{
    int c;
    p++;
    for(;;) {
        c = *p;
    redo:
        if (c == '\n' || c == (-1)) {
            break;
        } else if (c == '\\') {
            file->buf_ptr = p;
            c = handle_eob();
            p = file->buf_ptr;
            if (c == '\\') {
                { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                if (c == '\n') {
                    file->line_num++;
                    { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                } else if (c == '\r') {
                    { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                    if (c == '\n') {
                        file->line_num++;
                        { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                    }
                }
            } else {
                goto redo;
            }
        } else {
            p++;
        }
    }
    return p;
}
static uint8_t *parse_comment(uint8_t *p)
{
    int c;
    p++;
    for(;;) {
        for(;;) {
            c = *p;
            if (c == '\n' || c == '*' || c == '\\')
                break;
            p++;
            c = *p;
            if (c == '\n' || c == '*' || c == '\\')
                break;
            p++;
        }
        if (c == '\n') {
            file->line_num++;
            p++;
        } else if (c == '*') {
            p++;
            for(;;) {
                c = *p;
                if (c == '*') {
                    p++;
                } else if (c == '/') {
                    goto end_of_comment;
                } else if (c == '\\') {
                    file->buf_ptr = p;
                    c = handle_eob();
                    p = file->buf_ptr;
                    if (c == (-1))
                        tcc_error("unexpected end of file in comment");
                    if (c == '\\') {
                        while (c == '\\') {
                            { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                            if (c == '\n') {
                                file->line_num++;
                                { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                            } else if (c == '\r') {
                                { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                                if (c == '\n') {
                                    file->line_num++;
                                    { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                                }
                            } else {
                                goto after_star;
                            }
                        }
                    }
                } else {
                    break;
                }
            }
        after_star: ;
        } else {
            file->buf_ptr = p;
            c = handle_eob();
            p = file->buf_ptr;
            if (c == (-1)) {
                tcc_error("unexpected end of file in comment");
            } else if (c == '\\') {
                p++;
            }
        }
    }
 end_of_comment:
    p++;
    return p;
}
static int set_idnum(int c, int val)
{
    int prev = isidnum_table[c - (-1)];
    isidnum_table[c - (-1)] = val;
    return prev;
}
static inline void skip_spaces(void)
{
    while (isidnum_table[ch - (-1)] & 1)
        minp();
}
static inline int check_space(int t, int *spc)
{
    if (t < 256 && (isidnum_table[t - (-1)] & 1)) {
        if (*spc)
            return 1;
        *spc = 1;
    } else
        *spc = 0;
    return 0;
}
static uint8_t *parse_pp_string(uint8_t *p,
                                int sep, CString *str)
{
    int c;
    p++;
    for(;;) {
        c = *p;
        if (c == sep) {
            break;
        } else if (c == '\\') {
            file->buf_ptr = p;
            c = handle_eob();
            p = file->buf_ptr;
            if (c == (-1)) {
            unterminated_string:
                tcc_error("missing terminating %c character", sep);
            } else if (c == '\\') {
                { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                if (c == '\n') {
                    file->line_num++;
                    p++;
                } else if (c == '\r') {
                    { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
                    if (c != '\n')
                        expect("'\n' after '\r'");
                    file->line_num++;
                    p++;
                } else if (c == (-1)) {
                    goto unterminated_string;
                } else {
                    if (str) {
                        cstr_ccat(str, '\\');
                        cstr_ccat(str, c);
                    }
                    p++;
                }
            }
        } else if (c == '\n') {
            file->line_num++;
            goto add_char;
        } else if (c == '\r') {
            { p++; c = *p; if (c == '\\') { file->buf_ptr = p; c = handle_eob(); p = file->buf_ptr; }};
            if (c != '\n') {
                if (str)
                    cstr_ccat(str, '\r');
            } else {
                file->line_num++;
                goto add_char;
            }
        } else {
        add_char:
            if (str)
                cstr_ccat(str, c);
            p++;
        }
    }
    p++;
    return p;
}

static inline void tok_str_new(TokenString *s) {
    s->str = ((void*)0);
    s->len = s->lastlen = 0;
    s->allocated_len = 0;
    s->last_line_num = -1;
}
static TokenString *tok_str_alloc(void)
{
    TokenString *str = tal_realloc_impl(&tokstr_alloc, 0, sizeof *str);
    tok_str_new(str);
    return str;
}
static int *tok_str_dup(TokenString *s)
{
    int *str;
    str = tal_realloc_impl(&tokstr_alloc, 0, s->len * sizeof(int));
    memcpy(str, s->str, s->len * sizeof(int));
    return str;
}
static void tok_str_free_str(int *str)
{
    tal_free_impl(tokstr_alloc, str);
}
static void tok_str_free(TokenString *str)
{
    tok_str_free_str(str->str);
    tal_free_impl(tokstr_alloc, str);
}
static int *tok_str_realloc(TokenString *s, int new_size)
{
    int *str, size;
    size = s->allocated_len;
    if (size < 16)
        size = 16;
    while (size < new_size)
        size = size * 2;
    if (size > s->allocated_len) {
        str = tal_realloc_impl(&tokstr_alloc, s->str, size * sizeof(int));
        s->allocated_len = size;
        s->str = str;
    }
    return s->str;
}
static void tok_str_add(TokenString *s, int t)
{
    int len, *str;
    len = s->len;
    str = s->str;
    if (len >= s->allocated_len)
        str = tok_str_realloc(s, len + 1);
    str[len++] = t;
    s->len = len;
}
static void begin_macro(TokenString *str, int alloc)
{
    str->alloc = alloc;
    str->prev = macro_stack;
    str->prev_ptr = macro_ptr;
    str->save_line_num = file->line_num;
    macro_ptr = str->str;
    macro_stack = str;
}
static void end_macro(void)
{
    TokenString *str = macro_stack;
    macro_stack = str->prev;
    macro_ptr = str->prev_ptr;
    file->line_num = str->save_line_num;
    if (str->alloc == 2) {
        str->alloc = 3;
    } else {
        tok_str_free(str);
    }
}
static void tok_str_add2(TokenString *s, int t, CValue *cv)
{
    int len, *str;
    len = s->lastlen = s->len;
    str = s->str;
    if (len + 4 >= s->allocated_len)
        str = tok_str_realloc(s, len + 4 + 1);
    str[len++] = t;
    switch(t) {
    case 0xb5:
    case 0xb6:
    case 0xb3:
    case 0xb4:
    case 0xbb:
    case 0xc0:
    case 0xce:
    case 0xcf:
        str[len++] = cv->tab[0];
        break;
    case 0xbe:
    case 0xbf:
    case 0xb9:
    case 0xba:
        {
            size_t nb_words =
                1 + (cv->str.size + sizeof(int) - 1) / sizeof(int);
            if (len + nb_words >= s->allocated_len)
                str = tok_str_realloc(s, len + nb_words + 1);
            str[len] = cv->str.size;
            memcpy(&str[len + 1], cv->str.data, cv->str.size);
            len += nb_words;
        }
        break;
    case 0xbc:
    case 0xb7:
    case 0xb8:
        str[len++] = cv->tab[0];
        str[len++] = cv->tab[1];
        break;
    case 0xbd:
        str[len++] = cv->tab[0];
        str[len++] = cv->tab[1];
        str[len++] = cv->tab[2];
        break;
    default:
        break;
    }
    s->len = len;
}
static void tok_str_add_tok(TokenString *s)
{
    CValue cval;
    if (file->line_num != s->last_line_num) {
        s->last_line_num = file->line_num;
        cval.i = s->last_line_num;
        tok_str_add2(s, 0xc0, &cval);
    }
    tok_str_add2(s, tok, &tokc);
}
static inline void TOK_GET(int *t, const int **pp, CValue *cv)
{
    const int *p = *pp;
    int n, *tab;
    tab = cv->tab;
    switch(*t = *p++) {
    case 0xce:
    case 0xb5:
    case 0xb3:
    case 0xb4:
    case 0xc0:
        cv->i = *p++;
        break;
    case 0xcf:
    case 0xb6:
        cv->i = (unsigned)*p++;
        break;
    case 0xbb:
 tab[0] = *p++;
 break;
    case 0xb9:
    case 0xba:
    case 0xbe:
    case 0xbf:
        cv->str.size = *p++;
        cv->str.data = p;
        p += (cv->str.size + sizeof(int) - 1) / sizeof(int);
        break;
    case 0xbc:
    case 0xb7:
    case 0xb8:
        n = 2;
        goto copy;
    case 0xbd:
        n = 3;
    copy:
        do
            *tab++ = *p++;
        while (--n);
        break;
    default:
        break;
    }
    *pp = p;
}

static inline Sym *define_find(int v)
{
    v -= 256;
    if ((unsigned)v >= (unsigned)(tok_ident - 256))
        return ((void*)0);
    return table_ident[v]->sym_define;
}
static void free_defines(Sym *b)
{
    while (define_stack != b) {
        Sym *top = define_stack;
        define_stack = top->prev;
        tok_str_free_str(top->d);
        sym_free(top);
    }
    while (b) {
        int v = b->v;
        if (v >= 256 && v < tok_ident) {
            Sym **d = &table_ident[v - 256]->sym_define;
            if (!*d)
                *d = b;
        }
        b = b->prev;
    }
}
static Sym *label_find(int v)
{
    v -= 256;
    if ((unsigned)v >= (unsigned)(tok_ident - 256))
        return ((void*)0);
    return table_ident[v]->sym_label;
}
static Sym *label_push(Sym **ptop, int v, int flags)
{
    Sym *s, **ps;
    s = sym_push2(ptop, v, 0, 0);
    s->r = flags;
    ps = &table_ident[v - 256]->sym_label;
    if (ptop == &global_label_stack) {
        while (*ps != ((void*)0))
            ps = &(*ps)->prev_tok;
    }
    s->prev_tok = *ps;
    *ps = s;
    return s;
}

static void parse_escape_string(CString *outstr, const uint8_t *buf, int is_long) {
    int c, n;
    const uint8_t *p;
    p = buf;
    for(;;) {
        c = *p;
        if (c == '\0')
            break;
        if (c == '\\') {
            p++;
            c = *p;
            switch(c) {
            case '0': case '1': case '2': case '3':
            case '4': case '5': case '6': case '7':
                n = c - '0';
                p++;
                c = *p;
                if (isoct(c)) {
                    n = n * 8 + c - '0';
                    p++;
                    c = *p;
                    if (isoct(c)) {
                        n = n * 8 + c - '0';
                        p++;
                    }
                }
                c = n;
                goto add_char_nonext;
            case 'x':
            case 'u':
            case 'U':
                p++;
                n = 0;
                for(;;) {
                    c = *p;
                    if (c >= 'a' && c <= 'f')
                        c = c - 'a' + 10;
                    else if (c >= 'A' && c <= 'F')
                        c = c - 'A' + 10;
                    else if (isnum(c))
                        c = c - '0';
                    else
                        break;
                    n = n * 16 + c;
                    p++;
                }
                c = n;
                goto add_char_nonext;
            case 'a':
                c = '\a';
                break;
            case 'b':
                c = '\b';
                break;
            case 'f':
                c = '\f';
                break;
            case 'n':
                c = '\n';
                break;
            case 'r':
                c = '\r';
                break;
            case 't':
                c = '\t';
                break;
            case 'v':
                c = '\v';
                break;
            case 'e':
                if (!gnu_ext)
                    goto invalid_escape;
                c = 27;
                break;
            case '\'':
            case '\"':
            case '\\':
            case '?':
                break;
            default:
            invalid_escape:
                if (c >= '!' && c <= '~')
                    tcc_warning("unknown escape sequence: \'\\%c\'", c);
                else
                    tcc_warning("unknown escape sequence: \'\\x%x\'", c);
                break;
            }
        } else if (is_long && c >= 0x80) {
            int cont;
            int skip;
            int i;
            if (c < 0xC2) {
             skip = 1; goto invalid_utf8_sequence;
            } else if (c <= 0xDF) {
             cont = 1; n = c & 0x1f;
            } else if (c <= 0xEF) {
             cont = 2; n = c & 0xf;
            } else if (c <= 0xF4) {
             cont = 3; n = c & 0x7;
            } else {
             skip = 1; goto invalid_utf8_sequence;
            }
            for (i = 1; i <= cont; i++) {
                int l = 0x80, h = 0xBF;
                if (i == 1) {
                    switch (c) {
                    case 0xE0: l = 0xA0; break;
                    case 0xED: h = 0x9F; break;
                    case 0xF0: l = 0x90; break;
                    case 0xF4: h = 0x8F; break;
                    }
                }
                if (p[i] < l || p[i] > h) {
                    skip = i; goto invalid_utf8_sequence;
                }
                n = (n << 6) | (p[i] & 0x3f);
            }
            p += 1 + cont;
            c = n;
            goto add_char_nonext;
        invalid_utf8_sequence:
            tcc_warning("ill-formed UTF-8 subsequence starting with: \'\\x%x\'", c);
            c = 0xFFFD;
            p += skip;
            goto add_char_nonext;
        }
        p++;
    add_char_nonext:
        if (!is_long)
            cstr_ccat(outstr, c);
        else {
            cstr_wccat(outstr, c);
        }
    }
    if (!is_long)
        cstr_ccat(outstr, '\0');
    else
        cstr_wccat(outstr, '\0');
}
static void parse_string(const char *s, int len)
{
    uint8_t buf[1000], *p = buf;
    int is_long, sep;
    if ((is_long = *s == 'L'))
        ++s, --len;
    sep = *s++;
    len -= 2;
    if (len >= sizeof buf)
        p = tcc_malloc(len + 1);
    memcpy(p, s, len);
    p[len] = 0;
    cstr_reset(&tokcstr);
    parse_escape_string(&tokcstr, p, is_long);
    if (p != buf)
        tcc_free(p);
    if (sep == '\'') {
        int char_size, i, n, c;
        if (!is_long)
            tok = 0xb3, char_size = 1;
        else
            tok = 0xb4, char_size = sizeof(nwchar_t);
        n = tokcstr.size / char_size - 1;
        if (n < 1)
            tcc_error("empty character constant");
        if (n > 1)
            tcc_warning("multi-character character constant");
        for (c = i = 0; i < n; ++i) {
            if (is_long)
                c = ((nwchar_t *)tokcstr.data)[i];
            else
                c = (c << 8) | ((char *)tokcstr.data)[i];
        }
        tokc.i = c;
    } else {
        tokc.str.size = tokcstr.size;
        tokc.str.data = tokcstr.data;
        if (!is_long)
            tok = 0xb9;
        else
            tok = 0xba;
    }
}
static void bn_lshift(unsigned int *bn, int shift, int or_val)
{
    int i;
    unsigned int v;
    for(i=0;i<2;i++) {
        v = bn[i];
        bn[i] = (v << shift) | or_val;
        or_val = v >> (32 - shift);
    }
}
static void bn_zero(unsigned int *bn)
{
    int i;
    for(i=0;i<2;i++) {
        bn[i] = 0;
    }
}
static void parse_number(const char *p)
{
    int b, t, shift, frac_bits, s, exp_val, ch;
    char *q;
    unsigned int bn[2];
    double d;
    q = token_buf;
    ch = *p++;
    t = ch;
    ch = *p++;
    *q++ = t;
    b = 10;
    if (t == '.') {
        goto float_frac_parse;
    } else if (t == '0') {
        if (ch == 'x' || ch == 'X') {
            q--;
            ch = *p++;
            b = 16;
        } else if (tcc_ext && (ch == 'b' || ch == 'B')) {
            q--;
            ch = *p++;
            b = 2;
        }
    }
    while (1) {
        if (ch >= 'a' && ch <= 'f')
            t = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'F')
            t = ch - 'A' + 10;
        else if (isnum(ch))
            t = ch - '0';
        else
            break;
        if (t >= b)
            break;
        if (q >= token_buf + 1024) {
        num_too_long:
            tcc_error("number too long");
        }
        *q++ = ch;
        ch = *p++;
    }
    if (ch == '.' ||
        ((ch == 'e' || ch == 'E') && b == 10) ||
        ((ch == 'p' || ch == 'P') && (b == 16 || b == 2))) {
        if (b != 10) {
            *q = '\0';
            if (b == 16)
                shift = 4;
            else
                shift = 1;
            bn_zero(bn);
            q = token_buf;
            while (1) {
                t = *q++;
                if (t == '\0') {
                    break;
                } else if (t >= 'a') {
                    t = t - 'a' + 10;
                } else if (t >= 'A') {
                    t = t - 'A' + 10;
                } else {
                    t = t - '0';
                }
                bn_lshift(bn, shift, t);
            }
            frac_bits = 0;
            if (ch == '.') {
                ch = *p++;
                while (1) {
                    t = ch;
                    if (t >= 'a' && t <= 'f') {
                        t = t - 'a' + 10;
                    } else if (t >= 'A' && t <= 'F') {
                        t = t - 'A' + 10;
                    } else if (t >= '0' && t <= '9') {
                        t = t - '0';
                    } else {
                        break;
                    }
                    if (t >= b)
                        tcc_error("invalid digit");
                    bn_lshift(bn, shift, t);
                    frac_bits += shift;
                    ch = *p++;
                }
            }
            if (ch != 'p' && ch != 'P')
                expect("exponent");
            ch = *p++;
            s = 1;
            exp_val = 0;
            if (ch == '+') {
                ch = *p++;
            } else if (ch == '-') {
                s = -1;
                ch = *p++;
            }
            if (ch < '0' || ch > '9')
                expect("exponent digits");
            while (ch >= '0' && ch <= '9') {
                exp_val = exp_val * 10 + ch - '0';
                ch = *p++;
            }
            exp_val = exp_val * s;
            d = (double)bn[1] * 4294967296.0 + (double)bn[0];
            d = ldexp(d, exp_val - frac_bits);
            t = toup(ch);
            if (t == 'F') {
                ch = *p++;
                tok = 0xbb;
                tokc.f = (float)d;
            } else if (t == 'L') {
                ch = *p++;
                tok = 0xbd;
                tokc.ld = (long double)d;
            } else {
                tok = 0xbc;
                tokc.d = d;
            }
        } else {
            if (ch == '.') {
                if (q >= token_buf + 1024)
                    goto num_too_long;
                *q++ = ch;
                ch = *p++;
            float_frac_parse:
                while (ch >= '0' && ch <= '9') {
                    if (q >= token_buf + 1024)
                        goto num_too_long;
                    *q++ = ch;
                    ch = *p++;
                }
            }
            if (ch == 'e' || ch == 'E') {
                if (q >= token_buf + 1024)
                    goto num_too_long;
                *q++ = ch;
                ch = *p++;
                if (ch == '-' || ch == '+') {
                    if (q >= token_buf + 1024)
                        goto num_too_long;
                    *q++ = ch;
                    ch = *p++;
                }
                if (ch < '0' || ch > '9')
                    expect("exponent digits");
                while (ch >= '0' && ch <= '9') {
                    if (q >= token_buf + 1024)
                        goto num_too_long;
                    *q++ = ch;
                    ch = *p++;
                }
            }
            *q = '\0';
            t = toup(ch);
            if (t == 'F') {
                ch = *p++;
                tok = 0xbb;
                tokc.f = strtof(token_buf, ((void*)0));
            } else if (t == 'L') {
                ch = *p++;
                tok = 0xbd;
                tokc.ld = strtold(token_buf, ((void*)0));
            } else {
                tok = 0xbc;
                tokc.d = strtod(token_buf, ((void*)0));
            }
        }
    } else {
        unsigned long long n, n1;
        int lcount, ucount, ov = 0;
        const char *p1;
        *q = '\0';
        q = token_buf;
        if (b == 10 && *q == '0') {
            b = 8;
            q++;
        }
        n = 0;
        while(1) {
            t = *q++;
            if (t == '\0')
                break;
            else if (t >= 'a')
                t = t - 'a' + 10;
            else if (t >= 'A')
                t = t - 'A' + 10;
            else
                t = t - '0';
            if (t >= b)
                tcc_error("invalid digit");
            n1 = n;
            n = n * b + t;
            if (n1 >= 0x1000000000000000ULL && n / b != n1)
                ov = 1;
        }
        lcount = ucount = 0;
        p1 = p;
        for(;;) {
            t = toup(ch);
            if (t == 'L') {
                if (lcount >= 2)
                    tcc_error("three 'l's in integer constant");
                if (lcount && *(p - 1) != ch)
                    tcc_error("incorrect integer suffix: %s", p1);
                lcount++;
                ch = *p++;
            } else if (t == 'U') {
                if (ucount >= 1)
                    tcc_error("two 'u's in integer constant");
                ucount++;
                ch = *p++;
            } else {
                break;
            }
        }
        if (ucount == 0 && b == 10) {
            if (lcount <= (4 == 4)) {
                if (n >= 0x80000000U)
                    lcount = (4 == 4) + 1;
            }
            if (n >= 0x8000000000000000ULL)
                ov = 1, ucount = 1;
        } else {
            if (lcount <= (4 == 4)) {
                if (n >= 0x100000000ULL)
                    lcount = (4 == 4) + 1;
                else if (n >= 0x80000000U)
                    ucount = 1;
            }
            if (n >= 0x8000000000000000ULL)
                ucount = 1;
        }
        if (ov)
            tcc_warning("integer constant overflow");
        tok = 0xb5;
 if (lcount) {
            tok = 0xce;
            if (lcount == 2)
                tok = 0xb7;
 }
 if (ucount)
     ++tok;
        tokc.i = n;
    }
    if (ch)
        tcc_error("invalid number\n");
}
static inline void next_nomacro1(void)
{
    int t, c, is_long, len;
    TokenSym *ts;
    uint8_t *p, *p1;
    unsigned int h;
    p = file->buf_ptr;
 redo_no_start:
    c = *p;
    switch(c) {
    case ' ':
    case '\t':
        tok = c;
        p++;
        if (parse_flags & 0x0010)
            goto keep_tok_flags;
        while (isidnum_table[*p - (-1)] & 1)
            ++p;
        goto redo_no_start;
    case '\f':
    case '\v':
    case '\r':
        p++;
        goto redo_no_start;
    case '\\':
        c = handle_stray1(p);
        p = file->buf_ptr;
        if (c == '\\')
            goto parse_simple;
        if (c != (-1))
            goto redo_no_start;
        {
            TCCState *s1 = tcc_state;
            if ((parse_flags & 0x0004)
                && !(tok_flags & 0x0008)) {
                tok_flags |= 0x0008;
                tok = 10;
                goto keep_tok_flags;
            } else if (!(parse_flags & 0x0001)) {
                tok = (-1);
            } else if (s1->ifdef_stack_ptr != file->ifdef_stack_ptr) {
                tcc_error("missing #endif");
            } else if (s1->include_stack_ptr == s1->include_stack) {
                tok = (-1);
            } else {
                tok_flags &= ~0x0008;
                tcc_close();
                s1->include_stack_ptr--;
                p = file->buf_ptr;
                if (p == file->buffer)
                    tok_flags = 0x0002|0x0001;
                goto redo_no_start;
            }
        }
        break;
    case '\n':
        file->line_num++;
        tok_flags |= 0x0001;
        p++;
maybe_newline:
        if (0 == (parse_flags & 0x0004))
            goto redo_no_start;
        tok = 10;
        goto keep_tok_flags;
    case '#':
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        if ((tok_flags & 0x0001) &&
            (parse_flags & 0x0001)) {
            file->buf_ptr = p;
            p = file->buf_ptr;
            goto maybe_newline;
        } else {
            if (c == '#') {
                p++;
                tok = 0xca;
            } else {
                if (parse_flags & 0x0008) {
                    p = parse_line_comment(p - 1);
                    goto redo_no_start;
                } else {
                    tok = '#';
                }
            }
        }
        break;
    case '$':
        if (!(isidnum_table[c - (-1)] & 2)
         || (parse_flags & 0x0008))
            goto parse_simple;
    case 'a': case 'b': case 'c': case 'd':
    case 'e': case 'f': case 'g': case 'h':
    case 'i': case 'j': case 'k': case 'l':
    case 'm': case 'n': case 'o': case 'p':
    case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x':
    case 'y': case 'z':
    case 'A': case 'B': case 'C': case 'D':
    case 'E': case 'F': case 'G': case 'H':
    case 'I': case 'J': case 'K':
    case 'M': case 'N': case 'O': case 'P':
    case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X':
    case 'Y': case 'Z':
    case '_':
    parse_ident_fast:
        p1 = p;
        h = 1;
        h = ((h) + ((h) << 5) + ((h) >> 27) + (c));
        while (c = *++p, isidnum_table[c - (-1)] & (2|4))
            h = ((h) + ((h) << 5) + ((h) >> 27) + (c));
        len = p - p1;
        if (c != '\\') {
            TokenSym **pts;
            h &= (16384 - 1);
            pts = &hash_ident[h];
            for(;;) {
                ts = *pts;
                if (!ts)
                    break;
                if (ts->len == len && !memcmp(ts->str, p1, len))
                    goto token_found;
                pts = &(ts->hash_next);
            }
            ts = tok_alloc_new(pts, (char *) p1, len);
        token_found: ;
        } else {
            cstr_reset(&tokcstr);
            cstr_cat(&tokcstr, (char *) p1, len);
            p--;
            { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        parse_ident_slow:
            while (isidnum_table[c - (-1)] & (2|4))
            {
                cstr_ccat(&tokcstr, c);
                { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
            }
            ts = tok_alloc(tokcstr.data, tokcstr.size);
        }
        tok = ts->tok;
        break;
    case 'L':
        t = p[1];
        if (t != '\\' && t != '\'' && t != '\"') {
            goto parse_ident_fast;
        } else {
            { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
            if (c == '\'' || c == '\"') {
                is_long = 1;
                goto str_const;
            } else {
                cstr_reset(&tokcstr);
                cstr_ccat(&tokcstr, 'L');
                goto parse_ident_slow;
            }
        }
        break;
    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
    case '8': case '9':
        t = c;
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
    parse_num:
        cstr_reset(&tokcstr);
        for(;;) {
            cstr_ccat(&tokcstr, t);
            if (!((isidnum_table[c - (-1)] & (2|4))
                  || c == '.'
                  || ((c == '+' || c == '-')
                      && (((t == 'e' || t == 'E')
                            && !(parse_flags & 0x0008
                                && ((char*)tokcstr.data)[0] == '0'
                                && toup(((char*)tokcstr.data)[1]) == 'X'))
                          || t == 'p' || t == 'P'))))
                break;
            t = c;
            { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        }
        cstr_ccat(&tokcstr, '\0');
        tokc.str.size = tokcstr.size;
        tokc.str.data = tokcstr.data;
        tok = 0xbe;
        break;
    case '.':
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        if (isnum(c)) {
            t = '.';
            goto parse_num;
        } else if ((isidnum_table['.' - (-1)] & 2)
                   && (isidnum_table[c - (-1)] & (2|4))) {
            *--p = c = '.';
            goto parse_ident_fast;
        } else if (c == '.') {
            { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
            if (c == '.') {
                p++;
                tok = 0xc8;
            } else {
                *--p = '.';
                tok = '.';
            }
        } else {
            tok = '.';
        }
        break;
    case '\'':
    case '\"':
        is_long = 0;
    str_const:
        cstr_reset(&tokcstr);
        if (is_long)
            cstr_ccat(&tokcstr, 'L');
        cstr_ccat(&tokcstr, c);
        p = parse_pp_string(p, c, &tokcstr);
        cstr_ccat(&tokcstr, c);
        cstr_ccat(&tokcstr, '\0');
        tokc.str.size = tokcstr.size;
        tokc.str.data = tokcstr.data;
        tok = 0xbf;
        break;
    case '<':
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        if (c == '=') {
            p++;
            tok = 0x9e;
        } else if (c == '<') {
            { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
            if (c == '=') {
                p++;
                tok = 0x81;
            } else {
                tok = 0x01;
            }
        } else {
            tok = 0x9c;
        }
        break;
    case '>':
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        if (c == '=') {
            p++;
            tok = 0x9d;
        } else if (c == '>') {
            { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
            if (c == '=') {
                p++;
                tok = 0x82;
            } else {
                tok = 0x02;
            }
        } else {
            tok = 0x9f;
        }
        break;
    case '&':
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        if (c == '&') {
            p++;
            tok = 0xa0;
        } else if (c == '=') {
            p++;
            tok = 0xa6;
        } else {
            tok = '&';
        }
        break;
    case '|':
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        if (c == '|') {
            p++;
            tok = 0xa1;
        } else if (c == '=') {
            p++;
            tok = 0xfc;
        } else {
            tok = '|';
        }
        break;
    case '+':
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        if (c == '+') {
            p++;
            tok = 0xa4;
        } else if (c == '=') {
            p++;
            tok = 0xab;
        } else {
            tok = '+';
        }
        break;
    case '-':
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        if (c == '-') {
            p++;
            tok = 0xa2;
        } else if (c == '=') {
            p++;
            tok = 0xad;
        } else if (c == '>') {
            p++;
            tok = 0xc7;
        } else {
            tok = '-';
        }
        break;
    case '!': { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }}; if (c == '=') { p++; tok = 0x95; } else { tok = '!'; } break;
    case '=': { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }}; if (c == '=') { p++; tok = 0x94; } else { tok = '='; } break;
    case '*': { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }}; if (c == '=') { p++; tok = 0xaa; } else { tok = '*'; } break;
    case '%': { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }}; if (c == '=') { p++; tok = 0xa5; } else { tok = '%'; } break;
    case '^': { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }}; if (c == '=') { p++; tok = 0xde; } else { tok = '^'; } break;
    case '/':
        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
        if (c == '*') {
            p = parse_comment(p);
            tok = ' ';
            goto keep_tok_flags;
        } else if (c == '/') {
            p = parse_line_comment(p);
            tok = ' ';
            goto keep_tok_flags;
        } else if (c == '=') {
            p++;
            tok = 0xaf;
        } else {
            tok = '/';
        }
        break;
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case ',':
    case ';':
    case ':':
    case '?':
    case '~':
    case '@':
    parse_simple:
        tok = c;
        p++;
        break;
    default:
        if (c >= 0x80 && c <= 0xFF)
     goto parse_ident_fast;
        if (parse_flags & 0x0008)
            goto parse_simple;
        tcc_error("unrecognized character \\x%02x", c);
        break;
    }
    tok_flags = 0;
keep_tok_flags:
    file->buf_ptr = p;
}
static void next_nomacro_spc(void)
{
    if (macro_ptr) {
    redo:
        tok = *macro_ptr;
        if (tok) {
            TOK_GET(&tok, &macro_ptr, &tokc);
            if (tok == 0xc0) {
                file->line_num = tokc.i;
                goto redo;
            }
        }
    } else {
        next_nomacro1();
    }
}
static void next_nomacro(void)
{
    do {
        next_nomacro_spc();
    } while (tok < 256 && (isidnum_table[tok - (-1)] & 1));
}

static int *macro_arg_subst(Sym **nested_list, const int *macro_str, Sym *args) {
exit(1);
}

static inline int *macro_twosharps(const int *ptr0)
{
    int t;
    CValue cval;
    TokenString macro_str1;
    int start_of_nosubsts = -1;
    const int *ptr;
    for (ptr = ptr0;;) {
        TOK_GET(&t, &ptr, &cval);
        if (t == 0xcd)
            break;
        if (t == 0)
            return ((void*)0);
    }
    tok_str_new(&macro_str1);
    for (ptr = ptr0;;) {
        TOK_GET(&t, &ptr, &cval);
        if (t == 0)
            break;
        if (t == 0xcd)
            continue;
        while (*ptr == 0xcd) {
            int t1; CValue cv1;
            if (start_of_nosubsts >= 0)
                macro_str1.len = start_of_nosubsts;
            while ((t1 = *++ptr) == 0xcc)
                ;
            if (t1 && t1 != 0xcd) {
                TOK_GET(&t1, &ptr, &cv1);
                if (t != 0xcb || t1 != 0xcb) {
                    tok_str_add2(&macro_str1, t, &cval);
                    t = t1, cval = cv1;
                }
            }
        }
        if (t == 0xcc) {
            if (start_of_nosubsts < 0)
                start_of_nosubsts = macro_str1.len;
        } else {
            start_of_nosubsts = -1;
        }
        tok_str_add2(&macro_str1, t, &cval);
    }
    tok_str_add(&macro_str1, 0);
    return macro_str1.str;
}
static int next_argstream(Sym **nested_list, TokenString *ws_str)
{
    int t;
    const int *p;
    Sym *sa;
    for (;;) {
        if (macro_ptr) {
            p = macro_ptr, t = *p;
            if (ws_str) {
                while (is_space(t) || 10 == t || 0xcb == t)
                    tok_str_add(ws_str, t), t = *++p;
            }
            if (t == 0) {
                end_macro();
                sa = *nested_list;
                while (sa && sa->v == 0)
                    sa = sa->prev;
                if (sa)
                    sa->v = 0;
                continue;
            }
        } else {
            ch = handle_eob();
            if (ws_str) {
                while (is_space(ch) || ch == '\n' || ch == '/') {
                    if (ch == '/') {
                        int c;
                        uint8_t *p = file->buf_ptr;
                        { p++; c = *p; if (c == '\\') { c = handle_stray1(p); p = file->buf_ptr; }};
                        if (c == '*') {
                            p = parse_comment(p);
                            file->buf_ptr = p - 1;
                        } else if (c == '/') {
                            p = parse_line_comment(p);
                            file->buf_ptr = p - 1;
                        } else
                            break;
                        ch = ' ';
                    }
                    if (ch == '\n')
                        file->line_num++;
                    if (!(ch == '\f' || ch == '\v' || ch == '\r'))
                        tok_str_add(ws_str, ch);
                    minp();
                }
            }
            t = ch;
        }
        if (ws_str)
            return t;
        next_nomacro_spc();
        return tok;
    }
}

static int macro_subst_tok(
    TokenString *tok_str,
    Sym **nested_list,
    Sym *s) {
exit(1);
}

static void next(void)
{
 redo:
    if (parse_flags & 0x0010)
        next_nomacro_spc();
    else
        next_nomacro();
    if (macro_ptr) {
        if (tok == 0xcc || tok == 0xcb) {
            goto redo;
        } else if (tok == 0) {
            end_macro();
            goto redo;
        }
    }
    if (tok == 0xbe) {
        if (parse_flags & 0x0002)
            parse_number((char *)tokc.str.data);
    } else if (tok == 0xbf) {
        if (parse_flags & 0x0040)
            parse_string((char *)tokc.str.data, tokc.str.size - 1);
    }
}
static inline void unget_tok(int last_tok)
{
    TokenString *str = tok_str_alloc();
    tok_str_add2(str, tok, &tokc);
    tok_str_add(str, 0);
    begin_macro(str, 1);
    tok = last_tok;
}
static void preprocess_start(TCCState *s1, int is_asm)
{
    CString cstr;
    int i;
    s1->include_stack_ptr = s1->include_stack;
    s1->ifdef_stack_ptr = s1->ifdef_stack;
    file->ifdef_stack_ptr = s1->ifdef_stack_ptr;
    pp_expr = 0;
    pp_counter = 0;
    pp_debug_tok = pp_debug_symv = 0;
    pp_once++;
    pvtop = vtop = (__vstack + 1) - 1;
    s1->pack_stack[0] = 0;
    s1->pack_stack_ptr = s1->pack_stack;
    set_idnum('$', s1->dollars_in_identifiers ? 2 : 0);
    set_idnum('.', is_asm ? 2 : 0);
    cstr_new(&cstr);
    cstr_cat(&cstr, "\"", -1);
    cstr_cat(&cstr, file->filename, -1);
    cstr_cat(&cstr, "\"", 0);
    cstr_reset(&cstr);
    for (i = 0; i < s1->nb_cmd_include_files; i++) {
        cstr_cat(&cstr, "#include \"", -1);
        cstr_cat(&cstr, s1->cmd_include_files[i], -1);
        cstr_cat(&cstr, "\"\n", -1);
    }
    if (cstr.size) {
        *s1->include_stack_ptr++ = file;
 tcc_open_bf(s1, "<command line>", cstr.size);
 memcpy(file->buffer, cstr.data, cstr.size);
    }
    cstr_free(&cstr);
    parse_flags = is_asm ? 0x0008 : 0;
    tok_flags = 0x0001 | 0x0002;
}
static void preprocess_end(TCCState *s1)
{
    while (macro_stack)
        end_macro();
    macro_ptr = ((void*)0);
}
static void tccpp_new(TCCState *s)
{
    int i, c;
    const char *p, *r;
    s->include_stack_ptr = s->include_stack;
    s->ppfp = stdout;
    for(i = (-1); i<128; i++)
        set_idnum(i,
            is_space(i) ? 1
            : isid(i) ? 2
            : isnum(i) ? 4
            : 0);
    for(i = 128; i<256; i++)
        set_idnum(i, 2);
    tal_new(&toksym_alloc, 256, (768 * 1024));
    tal_new(&tokstr_alloc, 128, (768 * 1024));
    tal_new(&cstr_alloc, 1024, (256 * 1024));
    memset(hash_ident, 0, 16384 * sizeof(TokenSym *));
    cstr_new(&cstr_buf);
    cstr_realloc(&cstr_buf, 1024);
    tok_str_new(&tokstr_buf);
    tok_str_realloc(&tokstr_buf, 256);
    tok_ident = 256;
    p = tcc_keywords;
    while (*p) {
        r = p;
        for(;;) {
            c = *r++;
            if (c == '\0')
                break;
        }
        tok_alloc(p, r - p - 1);
        p = r;
    }
}
static void tccpp_delete(TCCState *s)
{
    int i, n;
    free_defines(((void*)0));
    n = tok_ident - 256;
    for(i = 0; i < n; i++)
        tal_free_impl(toksym_alloc, table_ident[i]);
    tcc_free(table_ident);
    table_ident = ((void*)0);
    cstr_free(&tokcstr);
    cstr_free(&cstr_buf);
    cstr_free(&macro_equal_buf);
    tok_str_free_str(tokstr_buf.str);
    tal_delete(toksym_alloc);
    toksym_alloc = ((void*)0);
    tal_delete(tokstr_alloc);
    tokstr_alloc = ((void*)0);
    tal_delete(cstr_alloc);
    cstr_alloc = ((void*)0);
}
static void tok_print(const char *msg, const int *str)
{
    FILE *fp;
    int t, s = 0;
    CValue cval;
    fp = tcc_state->ppfp;
    fprintf(fp, "%s", msg);
    while (str) {
 TOK_GET(&t, &str, &cval);
 if (!t)
     break;
 fprintf(fp, " %s" + s, get_tok_str(t, &cval)), s = 1;
    }
    fprintf(fp, "\n");
}
static void pp_line(TCCState *s1, BufferedFile *f, int level)
{
    int d = f->line_num - f->line_ref;
    if (s1->dflag & 4)
 return;
    if (s1->Pflag == LINE_MACRO_OUTPUT_FORMAT_NONE) {
        ;
    } else if (level == 0 && f->line_ref && d < 8) {
 while (d > 0)
     fputs("\n", s1->ppfp), --d;
    } else if (s1->Pflag == LINE_MACRO_OUTPUT_FORMAT_STD) {
 fprintf(s1->ppfp, "#line %d \"%s\"\n", f->line_num, f->filename);
    } else {
 fprintf(s1->ppfp, "# %d \"%s\"%s\n", f->line_num, f->filename,
     level > 0 ? " 1" : level < 0 ? " 2" : "");
    }
    f->line_ref = f->line_num;
}
static void define_print(TCCState *s1, int v)
{
    FILE *fp;
    Sym *s;
    if (((void*)0) == s || ((void*)0) == s->d)
        return;
    fp = s1->ppfp;
    fprintf(fp, "#define %s", get_tok_str(v, ((void*)0)));
    if (s->type.t == 1) {
        Sym *a = s->next;
        fprintf(fp,"(");
        if (a)
            for (;;) {
                fprintf(fp,"%s", get_tok_str(a->v & ~0x20000000, ((void*)0)));
                if (!(a = a->next))
                    break;
                fprintf(fp,",");
            }
        fprintf(fp,")");
    }
    tok_print("", s->d);
}

static int pp_need_space(int a, int b)
{
    return 'E' == a ? '+' == b || '-' == b
        : '+' == a ? 0xa4 == b || '+' == b
        : '-' == a ? 0xa2 == b || '-' == b
        : a >= 256 ? b >= 256
 : a == 0xbe ? b >= 256
        : 0;
}
static int pp_check_he0xE(int t, const char *p)
{
    if (t == 0xbe && toup(strchr(p, 0)[-1]) == 'E')
        return 'E';
    return t;
}
static int rsym, anon_sym, ind, loc;
static Sym *sym_free_first;
static void **sym_pools;
static int nb_sym_pools;
static Sym *global_stack;
static Sym *local_stack;
static Sym *define_stack;
static Sym *global_label_stack;
static Sym *local_label_stack;
static int local_scope;
static int in_sizeof;
static int section_sym;
static int vlas_in_scope;
static int vla_sp_root_loc;
static int vla_sp_loc;
static SValue __vstack[1+256], *vtop, *pvtop;
static int const_wanted;
static int nocode_wanted;
static int global_expr;
static CType func_vt;
static int func_var;
static int func_vc;
static int last_line_num, last_ind, func_ind;
static const char *funcname;
static int g_debug;
static CType char_pointer_type, func_old_type, int_type, size_type, ptrdiff_type;
static struct switch_t {
    struct case_t {
        int64_t v1, v2;
 int sym;
    } **p; int n;
    int def_sym;
} *cur_switch;
static void gen_cast(CType *type);
static void gen_cast_s(int t);
static inline CType *pointed_type(CType *type);
static int is_compatible_types(CType *type1, CType *type2);
static int parse_btype(CType *type, AttributeDef *ad);
static CType *type_decl(CType *type, AttributeDef *ad, int *v, int td);
static void parse_expr_type(CType *type);
static void init_putv(CType *type, Section *sec, unsigned long c);
static void decl_initializer(CType *type, Section *sec, unsigned long c, int first, int size_only);
static void block(int *bsym, int *csym, int is_expr);
static void decl_initializer_alloc(CType *type, AttributeDef *ad, int r, int has_init, int v, int scope);
static void decl(int l);
static int decl0(int l, int is_for_loop_init, Sym *);
static void expr_eq(void);
static void vla_runtime_type_size(CType *type, int *a);
static void vla_sp_restore(void);
static void vla_sp_restore_root(void);
static int is_compatible_unqualified_types(CType *type1, CType *type2);
static inline int64_t expr_const64(void);
static void vpush64(int ty, unsigned long long v);
static void vpush(CType *type);
static int gvtst(int inv, int t);
static void gen_inline_functions(TCCState *s);
static void skip_or_save_block(TokenString **str);
static void gv_dup(void);
static inline int is_float(int t)
{
    int bt;
    bt = t & 0x000f;
    return bt == 10 || bt == 9 || bt == 8 || bt == 14;
}
static int ieee_finite(double d)
{
    int p[4];
    memcpy(p, &d, sizeof(double));
    return ((unsigned)((p[1] | 0x800fffff) + 1)) >> 31;
}
static void test_lvalue(void)
{
    if (!(vtop->r & 0x0100))
        expect("lvalue");
}
static void check_vstack(void)
{
    if (pvtop != vtop)
        tcc_error("internal compiler error: vstack leak (%d)", vtop - pvtop);
}

static void tcc_debug_start(TCCState *s1) {
    put_elf_sym(symtab_section, 0, 0,
                (((0) << 4) + ((4) & 0xf)), 0,
                0xfff1, file->filename);
}

static void tcc_debug_end(TCCState *s1) {
    return;
}

static void tcc_debug_line(TCCState *s1) {
    return;
}

static void tcc_debug_funcstart(TCCState *s1, Sym *sym) {
    return;
}

static void tcc_debug_funcend(TCCState *s1, int size) {
    return;
}

static int tccgen_compile(TCCState *s1)
{
    cur_text_section = ((void*)0);
    funcname = "";
    anon_sym = 0x10000000;
    section_sym = 0;
    const_wanted = 0;
    nocode_wanted = 0x80000000;
    int_type.t = 3;
    char_pointer_type.t = 1;
    mk_pointer(&char_pointer_type);
    size_type.t = 3 | 0x0010;
    ptrdiff_type.t = 3;
    func_old_type.t = 6;
    func_old_type.ref = sym_push(0x20000000, &int_type, 0, 0);
    func_old_type.ref->f.func_call = 0;
    func_old_type.ref->f.func_type = 2;
    tcc_debug_start(s1);
    parse_flags = 0x0001 | 0x0002 | 0x0040;
    next();
    decl(0x0030);
    check_vstack();
    tcc_debug_end(s1);
    return 0;
}
static Elf32_Sym *elfsym(Sym *s)
{
  if (!s || !s->c)
    return ((void*)0);
  return &((Elf32_Sym *)symtab_section->data)[s->c];
}
static void update_storage(Sym *sym)
{
    Elf32_Sym *esym;
    int sym_bind, old_sym_bind;
    esym = elfsym(sym);
    if (!esym)
        return;
    if (sym->a.visibility)
        esym->st_other = (esym->st_other & ~((-1) & 0x03))
            | sym->a.visibility;
    if (sym->type.t & 0x00002000)
        sym_bind = 0;
    else if (sym->a.weak)
        sym_bind = 2;
    else
        sym_bind = 1;
    old_sym_bind = (((unsigned char) (esym->st_info)) >> 4);
    if (sym_bind != old_sym_bind) {
        esym->st_info = (((sym_bind) << 4) + ((((esym->st_info) & 0xf)) & 0xf));
    }
}
static void put_extern_sym2(Sym *sym, int sh_num,
                            Elf32_Addr value, unsigned long size,
                            int can_add_underscore)
{
    int sym_type, sym_bind, info, other, t;
    Elf32_Sym *esym;
    const char *name;
    char buf1[256];
    char buf[32];
    if (!sym->c) {
        name = get_tok_str(sym->v, ((void*)0));
        t = sym->type.t;
        if ((t & 0x000f) == 6) {
            sym_type = 2;
        } else if ((t & 0x000f) == 0) {
            sym_type = 0;
        } else {
            sym_type = 1;
        }
        if (t & 0x00002000)
            sym_bind = 0;
        else
            sym_bind = 1;
        other = 0;
        if (tcc_state->leading_underscore && can_add_underscore) {
            buf1[0] = '_';
            pstrcpy(buf1 + 1, sizeof(buf1) - 1, name);
            name = buf1;
        }
        if (sym->asm_label)
            name = get_tok_str(sym->asm_label, ((void*)0));
        info = (((sym_bind) << 4) + ((sym_type) & 0xf));
        sym->c = put_elf_sym(symtab_section, value, size, info, other, sh_num, name);
    } else {
        esym = elfsym(sym);
        esym->st_value = value;
        esym->st_size = size;
        esym->st_shndx = sh_num;
    }
    update_storage(sym);
}
static void put_extern_sym(Sym *sym, Section *section,
                           Elf32_Addr value, unsigned long size)
{
    int sh_num = section ? section->sh_num : 0;
    put_extern_sym2(sym, sh_num, value, size, 1);
}
static void greloca(Section *s, Sym *sym, unsigned long offset, int type,
                     Elf32_Addr addend)
{
    int c = 0;
    if (nocode_wanted && s == cur_text_section)
        return;
    if (sym) {
        if (0 == sym->c)
            put_extern_sym(sym, ((void*)0), 0, 0);
        c = sym->c;
    }
    put_elf_reloca(symtab_section, s, offset, type, c, addend);
}
static void greloc(Section *s, Sym *sym, unsigned long offset, int type)
{
    greloca(s, sym, offset, type, 0);
}
static Sym *__sym_malloc(void)
{
    Sym *sym_pool, *sym, *last_sym;
    int i;
    sym_pool = tcc_malloc((8192 / sizeof(Sym)) * sizeof(Sym));
    dynarray_add(&sym_pools, &nb_sym_pools, sym_pool);
    last_sym = sym_free_first;
    sym = sym_pool;
    for(i = 0; i < (8192 / sizeof(Sym)); i++) {
        sym->next = last_sym;
        last_sym = sym;
        sym++;
    }
    sym_free_first = last_sym;
    return last_sym;
}
static inline Sym *sym_malloc(void)
{
    Sym *sym;
    sym = sym_free_first;
    if (!sym)
        sym = __sym_malloc();
    sym_free_first = sym->next;
    return sym;
}
static inline void sym_free(Sym *sym)
{
    sym->next = sym_free_first;
    sym_free_first = sym;
}
static Sym *sym_push2(Sym **ps, int v, int t, int c)
{
    Sym *s;
    s = sym_malloc();
    memset(s, 0, sizeof *s);
    s->v = v;
    s->type.t = t;
    s->c = c;
    s->prev = *ps;
    *ps = s;
    return s;
}
static Sym *sym_find2(Sym *s, int v)
{
    while (s) {
        if (s->v == v)
            return s;
        else if (s->v == -1)
            return ((void*)0);
        s = s->prev;
    }
    return ((void*)0);
}
static inline Sym *struct_find(int v)
{
    v -= 256;
    if ((unsigned)v >= (unsigned)(tok_ident - 256))
        return ((void*)0);
    return table_ident[v]->sym_struct;
}
static inline Sym *sym_find(int v)
{
    v -= 256;
    if ((unsigned)v >= (unsigned)(tok_ident - 256))
        return ((void*)0);
    return table_ident[v]->sym_identifier;
}
static Sym *sym_push(int v, CType *type, int r, int c)
{
    Sym *s, **ps;
    TokenSym *ts;
    if (local_stack)
        ps = &local_stack;
    else
        ps = &global_stack;
    s = sym_push2(ps, v, type->t, c);
    s->type.ref = type->ref;
    s->r = r;
    if (!(v & 0x20000000) && (v & ~0x40000000) < 0x10000000) {
        ts = table_ident[(v & ~0x40000000) - 256];
        if (v & 0x40000000)
            ps = &ts->sym_struct;
        else
            ps = &ts->sym_identifier;
        s->prev_tok = *ps;
        *ps = s;
        s->sym_scope = local_scope;
        if (s->prev_tok && s->prev_tok->sym_scope == s->sym_scope)
            tcc_error("redeclaration of '%s'",
                get_tok_str(v & ~0x40000000, ((void*)0)));
    }
    return s;
}
static Sym *global_identifier_push(int v, int t, int c)
{
    Sym *s, **ps;
    s = sym_push2(&global_stack, v, t, c);
    if (v < 0x10000000) {
        ps = &table_ident[v - 256]->sym_identifier;
        while (*ps != ((void*)0) && (*ps)->sym_scope)
            ps = &(*ps)->prev_tok;
        s->prev_tok = *ps;
        *ps = s;
    }
    return s;
}
static void sym_pop(Sym **ptop, Sym *b, int keep)
{
    Sym *s, *ss, **ps;
    TokenSym *ts;
    int v;
    s = *ptop;
    while(s != b) {
        ss = s->prev;
        v = s->v;
        if (!(v & 0x20000000) && (v & ~0x40000000) < 0x10000000) {
            ts = table_ident[(v & ~0x40000000) - 256];
            if (v & 0x40000000)
                ps = &ts->sym_struct;
            else
                ps = &ts->sym_identifier;
            *ps = s->prev_tok;
        }
 if (!keep)
     sym_free(s);
        s = ss;
    }
    if (!keep)
 *ptop = b;
}
static void vsetc(CType *type, int r, CValue *vc)
{
    int v;
    if (vtop >= (__vstack + 1) + (256 - 1))
        tcc_error("memory full (vstack)");
    if (vtop >= (__vstack + 1) && !nocode_wanted) {
        v = vtop->r & 0x003f;
        if (v == 0x0033 || (v & ~1) == 0x0034)
            gv(0x0001);
    }
    vtop++;
    vtop->type = *type;
    vtop->r = r;
    vtop->r2 = 0x0030;
    vtop->c = *vc;
    vtop->sym = ((void*)0);
}
static void vswap(void)
{
    SValue tmp;
    if (vtop >= (__vstack + 1) && !nocode_wanted) {
        int v = vtop->r & 0x003f;
        if (v == 0x0033 || (v & ~1) == 0x0034)
            gv(0x0001);
    }
    tmp = vtop[0];
    vtop[0] = vtop[-1];
    vtop[-1] = tmp;
}
static void vpop(void)
{
    int v;
    v = vtop->r & 0x003f;
    if (v == TREG_ST0) {
        o(0xd8dd);
    } else
    if (v == 0x0034 || v == 0x0035) {
        gsym(vtop->c.i);
    }
    vtop--;
}
static void vpush(CType *type)
{
    vset(type, 0x0030, 0);
}
static void vpushi(int v)
{
    CValue cval;
    cval.i = v;
    vsetc(&int_type, 0x0030, &cval);
}
static void vpushs(Elf32_Addr v)
{
  CValue cval;
  cval.i = v;
  vsetc(&size_type, 0x0030, &cval);
}
static void vpush64(int ty, unsigned long long v)
{
    CValue cval;
    CType ctype;
    ctype.t = ty;
    ctype.ref = ((void*)0);
    cval.i = v;
    vsetc(&ctype, 0x0030, &cval);
}
static inline void vpushll(long long v)
{
    vpush64(4, v);
}
static void vset(CType *type, int r, int v)
{
    CValue cval;
    cval.i = v;
    vsetc(type, r, &cval);
}
static void vseti(int r, int v)
{
    CType type;
    type.t = 3;
    type.ref = ((void*)0);
    vset(&type, r, v);
}
static void vpushv(SValue *v)
{
    if (vtop >= (__vstack + 1) + (256 - 1))
        tcc_error("memory full (vstack)");
    vtop++;
    *vtop = *v;
}
static void vdup(void)
{
    vpushv(vtop);
}
static void vrotb(int n)
{
    int i;
    SValue tmp;
    tmp = vtop[-n + 1];
    for(i=-n+1;i!=0;i++)
        vtop[i] = vtop[i+1];
    vtop[0] = tmp;
}
static void vrote(SValue *e, int n)
{
    int i;
    SValue tmp;
    tmp = *e;
    for(i = 0;i < n - 1; i++)
        e[-i] = e[-i - 1];
    e[-n + 1] = tmp;
}
static void vrott(int n)
{
    vrote(vtop, n);
}
static inline void vpushsym(CType *type, Sym *sym)
{
    CValue cval;
    cval.i = 0;
    vsetc(type, 0x0030 | 0x0200, &cval);
    vtop->sym = sym;
}
static Sym *get_sym_ref(CType *type, Section *sec, unsigned long offset, unsigned long size)
{
    int v;
    Sym *sym;
    v = anon_sym++;
    sym = global_identifier_push(v, type->t | 0x00002000, 0);
    sym->type.ref = type->ref;
    sym->r = 0x0030 | 0x0200;
    put_extern_sym(sym, sec, offset, size);
    return sym;
}
static void vpush_ref(CType *type, Section *sec, unsigned long offset, unsigned long size)
{
    vpushsym(type, get_sym_ref(type, sec, offset, size));
}
static Sym *external_global_sym(int v, CType *type, int r)
{
    Sym *s;
    s = sym_find(v);
    if (!s) {
        s = global_identifier_push(v, type->t | 0x00001000, 0);
        s->type.ref = type->ref;
        s->r = r | 0x0030 | 0x0200;
    } else if ((((s)->type.t & (0x000f | (0 | 0x0010))) == (0 | 0x0010))) {
        s->type.t = type->t | (s->type.t & 0x00001000);
        s->type.ref = type->ref;
        update_storage(s);
    }
    return s;
}
static void patch_type(Sym *sym, CType *type)
{
    if (!(type->t & 0x00001000)) {
        if (!(sym->type.t & 0x00001000))
            tcc_error("redefinition of '%s'", get_tok_str(sym->v, ((void*)0)));
        sym->type.t &= ~0x00001000;
    }
    if ((((sym)->type.t & (0x000f | (0 | 0x0010))) == (0 | 0x0010))) {
        sym->type.t = type->t & (sym->type.t | ~0x00002000);
        sym->type.ref = type->ref;
    }
    if (!is_compatible_types(&sym->type, type)) {
        tcc_error("incompatible types for redefinition of '%s'",
                  get_tok_str(sym->v, ((void*)0)));
    } else if ((sym->type.t & 0x000f) == 6) {
        int static_proto = sym->type.t & 0x00002000;
        if ((type->t & 0x00002000) && !static_proto && !(type->t & 0x00008000))
            tcc_warning("static storage ignored for redefinition of '%s'",
                get_tok_str(sym->v, ((void*)0)));
        if (0 == (type->t & 0x00001000)) {
            sym->type.t = (type->t & ~0x00002000) | static_proto;
            if (type->t & 0x00008000)
                sym->type.t = type->t;
            sym->type.ref = type->ref;
        }
    } else {
        if ((sym->type.t & 0x0040) && type->ref->c >= 0) {
            if (sym->type.ref->c < 0)
                sym->type.ref->c = type->ref->c;
            else if (sym->type.ref->c != type->ref->c)
                tcc_error("conflicting type for '%s'", get_tok_str(sym->v, ((void*)0)));
        }
        if ((type->t ^ sym->type.t) & 0x00002000)
            tcc_warning("storage mismatch for redefinition of '%s'",
                get_tok_str(sym->v, ((void*)0)));
    }
}
static void patch_storage(Sym *sym, AttributeDef *ad, CType *type)
{
    if (type)
        patch_type(sym, type);
    sym->a.weak |= ad->a.weak;
    if (ad->a.visibility) {
        int vis = sym->a.visibility;
        int vis2 = ad->a.visibility;
        if (vis == 0)
            vis = vis2;
        else if (vis2 != 0)
            vis = (vis < vis2) ? vis : vis2;
        sym->a.visibility = vis;
    }
    if (ad->a.aligned)
        sym->a.aligned = ad->a.aligned;
    if (ad->asm_label)
        sym->asm_label = ad->asm_label;
    update_storage(sym);
}
static Sym *external_sym(int v, CType *type, int r, AttributeDef *ad)
{
    Sym *s;
    s = sym_find(v);
    if (!s) {
        s = sym_push(v, type, r | 0x0030 | 0x0200, 0);
        s->type.t |= 0x00001000;
        s->a = ad->a;
        s->sym_scope = 0;
    } else {
        if (s->type.ref == func_old_type.ref) {
            s->type.ref = type->ref;
            s->r = r | 0x0030 | 0x0200;
            s->type.t |= 0x00001000;
        }
        patch_storage(s, ad, type);
    }
    return s;
}
static void vpush_global_sym(CType *type, int v)
{
    vpushsym(type, external_global_sym(v, type, 0));
}
static void save_regs(int n)
{
    SValue *p, *p1;
    for(p = (__vstack + 1), p1 = vtop - n; p <= p1; p++)
        save_reg(p->r);
}
static void save_reg(int r)
{
    save_reg_upstack(r, 0);
}
static void save_reg_upstack(int r, int n)
{
    int l, saved, size, align;
    SValue *p, *p1, sv;
    CType *type;
    if ((r &= 0x003f) >= 0x0030)
        return;
    if (nocode_wanted)
        return;
    saved = 0;
    l = 0;
    for(p = (__vstack + 1), p1 = vtop - n; p <= p1; p++) {
        if ((p->r & 0x003f) == r ||
            ((p->type.t & 0x000f) == 4 && (p->r2 & 0x003f) == r)) {
            if (!saved) {
                r = p->r & 0x003f;
                type = &p->type;
                if ((p->r & 0x0100) ||
                    (!is_float(type->t) && (type->t & 0x000f) != 4))
                    type = &int_type;
                size = type_size(type, &align);
                loc = (loc - size) & -align;
                sv.type.t = type->t;
                sv.r = 0x0032 | 0x0100;
                sv.c.i = loc;
                store(r, &sv);
                if (r == TREG_ST0) {
                    o(0xd8dd);
                }
                if ((type->t & 0x000f) == 4) {
                    sv.c.i += 4;
                    store(p->r2, &sv);
                }
                l = loc;
                saved = 1;
            }
            if (p->r & 0x0100) {
                p->r = (p->r & ~(0x003f | 0x8000)) | 0x0031;
            } else {
                p->r = lvalue_type(p->type.t) | 0x0032;
            }
            p->r2 = 0x0030;
            p->c.i = l;
        }
    }
}
static int get_reg(int rc)
{
    int r;
    SValue *p;
    for(r=0;r<5;r++) {
        if (reg_classes[r] & rc) {
            if (nocode_wanted)
                return r;
            for(p=(__vstack + 1);p<=vtop;p++) {
                if ((p->r & 0x003f) == r ||
                    (p->r2 & 0x003f) == r)
                    goto notfound;
            }
            return r;
        }
    notfound: ;
    }
    for(p=(__vstack + 1);p<=vtop;p++) {
        r = p->r2 & 0x003f;
        if (r < 0x0030 && (reg_classes[r] & rc))
            goto save_found;
        r = p->r & 0x003f;
        if (r < 0x0030 && (reg_classes[r] & rc)) {
        save_found:
            save_reg(r);
            return r;
        }
    }
    return -1;
}
static void move_reg(int r, int s, int t)
{
    SValue sv;
    if (r != s) {
        save_reg(r);
        sv.type.t = t;
        sv.type.ref = ((void*)0);
        sv.r = s;
        sv.c.i = 0;
        load(r, &sv);
    }
}
static void gaddrof(void)
{
    vtop->r &= ~0x0100;
    if ((vtop->r & 0x003f) == 0x0031)
        vtop->r = (vtop->r & ~(0x003f | (0x1000 | 0x2000 | 0x4000))) | 0x0032 | 0x0100;
}

static void gbound(void) {
exit(1);
}

static void incr_bf_adr(int o) {
exit(1);
}

static void load_packed_bf(CType *type, int bit_pos, int bit_size) {
exit(1);
}

static void store_packed_bf(int bit_pos, int bit_size) {
exit(1);
}

static int adjust_bf(SValue *sv, int bit_pos, int bit_size) {
exit(1);
}

static int gv(int rc)
{
    int r, bit_pos, bit_size, size, align, rc2;
    if (vtop->type.t & 0x0080) {
        CType type;
        bit_pos = (((vtop->type.t) >> 20) & 0x3f);
        bit_size = (((vtop->type.t) >> (20 + 6)) & 0x3f);
        vtop->type.t &= ~(((1 << (6+6)) - 1) << 20 | 0x0080);
        type.ref = ((void*)0);
        type.t = vtop->type.t & 0x0010;
        if ((vtop->type.t & 0x000f) == 11)
            type.t |= 0x0010;
        r = adjust_bf(vtop, bit_pos, bit_size);
        if ((vtop->type.t & 0x000f) == 4)
            type.t |= 4;
        else
            type.t |= 3;
        if (r == 7) {
            load_packed_bf(&type, bit_pos, bit_size);
        } else {
            int bits = (type.t & 0x000f) == 4 ? 64 : 32;
            gen_cast(&type);
            vpushi(bits - (bit_pos + bit_size));
            gen_op(0x01);
            vpushi(bits - bit_size);
            gen_op(0x02);
        }
        r = gv(rc);
    } else {
        if (is_float(vtop->type.t) &&
            (vtop->r & (0x003f | 0x0100)) == 0x0030) {
            unsigned long offset;
            size = type_size(&vtop->type, &align);
            if ((nocode_wanted > 0))
                size = 0, align = 1;
            offset = section_add(data_section, size, align);
            vpush_ref(&vtop->type, data_section, offset, size);
     vswap();
     init_putv(&vtop->type, data_section, offset);
     vtop->r |= 0x0100;
        }
        if (vtop->r & 0x0800)
            gbound();
        r = vtop->r & 0x003f;
        rc2 = (rc & 0x0002) ? 0x0002 : 0x0001;
        if (rc == 0x0004)
            rc2 = 0x0020;
        if (r >= 0x0030
         || (vtop->r & 0x0100)
         || !(reg_classes[r] & rc)
         || ((vtop->type.t & 0x000f) == 4 && !(reg_classes[vtop->r2] & rc2))
            )
        {
            r = get_reg(rc);
            if ((vtop->type.t & 0x000f) == 4) {
                int addr_type = 3, load_size = 4, load_type = 3;
                unsigned long long ll;
                int r2, original_type;
                original_type = vtop->type.t;
                if ((vtop->r & (0x003f | 0x0100)) == 0x0030) {
                    ll = vtop->c.i;
                    vtop->c.i = ll;
                    load(r, vtop);
                    vtop->r = r;
                    vpushi(ll >> 32);
                } else
                if (vtop->r & 0x0100) {
                    save_reg_upstack(vtop->r, 1);
                    vtop->type.t = load_type;
                    load(r, vtop);
                    vdup();
                    vtop[-1].r = r;
                    vtop->type.t = addr_type;
                    gaddrof();
                    vpushi(load_size);
                    gen_op('+');
                    vtop->r |= 0x0100;
                    vtop->type.t = load_type;
                } else {
                    load(r, vtop);
                    vdup();
                    vtop[-1].r = r;
                    vtop->r = vtop[-1].r2;
                }
                r2 = get_reg(rc2);
                load(r2, vtop);
                vpop();
                vtop->r2 = r2;
                vtop->type.t = original_type;
            } else if ((vtop->r & 0x0100) && !is_float(vtop->type.t)) {
                int t1, t;
                t = vtop->type.t;
                t1 = t;
                if (vtop->r & 0x1000)
                    t = 1;
                else if (vtop->r & 0x2000)
                    t = 2;
                if (vtop->r & 0x4000)
                    t |= 0x0010;
                vtop->type.t = t;
                load(r, vtop);
                vtop->type.t = t1;
            } else {
                load(r, vtop);
            }
        }
        vtop->r = r;
    }
    return r;
}
static void gv2(int rc1, int rc2)
{
    int v;
    v = vtop[0].r & 0x003f;
    if (v != 0x0033 && (v & ~1) != 0x0034 && rc1 <= rc2) {
        vswap();
        gv(rc1);
        vswap();
        gv(rc2);
        if ((vtop[-1].r & 0x003f) >= 0x0030) {
            vswap();
            gv(rc1);
            vswap();
        }
    } else {
        gv(rc2);
        vswap();
        gv(rc1);
        vswap();
        if ((vtop[0].r & 0x003f) >= 0x0030) {
            gv(rc2);
        }
    }
}
static int rc_fret(int t)
{
    return 0x0008;
}
static int reg_fret(int t)
{
    return TREG_ST0;
}
static void lexpand(void)
{
    int u, v;
    u = vtop->type.t & (0x0020 | 0x0010);
    v = vtop->r & (0x003f | 0x0100);
    if (v == 0x0030) {
        vdup();
        vtop[0].c.i >>= 32;
    } else if (v == (0x0100|0x0030) || v == (0x0100|0x0032)) {
        vdup();
        vtop[0].c.i += 4;
    } else {
        gv(0x0001);
        vdup();
        vtop[0].r = vtop[-1].r2;
        vtop[0].r2 = vtop[-1].r2 = 0x0030;
    }
    vtop[0].type.t = vtop[-1].type.t = 3 | u;
}
static void lbuild(int t)
{
    gv2(0x0001, 0x0001);
    vtop[-1].r2 = vtop[0].r;
    vtop[-1].type.t = t;
    vpop();
}
static void gv_dup(void)
{
    int rc, t, r, r1;
    SValue sv;
    t = vtop->type.t;
    if ((t & 0x000f) == 4) {
        if (t & 0x0080) {
            gv(0x0001);
            t = vtop->type.t;
        }
        lexpand();
        gv_dup();
        vswap();
        vrotb(3);
        gv_dup();
        vrotb(4);
        lbuild(t);
        vrotb(3);
        vrotb(3);
        vswap();
        lbuild(t);
        vswap();
    } else
    {
        rc = 0x0001;
        sv.type.t = 3;
        if (is_float(t)) {
            rc = 0x0002;
            sv.type.t = t;
        }
        r = gv(rc);
        r1 = get_reg(rc);
        sv.r = r;
        sv.c.i = 0;
        load(r1, &sv);
        vdup();
        if (r != r1)
            vtop->r = r1;
    }
}
static int gvtst(int inv, int t)
{
    int v = vtop->r & 0x003f;
    if (v != 0x0033 && v != 0x0034 && v != 0x0035) {
        vpushi(0);
        gen_op(0x95);
    }
    if ((vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030) {
        if ((vtop->c.i != 0) != inv)
            t = gjmp(t);
        vtop--;
        return t;
    }
    return gtst(inv, t);
}
static void gen_opl(int op)
{
    int t, a, b, op1, c, i;
    int func;
    unsigned short reg_iret = TREG_EAX;
    unsigned short reg_lret = TREG_EDX;
    SValue tmp;
    switch(op) {
    case '/':
    case 0xb2:
        func = TOK___divdi3;
        goto gen_func;
    case 0xb0:
        func = TOK___udivdi3;
        goto gen_func;
    case '%':
        func = TOK___moddi3;
        goto gen_mod_func;
    case 0xb1:
        func = TOK___umoddi3;
    gen_mod_func:
    gen_func:
        vpush_global_sym(&func_old_type, func);
        vrott(3);
        gfunc_call(2);
        vpushi(0);
        vtop->r = reg_iret;
        vtop->r2 = reg_lret;
        break;
    case '^':
    case '&':
    case '|':
    case '*':
    case '+':
    case '-':
        t = vtop->type.t;
        vswap();
        lexpand();
        vrotb(3);
        lexpand();
        tmp = vtop[0];
        vtop[0] = vtop[-3];
        vtop[-3] = tmp;
        tmp = vtop[-2];
        vtop[-2] = vtop[-3];
        vtop[-3] = tmp;
        vswap();
        if (op == '*') {
            vpushv(vtop - 1);
            vpushv(vtop - 1);
            gen_op(0xc2);
            lexpand();
            for(i=0;i<4;i++)
                vrotb(6);
            tmp = vtop[0];
            vtop[0] = vtop[-2];
            vtop[-2] = tmp;
            gen_op('*');
            vrotb(3);
            vrotb(3);
            gen_op('*');
            gen_op('+');
            gen_op('+');
        } else if (op == '+' || op == '-') {
            if (op == '+')
                op1 = 0xc3;
            else
                op1 = 0xc5;
            gen_op(op1);
            vrotb(3);
            vrotb(3);
            gen_op(op1 + 1);
        } else {
            gen_op(op);
            vrotb(3);
            vrotb(3);
            gen_op(op);
        }
        lbuild(t);
        break;
    case 0x02:
    case 0xc9:
    case 0x01:
        if ((vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030) {
            t = vtop[-1].type.t;
            vswap();
            lexpand();
            vrotb(3);
            c = (int)vtop->c.i;
            vpop();
            if (op != 0x01)
                vswap();
            if (c >= 32) {
                vpop();
                if (c > 32) {
                    vpushi(c - 32);
                    gen_op(op);
                }
                if (op != 0x02) {
                    vpushi(0);
                } else {
                    gv_dup();
                    vpushi(31);
                    gen_op(0x02);
                }
                vswap();
            } else {
                vswap();
                gv_dup();
                vpushi(c);
                gen_op(op);
                vswap();
                vpushi(32 - c);
                if (op == 0x01)
                    gen_op(0xc9);
                else
                    gen_op(0x01);
                vrotb(3);
                vpushi(c);
                if (op == 0x01)
                    gen_op(0x01);
                else
                    gen_op(0xc9);
                gen_op('|');
            }
            if (op != 0x01)
                vswap();
            lbuild(t);
        } else {
            switch(op) {
            case 0x02:
                func = TOK___ashrdi3;
                goto gen_func;
            case 0xc9:
                func = TOK___lshrdi3;
                goto gen_func;
            case 0x01:
                func = TOK___ashldi3;
                goto gen_func;
            }
        }
        break;
    default:
        t = vtop->type.t;
        vswap();
        lexpand();
        vrotb(3);
        lexpand();
        tmp = vtop[-1];
        vtop[-1] = vtop[-2];
        vtop[-2] = tmp;
        op1 = op;
        if (op1 == 0x9c)
            op1 = 0x9e;
        else if (op1 == 0x9f)
            op1 = 0x9d;
        else if (op1 == 0x92)
            op1 = 0x96;
        else if (op1 == 0x97)
            op1 = 0x93;
        a = 0;
        b = 0;
        gen_op(op1);
        if (op == 0x95) {
            b = gvtst(0, 0);
        } else {
            a = gvtst(1, 0);
            if (op != 0x94) {
                vpushi(0x95);
                vtop->r = 0x0033;
                b = gvtst(0, 0);
            }
        }
        op1 = op;
        if (op1 == 0x9c)
            op1 = 0x92;
        else if (op1 == 0x9e)
            op1 = 0x96;
        else if (op1 == 0x9f)
            op1 = 0x97;
        else if (op1 == 0x9d)
            op1 = 0x93;
        gen_op(op1);
        a = gvtst(1, a);
        gsym(b);
        vseti(0x0035, a);
        break;
    }
}
static uint64_t gen_opic_sdiv(uint64_t a, uint64_t b)
{
    uint64_t x = (a >> 63 ? -a : a) / (b >> 63 ? -b : b);
    return (a ^ b) >> 63 ? -x : x;
}
static int gen_opic_lt(uint64_t a, uint64_t b)
{
    return (a ^ (uint64_t)1 << 63) < (b ^ (uint64_t)1 << 63);
}
static void gen_opic(int op)
{
    SValue *v1 = vtop - 1;
    SValue *v2 = vtop;
    int t1 = v1->type.t & 0x000f;
    int t2 = v2->type.t & 0x000f;
    int c1 = (v1->r & (0x003f | 0x0100 | 0x0200)) == 0x0030;
    int c2 = (v2->r & (0x003f | 0x0100 | 0x0200)) == 0x0030;
    uint64_t l1 = c1 ? v1->c.i : 0;
    uint64_t l2 = c2 ? v2->c.i : 0;
    int shm = (t1 == 4) ? 63 : 31;
    if (t1 != 4 && (4 != 8 || t1 != 5))
        l1 = ((uint32_t)l1 |
              (v1->type.t & 0x0010 ? 0 : -(l1 & 0x80000000)));
    if (t2 != 4 && (4 != 8 || t2 != 5))
        l2 = ((uint32_t)l2 |
              (v2->type.t & 0x0010 ? 0 : -(l2 & 0x80000000)));
    if (c1 && c2) {
        switch(op) {
        case '+': l1 += l2; break;
        case '-': l1 -= l2; break;
        case '&': l1 &= l2; break;
        case '^': l1 ^= l2; break;
        case '|': l1 |= l2; break;
        case '*': l1 *= l2; break;
        case 0xb2:
        case '/':
        case '%':
        case 0xb0:
        case 0xb1:
            if (l2 == 0) {
                if (const_wanted)
                    tcc_error("division by zero in constant");
                goto general_case;
            }
            switch(op) {
            default: l1 = gen_opic_sdiv(l1, l2); break;
            case '%': l1 = l1 - l2 * gen_opic_sdiv(l1, l2); break;
            case 0xb0: l1 = l1 / l2; break;
            case 0xb1: l1 = l1 % l2; break;
            }
            break;
        case 0x01: l1 <<= (l2 & shm); break;
        case 0xc9: l1 >>= (l2 & shm); break;
        case 0x02:
            l1 = (l1 >> 63) ? ~(~l1 >> (l2 & shm)) : l1 >> (l2 & shm);
            break;
        case 0x92: l1 = l1 < l2; break;
        case 0x93: l1 = l1 >= l2; break;
        case 0x94: l1 = l1 == l2; break;
        case 0x95: l1 = l1 != l2; break;
        case 0x96: l1 = l1 <= l2; break;
        case 0x97: l1 = l1 > l2; break;
        case 0x9c: l1 = gen_opic_lt(l1, l2); break;
        case 0x9d: l1 = !gen_opic_lt(l1, l2); break;
        case 0x9e: l1 = !gen_opic_lt(l2, l1); break;
        case 0x9f: l1 = gen_opic_lt(l2, l1); break;
        case 0xa0: l1 = l1 && l2; break;
        case 0xa1: l1 = l1 || l2; break;
        default:
            goto general_case;
        }
 if (t1 != 4 && (4 != 8 || t1 != 5))
     l1 = ((uint32_t)l1 |
  (v1->type.t & 0x0010 ? 0 : -(l1 & 0x80000000)));
        v1->c.i = l1;
        vtop--;
    } else {
        if (c1 && (op == '+' || op == '&' || op == '^' ||
                   op == '|' || op == '*')) {
            vswap();
            c2 = c1;
            l2 = l1;
        }
        if (!const_wanted &&
            c1 && ((l1 == 0 &&
                    (op == 0x01 || op == 0xc9 || op == 0x02)) ||
                   (l1 == -1 && op == 0x02))) {
            vtop--;
        } else if (!const_wanted &&
                   c2 && ((l2 == 0 && (op == '&' || op == '*')) ||
                          (op == '|' &&
                            (l2 == -1 || (l2 == 0xFFFFFFFF && t2 != 4))) ||
                          (l2 == 1 && (op == '%' || op == 0xb1)))) {
            if (l2 == 1)
                vtop->c.i = 0;
            vswap();
            vtop--;
        } else if (c2 && (((op == '*' || op == '/' || op == 0xb0 ||
                          op == 0xb2) &&
                           l2 == 1) ||
                          ((op == '+' || op == '-' || op == '|' || op == '^' ||
                            op == 0x01 || op == 0xc9 || op == 0x02) &&
                           l2 == 0) ||
                          (op == '&' &&
                            (l2 == -1 || (l2 == 0xFFFFFFFF && t2 != 4))))) {
            vtop--;
        } else if (c2 && (op == '*' || op == 0xb2 || op == 0xb0)) {
            if (l2 > 0 && (l2 & (l2 - 1)) == 0) {
                int n = -1;
                while (l2) {
                    l2 >>= 1;
                    n++;
                }
                vtop->c.i = n;
                if (op == '*')
                    op = 0x01;
                else if (op == 0xb2)
                    op = 0x02;
                else
                    op = 0xc9;
            }
            goto general_case;
        } else if (c2 && (op == '+' || op == '-') &&
                   (((vtop[-1].r & (0x003f | 0x0100 | 0x0200)) == (0x0030 | 0x0200))
                    || (vtop[-1].r & (0x003f | 0x0100)) == 0x0032)) {
            if (op == '-')
                l2 = -l2;
     l2 += vtop[-1].c.i;
     if ((int)l2 != l2)
         goto general_case;
            vtop--;
            vtop->c.i = l2;
        } else {
        general_case:
                if (t1 == 4 || t2 == 4 ||
                    (4 == 8 && (t1 == 5 || t2 == 5)))
                    gen_opl(op);
                else
                    gen_opi(op);
        }
    }
}

static inline int is_null_pointer(SValue *p)
{
    if ((p->r & (0x003f | 0x0100 | 0x0200)) != 0x0030)
        return 0;
    return ((p->type.t & 0x000f) == 3 && (uint32_t)p->c.i == 0) ||
        ((p->type.t & 0x000f) == 4 && p->c.i == 0) ||
        ((p->type.t & 0x000f) == 5 &&
         (4 == 4 ? (uint32_t)p->c.i == 0 : p->c.i == 0));
}
static inline int is_integer_btype(int bt)
{
    return (bt == 1 || bt == 2 ||
            bt == 3 || bt == 4);
}
static void check_comparison_pointer_types(SValue *p1, SValue *p2, int op)
{
    CType *type1, *type2, tmp_type1, tmp_type2;
    int bt1, bt2;
    if (is_null_pointer(p1) || is_null_pointer(p2))
        return;
    type1 = &p1->type;
    type2 = &p2->type;
    bt1 = type1->t & 0x000f;
    bt2 = type2->t & 0x000f;
    if ((is_integer_btype(bt1) || is_integer_btype(bt2)) && op != '-') {
        if (op != 0xa1 && op != 0xa0 )
            tcc_warning("comparison between pointer and integer");
        return;
    }
    if (bt1 == 5) {
        type1 = pointed_type(type1);
    } else if (bt1 != 6)
        goto invalid_operands;
    if (bt2 == 5) {
        type2 = pointed_type(type2);
    } else if (bt2 != 6) {
    invalid_operands:
        tcc_error("invalid operands to binary %s", get_tok_str(op, ((void*)0)));
    }
    if ((type1->t & 0x000f) == 0 ||
        (type2->t & 0x000f) == 0)
        return;
    tmp_type1 = *type1;
    tmp_type2 = *type2;
    tmp_type1.t &= ~(0x0020 | 0x0010 | 0x0100 | 0x0200);
    tmp_type2.t &= ~(0x0020 | 0x0010 | 0x0100 | 0x0200);
    if (!is_compatible_types(&tmp_type1, &tmp_type2)) {
        if (op == '-')
            goto invalid_operands;
        else
            tcc_warning("comparison of distinct pointer types lacks a cast");
    }
}
static void gen_op(int op)
{
    int u, t1, t2, bt1, bt2, t;
    CType type1;
redo:
    t1 = vtop[-1].type.t;
    t2 = vtop[0].type.t;
    bt1 = t1 & 0x000f;
    bt2 = t2 & 0x000f;
    if (bt1 == 7 || bt2 == 7) {
        tcc_error("operation on a struct");
    } else if (bt1 == 6 || bt2 == 6) {
 if (bt2 == 6) {
     mk_pointer(&vtop->type);
     gaddrof();
 }
 if (bt1 == 6) {
     vswap();
     mk_pointer(&vtop->type);
     gaddrof();
     vswap();
 }
 goto redo;
    } else if (bt1 == 5 || bt2 == 5) {
        if (op >= 0x92 && op <= 0xa1) {
            check_comparison_pointer_types(vtop - 1, vtop, op);
            t = 3 | 0x0010;
            goto std_op;
        }
        if (bt1 == 5 && bt2 == 5) {
            if (op != '-')
                tcc_error("cannot use pointers here");
            check_comparison_pointer_types(vtop - 1, vtop, op);
            vrott(3);
            gen_opic(op);
            vtop->type.t = ptrdiff_type.t;
            vswap();
            gen_op(0xb2);
        } else {
            if (op != '-' && op != '+')
                tcc_error("cannot use pointers here");
            if (bt2 == 5) {
                vswap();
                t = t1, t1 = t2, t2 = t;
            }
            if ((vtop[0].type.t & 0x000f) == 4)
                gen_cast_s(3);
            type1 = vtop[-1].type;
            type1.t &= ~0x0040;
            gen_op('*');
            {
                gen_opic(op);
            }
            vtop->type = type1;
        }
    } else if (is_float(bt1) || is_float(bt2)) {
        if (bt1 == 10 || bt2 == 10) {
            t = 10;
        } else if (bt1 == 9 || bt2 == 9) {
            t = 9;
        } else {
            t = 8;
        }
        if (op != '+' && op != '-' && op != '*' && op != '/' &&
            (op < 0x92 || op > 0x9f))
            tcc_error("invalid operands for binary operation");
        goto std_op;
    } else if (op == 0xc9 || op == 0x02 || op == 0x01) {
        t = bt1 == 4 ? 4 : 3;
        if ((t1 & (0x000f | 0x0010 | 0x0080)) == (t | 0x0010))
          t |= 0x0010;
        t |= (0x0800 & t1);
        goto std_op;
    } else if (bt1 == 4 || bt2 == 4) {
        t = 4 | 0x0800;
        if (bt1 == 4)
            t &= t1;
        if (bt2 == 4)
            t &= t2;
        if ((t1 & (0x000f | 0x0010 | 0x0080)) == (4 | 0x0010) ||
            (t2 & (0x000f | 0x0010 | 0x0080)) == (4 | 0x0010))
            t |= 0x0010;
        goto std_op;
    } else {
        t = 3 | (0x0800 & (t1 | t2));
        if ((t1 & (0x000f | 0x0010 | 0x0080)) == (3 | 0x0010) ||
            (t2 & (0x000f | 0x0010 | 0x0080)) == (3 | 0x0010))
            t |= 0x0010;
    std_op:
        if (t & 0x0010) {
            if (op == 0x02)
                op = 0xc9;
            else if (op == '/')
                op = 0xb0;
            else if (op == '%')
                op = 0xb1;
            else if (op == 0x9c)
                op = 0x92;
            else if (op == 0x9f)
                op = 0x97;
            else if (op == 0x9e)
                op = 0x96;
            else if (op == 0x9d)
                op = 0x93;
        }
        vswap();
        type1.t = t;
        type1.ref = ((void*)0);
        gen_cast(&type1);
        vswap();
        if (op == 0xc9 || op == 0x02 || op == 0x01)
            type1.t = 3;
        gen_cast(&type1);
        gen_opic(op);
        if (op >= 0x92 && op <= 0x9f) {
            vtop->type.t = 3;
        } else {
            vtop->type.t = t;
        }
    }
    if (vtop->r & 0x0100)
        gv(is_float(vtop->type.t & 0x000f) ? 0x0002 : 0x0001);
}
static void gen_cvt_itof1(int t)
{
    if ((vtop->type.t & (0x000f | 0x0010)) ==
        (4 | 0x0010)) {
        if (t == 8)
            vpush_global_sym(&func_old_type, TOK___floatundisf);
        else if (t == 10)
            vpush_global_sym(&func_old_type, TOK___floatundixf);
        else
            vpush_global_sym(&func_old_type, TOK___floatundidf);
        vrott(2);
        gfunc_call(1);
        vpushi(0);
        vtop->r = reg_fret(t);
    } else {
        gen_cvt_itof(t);
    }
}
static void gen_cvt_ftoi1(int t)
{
    int st;
    if (t == (4 | 0x0010)) {
        st = vtop->type.t & 0x000f;
        if (st == 8)
            vpush_global_sym(&func_old_type, TOK___fixunssfdi);
        else if (st == 10)
            vpush_global_sym(&func_old_type, TOK___fixunsxfdi);
        else
            vpush_global_sym(&func_old_type, TOK___fixunsdfdi);
        vrott(2);
        gfunc_call(1);
        vpushi(0);
        vtop->r = TREG_EAX;
        vtop->r2 = TREG_EDX;
    } else {
        gen_cvt_ftoi(t);
    }
}
static void force_charshort_cast(int t)
{
    int bits, dbt;
    if ((nocode_wanted & 0xC0000000))
 return;
    dbt = t & 0x000f;
    if (dbt == 1)
        bits = 8;
    else
        bits = 16;
    if (t & 0x0010) {
        vpushi((1 << bits) - 1);
        gen_op('&');
    } else {
        if ((vtop->type.t & 0x000f) == 4)
            bits = 64 - bits;
        else
            bits = 32 - bits;
        vpushi(bits);
        gen_op(0x01);
        vtop->type.t &= ~0x0010;
        vpushi(bits);
        gen_op(0x02);
    }
}
static void gen_cast_s(int t)
{
    CType type;
    type.t = t;
    type.ref = ((void*)0);
    gen_cast(&type);
}
static void gen_cast(CType *type)
{
    int sbt, dbt, sf, df, c, p;
    if (vtop->r & 0x0400) {
        vtop->r &= ~0x0400;
        force_charshort_cast(vtop->type.t);
    }
    if (vtop->type.t & 0x0080) {
        gv(0x0001);
    }
    dbt = type->t & (0x000f | 0x0010);
    sbt = vtop->type.t & (0x000f | 0x0010);
    if (sbt != dbt) {
        sf = is_float(sbt);
        df = is_float(dbt);
        c = (vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030;
        p = (vtop->r & (0x003f | 0x0100 | 0x0200)) == (0x0030 | 0x0200);
        if (c) {
            if (sbt == 8)
                vtop->c.ld = vtop->c.f;
            else if (sbt == 9)
                vtop->c.ld = vtop->c.d;
            if (df) {
                if ((sbt & 0x000f) == 4) {
                    if ((sbt & 0x0010) || !(vtop->c.i >> 63))
                        vtop->c.ld = vtop->c.i;
                    else
                        vtop->c.ld = -(long double)-vtop->c.i;
                } else if(!sf) {
                    if ((sbt & 0x0010) || !(vtop->c.i >> 31))
                        vtop->c.ld = (uint32_t)vtop->c.i;
                    else
                        vtop->c.ld = -(long double)-(uint32_t)vtop->c.i;
                }
                if (dbt == 8)
                    vtop->c.f = (float)vtop->c.ld;
                else if (dbt == 9)
                    vtop->c.d = (double)vtop->c.ld;
            } else if (sf && dbt == (4|0x0010)) {
                vtop->c.i = vtop->c.ld;
            } else if (sf && dbt == 11) {
                vtop->c.i = (vtop->c.ld != 0);
            } else {
                if(sf)
                    vtop->c.i = vtop->c.ld;
                else if (sbt == (4|0x0010))
                    ;
                else if (sbt & 0x0010)
                    vtop->c.i = (uint32_t)vtop->c.i;
                else if (sbt != 4)
                    vtop->c.i = ((uint32_t)vtop->c.i |
                                  -(vtop->c.i & 0x80000000));
                if (dbt == (4|0x0010))
                    ;
                else if (dbt == 11)
                    vtop->c.i = (vtop->c.i != 0);
                else if (dbt != 4) {
                    uint32_t m = ((dbt & 0x000f) == 1 ? 0xff :
                                  (dbt & 0x000f) == 2 ? 0xffff :
                                  0xffffffff);
                    vtop->c.i &= m;
                    if (!(dbt & 0x0010))
                        vtop->c.i |= -(vtop->c.i & ((m >> 1) + 1));
                }
            }
        } else if (p && dbt == 11) {
            vtop->r = 0x0030;
            vtop->c.i = 1;
        } else {
            if (sf && df) {
                gen_cvt_ftof(dbt);
            } else if (df) {
                gen_cvt_itof1(dbt);
            } else if (sf) {
                if (dbt == 11) {
                     vpushi(0);
                     gen_op(0x95);
                } else {
                    if (dbt != (3 | 0x0010) &&
                        dbt != (4 | 0x0010) &&
                        dbt != 4)
                        dbt = 3;
                    gen_cvt_ftoi1(dbt);
                    if (dbt == 3 && (type->t & (0x000f | 0x0010)) != dbt) {
                        vtop->type.t = dbt;
                        gen_cast(type);
                    }
                }
            } else if ((dbt & 0x000f) == 4) {
                if ((sbt & 0x000f) != 4) {
                    gv(0x0001);
                    if (sbt == (3 | 0x0010)) {
                        vpushi(0);
                        gv(0x0001);
                    } else {
                        if (sbt == 5) {
                            gen_cast_s(3);
                        }
                        gv_dup();
                        vpushi(31);
                        gen_op(0x02);
                    }
                    vtop[-1].r2 = vtop->r;
                    vpop();
                }
            } else if (dbt == 11) {
                vpushi(0);
                gen_op(0x95);
            } else if ((dbt & 0x000f) == 1 ||
                       (dbt & 0x000f) == 2) {
                if (sbt == 5) {
                    vtop->type.t = 3;
                    tcc_warning("nonportable conversion from pointer to char/short");
                }
                force_charshort_cast(dbt);
            } else if ((dbt & 0x000f) == 3) {
                if ((sbt & 0x000f) == 4) {
                    lexpand();
                    vpop();
                }
            }
        }
    } else if ((dbt & 0x000f) == 5 && !(vtop->r & 0x0100)) {
        vtop->r = (vtop->r & ~(0x1000 | 0x2000 | 0x4000))
                  | (lvalue_type(type->ref->type.t) & (0x1000 | 0x2000 | 0x4000));
    }
    vtop->type = *type;
}
static int type_size(CType *type, int *a)
{
    Sym *s;
    int bt;
    bt = type->t & 0x000f;
    if (bt == 7) {
        s = type->ref;
        *a = s->r;
        return s->c;
    } else if (bt == 5) {
        if (type->t & 0x0040) {
            int ts;
            s = type->ref;
            ts = type_size(&s->type, a);
            if (ts < 0 && s->c < 0)
                ts = -ts;
            return ts * s->c;
        } else {
            *a = 4;
            return 4;
        }
    } else if (((type->t & (((1 << (6+6)) - 1) << 20 | 0x0080)) == (2 << 20)) && type->ref->c == -1) {
        return -1;
    } else if (bt == 10) {
        *a = 4;
        return 12;
    } else if (bt == 9 || bt == 4) {
        *a = 4;
        return 8;
    } else if (bt == 3 || bt == 8) {
        *a = 4;
        return 4;
    } else if (bt == 2) {
        *a = 2;
        return 2;
    } else if (bt == 13 || bt == 14) {
        *a = 8;
        return 16;
    } else {
        *a = 1;
        return 1;
    }
}
static void vla_runtime_type_size(CType *type, int *a)
{
    if (type->t & 0x0400) {
        type_size(&type->ref->type, a);
        vset(&int_type, 0x0032|0x0100, type->ref->c);
    } else {
        vpushi(type_size(type, a));
    }
}
static void vla_sp_restore(void) {
    if (vlas_in_scope) {
        gen_vla_sp_restore(vla_sp_loc);
    }
}
static void vla_sp_restore_root(void) {
    if (vlas_in_scope) {
        gen_vla_sp_restore(vla_sp_root_loc);
    }
}
static inline CType *pointed_type(CType *type)
{
    return &type->ref->type;
}
static void mk_pointer(CType *type)
{
    Sym *s;
    s = sym_push(0x20000000, type, 0, -1);
    type->t = 5 | (type->t & (0x00001000 | 0x00002000 | 0x00004000 | 0x00008000));
    type->ref = s;
}
static int is_compatible_func(CType *type1, CType *type2)
{
    Sym *s1, *s2;
    s1 = type1->ref;
    s2 = type2->ref;
    if (!is_compatible_types(&s1->type, &s2->type))
        return 0;
    if (s1->f.func_call != s2->f.func_call)
        return 0;
    if (s1->f.func_type == 2 || s2->f.func_type == 2)
        return 1;
    if (s1->f.func_type != s2->f.func_type)
        return 0;
    while (s1 != ((void*)0)) {
        if (s2 == ((void*)0))
            return 0;
        if (!is_compatible_unqualified_types(&s1->type, &s2->type))
            return 0;
        s1 = s1->next;
        s2 = s2->next;
    }
    if (s2)
        return 0;
    return 1;
}
static int compare_types(CType *type1, CType *type2, int unqualified)
{
    int bt1, t1, t2;
    t1 = type1->t & (~((0x00001000 | 0x00002000 | 0x00004000 | 0x00008000)|(((1 << (6+6)) - 1) << 20 | 0x0080)));
    t2 = type2->t & (~((0x00001000 | 0x00002000 | 0x00004000 | 0x00008000)|(((1 << (6+6)) - 1) << 20 | 0x0080)));
    if (unqualified) {
        t1 &= ~(0x0100 | 0x0200);
        t2 &= ~(0x0100 | 0x0200);
    }
    if ((t1 & 0x000f) != 1) {
        t1 &= ~0x0020;
        t2 &= ~0x0020;
    }
    if (t1 != t2)
        return 0;
    bt1 = t1 & 0x000f;
    if (bt1 == 5) {
        type1 = pointed_type(type1);
        type2 = pointed_type(type2);
        return is_compatible_types(type1, type2);
    } else if (bt1 == 7) {
        return (type1->ref == type2->ref);
    } else if (bt1 == 6) {
        return is_compatible_func(type1, type2);
    } else {
        return 1;
    }
}
static int is_compatible_types(CType *type1, CType *type2)
{
    return compare_types(type1,type2,0);
}
static int is_compatible_unqualified_types(CType *type1, CType *type2)
{
    return compare_types(type1,type2,1);
}

static void gen_assign_cast(CType *dt)
{
    CType *st, *type1, *type2;
    char buf1[256], buf2[256];
    int dbt, sbt;
    st = &vtop->type;
    dbt = dt->t & 0x000f;
    sbt = st->t & 0x000f;
    if (sbt == 0 || dbt == 0) {
 if (sbt == 0 && dbt == 0)
     ;
 else
         tcc_error("cannot cast from/to void");
    }
    if (dt->t & 0x0100)
        tcc_warning("assignment of read-only location");
    switch(dbt) {
    case 5:
        if (is_null_pointer(vtop))
            goto type_ok;
        if (is_integer_btype(sbt)) {
            tcc_warning("assignment makes pointer from integer without a cast");
            goto type_ok;
        }
        type1 = pointed_type(dt);
        if (sbt == 6) {
            if ((type1->t & 0x000f) != 0 &&
                !is_compatible_types(pointed_type(dt), st))
                tcc_warning("assignment from incompatible pointer type");
            goto type_ok;
        }
        if (sbt != 5)
            goto error;
        type2 = pointed_type(st);
        if ((type1->t & 0x000f) == 0 ||
            (type2->t & 0x000f) == 0) {
        } else {
            if (!is_compatible_unqualified_types(type1, type2)) {
  if ((type1->t & (0x000f|0x0800)) != (type2->t & (0x000f|0x0800))
                    || ((type1->t & (((1 << (6+6)) - 1) << 20 | 0x0080)) == (2 << 20)) || ((type2->t & (((1 << (6+6)) - 1) << 20 | 0x0080)) == (2 << 20))
                    )
      tcc_warning("assignment from incompatible pointer type");
     }
        }
        if ((!(type1->t & 0x0100) && (type2->t & 0x0100)) ||
            (!(type1->t & 0x0200) && (type2->t & 0x0200)))
            tcc_warning("assignment discards qualifiers from pointer target type");
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        if (sbt == 5 || sbt == 6) {
            tcc_warning("assignment makes integer from pointer without a cast");
        } else if (sbt == 7) {
            goto case_VT_STRUCT;
        }
        break;
    case 7:
    case_VT_STRUCT:
        if (!is_compatible_unqualified_types(dt, st)) {
        error:
            tcc_error("cannot cast '%s' to '%s'", buf1, buf2);
        }
        break;
    }
 type_ok:
    gen_cast(dt);
}
static void vstore(void)
{
    int sbt, dbt, ft, r, t, size, align, bit_size, bit_pos, rc, delayed_cast;
    ft = vtop[-1].type.t;
    sbt = vtop->type.t & 0x000f;
    dbt = ft & 0x000f;
    if ((((sbt == 3 || sbt == 2) && dbt == 1) ||
         (sbt == 3 && dbt == 2))
 && !(vtop->type.t & 0x0080)) {
        delayed_cast = 0x0400;
        vtop->type.t = ft & (~((0x00001000 | 0x00002000 | 0x00004000 | 0x00008000)|(((1 << (6+6)) - 1) << 20 | 0x0080)));
        if (ft & 0x0100)
            tcc_warning("assignment of read-only location");
    } else {
        delayed_cast = 0;
        if (!(ft & 0x0080))
            gen_assign_cast(&vtop[-1].type);
    }
    if (sbt == 7) {
            size = type_size(&vtop->type, &align);
            vswap();
            vtop->type.t = 5;
            gaddrof();
            vpush_global_sym(&func_old_type, TOK_memmove);
            vswap();
            vpushv(vtop - 2);
            vtop->type.t = 5;
            gaddrof();
            vpushi(size);
            gfunc_call(3);
    } else if (ft & 0x0080) {
        vdup(), vtop[-1] = vtop[-2];
        bit_pos = (((ft) >> 20) & 0x3f);
        bit_size = (((ft) >> (20 + 6)) & 0x3f);
        vtop[-1].type.t = ft & ~(((1 << (6+6)) - 1) << 20 | 0x0080);
        if ((ft & 0x000f) == 11) {
            gen_cast(&vtop[-1].type);
            vtop[-1].type.t = (vtop[-1].type.t & ~0x000f) | (1 | 0x0010);
        }
        r = adjust_bf(vtop - 1, bit_pos, bit_size);
        if (r == 7) {
            gen_cast_s((ft & 0x000f) == 4 ? 4 : 3);
            store_packed_bf(bit_pos, bit_size);
        } else {
            unsigned long long mask = (1ULL << bit_size) - 1;
            if ((ft & 0x000f) != 11) {
                if ((vtop[-1].type.t & 0x000f) == 4)
                    vpushll(mask);
                else
                    vpushi((unsigned)mask);
                gen_op('&');
            }
            vpushi(bit_pos);
            gen_op(0x01);
            vswap();
            vdup();
            vrott(3);
            if ((vtop->type.t & 0x000f) == 4)
                vpushll(~(mask << bit_pos));
            else
                vpushi(~((unsigned)mask << bit_pos));
            gen_op('&');
            gen_op('|');
            vstore();
            vpop();
        }
    } else if (dbt == 0) {
        --vtop;
    } else {
            if (vtop[-1].r & 0x0800) {
                vswap();
                gbound();
                vswap();
            }
            rc = 0x0001;
            if (is_float(ft)) {
                rc = 0x0002;
            }
            r = gv(rc);
            if ((vtop[-1].r & 0x003f) == 0x0031) {
                SValue sv;
                t = get_reg(0x0001);
                sv.type.t = 3;
                sv.r = 0x0032 | 0x0100;
                sv.c.i = vtop[-1].c.i;
                load(t, &sv);
                vtop[-1].r = t | 0x0100;
            }
            if ((ft & 0x000f) == 4) {
                int addr_type = 3, load_size = 4, load_type = 3;
                vtop[-1].type.t = load_type;
                store(r, vtop - 1);
                vswap();
                vtop->type.t = addr_type;
                gaddrof();
                vpushi(load_size);
                gen_op('+');
                vtop->r |= 0x0100;
                vswap();
                vtop[-1].type.t = load_type;
                store(vtop->r2, vtop - 1);
            } else {
                store(r, vtop - 1);
            }
        vswap();
        vtop--;
        vtop->r |= delayed_cast;
    }
}
static void inc(int post, int c)
{
    test_lvalue();
    vdup();
    if (post) {
        gv_dup();
        vrotb(3);
        vrotb(3);
    }
    vpushi(c - 0xa3);
    gen_op('+');
    vstore();
    if (post)
        vpop();
}
static void parse_mult_str (CString *astr, const char *msg)
{
    if (tok != 0xb9)
        expect(msg);
    cstr_new(astr);
    while (tok == 0xb9) {
        cstr_cat(astr, tokc.str.data, -1);
        next();
    }
    cstr_ccat(astr, '\0');
}
static int exact_log2p1(int i)
{
  int ret;
  if (!i)
    return 0;
  for (ret = 1; i >= 1 << 8; ret += 8)
    i >>= 8;
  if (i >= 1 << 4)
    ret += 4, i >>= 4;
  if (i >= 1 << 2)
    ret += 2, i >>= 2;
  if (i >= 1 << 1)
    ret++;
  return ret;
}

static Sym * find_field (CType *type, int v)
{
    Sym *s = type->ref;
    v |= 0x20000000;
    while ((s = s->next) != ((void*)0)) {
 if ((s->v & 0x20000000) &&
     (s->type.t & 0x000f) == 7 &&
     (s->v & ~0x20000000) >= 0x10000000) {
     Sym *ret = find_field (&s->type, v);
     if (ret)
         return ret;
 }
 if (s->v == v)
   break;
    }
    return s;
}

static void sym_to_attr(AttributeDef *ad, Sym *s)
{
    if (s->a.aligned && 0 == ad->a.aligned)
        ad->a.aligned = s->a.aligned;
    if (s->f.func_call && 0 == ad->f.func_call)
        ad->f.func_call = s->f.func_call;
    if (s->f.func_type && 0 == ad->f.func_type)
        ad->f.func_type = s->f.func_type;
    if (s->a.packed)
        ad->a.packed = 1;
}
static void parse_btype_qualify(CType *type, int qualifiers)
{
    while (type->t & 0x0040) {
        type->ref = sym_push(0x20000000, &type->ref->type, 0, type->ref->c);
        type = &type->ref->type;
    }
    type->t |= qualifiers;
}
static int parse_btype(CType *type, AttributeDef *ad)
{
    int t, u, bt, st, type_found, typespec_found, g;
    Sym *s;
    CType type1;
    memset(ad, 0, sizeof(AttributeDef));
    type_found = 0;
    typespec_found = 0;
    t = 3;
    bt = st = -1;
    type->ref = ((void*)0);
    while(1) {
        switch(tok) {
        case TOK_EXTENSION:
            next();
            continue;
        case TOK_CHAR:
            u = 1;
        basic_type:
            next();
        basic_type1:
            if (u == 2 || u == 0x0800) {
                if (st != -1 || (bt != -1 && bt != 3))
                    tmbt: tcc_error("too many basic types");
                st = u;
            } else {
                if (bt != -1 || (st != -1 && u != 3))
                    goto tmbt;
                bt = u;
            }
            if (u != 3)
                t = (t & ~(0x000f|0x0800)) | u;
            typespec_found = 1;
            break;
        case TOK_VOID:
            u = 0;
            goto basic_type;
        case TOK_SHORT:
            u = 2;
            goto basic_type;
        case TOK_INT:
            u = 3;
            goto basic_type;
        case TOK_LONG:
            if ((t & 0x000f) == 9) {
                t = (t & ~(0x000f|0x0800)) | 10;
            } else if ((t & (0x000f|0x0800)) == 0x0800) {
                t = (t & ~(0x000f|0x0800)) | 4;
            } else {
                u = 0x0800;
                goto basic_type;
            }
            next();
            break;
        case TOK_BOOL:
            u = 11;
            goto basic_type;
        case TOK_FLOAT:
            u = 8;
            goto basic_type;
        case TOK_DOUBLE:
            if ((t & (0x000f|0x0800)) == 0x0800) {
                t = (t & ~(0x000f|0x0800)) | 10;
            } else {
                u = 9;
                goto basic_type;
            }
            next();
            break;
        basic_type2:
            u = type1.t;
            type->ref = type1.ref;
            goto basic_type1;
        case TOK_CONST1:
        case TOK_CONST2:
        case TOK_CONST3:
            type->t = t;
            parse_btype_qualify(type, 0x0100);
            t = type->t;
            next();
            break;
        case TOK_VOLATILE1:
        case TOK_VOLATILE2:
        case TOK_VOLATILE3:
            type->t = t;
            parse_btype_qualify(type, 0x0200);
            t = type->t;
            next();
            break;
        case TOK_SIGNED1:
        case TOK_SIGNED2:
        case TOK_SIGNED3:
            if ((t & (0x0020|0x0010)) == (0x0020|0x0010))
                tcc_error("signed and unsigned modifier");
            t |= 0x0020;
            next();
            typespec_found = 1;
            break;
        case TOK_REGISTER:
        case TOK_AUTO:
        case TOK_RESTRICT1:
        case TOK_RESTRICT2:
        case TOK_RESTRICT3:
            next();
            break;
        case TOK_UNSIGNED:
            if ((t & (0x0020|0x0010)) == 0x0020)
                tcc_error("signed and unsigned modifier");
            t |= 0x0020 | 0x0010;
            next();
            typespec_found = 1;
            break;
        case TOK_EXTERN:
            g = 0x00001000;
            goto storage;
        case TOK_STATIC:
            g = 0x00002000;
            goto storage;
        case TOK_TYPEDEF:
            g = 0x00004000;
            goto storage;
       storage:
            if (t & (0x00001000|0x00002000|0x00004000) & ~g)
                tcc_error("multiple storage classes");
            t |= g;
            next();
            break;
        case TOK_INLINE1:
        case TOK_INLINE2:
        case TOK_INLINE3:
            t |= 0x00008000;
            next();
            break;
        case TOK_TYPEOF1:
        case TOK_TYPEOF2:
        case TOK_TYPEOF3:
            next();
            parse_expr_type(&type1);
            type1.t &= ~((0x00001000 | 0x00002000 | 0x00004000 | 0x00008000)&~0x00004000);
     if (type1.ref)
                sym_to_attr(ad, type1.ref);
            goto basic_type2;
        default:
            if (typespec_found)
                goto the_end;
            s = sym_find(tok);
            if (!s || !(s->type.t & 0x00004000))
                goto the_end;
            t &= ~(0x000f|0x0800);
            u = t & ~(0x0100 | 0x0200), t ^= u;
            type->t = (s->type.t & ~0x00004000) | u;
            type->ref = s->type.ref;
            if (t)
                parse_btype_qualify(type, t);
            t = type->t;
            sym_to_attr(ad, s);
            next();
            typespec_found = 1;
            st = bt = -2;
            break;
        }
        type_found = 1;
    }
the_end:
    if (tcc_state->char_is_unsigned) {
        if ((t & (0x0020|0x000f)) == 1)
            t |= 0x0010;
    }
    bt = t & (0x000f|0x0800);
    if (bt == 0x0800)
        t |= 4 == 8 ? 4 : 3;
    type->t = t;
    return type_found;
}
static inline void convert_parameter_type(CType *pt)
{
    pt->t &= ~(0x0100 | 0x0200);
    pt->t &= ~0x0040;
    if ((pt->t & 0x000f) == 6) {
        mk_pointer(pt);
    }
}

static int post_type(CType *type, AttributeDef *ad, int storage, int td)
{
    int n, l, t1, arg_size, align;
    Sym **plast, *s, *first;
    AttributeDef ad1;
    CType pt;
    if (tok == '(') {
        next();
 if (td && !(td & 1))
   return 0;
 if (tok == ')')
   l = 0;
 else if (parse_btype(&pt, &ad1))
   l = 1;
 else if (td)
   return 0;
 else
   l = 2;
        first = ((void*)0);
        plast = &first;
        arg_size = 0;
        if (l) {
            for(;;) {
                if (l != 2) {
                    if ((pt.t & 0x000f) == 0 && tok == ')')
                        break;
                    type_decl(&pt, &ad1, &n, 2 | 1);
                    if ((pt.t & 0x000f) == 0)
                        tcc_error("parameter declared as void");
                    arg_size += (type_size(&pt, &align) + 4 - 1) / 4;
                } else {
                    n = tok;
                    if (n < TOK_DEFINE)
                        expect("identifier");
                    pt.t = 0;
                    next();
                }
                convert_parameter_type(&pt);
                s = sym_push(n | 0x20000000, &pt, 0, 0);
                *plast = s;
                plast = &s->next;
                if (tok == ')')
                    break;
                skip(',');
                if (l == 1 && tok == 0xc8) {
                    l = 3;
                    next();
                    break;
                }
  if (l == 1 && !parse_btype(&pt, &ad1))
      tcc_error("invalid type");
            }
        } else
            l = 2;
        skip(')');
        type->t &= ~0x0100;
        if (tok == '[') {
            next();
            skip(']');
            mk_pointer(type);
        }
        ad->f.func_args = arg_size;
        ad->f.func_type = l;
        s = sym_push(0x20000000, type, 0, 0);
        s->a = ad->a;
        s->f = ad->f;
        s->next = first;
        type->t = 6;
        type->ref = s;
    } else if (tok == '[') {
 int saved_nocode_wanted = nocode_wanted;
        next();
        if (tok == TOK_RESTRICT1)
            next();
        n = -1;
        t1 = 0;
        if (tok != ']') {
            if (!local_stack || (storage & 0x00002000))
                vpushi(expr_const());
            else {
  nocode_wanted = 0;
  gexpr();
     }
            if ((vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030) {
                n = vtop->c.i;
                if (n < 0)
                    tcc_error("invalid array size");
            } else {
                if (!is_integer_btype(vtop->type.t & 0x000f))
                    tcc_error("size of variable length array should be an integer");
                t1 = 0x0400;
            }
        }
        skip(']');
        post_type(type, ad, storage, 0);
        if (type->t == 6)
            tcc_error("declaration of an array of functions");
        t1 |= type->t & 0x0400;
        if (t1 & 0x0400) {
            loc -= type_size(&int_type, &align);
            loc &= -align;
            n = loc;
            vla_runtime_type_size(type, &align);
            gen_op('*');
            vset(&int_type, 0x0032|0x0100, n);
            vswap();
            vstore();
        }
        if (n != -1)
            vpop();
 nocode_wanted = saved_nocode_wanted;
        s = sym_push(0x20000000, type, 0, n);
        type->t = (t1 ? 0x0400 : 0x0040) | 5;
        type->ref = s;
    }
    return 1;
}
static CType *type_decl(CType *type, AttributeDef *ad, int *v, int td)
{
    CType *post, *ret;
    int qualifiers, storage;
    storage = type->t & (0x00001000 | 0x00002000 | 0x00004000 | 0x00008000);
    type->t &= ~(0x00001000 | 0x00002000 | 0x00004000 | 0x00008000);
    post = ret = type;
    while (tok == '*') {
        qualifiers = 0;
    redo:
        next();
        switch(tok) {
        case TOK_CONST1:
        case TOK_CONST2:
        case TOK_CONST3:
            qualifiers |= 0x0100;
            goto redo;
        case TOK_VOLATILE1:
        case TOK_VOLATILE2:
        case TOK_VOLATILE3:
            qualifiers |= 0x0200;
            goto redo;
        case TOK_RESTRICT1:
        case TOK_RESTRICT2:
        case TOK_RESTRICT3:
            goto redo;
        }
        mk_pointer(type);
        type->t |= qualifiers;
 if (ret == type)
     ret = pointed_type(type);
    }
    if (tok == '(') {
 if (!post_type(type, ad, 0, td)) {
     post = type_decl(type, ad, v, td);
     skip(')');
 }
    } else if (tok >= 256 && (td & 2)) {
 *v = tok;
 next();
    } else {
 if (!(td & 1))
   expect("identifier");
 *v = 0;
    }
    post_type(post, ad, storage, 0);
    type->t |= storage;
    return ret;
}
static int lvalue_type(int t)
{
    int bt, r;
    r = 0x0100;
    bt = t & 0x000f;
    if (bt == 1 || bt == 11)
        r |= 0x1000;
    else if (bt == 2)
        r |= 0x2000;
    else
        return r;
    if (t & 0x0010)
        r |= 0x4000;
    return r;
}
static void indir(void)
{
    if ((vtop->type.t & 0x000f) != 5) {
        if ((vtop->type.t & 0x000f) == 6)
            return;
        expect("pointer");
    }
    if (vtop->r & 0x0100)
        gv(0x0001);
    vtop->type = *pointed_type(&vtop->type);
    if (!(vtop->type.t & 0x0040) && !(vtop->type.t & 0x0400)
        && (vtop->type.t & 0x000f) != 6) {
        vtop->r |= lvalue_type(vtop->type.t);
    }
}
static void gfunc_param_typed(Sym *func, Sym *arg)
{
    int func_type;
    CType type;
    func_type = func->f.func_type;
    if (func_type == 2 ||
        (func_type == 3 && arg == ((void*)0))) {
        if ((vtop->type.t & 0x000f) == 8) {
            gen_cast_s(9);
        } else if (vtop->type.t & 0x0080) {
            type.t = vtop->type.t & (0x000f | 0x0010);
     type.ref = vtop->type.ref;
            gen_cast(&type);
        }
    } else if (arg == ((void*)0)) {
        tcc_error("too many arguments to function");
    } else {
        type = arg->type;
        type.t &= ~0x0100;
        gen_assign_cast(&type);
    }
}
static void expr_type(CType *type, void (*expr_fn)(void))
{
    nocode_wanted++;
    expr_fn();
    *type = vtop->type;
    vpop();
    nocode_wanted--;
}
static void parse_expr_type(CType *type)
{
    int n;
    AttributeDef ad;
    skip('(');
    if (parse_btype(type, &ad)) {
        type_decl(type, &ad, &n, 1);
    } else {
        expr_type(type, gexpr);
    }
    skip(')');
}
static void parse_type(CType *type)
{
    AttributeDef ad;
    int n;
    if (!parse_btype(type, &ad)) {
        expect("type");
    }
    type_decl(type, &ad, &n, 1);
}
static void parse_builtin_params(int nc, const char *args)
{
    char c, sep = '(';
    CType t;
    if (nc)
        nocode_wanted++;
    next();
    while ((c = *args++)) {
 skip(sep);
 sep = ',';
 switch (c) {
     case 'e': expr_eq(); continue;
     case 't': parse_type(&t); vpush(&t); continue;
     default: tcc_error("internal error"); break;
 }
    }
    skip(')');
    if (nc)
        nocode_wanted--;
}
static void unary(void)
{
    int n, t, align, size, r, sizeof_caller;
    CType type;
    Sym *s;
    AttributeDef ad;
    sizeof_caller = in_sizeof;
    in_sizeof = 0;
    type.ref = ((void*)0);
 tok_next:
    switch(tok) {
    case TOK_EXTENSION:
        next();
        goto tok_next;
    case 0xb4:
    case 0xb5:
    case 0xb3:
 t = 3;
 push_tokc:
 type.t = t;
 vsetc(&type, 0x0030, &tokc);
        next();
        break;
    case 0xb6:
        t = 3 | 0x0010;
        goto push_tokc;
    case 0xb7:
        t = 4;
 goto push_tokc;
    case 0xb8:
        t = 4 | 0x0010;
 goto push_tokc;
    case 0xbb:
        t = 8;
 goto push_tokc;
    case 0xbc:
        t = 9;
 goto push_tokc;
    case 0xbd:
        t = 10;
 goto push_tokc;
    case 0xce:
        t = (4 == 8 ? 4 : 3) | 0x0800;
 goto push_tokc;
    case 0xcf:
        t = (4 == 8 ? 4 : 3) | 0x0800 | 0x0010;
 goto push_tokc;
    case TOK___FUNCTION__:
        if (!gnu_ext)
            goto tok_identifier;
    case TOK___FUNC__:
        {
            void *ptr;
            int len;
            len = strlen(funcname) + 1;
            type.t = 1;
            mk_pointer(&type);
            type.t |= 0x0040;
            type.ref->c = len;
            vpush_ref(&type, data_section, data_section->data_offset, len);
            if (!(nocode_wanted > 0)) {
                ptr = section_ptr_add(data_section, len);
                memcpy(ptr, funcname, len);
            }
            next();
        }
        break;
    case 0xba:
        t = 3;
        goto str_init;
    case 0xb9:
        t = 1;
        if (tcc_state->char_is_unsigned)
            t = 1 | 0x0010;
    str_init:
        if (tcc_state->warn_write_strings)
            t |= 0x0100;
        type.t = t;
        mk_pointer(&type);
        type.t |= 0x0040;
        memset(&ad, 0, sizeof(AttributeDef));
        decl_initializer_alloc(&type, &ad, 0x0030, 2, 0, 0);
        break;
    case '(':
        next();
        if (parse_btype(&type, &ad)) {
            type_decl(&type, &ad, &n, 1);
            skip(')');
            if (tok == '{') {
                if (global_expr)
                    r = 0x0030;
                else
                    r = 0x0032;
                if (!(type.t & 0x0040))
                    r |= lvalue_type(type.t);
                memset(&ad, 0, sizeof(AttributeDef));
                decl_initializer_alloc(&type, &ad, r, 1, 0, 0);
            } else {
                if (sizeof_caller) {
                    vpush(&type);
                    return;
                }
                unary();
                gen_cast(&type);
            }
        } else if (tok == '{') {
     int saved_nocode_wanted = nocode_wanted;
            if (const_wanted)
                tcc_error("expected constant");
            save_regs(0);
            block(((void*)0), ((void*)0), 1);
     nocode_wanted = saved_nocode_wanted;
            skip(')');
        } else {
            gexpr();
            skip(')');
        }
        break;
    case '*':
        next();
        unary();
        indir();
        break;
    case '&':
        next();
        unary();
        if ((vtop->type.t & 0x000f) != 6 &&
            !(vtop->type.t & 0x0040))
            test_lvalue();
        mk_pointer(&vtop->type);
        gaddrof();
        break;
    case '!':
        next();
        unary();
        if ((vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030) {
            gen_cast_s(11);
            vtop->c.i = !vtop->c.i;
        } else if ((vtop->r & 0x003f) == 0x0033)
            vtop->c.i ^= 1;
        else {
            save_regs(1);
            vseti(0x0034, gvtst(1, 0));
        }
        break;
    case '~':
        next();
        unary();
        vpushi(-1);
        gen_op('^');
        break;
    case '+':
        next();
        unary();
        if ((vtop->type.t & 0x000f) == 5)
            tcc_error("pointer not accepted for unary plus");
 if (!is_float(vtop->type.t)) {
     vpushi(0);
     gen_op('+');
 }
        break;
    case TOK_SIZEOF:
    case TOK_ALIGNOF1:
    case TOK_ALIGNOF2:
        t = tok;
        next();
        in_sizeof++;
        expr_type(&type, unary);
        s = vtop[1].sym;
        size = type_size(&type, &align);
        if (s && s->a.aligned)
            align = 1 << (s->a.aligned - 1);
        if (t == TOK_SIZEOF) {
            if (!(type.t & 0x0400)) {
                if (size < 0)
                    tcc_error("sizeof applied to an incomplete type");
                vpushs(size);
            } else {
                vla_runtime_type_size(&type, &align);
            }
        } else {
            vpushs(align);
        }
        vtop->type.t |= 0x0010;
        break;
    case TOK_builtin_expect:
 parse_builtin_params(0, "ee");
 vpop();
        break;
    case TOK_builtin_types_compatible_p:
 parse_builtin_params(0, "tt");
 vtop[-1].type.t &= ~(0x0100 | 0x0200);
 vtop[0].type.t &= ~(0x0100 | 0x0200);
 n = is_compatible_types(&vtop[-1].type, &vtop[0].type);
 vtop -= 2;
 vpushi(n);
        break;
    case TOK_builtin_choose_expr:
 {
     int64_t c;
     next();
     skip('(');
     c = expr_const64();
     skip(',');
     if (!c) {
  nocode_wanted++;
     }
     expr_eq();
     if (!c) {
  vpop();
  nocode_wanted--;
     }
     skip(',');
     if (c) {
  nocode_wanted++;
     }
     expr_eq();
     if (c) {
  vpop();
  nocode_wanted--;
     }
     skip(')');
 }
        break;
    case TOK_builtin_constant_p:
 parse_builtin_params(1, "e");
 n = (vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030;
 vtop--;
 vpushi(n);
        break;
    case TOK_builtin_frame_address:
    case TOK_builtin_return_address:
        {
            int tok1 = tok;
            int level;
            next();
            skip('(');
            if (tok != 0xb5) {
                tcc_error("%s only takes positive integers",
                          tok1 == TOK_builtin_return_address ?
                          "__builtin_return_address" :
                          "__builtin_frame_address");
            }
            level = (uint32_t)tokc.i;
            next();
            skip(')');
            type.t = 0;
            mk_pointer(&type);
            vset(&type, 0x0032, 0);
            while (level--) {
                mk_pointer(&vtop->type);
                indir();
            }
            if (tok1 == TOK_builtin_return_address) {
                vpushi(4);
                gen_op('+');
                mk_pointer(&vtop->type);
                indir();
            }
        }
        break;
    case 0xa4:
    case 0xa2:
        t = tok;
        next();
        unary();
        inc(0, t);
        break;
    case '-':
        next();
        unary();
        t = vtop->type.t & 0x000f;
 if (is_float(t)) {
     vpush(&vtop->type);
     if (t == 8)
         vtop->c.f = -1.0 * 0.0;
     else if (t == 9)
         vtop->c.d = -1.0 * 0.0;
     else
         vtop->c.ld = -1.0 * 0.0;
 } else
     vpushi(0);
 vswap();
 gen_op('-');
        break;
    case 0xa0:
        if (!gnu_ext)
            goto tok_identifier;
        next();
        if (tok < TOK_DEFINE)
            expect("label identifier");
        s = label_find(tok);
        if (!s) {
            s = label_push(&global_label_stack, tok, 1);
        } else {
            if (s->r == 2)
                s->r = 1;
        }
        if (!s->type.t) {
            s->type.t = 0;
            mk_pointer(&s->type);
            s->type.t |= 0x00002000;
        }
        vpushsym(&s->type, s);
        next();
        break;
    case TOK_GENERIC:
    {
 CType controlling_type;
 int has_default = 0;
 int has_match = 0;
 int learn = 0;
 TokenString *str = ((void*)0);
 next();
 skip('(');
 expr_type(&controlling_type, expr_eq);
 controlling_type.t &= ~(0x0100 | 0x0200 | 0x0040);
 for (;;) {
     learn = 0;
     skip(',');
     if (tok == TOK_DEFAULT) {
  if (has_default)
      tcc_error("too many 'default'");
  has_default = 1;
  if (!has_match)
      learn = 1;
  next();
     } else {
         AttributeDef ad_tmp;
  int itmp;
         CType cur_type;
  parse_btype(&cur_type, &ad_tmp);
  type_decl(&cur_type, &ad_tmp, &itmp, 1);
  if (compare_types(&controlling_type, &cur_type, 0)) {
      if (has_match) {
        tcc_error("type match twice");
      }
      has_match = 1;
      learn = 1;
  }
     }
     skip(':');
     if (learn) {
  if (str)
      tok_str_free(str);
  skip_or_save_block(&str);
     } else {
  skip_or_save_block(((void*)0));
     }
     if (tok == ')')
  break;
 }
 if (!str) {
     char buf[60];
     tcc_error("type '%s' does not match any association", buf);
 }
 begin_macro(str, 1);
 next();
 expr_eq();
 if (tok != (-1))
     expect(",");
 end_macro();
        next();
 break;
    }
    case TOK___NAN__:
        vpush64(9, 0x7ff8000000000000ULL);
        next();
        break;
    case TOK___SNAN__:
        vpush64(9, 0x7ff0000000000001ULL);
        next();
        break;
    case TOK___INF__:
        vpush64(9, 0x7ff0000000000000ULL);
        next();
        break;
    default:
    tok_identifier:
        t = tok;
        next();
        if (t < TOK_DEFINE)
            expect("identifier");
        s = sym_find(t);
        if (!s || (((s)->type.t & (0x000f | (0 | 0x0010))) == (0 | 0x0010))) {
            const char *name = get_tok_str(t, ((void*)0));
            if (tok != '(')
                tcc_error("'%s' undeclared", name);
            if (tcc_state->warn_implicit_function_declaration
            )
                tcc_warning("implicit declaration of function '%s'", name);
            s = external_global_sym(t, &func_old_type, 0);
        }
        r = s->r;
        if ((r & 0x003f) < 0x0030)
            r = (r & ~0x003f) | 0x0032;
        vset(&s->type, r, s->c);
 vtop->sym = s;
        if (r & 0x0200) {
            vtop->c.i = 0;
        } else if (r == 0x0030 && ((s->type.t & (((1 << (6+6)) - 1) << 20 | 0x0080)) == (3 << 20))) {
            vtop->c.i = s->enum_val;
        }
        break;
    }
    while (1) {
        if (tok == 0xa4 || tok == 0xa2) {
            inc(1, tok);
            next();
        } else if (tok == '.' || tok == 0xc7 || tok == 0xbc) {
            int qualifiers;
            if (tok == 0xc7)
                indir();
            qualifiers = vtop->type.t & (0x0100 | 0x0200);
            test_lvalue();
            gaddrof();
            if ((vtop->type.t & 0x000f) != 7)
                expect("struct or union");
            if (tok == 0xbc)
                expect("field name");
            next();
            if (tok == 0xb5 || tok == 0xb6)
                expect("field name");
     s = find_field(&vtop->type, tok);
            if (!s)
                tcc_error("field not found: %s", get_tok_str(tok & ~0x20000000, &tokc));
            vtop->type = char_pointer_type;
            vpushi(s->c);
            gen_op('+');
            vtop->type = s->type;
            vtop->type.t |= qualifiers;
            if (!(vtop->type.t & 0x0040)) {
                vtop->r |= lvalue_type(vtop->type.t);
            }
            next();
        } else if (tok == '[') {
            next();
            gexpr();
            gen_op('+');
            indir();
            skip(']');
        } else if (tok == '(') {
            SValue ret;
            Sym *sa;
            int nb_args, ret_nregs, ret_align, regsize, variadic;
            if ((vtop->type.t & 0x000f) != 6) {
                if ((vtop->type.t & (0x000f | 0x0040)) == 5) {
                    vtop->type = *pointed_type(&vtop->type);
                    if ((vtop->type.t & 0x000f) != 6)
                        goto error_func;
                } else {
                error_func:
                    expect("function pointer");
                }
            } else {
                vtop->r &= ~0x0100;
            }
            s = vtop->type.ref;
            next();
            sa = s->next;
            nb_args = regsize = 0;
            ret.r2 = 0x0030;
            if ((s->type.t & 0x000f) == 7) {
                variadic = (s->f.func_type == 3);
                ret_nregs = gfunc_sret(&s->type, variadic, &ret.type,
                                       &ret_align, &regsize);
                if (!ret_nregs) {
                    size = type_size(&s->type, &align);
                    loc = (loc - size) & -align;
                    ret.type = s->type;
                    ret.r = 0x0032 | 0x0100;
                    vseti(0x0032, loc);
                    ret.c = vtop->c;
                    nb_args++;
                }
            } else {
                ret_nregs = 1;
                ret.type = s->type;
            }
            if (ret_nregs) {
                if (is_float(ret.type.t)) {
                    ret.r = reg_fret(ret.type.t);
                } else {
                    if ((ret.type.t & 0x000f) == 4)
                        ret.r2 = TREG_EDX;
                    ret.r = TREG_EAX;
                }
                ret.c.i = 0;
            }
            if (tok != ')') {
                for(;;) {
                    expr_eq();
                    gfunc_param_typed(s, sa);
                    nb_args++;
                    if (sa)
                        sa = sa->next;
                    if (tok == ')')
                        break;
                    skip(',');
                }
            }
            if (sa)
                tcc_error("too few arguments to function");
            skip(')');
            gfunc_call(nb_args);
            for (r = ret.r + ret_nregs + !ret_nregs; r-- > ret.r;) {
                vsetc(&ret.type, r, &ret.c);
                vtop->r2 = ret.r2;
            }
            if (((s->type.t & 0x000f) == 7) && ret_nregs) {
                int addr, offset;
                size = type_size(&s->type, &align);
  if (regsize > align)
    align = regsize;
                loc = (loc - size) & -align;
                addr = loc;
                offset = 0;
                for (;;) {
                    vset(&ret.type, 0x0032 | 0x0100, addr + offset);
                    vswap();
                    vstore();
                    vtop--;
                    if (--ret_nregs == 0)
                        break;
                    offset += regsize;
                }
                vset(&s->type, 0x0032 | 0x0100, addr);
            }
        } else {
            break;
        }
    }
}
static void expr_prod(void)
{
    int t;
    unary();
    while (tok == '*' || tok == '/' || tok == '%') {
        t = tok;
        next();
        unary();
        gen_op(t);
    }
}
static void expr_sum(void)
{
    int t;
    expr_prod();
    while (tok == '+' || tok == '-') {
        t = tok;
        next();
        expr_prod();
        gen_op(t);
    }
}
static void expr_shift(void)
{
    int t;
    expr_sum();
    while (tok == 0x01 || tok == 0x02) {
        t = tok;
        next();
        expr_sum();
        gen_op(t);
    }
}
static void expr_cmp(void)
{
    int t;
    expr_shift();
    while ((tok >= 0x96 && tok <= 0x9f) ||
           tok == 0x92 || tok == 0x93) {
        t = tok;
        next();
        expr_shift();
        gen_op(t);
    }
}
static void expr_cmpeq(void)
{
    int t;
    expr_cmp();
    while (tok == 0x94 || tok == 0x95) {
        t = tok;
        next();
        expr_cmp();
        gen_op(t);
    }
}
static void expr_and(void)
{
    expr_cmpeq();
    while (tok == '&') {
        next();
        expr_cmpeq();
        gen_op('&');
    }
}
static void expr_xor(void)
{
    expr_and();
    while (tok == '^') {
        next();
        expr_and();
        gen_op('^');
    }
}
static void expr_or(void)
{
    expr_xor();
    while (tok == '|') {
        next();
        expr_xor();
        gen_op('|');
    }
}
static void expr_land(void)
{
    expr_or();
    if (tok == 0xa0) {
 int t = 0;
 for(;;) {
     if ((vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030) {
                gen_cast_s(11);
  if (vtop->c.i) {
      vpop();
  } else {
      nocode_wanted++;
      while (tok == 0xa0) {
   next();
   expr_or();
   vpop();
      }
      nocode_wanted--;
      if (t)
        gsym(t);
      gen_cast_s(3);
      break;
  }
     } else {
  if (!t)
    save_regs(1);
  t = gvtst(1, t);
     }
     if (tok != 0xa0) {
  if (t)
    vseti(0x0035, t);
  else
    vpushi(1);
  break;
     }
     next();
     expr_or();
 }
    }
}
static void expr_lor(void)
{
    expr_land();
    if (tok == 0xa1) {
 int t = 0;
 for(;;) {
     if ((vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030) {
                gen_cast_s(11);
  if (!vtop->c.i) {
      vpop();
  } else {
      nocode_wanted++;
      while (tok == 0xa1) {
   next();
   expr_land();
   vpop();
      }
      nocode_wanted--;
      if (t)
        gsym(t);
      gen_cast_s(3);
      break;
  }
     } else {
  if (!t)
    save_regs(1);
  t = gvtst(0, t);
     }
     if (tok != 0xa1) {
  if (t)
    vseti(0x0034, t);
  else
    vpushi(0);
  break;
     }
     next();
     expr_land();
 }
    }
}
static int condition_3way(void)
{
    int c = -1;
    if ((vtop->r & (0x003f | 0x0100)) == 0x0030 &&
 (!(vtop->r & 0x0200) || !vtop->sym->a.weak)) {
 vdup();
        gen_cast_s(11);
 c = vtop->c.i;
 vpop();
    }
    return c;
}
static void expr_cond(void)
{
    int tt, u, r1, r2, rc, t1, t2, bt1, bt2, islv, c, g;
    SValue sv;
    CType type, type1, type2;
    expr_lor();
    if (tok == '?') {
        next();
 c = condition_3way();
        g = (tok == ':' && gnu_ext);
        if (c < 0) {
            if (is_float(vtop->type.t)) {
                rc = 0x0002;
            } else
                rc = 0x0001;
            gv(rc);
            save_regs(1);
            if (g)
                gv_dup();
            tt = gvtst(1, 0);
        } else {
            if (!g)
                vpop();
            tt = 0;
        }
        if (1) {
            if (c == 0)
                nocode_wanted++;
            if (!g)
                gexpr();
            type1 = vtop->type;
            sv = *vtop;
            vtop--;
            skip(':');
            u = 0;
            if (c < 0)
                u = gjmp(0);
            gsym(tt);
            if (c == 0)
                nocode_wanted--;
            if (c == 1)
                nocode_wanted++;
            expr_cond();
            if (c == 1)
                nocode_wanted--;
            type2 = vtop->type;
            t1 = type1.t;
            bt1 = t1 & 0x000f;
            t2 = type2.t;
            bt2 = t2 & 0x000f;
            type.ref = ((void*)0);
            if (is_float(bt1) || is_float(bt2)) {
                if (bt1 == 10 || bt2 == 10) {
                    type.t = 10;
                } else if (bt1 == 9 || bt2 == 9) {
                    type.t = 9;
                } else {
                    type.t = 8;
                }
            } else if (bt1 == 4 || bt2 == 4) {
                type.t = 4 | 0x0800;
                if (bt1 == 4)
                    type.t &= t1;
                if (bt2 == 4)
                    type.t &= t2;
                if ((t1 & (0x000f | 0x0010 | 0x0080)) == (4 | 0x0010) ||
                    (t2 & (0x000f | 0x0010 | 0x0080)) == (4 | 0x0010))
                    type.t |= 0x0010;
            } else if (bt1 == 5 || bt2 == 5) {
  if (is_null_pointer (vtop))
    type = type1;
  else if (is_null_pointer (&sv))
    type = type2;
  else
    type = type1;
            } else if (bt1 == 6 || bt2 == 6) {
                type = bt1 == 6 ? type1 : type2;
            } else if (bt1 == 7 || bt2 == 7) {
                type = bt1 == 7 ? type1 : type2;
            } else if (bt1 == 0 || bt2 == 0) {
                type.t = 0;
            } else {
                type.t = 3 | (0x0800 & (t1 | t2));
                if ((t1 & (0x000f | 0x0010 | 0x0080)) == (3 | 0x0010) ||
                    (t2 & (0x000f | 0x0010 | 0x0080)) == (3 | 0x0010))
                    type.t |= 0x0010;
            }
            islv = (vtop->r & 0x0100) && (sv.r & 0x0100) && 7 == (type.t & 0x000f);
            islv &= c < 0;
            if (c != 1) {
                gen_cast(&type);
                if (islv) {
                    mk_pointer(&vtop->type);
                    gaddrof();
                } else if (7 == (vtop->type.t & 0x000f))
                    gaddrof();
            }
            rc = 0x0001;
            if (is_float(type.t)) {
                rc = 0x0002;
            } else if ((type.t & 0x000f) == 4) {
                rc = 0x0004;
            }
            tt = r2 = 0;
            if (c < 0) {
                r2 = gv(rc);
                tt = gjmp(0);
            }
            gsym(u);
            if (c != 0) {
                *vtop = sv;
                gen_cast(&type);
                if (islv) {
                    mk_pointer(&vtop->type);
                    gaddrof();
                } else if (7 == (vtop->type.t & 0x000f))
                    gaddrof();
            }
            if (c < 0) {
                r1 = gv(rc);
                move_reg(r2, r1, type.t);
                vtop->r = r2;
                gsym(tt);
                if (islv)
                    indir();
            }
        }
    }
}
static void expr_eq(void)
{
    int t;
    expr_cond();
    if (tok == '=' ||
        (tok >= 0xa5 && tok <= 0xaf) ||
        tok == 0xde || tok == 0xfc ||
        tok == 0x81 || tok == 0x82) {
        test_lvalue();
        t = tok;
        next();
        if (t == '=') {
            expr_eq();
        } else {
            vdup();
            expr_eq();
            gen_op(t & 0x7f);
        }
        vstore();
    }
}
static void gexpr(void)
{
    while (1) {
        expr_eq();
        if (tok != ',')
            break;
        vpop();
        next();
    }
}
static void expr_const1(void)
{
    const_wanted++;
    nocode_wanted++;
    expr_cond();
    nocode_wanted--;
    const_wanted--;
}
static inline int64_t expr_const64(void)
{
    int64_t c;
    expr_const1();
    if ((vtop->r & (0x003f | 0x0100 | 0x0200)) != 0x0030)
        expect("constant expression");
    c = vtop->c.i;
    vpop();
    return c;
}
static int expr_const(void)
{
    int c;
    int64_t wc = expr_const64();
    c = wc;
    if (c != wc && (unsigned)c != wc)
        tcc_error("constant exceeds 32 bit");
    return c;
}
static int is_label(void)
{
    int last_tok;
    if (tok < TOK_DEFINE)
        return 0;
    last_tok = tok;
    next();
    if (tok == ':') {
        return last_tok;
    } else {
        unget_tok(last_tok);
        return 0;
    }
}
static void gfunc_return(CType *func_type)
{
    if ((func_type->t & 0x000f) == 7) {
        CType type, ret_type;
        int ret_align, ret_nregs, regsize;
        ret_nregs = gfunc_sret(func_type, func_var, &ret_type,
                               &ret_align, &regsize);
        if (0 == ret_nregs) {
            type = *func_type;
            mk_pointer(&type);
            vset(&type, 0x0032 | 0x0100, func_vc);
            indir();
            vswap();
            vstore();
        } else {
            int r, size, addr, align;
            size = type_size(func_type,&align);
            if ((vtop->r != (0x0032 | 0x0100) ||
                 (vtop->c.i & (ret_align-1)))
                && (align & (ret_align-1))) {
                loc = (loc - size) & -ret_align;
                addr = loc;
                type = *func_type;
                vset(&type, 0x0032 | 0x0100, addr);
                vswap();
                vstore();
                vpop();
                vset(&ret_type, 0x0032 | 0x0100, addr);
            }
            vtop->type = ret_type;
            if (is_float(ret_type.t))
                r = rc_fret(ret_type.t);
            else
                r = 0x0004;
            if (ret_nregs == 1)
                gv(r);
            else {
                for (;;) {
                    vdup();
                    gv(r);
                    vpop();
                    if (--ret_nregs == 0)
                      break;
                    r <<= 1;
                    vtop->c.i += regsize;
                }
            }
        }
    } else if (is_float(func_type->t)) {
        gv(rc_fret(func_type->t));
    } else {
        gv(0x0004);
    }
    vtop--;
}

static void block(int *bsym, int *csym, int is_expr)
{
    int a, b, c, d, cond;
    Sym *s;
    if (tcc_state->do_debug)
        tcc_debug_line(tcc_state);
    if (is_expr) {
        vpushi(0);
        vtop->type.t = 0;
    }
    if (tok == TOK_IF) {
 int saved_nocode_wanted = nocode_wanted;
        next();
        skip('(');
        gexpr();
        skip(')');
 cond = condition_3way();
        if (cond == 1)
            a = 0, vpop();
        else
            a = gvtst(1, 0);
        if (cond == 0)
     nocode_wanted |= 0x20000000;
        block(bsym, csym, 0);
 if (cond != 1)
     nocode_wanted = saved_nocode_wanted;
        c = tok;
        if (c == TOK_ELSE) {
            next();
            d = gjmp(0);
            gsym(a);
     if (cond == 1)
         nocode_wanted |= 0x20000000;
            block(bsym, csym, 0);
            gsym(d);
     if (cond != 0)
  nocode_wanted = saved_nocode_wanted;
        } else
            gsym(a);
    } else if (tok == TOK_WHILE) {
 int saved_nocode_wanted;
 nocode_wanted &= ~0x20000000;
        next();
        d = ind;
        vla_sp_restore();
        skip('(');
        gexpr();
        skip(')');
        a = gvtst(1, 0);
        b = 0;
        ++local_scope;
 saved_nocode_wanted = nocode_wanted;
        block(&a, &b, 0);
 nocode_wanted = saved_nocode_wanted;
        --local_scope;
        gjmp_addr(d);
        gsym(a);
        gsym_addr(b, d);
    } else if (tok == '{') {
        Sym *llabel;
        int block_vla_sp_loc = vla_sp_loc, saved_vlas_in_scope = vlas_in_scope;
        next();
        s = local_stack;
        llabel = local_label_stack;
        ++local_scope;
        while (tok != '}') {
     if ((a = is_label()))
  unget_tok(a);
     else
         decl(0x0032);
            if (tok != '}') {
                if (is_expr)
                    vpop();
                block(bsym, csym, is_expr);
            }
        }
        --local_scope;
 sym_pop(&local_stack, s, is_expr);
        if (vlas_in_scope > saved_vlas_in_scope) {
            vla_sp_loc = saved_vlas_in_scope ? block_vla_sp_loc : vla_sp_root_loc;
            vla_sp_restore();
        }
        vlas_in_scope = saved_vlas_in_scope;
        next();
    } else if (tok == TOK_RETURN) {
        next();
        if (tok != ';') {
            gexpr();
            gen_assign_cast(&func_vt);
            if ((func_vt.t & 0x000f) == 0)
                vtop--;
            else
                gfunc_return(&func_vt);
        }
        skip(';');
        if (tok != '}' || local_scope != 1)
            rsym = gjmp(rsym);
 nocode_wanted |= 0x20000000;
    } else if (tok == TOK_BREAK) {
        if (!bsym)
            tcc_error("cannot break");
        *bsym = gjmp(*bsym);
        next();
        skip(';');
 nocode_wanted |= 0x20000000;
    } else {
        b = is_label();
        if (b) {
     next();
            s = label_find(b);
            if (s) {
                if (s->r == 0)
                    tcc_error("duplicate label '%s'", get_tok_str(s->v, ((void*)0)));
                gsym(s->jnext);
                s->r = 0;
            } else {
                s = label_push(&global_label_stack, b, 0);
            }
            s->jnext = ind;
            vla_sp_restore();
        block_after_label:
     nocode_wanted &= ~0x20000000;
            if (tok == '}') {
                tcc_warning("deprecated use of label at end of compound statement");
            } else {
                if (is_expr)
                    vpop();
                block(bsym, csym, is_expr);
            }
        } else {
            if (tok != ';') {
                if (is_expr) {
                    vpop();
                    gexpr();
                } else {
                    gexpr();
                    vpop();
                }
            }
            skip(';');
        }
    }
}
static void skip_or_save_block(TokenString **str)
{
    int braces = tok == '{';
    int level = 0;
    if (str)
      *str = tok_str_alloc();
    while ((level > 0 || (tok != '}' && tok != ',' && tok != ';' && tok != ')'))) {
 int t;
 if (tok == (-1)) {
      if (str || level > 0)
        tcc_error("unexpected end of file");
      else
        break;
 }
 if (str)
   tok_str_add_tok(*str);
 t = tok;
 next();
 if (t == '{' || t == '(') {
     level++;
 } else if (t == '}' || t == ')') {
     level--;
     if (level == 0 && braces && t == '}')
       break;
 }
    }
    if (str) {
 tok_str_add(*str, -1);
 tok_str_add(*str, 0);
    }
}
static void parse_init_elem(int expr_type)
{
    int saved_global_expr;
    switch(expr_type) {
    case 1:
        saved_global_expr = global_expr;
        global_expr = 1;
        expr_const1();
        global_expr = saved_global_expr;
        if (((vtop->r & (0x003f | 0x0100)) != 0x0030
      && ((vtop->r & (0x0200|0x0100)) != (0x0200|0x0100)
   || vtop->sym->v < 0x10000000))
            )
            tcc_error("initializer element is not constant");
        break;
    case 2:
        expr_eq();
        break;
    }
}
static void init_putz(Section *sec, unsigned long c, int size)
{
    if (sec) {
    } else {
        vpush_global_sym(&func_old_type, TOK_memset);
        vseti(0x0032, c);
        vpushi(0);
        vpushs(size);
        gfunc_call(3);
    }
}

static int decl_designator(CType *type, Section *sec, unsigned long c,
                           Sym **cur_field, int size_only, int al) {
exit(1);
}

static void init_putv(CType *type, Section *sec, unsigned long c)
{
    int bt;
    void *ptr;
    CType dtype;
    dtype = *type;
    dtype.t &= ~0x0100;
    if (sec) {
 int size, align;
        gen_assign_cast(&dtype);
        bt = type->t & 0x000f;
        if ((vtop->r & 0x0200)
            && bt != 5
            && bt != 6
            && (bt != (4 == 8 ? 4 : 3)
                || (type->t & 0x0080))
            && !((vtop->r & 0x0030) && vtop->sym->v >= 0x10000000)
            )
            tcc_error("initializer element is not computable at load time");
        if ((nocode_wanted > 0)) {
            vtop--;
            return;
        }
 size = type_size(type, &align);
 section_reserve(sec, c + size);
        ptr = sec->data + c;
 if ((vtop->r & (0x0200|0x0030)) == (0x0200|0x0030) &&
     vtop->sym->v >= 0x10000000 &&
     (vtop->type.t & 0x000f) != 5) {
     Section *ssec;
     Elf32_Sym *esym;
     Elf32_Rel *rel;
     esym = elfsym(vtop->sym);
     ssec = tcc_state->sections[esym->st_shndx];
     memmove (ptr, ssec->data + esym->st_value, size);
     if (ssec->reloc) {
  int num_relocs = ssec->reloc->data_offset / sizeof(*rel);
  rel = (Elf32_Rel*)(ssec->reloc->data + ssec->reloc->data_offset);
  while (num_relocs--) {
      rel--;
      if (rel->r_offset >= esym->st_value + size)
        continue;
      if (rel->r_offset < esym->st_value)
        break;
      put_elf_reloca(symtab_section, sec,
       c + rel->r_offset - esym->st_value,
       ((rel->r_info) & 0xff),
       ((rel->r_info) >> 8),
       0
      );
  }
     }
 } else {
            if (type->t & 0x0080) {
                int bit_pos, bit_size, bits, n;
                unsigned char *p, v, m;
                bit_pos = (((vtop->type.t) >> 20) & 0x3f);
                bit_size = (((vtop->type.t) >> (20 + 6)) & 0x3f);
                p = (unsigned char*)ptr + (bit_pos >> 3);
                bit_pos &= 7, bits = 0;
                while (bit_size) {
                    n = 8 - bit_pos;
                    if (n > bit_size)
                        n = bit_size;
                    v = vtop->c.i >> bits << bit_pos;
                    m = ((1 << n) - 1) << bit_pos;
                    *p = (*p & ~m) | (v & m);
                    bits += n, bit_size -= n, bit_pos = 0, ++p;
                }
            } else
            switch(bt) {
     case 11:
  vtop->c.i = vtop->c.i != 0;
     case 1:
  *(char *)ptr |= vtop->c.i;
  break;
     case 2:
  *(short *)ptr |= vtop->c.i;
  break;
     case 8:
  *(float*)ptr = vtop->c.f;
  break;
     case 9:
  *(double *)ptr = vtop->c.d;
  break;
     case 10:
                if (sizeof (long double) >= 10)
                    memcpy(ptr, &vtop->c.ld, 10);
                else if (vtop->c.ld == 0.0)
                    ;
                else
                if (sizeof(long double) == 12)
      *(long double*)ptr = vtop->c.ld;
                else if (sizeof(double) == 12)
      *(double *)ptr = (double)vtop->c.ld;
                else
                    tcc_error("can't cross compile long double constants");
  break;
     case 4:
  *(long long *)ptr |= vtop->c.i;
  break;
     case 5:
  {
      Elf32_Addr val = vtop->c.i;
      if (vtop->r & 0x0200)
        greloc(sec, vtop->sym, c, 1);
      *(Elf32_Addr *)ptr |= val;
      break;
  }
     default:
  {
      int val = vtop->c.i;
      if (vtop->r & 0x0200)
        greloc(sec, vtop->sym, c, 1);
      *(int *)ptr |= val;
      break;
  }
     }
 }
        vtop--;
    } else {
        vset(&dtype, 0x0032|0x0100, c);
        vswap();
        vstore();
        vpop();
    }
}
static void decl_initializer(CType *type, Section *sec, unsigned long c,
                             int first, int size_only)
{
    int len, n, no_oblock, nb, i;
    int size1, align1;
    int have_elem;
    Sym *s, *f;
    Sym indexsym;
    CType *t1;
    have_elem = tok == '}' || tok == ',';
    if (!have_elem && tok != '{' &&
 tok != 0xba && tok != 0xb9 &&
 !size_only) {
 parse_init_elem(!sec ? 2 : 1);
 have_elem = 1;
    }
    if (have_elem &&
 !(type->t & 0x0040) &&
 is_compatible_unqualified_types(type, &vtop->type)) {
        init_putv(type, sec, c);
    } else if (type->t & 0x0040) {
        s = type->ref;
        n = s->c;
        t1 = pointed_type(type);
        size1 = type_size(t1, &align1);
        no_oblock = 1;
        if ((first && tok != 0xba && tok != 0xb9) ||
            tok == '{') {
            if (tok != '{')
                tcc_error("character array initializer must be a literal,"
                    " optionally enclosed in braces");
            skip('{');
            no_oblock = 0;
        }
        if ((tok == 0xba &&
             (t1->t & 0x000f) == 3
            ) || (tok == 0xb9 && (t1->t & 0x000f) == 1)) {
     len = 0;
            while (tok == 0xb9 || tok == 0xba) {
                int cstr_len, ch;
                if (tok == 0xb9)
                    cstr_len = tokc.str.size;
                else
                    cstr_len = tokc.str.size / sizeof(nwchar_t);
                cstr_len--;
                nb = cstr_len;
                if (n >= 0 && nb > (n - len))
                    nb = n - len;
                if (!size_only) {
                    if (cstr_len > nb)
                        tcc_warning("initializer-string for array is too long");
                    if (sec && tok == 0xb9 && size1 == 1) {
                        if (!(nocode_wanted > 0))
                            memcpy(sec->data + c + len, tokc.str.data, nb);
                    } else {
                        for(i=0;i<nb;i++) {
                            if (tok == 0xb9)
                                ch = ((unsigned char *)tokc.str.data)[i];
                            else
                                ch = ((nwchar_t *)tokc.str.data)[i];
       vpushi(ch);
                            init_putv(t1, sec, c + (len + i) * size1);
                        }
                    }
                }
                len += nb;
                next();
            }
            if (n < 0 || len < n) {
                if (!size_only) {
      vpushi(0);
                    init_putv(t1, sec, c + (len * size1));
                }
                len++;
            }
     len *= size1;
        } else {
     indexsym.c = 0;
     f = &indexsym;
          do_init_list:
     len = 0;
     while (tok != '}' || have_elem) {
  len = decl_designator(type, sec, c, &f, size_only, len);
  have_elem = 0;
  if (type->t & 0x0040) {
      ++indexsym.c;
      if (no_oblock && len >= n*size1)
          break;
  } else {
      if (s->type.t == (1 << 20 | 7))
          f = ((void*)0);
      else
          f = f->next;
      if (no_oblock && f == ((void*)0))
          break;
  }
  if (tok == '}')
      break;
  skip(',');
     }
        }
 if (!size_only && len < n*size1)
     init_putz(sec, c + len, n*size1 - len);
        if (!no_oblock)
            skip('}');
        if (n < 0)
            s->c = size1 == 1 ? len : ((len + size1 - 1)/size1);
    } else if ((type->t & 0x000f) == 7) {
 size1 = 1;
        no_oblock = 1;
        if (first || tok == '{') {
            skip('{');
            no_oblock = 0;
        }
        s = type->ref;
        f = s->next;
        n = s->c;
 goto do_init_list;
    } else if (tok == '{') {
        next();
        decl_initializer(type, sec, c, first, size_only);
        skip('}');
    } else if (size_only) {
        skip_or_save_block(((void*)0));
    } else {
 if (!have_elem) {
     if (tok != 0xb9 && tok != 0xba)
       expect("string constant");
     parse_init_elem(!sec ? 2 : 1);
 }
        init_putv(type, sec, c);
    }
}
static void decl_initializer_alloc(CType *type, AttributeDef *ad, int r,
                                   int has_init, int v, int scope)
{
    int size, align, addr;
    TokenString *init_str = ((void*)0);
    Section *sec;
    Sym *flexible_array;
    Sym *sym = ((void*)0);
    int saved_nocode_wanted = nocode_wanted;
    if (type->t & 0x00002000)
        nocode_wanted |= (nocode_wanted > 0) ? 0x40000000 : 0x80000000;
    flexible_array = ((void*)0);
    if ((type->t & 0x000f) == 7) {
        Sym *field = type->ref->next;
        if (field) {
            while (field->next)
                field = field->next;
            if (field->type.t & 0x0040 && field->type.ref->c < 0)
                flexible_array = field;
        }
    }
    size = type_size(type, &align);
    if (size < 0 || (flexible_array && has_init)) {
        if (!has_init)
            tcc_error("unknown type size");
        if (has_init == 2) {
     init_str = tok_str_alloc();
            while (tok == 0xb9 || tok == 0xba) {
                tok_str_add_tok(init_str);
                next();
            }
     tok_str_add(init_str, -1);
     tok_str_add(init_str, 0);
        } else {
     skip_or_save_block(&init_str);
        }
        unget_tok(0);
        begin_macro(init_str, 1);
        next();
        decl_initializer(type, ((void*)0), 0, 1, 1);
        macro_ptr = init_str->str;
        next();
        size = type_size(type, &align);
        if (size < 0)
            tcc_error("unknown type size");
    }
    if (ad->a.aligned) {
 int speca = 1 << (ad->a.aligned - 1);
        if (speca > align)
            align = speca;
    } else if (ad->a.packed) {
        align = 1;
    }
    if ((nocode_wanted > 0))
        size = 0, align = 1;
    if ((r & 0x003f) == 0x0032) {
        sec = ((void*)0);
        loc = (loc - size) & -align;
        addr = loc;
        if (v) {
            sym = sym_push(v, type, r, addr);
            sym->a = ad->a;
        } else {
            vset(type, r, addr);
        }
    } else {
        if (v && scope == 0x0030) {
            sym = sym_find(v);
            if (sym) {
                patch_storage(sym, ad, type);
                if (!has_init && sym->c && elfsym(sym)->st_shndx != 0)
                    goto no_alloc;
            }
        }
        sec = ad->section;
        if (!sec) {
            if (has_init)
                sec = data_section;
            else if (tcc_state->nocommon)
                sec = bss_section;
        }
        if (sec) {
     addr = section_add(sec, size, align);
        } else {
            addr = align;
     sec = common_section;
        }
        if (v) {
            if (!sym) {
                sym = sym_push(v, type, r | 0x0200, 0);
                patch_storage(sym, ad, ((void*)0));
            }
            sym->sym_scope = 0;
     put_extern_sym(sym, sec, addr, size);
        } else {
            sym = get_sym_ref(type, sec, addr, size);
     vpushsym(type, sym);
     vtop->r |= r;
        }
    }
    if (type->t & 0x0400) {
        int a;
        if ((nocode_wanted > 0))
            goto no_alloc;
        if (vlas_in_scope == 0) {
            if (vla_sp_root_loc == -1)
                vla_sp_root_loc = (loc -= 4);
            gen_vla_sp_save(vla_sp_root_loc);
        }
        vla_runtime_type_size(type, &a);
        gen_vla_alloc(type, a);
        gen_vla_sp_save(addr);
        vla_sp_loc = addr;
        vlas_in_scope++;
    } else if (has_init) {
 size_t oldreloc_offset = 0;
 if (sec && sec->reloc)
   oldreloc_offset = sec->reloc->data_offset;
        decl_initializer(type, sec, addr, 1, 0);
        if (flexible_array)
            flexible_array->type.ref->c = -1;
    }
 no_alloc:
    if (init_str) {
        end_macro();
        next();
    }
    nocode_wanted = saved_nocode_wanted;
}
static void gen_function(Sym *sym)
{
    nocode_wanted = 0;
    ind = cur_text_section->data_offset;
    put_extern_sym(sym, cur_text_section, ind, 0);
    funcname = get_tok_str(sym->v, ((void*)0));
    func_ind = ind;
    vla_sp_loc = -1;
    vla_sp_root_loc = -1;
    tcc_debug_funcstart(tcc_state, sym);
    sym_push2(&local_stack, 0x20000000, 0, 0);
    local_scope = 1;
    gfunc_prolog(&sym->type);
    local_scope = 0;
    rsym = 0;
    block(((void*)0), ((void*)0), 0);
    nocode_wanted = 0;
    gsym(rsym);
    gfunc_epilog();
    cur_text_section->data_offset = ind;
    local_scope = 0;
    sym_pop(&local_stack, ((void*)0), 0);
    elfsym(sym)->st_size = ind - func_ind;
    tcc_debug_funcend(tcc_state, ind - func_ind);
    cur_text_section = ((void*)0);
    funcname = "";
    func_vt.t = 0;
    func_var = 0;
    ind = 0;
    nocode_wanted = 0x80000000;
    check_vstack();
}

static int decl0(int l, int is_for_loop_init, Sym *func_sym)
{
    int v, has_init, r;
    CType type, btype;
    Sym *sym;
    AttributeDef ad;
    while (1) {
        if (!parse_btype(&btype, &ad)) {
            if (is_for_loop_init)
                return 0;
            if (tok == ';' && l != 0x0033) {
                next();
                continue;
            }
            if (l != 0x0030)
                break;
            if (tok >= TOK_DEFINE) {
                btype.t = 3;
            } else {
                if (tok != (-1))
                    expect("declaration");
                break;
            }
        }
        if (tok == ';') {
     if ((btype.t & 0x000f) == 7) {
  int v = btype.ref->v;
  if (!(v & 0x20000000) && (v & ~0x40000000) >= 0x10000000)
             tcc_warning("unnamed struct/union that defines no instances");
                next();
                continue;
     }
            if (((btype.t & (((1 << (6+6)) - 1) << 20 | 0x0080)) == (2 << 20))) {
                next();
                continue;
            }
        }
        while (1) {
            type = btype;
     if ((type.t & 0x0040) && type.ref->c < 0) {
  type.ref = sym_push(0x20000000, &type.ref->type, 0, type.ref->c);
     }
            type_decl(&type, &ad, &v, 2);
            if ((type.t & 0x000f) == 6) {
                if ((type.t & 0x00002000) && (l == 0x0032)) {
                    tcc_error("function without file scope cannot be static");
                }
                sym = type.ref;
                if (sym->f.func_type == 2 && l == 0x0030)
                    decl0(0x0033, 0, sym);
            }
            if (tok == '{') {
                if (l != 0x0030)
                    tcc_error("cannot use local functions");
                if ((type.t & 0x000f) != 6)
                    expect("function definition");
                sym = type.ref;
                while ((sym = sym->next) != ((void*)0)) {
                    if (!(sym->v & ~0x20000000))
                        expect("identifier");
      if (sym->type.t == 0)
          sym->type = int_type;
  }
                if ((type.t & (0x00001000 | 0x00008000)) == (0x00001000 | 0x00008000))
                    type.t = (type.t & ~0x00001000) | 0x00002000;
                sym = external_global_sym(v, &type, 0);
                type.t &= ~0x00001000;
                patch_storage(sym, &ad, &type);
                if ((type.t & (0x00008000 | 0x00002000)) ==
                    (0x00008000 | 0x00002000)) {
                    struct InlineFunc *fn;
                    const char *filename;
                    filename = file ? file->filename : "";
                    fn = tcc_malloc(sizeof *fn + strlen(filename));
                    strcpy(fn->filename, filename);
                    fn->sym = sym;
      skip_or_save_block(&fn->func_str);
                    dynarray_add(&tcc_state->inline_fns,
     &tcc_state->nb_inline_fns, fn);
                } else {
                    cur_text_section = ad.section;
                    if (!cur_text_section)
                        cur_text_section = text_section;
                    gen_function(sym);
                }
                break;
            } else {
  if (l == 0x0033) {
      for (sym = func_sym->next; sym; sym = sym->next)
   if ((sym->v & ~0x20000000) == v)
       goto found;
      tcc_error("declaration for parameter '%s' but no such parameter",
         get_tok_str(v, ((void*)0)));
found:
      if (type.t & (0x00001000 | 0x00002000 | 0x00004000 | 0x00008000))
          tcc_error("storage class specified for '%s'",
      get_tok_str(v, ((void*)0)));
      if (sym->type.t != 0)
          tcc_error("redefinition of parameter '%s'",
      get_tok_str(v, ((void*)0)));
      convert_parameter_type(&type);
      sym->type = type;
  } else if (type.t & 0x00004000) {
                    sym = sym_find(v);
                    if (sym && sym->sym_scope == local_scope) {
                        if (!is_compatible_types(&sym->type, &type)
                            || !(sym->type.t & 0x00004000))
                            tcc_error("incompatible redefinition of '%s'",
                                get_tok_str(v, ((void*)0)));
                        sym->type = type;
                    } else {
                        sym = sym_push(v, &type, 0, 0);
                    }
                    sym->a = ad.a;
                    sym->f = ad.f;
                } else {
                    r = 0;
                    if ((type.t & 0x000f) == 6) {
                        type.ref->f = ad.f;
                    } else if (!(type.t & 0x0040)) {
                        r |= lvalue_type(type.t);
                    }
                    has_init = (tok == '=');
                    if (has_init && (type.t & 0x0400))
                        tcc_error("variable length array cannot be initialized");
                    if (((type.t & 0x00001000) && (!has_init || l != 0x0030)) ||
   ((type.t & 0x000f) == 6) ||
                        ((type.t & 0x0040) && (type.t & 0x00002000) &&
                         !has_init && l == 0x0030 && type.ref->c < 0)) {
                        type.t |= 0x00001000;
                        sym = external_sym(v, &type, r, &ad);
                        if (ad.alias_target) {
                            Elf32_Sym *esym;
                            Sym *alias_target;
                            alias_target = sym_find(ad.alias_target);
                            esym = elfsym(alias_target);
                            if (!esym)
                                tcc_error("unsupported forward __alias__ attribute");
                            sym->sym_scope = 0;
                            put_extern_sym2(sym, esym->st_shndx, esym->st_value, esym->st_size, 0);
                        }
                    } else {
                        if (type.t & 0x00002000)
                            r |= 0x0030;
                        else
                            r |= l;
                        if (has_init)
                            next();
                        else if (l == 0x0030)
                            type.t |= 0x00001000;
                        decl_initializer_alloc(&type, &ad, r, has_init, v, l);
                    }
                }
                if (tok != ',') {
                    if (is_for_loop_init)
                        return 1;
                    skip(';');
                    break;
                }
                next();
            }
            ad.a.aligned = 0;
        }
    }
    return 0;
}
static void decl(int l)
{
    decl0(l, 0, ((void*)0));
}
static Section *text_section, *data_section, *bss_section;
static Section *common_section;
static Section *cur_text_section;
static Section *last_text_section;
static Section *bounds_section;
static Section *lbounds_section;
static Section *symtab_section;
static Section *stab_section, *stabstr_section;
static int new_undef_sym = 0;
static void tccelf_new(TCCState *s)
{
    dynarray_add(&s->sections, &s->nb_sections, ((void*)0));
    text_section = new_section(s, ".text", 1, (1 << 1) | (1 << 2));
    data_section = new_section(s, ".data", 1, (1 << 1) | (1 << 0));
    bss_section = new_section(s, ".bss", 8, (1 << 1) | (1 << 0));
    common_section = new_section(s, ".common", 8, 0x80000000);
    common_section->sh_num = 0xfff2;
    symtab_section = new_symtab(s, ".symtab", 2, 0,
                                ".strtab",
                                ".hashtab", 0x80000000);
    s->symtab = symtab_section;
    s->dynsymtab_section = new_symtab(s, ".dynsymtab", 2, 0x80000000|0x40000000,
                                      ".dynstrtab",
                                      ".dynhashtab", 0x80000000);
    get_sym_attr(s, 0, 1);
}

static void free_section(Section *s)
{
    tcc_free(s->data);
}
static void tccelf_delete(TCCState *s1)
{
    int i;
    for(i = 1; i < s1->nb_sections; i++) {
        free_section(s1->sections[i]);
    }
    dynarray_reset(&s1->sections, &s1->nb_sections);
    for(i = 0; i < s1->nb_priv_sections; i++) {
        free_section(s1->priv_sections[i]);
    }
    dynarray_reset(&s1->priv_sections, &s1->nb_priv_sections);
    dynarray_reset(&s1->loaded_dlls, &s1->nb_loaded_dlls);
    tcc_free(s1->sym_attrs);
    symtab_section = ((void*)0);
}
static void tccelf_begin_file(TCCState *s1)
{
    Section *s; int i;
    for (i = 1; i < s1->nb_sections; i++) {
        s = s1->sections[i];
        s->sh_offset = s->data_offset;
    }
    s = s1->symtab, s->reloc = s->hash, s->hash = ((void*)0);
}
static void tccelf_end_file(TCCState *s1)
{
    Section *s = s1->symtab;
    int first_sym, nb_syms, *tr, i;
    first_sym = s->sh_offset / sizeof (Elf32_Sym);
    nb_syms = s->data_offset / sizeof (Elf32_Sym) - first_sym;
    s->data_offset = s->sh_offset;
    s->link->data_offset = s->link->sh_offset;
    s->hash = s->reloc, s->reloc = ((void*)0);
    tr = tcc_mallocz(nb_syms * sizeof *tr);
    for (i = 0; i < nb_syms; ++i) {
        Elf32_Sym *sym = (Elf32_Sym*)s->data + first_sym + i;
        if (sym->st_shndx == 0
            && (((unsigned char) (sym->st_info)) >> 4) == 0)
            sym->st_info = (((1) << 4) + ((((sym->st_info) & 0xf)) & 0xf));
        tr[i] = set_elf_sym(s, sym->st_value, sym->st_size, sym->st_info,
            sym->st_other, sym->st_shndx, s->link->data + sym->st_name);
    }
    for (i = 1; i < s1->nb_sections; i++) {
        Section *sr = s1->sections[i];
        if (sr->sh_type == 9 && sr->link == s) {
            Elf32_Rel *rel = (Elf32_Rel*)(sr->data + sr->sh_offset);
            Elf32_Rel *rel_end = (Elf32_Rel*)(sr->data + sr->data_offset);
            for (; rel < rel_end; ++rel) {
                int n = ((rel->r_info) >> 8) - first_sym;
                rel->r_info = (((tr[n]) << 8) + ((((rel->r_info) & 0xff)) & 0xff));
            }
        }
    }
    tcc_free(tr);
}
static Section *new_section(TCCState *s1, const char *name, int sh_type, int sh_flags)
{
    Section *sec;
    sec = tcc_mallocz(sizeof(Section) + strlen(name));
    strcpy(sec->name, name);
    sec->sh_type = sh_type;
    sec->sh_flags = sh_flags;
    switch(sh_type) {
    case 5:
    case 9:
    case 4:
    case 11:
    case 2:
    case 6:
        sec->sh_addralign = 4;
        break;
    case 3:
        sec->sh_addralign = 1;
        break;
    default:
        sec->sh_addralign = 4;
        break;
    }
    if (sh_flags & 0x80000000) {
        dynarray_add(&s1->priv_sections, &s1->nb_priv_sections, sec);
    } else {
        sec->sh_num = s1->nb_sections;
        dynarray_add(&s1->sections, &s1->nb_sections, sec);
    }
    return sec;
}
static Section *new_symtab(TCCState *s1,
                           const char *symtab_name, int sh_type, int sh_flags,
                           const char *strtab_name,
                           const char *hash_name, int hash_sh_flags)
{
    Section *symtab, *strtab, *hash;
    int *ptr, nb_buckets;
    symtab = new_section(s1, symtab_name, sh_type, sh_flags);
    symtab->sh_entsize = sizeof(Elf32_Sym);
    strtab = new_section(s1, strtab_name, 3, sh_flags);
    put_elf_str(strtab, "");
    symtab->link = strtab;
    put_elf_sym(symtab, 0, 0, 0, 0, 0, ((void*)0));
    nb_buckets = 1;
    hash = new_section(s1, hash_name, 5, hash_sh_flags);
    hash->sh_entsize = sizeof(int);
    symtab->hash = hash;
    hash->link = symtab;
    ptr = section_ptr_add(hash, (2 + nb_buckets + 1) * sizeof(int));
    ptr[0] = nb_buckets;
    ptr[1] = 1;
    memset(ptr + 2, 0, (nb_buckets + 1) * sizeof(int));
    return symtab;
}
static void section_realloc(Section *sec, unsigned long new_size)
{
    unsigned long size;
    unsigned char *data;
    size = sec->data_allocated;
    if (size == 0)
        size = 1;
    while (size < new_size)
        size = size * 2;
    data = tcc_realloc(sec->data, size);
    memset(data + sec->data_allocated, 0, size - sec->data_allocated);
    sec->data = data;
    sec->data_allocated = size;
}
static size_t section_add(Section *sec, Elf32_Addr size, int align)
{
    size_t offset, offset1;
    offset = (sec->data_offset + align - 1) & -align;
    offset1 = offset + size;
    if (sec->sh_type != 8 && offset1 > sec->data_allocated)
        section_realloc(sec, offset1);
    sec->data_offset = offset1;
    return offset;
}
static void *section_ptr_add(Section *sec, Elf32_Addr size)
{
    size_t offset = section_add(sec, size, 1);
    return sec->data + offset;
}
static void section_reserve(Section *sec, unsigned long size)
{
    if (size > sec->data_allocated)
        section_realloc(sec, size);
    if (size > sec->data_offset)
        sec->data_offset = size;
}

static int put_elf_str(Section *s, const char *sym)
{
    int offset, len;
    char *ptr;
    len = strlen(sym) + 1;
    offset = s->data_offset;
    ptr = section_ptr_add(s, len);
    memmove(ptr, sym, len);
    return offset;
}
static unsigned long elf_hash(const unsigned char *name)
{
    unsigned long h = 0, g;
    while (*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        if (g)
            h ^= g >> 24;
        h &= ~g;
    }
    return h;
}
static void rebuild_hash(Section *s, unsigned int nb_buckets)
{
    Elf32_Sym *sym;
    int *ptr, *hash, nb_syms, sym_index, h;
    unsigned char *strtab;
    strtab = s->link->data;
    nb_syms = s->data_offset / sizeof(Elf32_Sym);
    if (!nb_buckets)
        nb_buckets = ((int*)s->hash->data)[0];
    s->hash->data_offset = 0;
    ptr = section_ptr_add(s->hash, (2 + nb_buckets + nb_syms) * sizeof(int));
    ptr[0] = nb_buckets;
    ptr[1] = nb_syms;
    ptr += 2;
    hash = ptr;
    memset(hash, 0, (nb_buckets + 1) * sizeof(int));
    ptr += nb_buckets + 1;
    sym = (Elf32_Sym *)s->data + 1;
    for(sym_index = 1; sym_index < nb_syms; sym_index++) {
        if ((((unsigned char) (sym->st_info)) >> 4) != 0) {
            h = elf_hash(strtab + sym->st_name) % nb_buckets;
            *ptr = hash[h];
            hash[h] = sym_index;
        } else {
            *ptr = 0;
        }
        ptr++;
        sym++;
    }
}
static int put_elf_sym(Section *s, Elf32_Addr value, unsigned long size,
    int info, int other, int shndx, const char *name)
{
    int name_offset, sym_index;
    int nbuckets, h;
    Elf32_Sym *sym;
    Section *hs;
    sym = section_ptr_add(s, sizeof(Elf32_Sym));
    if (name && name[0])
        name_offset = put_elf_str(s->link, name);
    else
        name_offset = 0;
    sym->st_name = name_offset;
    sym->st_value = value;
    sym->st_size = size;
    sym->st_info = info;
    sym->st_other = other;
    sym->st_shndx = shndx;
    sym_index = sym - (Elf32_Sym *)s->data;
    hs = s->hash;
    if (hs) {
        int *ptr, *base;
        ptr = section_ptr_add(hs, sizeof(int));
        base = (int *)hs->data;
        if ((((unsigned char) (info)) >> 4) != 0) {
            nbuckets = base[0];
            h = elf_hash((unsigned char *)s->link->data + name_offset) % nbuckets;
            *ptr = base[2 + h];
            base[2 + h] = sym_index;
            base[1]++;
            hs->nb_hashed_syms++;
            if (hs->nb_hashed_syms > 2 * nbuckets) {
                rebuild_hash(s, 2 * nbuckets);
            }
        } else {
            *ptr = 0;
            base[1]++;
        }
    }
    return sym_index;
}
static int find_elf_sym(Section *s, const char *name)
{
    Elf32_Sym *sym;
    Section *hs;
    int nbuckets, sym_index, h;
    const char *name1;
    hs = s->hash;
    if (!hs)
        return 0;
    nbuckets = ((int *)hs->data)[0];
    h = elf_hash((unsigned char *) name) % nbuckets;
    sym_index = ((int *)hs->data)[2 + h];
    while (sym_index != 0) {
        sym = &((Elf32_Sym *)s->data)[sym_index];
        name1 = (char *) s->link->data + sym->st_name;
        if (!strcmp(name, name1))
            return sym_index;
        sym_index = ((int *)hs->data)[2 + nbuckets + sym_index];
    }
    return 0;
}
static Elf32_Addr get_elf_sym_addr(TCCState *s, const char *name, int err)
{
    int sym_index;
    Elf32_Sym *sym;
    sym_index = find_elf_sym(s->symtab, name);
    sym = &((Elf32_Sym *)s->symtab->data)[sym_index];
    if (!sym_index || sym->st_shndx == 0) {
        if (err)
            tcc_error("%s not defined", name);
        return 0;
    }
    return sym->st_value;
}
 void *tcc_get_symbol(TCCState *s, const char *name)
{
    return (void*)(uintptr_t)get_elf_sym_addr(s, name, 0);
}
static void* tcc_get_symbol_err(TCCState *s, const char *name)
{
    return (void*)(uintptr_t)get_elf_sym_addr(s, name, 1);
}
static int set_elf_sym(Section *s, Elf32_Addr value, unsigned long size,
                       int info, int other, int shndx, const char *name)
{
    Elf32_Sym *esym;
    int sym_bind, sym_index, sym_type, esym_bind;
    unsigned char sym_vis, esym_vis, new_vis;
    sym_bind = (((unsigned char) (info)) >> 4);
    sym_type = ((info) & 0xf);
    sym_vis = ((other) & 0x03);
    if (sym_bind != 0) {
        sym_index = find_elf_sym(s, name);
        if (!sym_index)
            goto do_def;
        esym = &((Elf32_Sym *)s->data)[sym_index];
        if (esym->st_value == value && esym->st_size == size && esym->st_info == info
            && esym->st_other == other && esym->st_shndx == shndx)
            return sym_index;
        if (esym->st_shndx != 0) {
            esym_bind = (((unsigned char) (esym->st_info)) >> 4);
            esym_vis = ((esym->st_other) & 0x03);
            if (esym_vis == 0) {
                new_vis = sym_vis;
            } else if (sym_vis == 0) {
                new_vis = esym_vis;
            } else {
                new_vis = (esym_vis < sym_vis) ? esym_vis : sym_vis;
            }
            esym->st_other = (esym->st_other & ~((-1) & 0x03))
                             | new_vis;
            other = esym->st_other;
            if (shndx == 0) {
            } else if (sym_bind == 1 && esym_bind == 2) {
                goto do_patch;
            } else if (sym_bind == 2 && esym_bind == 1) {
            } else if (sym_bind == 2 && esym_bind == 2) {
            } else if (sym_vis == 2 || sym_vis == 1) {
            } else if ((esym->st_shndx == 0xfff2
                            || esym->st_shndx == bss_section->sh_num)
                        && (shndx < 0xff00
                            && shndx != bss_section->sh_num)) {
                goto do_patch;
            } else if (shndx == 0xfff2 || shndx == bss_section->sh_num) {
            } else if (s->sh_flags & 0x40000000) {
     } else if (esym->st_other & 0x04) {
  goto do_patch;
            } else {
                tcc_error_noabort("'%s' defined twice", name);
            }
        } else {
        do_patch:
            esym->st_info = (((sym_bind) << 4) + ((sym_type) & 0xf));
            esym->st_shndx = shndx;
            new_undef_sym = 1;
            esym->st_value = value;
            esym->st_size = size;
            esym->st_other = other;
        }
    } else {
    do_def:
        sym_index = put_elf_sym(s, value, size,
                                (((sym_bind) << 4) + ((sym_type) & 0xf)), other,
                                shndx, name);
    }
    return sym_index;
}
static void put_elf_reloca(Section *symtab, Section *s, unsigned long offset,
                            int type, int symbol, Elf32_Addr addend)
{
    char buf[256];
    Section *sr;
    Elf32_Rel *rel;
    sr = s->reloc;
    if (!sr) {
        snprintf(buf, sizeof(buf), ".rel%s", s->name);
        sr = new_section(tcc_state, buf, 9, symtab->sh_flags);
        sr->sh_entsize = sizeof(Elf32_Rel);
        sr->link = symtab;
        sr->sh_info = s->sh_num;
        s->reloc = sr;
    }
    rel = section_ptr_add(sr, sizeof(Elf32_Rel));
    rel->r_offset = offset;
    rel->r_info = (((symbol) << 8) + ((type) & 0xff));
    if (addend)
        tcc_error("non-zero addend on REL architecture");
}

static struct sym_attr *get_sym_attr(TCCState *s1, int index, int alloc)
{
    int n;
    struct sym_attr *tab;
    if (index >= s1->nb_sym_attrs) {
        if (!alloc)
            return s1->sym_attrs;
        n = 1;
        while (index >= n)
            n *= 2;
        tab = tcc_realloc(s1->sym_attrs, n * sizeof(*s1->sym_attrs));
        s1->sym_attrs = tab;
        memset(s1->sym_attrs + s1->nb_sym_attrs, 0,
               (n - s1->nb_sym_attrs) * sizeof(*s1->sym_attrs));
        s1->nb_sym_attrs = n;
    }
    return &s1->sym_attrs[index];
}
static void sort_syms(TCCState *s1, Section *s)
{
    int *old_to_new_syms;
    Elf32_Sym *new_syms;
    int nb_syms, i;
    Elf32_Sym *p, *q;
    Elf32_Rel *rel;
    Section *sr;
    int type, sym_index;
    nb_syms = s->data_offset / sizeof(Elf32_Sym);
    new_syms = tcc_malloc(nb_syms * sizeof(Elf32_Sym));
    old_to_new_syms = tcc_malloc(nb_syms * sizeof(int));
    p = (Elf32_Sym *)s->data;
    q = new_syms;
    for(i = 0; i < nb_syms; i++) {
        if ((((unsigned char) (p->st_info)) >> 4) == 0) {
            old_to_new_syms[i] = q - new_syms;
            *q++ = *p;
        }
        p++;
    }
    if( s->sh_size )
        s->sh_info = q - new_syms;
    p = (Elf32_Sym *)s->data;
    for(i = 0; i < nb_syms; i++) {
        if ((((unsigned char) (p->st_info)) >> 4) != 0) {
            old_to_new_syms[i] = q - new_syms;
            *q++ = *p;
        }
        p++;
    }
    memcpy(s->data, new_syms, nb_syms * sizeof(Elf32_Sym));
    tcc_free(new_syms);
    for(i = 1; i < s1->nb_sections; i++) {
        sr = s1->sections[i];
        if (sr->sh_type == 9 && sr->link == s) {
            for (rel = (Elf32_Rel *) sr->data + 0; rel < (Elf32_Rel *) (sr->data + sr->data_offset); rel++) {
                sym_index = ((rel->r_info) >> 8);
                type = ((rel->r_info) & 0xff);
                sym_index = old_to_new_syms[sym_index];
                rel->r_info = (((sym_index) << 8) + ((type) & 0xff));
            }
        }
    }
    tcc_free(old_to_new_syms);
}

static int alloc_sec_names(TCCState *s1, int file_type, Section *strsec)
{
    int i;
    Section *s;
    int textrel = 0;
    for(i = 1; i < s1->nb_sections; i++) {
        s = s1->sections[i];
        s->sh_size = s->data_offset;
        if (s->sh_size || (s->sh_flags & (1 << 1)))
            s->sh_name = put_elf_str(strsec, s->name);
    }
    strsec->sh_size = strsec->data_offset;
    return textrel;
}

struct dyn_inf {
    Section *dynamic;
    Section *dynstr;
    unsigned long data_offset;
    Elf32_Addr rel_addr;
    Elf32_Addr rel_size;
};

static int layout_sections(TCCState *s1, Elf32_Phdr *phdr, int phnum,
                           Section *interp, Section* strsec,
                           struct dyn_inf *dyninf, int *sec_order) {
    int i, j, k, file_type, sh_order_index, file_offset;
    unsigned long s_align;
    long long tmp;
    Elf32_Addr addr;
    Elf32_Phdr *ph;
    Section *s;
    file_type = s1->output_type;
    sh_order_index = 1;
    file_offset = 0;
    if (s1->output_format == 0)
        file_offset = sizeof(Elf32_Ehdr) + phnum * sizeof(Elf32_Phdr);
    s_align = 0x1000;
    for(i = 1; i < s1->nb_sections; i++) {
        s = s1->sections[i];
        sec_order[sh_order_index++] = i;
        file_offset = (file_offset + s->sh_addralign - 1) &
            ~(s->sh_addralign - 1);
        s->sh_offset = file_offset;
        if (s->sh_type != 8)
            file_offset += s->sh_size;
    }
    return file_offset;
}

static void tcc_output_elf(TCCState *s1, FILE *f, int phnum, Elf32_Phdr *phdr,
                           int file_offset, int *sec_order)
{
    int i, shnum, offset, size, file_type;
    Section *s;
    Elf32_Ehdr ehdr;
    Elf32_Shdr shdr, *sh;
    file_type = s1->output_type;
    shnum = s1->nb_sections;
    memset(&ehdr, 0, sizeof(ehdr));
    file_offset = (file_offset + 3) & -4;
    ehdr.e_ident[0] = 0x7f;
    ehdr.e_ident[1] = 'E';
    ehdr.e_ident[2] = 'L';
    ehdr.e_ident[3] = 'F';
    ehdr.e_ident[4] = 1;
    ehdr.e_ident[5] = 1;
    ehdr.e_ident[6] = 1;
    ehdr.e_type = 1;
    ehdr.e_machine = 3;
    ehdr.e_version = 1;
    ehdr.e_shoff = file_offset;
    ehdr.e_ehsize = sizeof(Elf32_Ehdr);
    ehdr.e_shentsize = sizeof(Elf32_Shdr);
    ehdr.e_shnum = shnum;
    ehdr.e_shstrndx = shnum - 1;
    fwrite(&ehdr, 1, sizeof(Elf32_Ehdr), f);
    fwrite(phdr, 1, phnum * sizeof(Elf32_Phdr), f);
    offset = sizeof(Elf32_Ehdr) + phnum * sizeof(Elf32_Phdr);
    sort_syms(s1, symtab_section);
    for(i = 1; i < s1->nb_sections; i++) {
        s = s1->sections[sec_order[i]];
        if (s->sh_type != 8) {
            while (offset < s->sh_offset) {
                fputc(0, f);
                offset++;
            }
            size = s->sh_size;
            if (size)
                fwrite(s->data, 1, size, f);
            offset += size;
        }
    }
    while (offset < ehdr.e_shoff) {
        fputc(0, f);
        offset++;
    }
    for(i = 0; i < s1->nb_sections; i++) {
        sh = &shdr;
        memset(sh, 0, sizeof(Elf32_Shdr));
        s = s1->sections[i];
        if (s) {
            sh->sh_name = s->sh_name;
            sh->sh_type = s->sh_type;
            sh->sh_flags = s->sh_flags;
            sh->sh_entsize = s->sh_entsize;
            sh->sh_info = s->sh_info;
            if (s->link)
                sh->sh_link = s->link->sh_num;
            sh->sh_addralign = s->sh_addralign;
            sh->sh_addr = s->sh_addr;
            sh->sh_offset = s->sh_offset;
            sh->sh_size = s->sh_size;
        }
        fwrite(sh, 1, sizeof(Elf32_Shdr), f);
    }
}

static int tcc_write_elf_file(TCCState *s1, const char *filename, int phnum,
                              Elf32_Phdr *phdr, int file_offset, int *sec_order)
{
    int fd, mode, file_type;
    FILE *f;
    file_type = s1->output_type;
    mode = 0666;
    unlink(filename);
    f = fopen(filename, "wb");
    tcc_output_elf(s1, f, phnum, phdr, file_offset, sec_order);
    fclose(f);
    return 0;
}

static int elf_output_file(TCCState *s1, const char *filename) {
    int i, ret, phnum, shnum, file_type, file_offset, *sec_order;
    struct dyn_inf dyninf = {0};
    Elf32_Phdr *phdr;
    Elf32_Sym *sym;
    Section *strsec, *interp, *dynamic, *dynstr;
    int textrel;
    file_type = s1->output_type;
    s1->nb_errors = 0;
    ret = -1;
    phdr = ((void*)0);
    sec_order = ((void*)0);
    interp = dynamic = dynstr = ((void*)0);
    textrel = 0;
    strsec = new_section(s1, ".shstrtab", 3, 0);
    put_elf_str(strsec, "");
    textrel = alloc_sec_names(s1, file_type, strsec);
    phnum = 0;
    phdr = tcc_mallocz(phnum * sizeof(Elf32_Phdr));
    shnum = s1->nb_sections;
    sec_order = tcc_malloc(sizeof(int) * shnum);
    sec_order[0] = 0;
    file_offset = layout_sections(s1, phdr, phnum, interp, strsec, &dyninf,
                                  sec_order);
    ret = tcc_write_elf_file(s1, filename, phnum, phdr, file_offset, sec_order);
    s1->nb_sections = shnum;
    tcc_free(sec_order);
    tcc_free(phdr);
    return ret;
}
 int tcc_output_file(TCCState *s, const char *filename)
{
    int ret;
        ret = elf_output_file(s, filename);
    return ret;
}

static void *load_data(int fd, unsigned long file_offset, unsigned long size) {
exit(1);
}

static int tcc_object_type(int fd, Elf32_Ehdr *h) {
exit(1);
}

static int tcc_load_object_file(TCCState *s1,
                                int fd, unsigned long file_offset) {
exit(1);
}

static int tcc_load_alacarte(TCCState *s1, int fd, int size, int entrysize) {
exit(1);
}

static int tcc_load_archive(TCCState *s1, int fd) {
exit(1);
}

static int ld_next(TCCState *s1, char *name, int name_size) {
exit(1);
}

static inline int new_undef_syms(void) {
exit(1);
}

static int ld_add_file_list(TCCState *s1, const char *cmd, int as_needed) {
exit(1);
}

extern void *mmap (void *__addr, size_t __len, int __prot,
     int __flags, int __fd, __off_t __offset) ;
extern void *mmap64 (void *__addr, size_t __len, int __prot,
       int __flags, int __fd, __off64_t __offset) ;
extern int munmap (void *__addr, size_t __len) ;
extern int mprotect (void *__addr, size_t __len, int __prot) ;
extern int msync (void *__addr, size_t __len, int __flags) ;
extern int madvise (void *__addr, size_t __len, int __advice) ;
extern int posix_madvise (void *__addr, size_t __len, int __advice) ;
extern int mlock (__const void *__addr, size_t __len) ;
extern int munlock (__const void *__addr, size_t __len) ;
extern int mlockall (int __flags) ;
extern int munlockall (void) ;
extern void *mremap (void *__addr, size_t __old_len, size_t __new_len,
       int __may_move) ;
extern int mincore (void *__start, size_t __len, unsigned char *__vec);
extern int shm_open (__const char *__name, int __oflag, mode_t __mode);
extern int shm_unlink (__const char *__name);
extern int __sigismember (__const __sigset_t *, int);
extern int __sigaddset (__sigset_t *, int);
extern int __sigdelset (__sigset_t *, int);
typedef __sig_atomic_t sig_atomic_t;
typedef void (*__sighandler_t) (int);
extern __sighandler_t __sysv_signal (int __sig, __sighandler_t __handler)
     ;
extern __sighandler_t sysv_signal (int __sig, __sighandler_t __handler)
     ;
extern __sighandler_t signal (int __sig, __sighandler_t __handler) ;
extern __sighandler_t bsd_signal (int __sig, __sighandler_t __handler) ;
extern int kill (__pid_t __pid, int __sig) ;
extern int killpg (__pid_t __pgrp, int __sig) ;
extern int raise (int __sig) ;
extern __sighandler_t ssignal (int __sig, __sighandler_t __handler) ;
extern int gsignal (int __sig) ;
extern void psignal (int __sig, __const char *__s) ;
extern int __sigpause (int __sig_or_mask, int __is_sig) ;
extern int sigpause (int __mask) ;
extern int sigblock (int __mask) ;
extern int sigsetmask (int __mask) ;
extern int siggetmask (void) ;
typedef __sighandler_t sighandler_t;
typedef __sighandler_t sig_t;
typedef union sigval
  {
    int sival_int;
    void *sival_ptr;
  } sigval_t;
typedef struct siginfo
  {
    int si_signo;
    int si_errno;
    int si_code;
    union
      {
 int _pad[((128 / sizeof (int)) - 3)];
 struct
   {
     __pid_t si_pid;
     __uid_t si_uid;
   } _kill;
 struct
   {
     unsigned int _timer1;
     unsigned int _timer2;
   } _timer;
 struct
   {
     __pid_t si_pid;
     __uid_t si_uid;
     sigval_t si_sigval;
   } _rt;
 struct
   {
     __pid_t si_pid;
     __uid_t si_uid;
     int si_status;
     __clock_t si_utime;
     __clock_t si_stime;
   } _sigchld;
 struct
   {
     void *si_addr;
   } _sigfault;
 struct
   {
     long int si_band;
     int si_fd;
   } _sigpoll;
      } _sifields;
  } siginfo_t;
enum
{
  SI_ASYNCNL = -6,
  SI_SIGIO,
  SI_ASYNCIO,
  SI_MESGQ,
  SI_TIMER,
  SI_QUEUE,
  SI_USER,
  SI_KERNEL = 0x80
};
enum
{
  ILL_ILLOPC = 1,
  ILL_ILLOPN,
  ILL_ILLADR,
  ILL_ILLTRP,
  ILL_PRVOPC,
  ILL_PRVREG,
  ILL_COPROC,
  ILL_BADSTK
};
enum
{
  FPE_INTDIV = 1,
  FPE_INTOVF,
  FPE_FLTDIV,
  FPE_FLTOVF,
  FPE_FLTUND,
  FPE_FLTRES,
  FPE_FLTINV,
  FPE_FLTSUB
};
enum
{
  SEGV_MAPERR = 1,
  SEGV_ACCERR
};
enum
{
  BUS_ADRALN = 1,
  BUS_ADRERR,
  BUS_OBJERR
};
enum
{
  TRAP_BRKPT = 1,
  TRAP_TRACE
};
enum
{
  CLD_EXITED = 1,
  CLD_KILLED,
  CLD_DUMPED,
  CLD_TRAPPED,
  CLD_STOPPED,
  CLD_CONTINUED
};
enum
{
  POLL_IN = 1,
  POLL_OUT,
  POLL_MSG,
  POLL_ERR,
  POLL_PRI,
  POLL_HUP
};
struct __pthread_attr_s;
typedef struct sigevent
  {
    sigval_t sigev_value;
    int sigev_signo;
    int sigev_notify;
    union
      {
 int _pad[((64 / sizeof (int)) - 3)];
 struct
   {
     void (*_function) (sigval_t);
     struct __pthread_attr_s *_attribute;
   } _sigev_thread;
      } _sigev_un;
  } sigevent_t;
enum
{
  SIGEV_SIGNAL = 0,
  SIGEV_NONE,
  SIGEV_THREAD
};
extern int sigemptyset (sigset_t *__set) ;
extern int sigfillset (sigset_t *__set) ;
extern int sigaddset (sigset_t *__set, int __signo) ;
extern int sigdelset (sigset_t *__set, int __signo) ;
extern int sigismember (__const sigset_t *__set, int __signo) ;
extern int sigisemptyset (__const sigset_t *__set) ;
extern int sigandset (sigset_t *__set, __const sigset_t *__left,
        __const sigset_t *__right) ;
extern int sigorset (sigset_t *__set, __const sigset_t *__left,
       __const sigset_t *__right) ;
struct sigaction
  {
    union
      {
 __sighandler_t sa_handler;
 void (*sa_sigaction) (int, siginfo_t *, void *);
      }
    __sigaction_handler;
    __sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer) (void);
  };
extern int sigprocmask (int __how, __const sigset_t *__restrict __set,
   sigset_t *__restrict __oset) ;
extern int sigsuspend (__const sigset_t *__set) ;
extern int sigaction (int __sig, __const struct sigaction *__restrict __act,
        struct sigaction *__restrict __oact) ;
extern int sigpending (sigset_t *__set) ;
extern int sigwait (__const sigset_t *__restrict __set, int *__restrict __sig)
     ;
extern int sigwaitinfo (__const sigset_t *__restrict __set,
   siginfo_t *__restrict __info) ;
extern int sigtimedwait (__const sigset_t *__restrict __set,
    siginfo_t *__restrict __info,
    __const struct timespec *__restrict __timeout)
     ;
extern int sigqueue (__pid_t __pid, int __sig, __const union sigval __val)
     ;
extern __const char *__const _sys_siglist[64];
extern __const char *__const sys_siglist[64];
struct sigvec
  {
    __sighandler_t sv_handler;
    int sv_mask;
    int sv_flags;
  };
extern int sigvec (int __sig, __const struct sigvec *__vec,
     struct sigvec *__ovec) ;
struct _fpreg {
 unsigned short significand[4];
 unsigned short exponent;
};
struct _fpxreg {
 unsigned short significand[4];
 unsigned short exponent;
 unsigned short padding[3];
};
struct _xmmreg {
 unsigned long element[4];
};
struct _fpstate {
 unsigned long cw;
 unsigned long sw;
 unsigned long tag;
 unsigned long ipoff;
 unsigned long cssel;
 unsigned long dataoff;
 unsigned long datasel;
 struct _fpreg _st[8];
 unsigned short status;
 unsigned short magic;
 unsigned long _fxsr_env[6];
 unsigned long mxcsr;
 unsigned long reserved;
 struct _fpxreg _fxsr_st[8];
 struct _xmmreg _xmm[8];
 unsigned long padding[56];
};
struct sigcontext {
 unsigned short gs, __gsh;
 unsigned short fs, __fsh;
 unsigned short es, __esh;
 unsigned short ds, __dsh;
 unsigned long edi;
 unsigned long esi;
 unsigned long ebp;
 unsigned long esp;
 unsigned long ebx;
 unsigned long edx;
 unsigned long ecx;
 unsigned long eax;
 unsigned long trapno;
 unsigned long err;
 unsigned long eip;
 unsigned short cs, __csh;
 unsigned long eflags;
 unsigned long esp_at_signal;
 unsigned short ss, __ssh;
 struct _fpstate * fpstate;
 unsigned long oldmask;
 unsigned long cr2;
};
extern int sigreturn (struct sigcontext *__scp) ;
extern int siginterrupt (int __sig, int __interrupt) ;
struct sigstack
  {
    void *ss_sp;
    int ss_onstack;
  };
enum
{
  SS_ONSTACK = 1,
  SS_DISABLE
};
typedef struct sigaltstack
  {
    void *ss_sp;
    int ss_flags;
    size_t ss_size;
  } stack_t;
typedef int greg_t;
typedef greg_t gregset_t[19];
enum
{
  REG_GS = 0,
  REG_FS,
  REG_ES,
  REG_DS,
  REG_EDI,
  REG_ESI,
  REG_EBP,
  REG_ESP,
  REG_EBX,
  REG_EDX,
  REG_ECX,
  REG_EAX,
  REG_TRAPNO,
  REG_ERR,
  REG_EIP,
  REG_CS,
  REG_EFL,
  REG_UESP,
  REG_SS
};
struct _libc_fpreg
{
  unsigned short int significand[4];
  unsigned short int exponent;
};
struct _libc_fpstate
{
  unsigned long int cw;
  unsigned long int sw;
  unsigned long int tag;
  unsigned long int ipoff;
  unsigned long int cssel;
  unsigned long int dataoff;
  unsigned long int datasel;
  struct _libc_fpreg _st[8];
  unsigned long int status;
};
typedef struct _libc_fpstate *fpregset_t;
typedef struct
  {
    gregset_t gregs;
    fpregset_t fpregs;
    unsigned long int oldmask;
    unsigned long int cr2;
  } mcontext_t;
typedef struct ucontext
  {
    unsigned long int uc_flags;
    struct ucontext *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    __sigset_t uc_sigmask;
    struct _libc_fpstate __fpregs_mem;
  } ucontext_t;
extern int getcontext (ucontext_t *__ucp) ;
extern int setcontext (__const ucontext_t *__ucp) ;
extern int swapcontext (ucontext_t *__restrict __oucp,
   __const ucontext_t *__restrict __ucp) ;
extern void makecontext (ucontext_t *__ucp, void (*__func) (void),
    int __argc, ...) ;
extern int sigstack (struct sigstack *__ss, struct sigstack *__oss) ;
extern int sigaltstack (__const struct sigaltstack *__restrict __ss,
   struct sigaltstack *__restrict __oss) ;
extern int sighold (int __sig) ;
extern int sigrelse (int __sig) ;
extern int sigignore (int __sig) ;
extern __sighandler_t sigset (int __sig, __sighandler_t __disp) ;
extern int pthread_sigmask (int __how,
       __const __sigset_t *__restrict __newmask,
       __sigset_t *__restrict __oldmask);
extern int __libc_current_sigrtmin (void) ;
extern int __libc_current_sigrtmax (void) ;
static int rt_num_callers = 6;
static const char **rt_bound_error_msg;
static void *rt_prog_main;

static Elf32_Addr rt_printline(Elf32_Addr wanted_pc, const char *msg) {
exit(1);
}

static const int reg_classes[5] = {
              0x0001 | 0x0004,
              0x0001 | 0x0010,
              0x0001 | 0x0020,
              (0x0001 | 0x0040) * 0,
              0x0002 | 0x0008,
};
static unsigned long func_sub_sp_offset;
static int func_ret_sub;
static Elf32_Addr func_bound_offset;
static unsigned long func_bound_ind;
static void g(int c)
{
    int ind1;
    if (nocode_wanted)
        return;
    ind1 = ind + 1;
    if (ind1 > cur_text_section->data_allocated)
        section_realloc(cur_text_section, ind1);
    cur_text_section->data[ind] = c;
    ind = ind1;
}
static void o(unsigned int c)
{
    while (c) {
        g(c);
        c = c >> 8;
    }
}
static void gen_le16(int v)
{
    g(v);
    g(v >> 8);
}
static void gen_le32(int c)
{
    g(c);
    g(c >> 8);
    g(c >> 16);
    g(c >> 24);
}
static void gsym_addr(int t, int a)
{
    while (t) {
        unsigned char *ptr = cur_text_section->data + t;
        uint32_t n = read32le(ptr);
        write32le(ptr, a - t - 4);
        t = n;
    }
}
static void gsym(int t)
{
    gsym_addr(t, ind);
}
static int oad(int c, int s)
{
    int t;
    if (nocode_wanted)
        return s;
    o(c);
    t = ind;
    gen_le32(s);
    return t;
}
static void gen_addr32(int r, Sym *sym, int c)
{
    if (r & 0x0200)
        greloc(cur_text_section, sym, ind, 1);
    gen_le32(c);
}
static void gen_addrpc32(int r, Sym *sym, int c)
{
    if (r & 0x0200)
        greloc(cur_text_section, sym, ind, 2);
    gen_le32(c - 4);
}
static void gen_modrm(int op_reg, int r, Sym *sym, int c)
{
    op_reg = op_reg << 3;
    if ((r & 0x003f) == 0x0030) {
        o(0x05 | op_reg);
        gen_addr32(r, sym, c);
    } else if ((r & 0x003f) == 0x0032) {
        if (c == (char)c) {
            o(0x45 | op_reg);
            g(c);
        } else {
            oad(0x85 | op_reg, c);
        }
    } else {
        g(0x00 | op_reg | (r & 0x003f));
    }
}
static void load(int r, SValue *sv)
{
    int v, t, ft, fc, fr;
    SValue v1;
    fr = sv->r;
    ft = sv->type.t & ~0x0020;
    fc = sv->c.i;
    ft &= ~(0x0200 | 0x0100);
    v = fr & 0x003f;
    if (fr & 0x0100) {
        if (v == 0x0031) {
            v1.type.t = 3;
            v1.r = 0x0032 | 0x0100;
            v1.c.i = fc;
            fr = r;
            if (!(reg_classes[fr] & 0x0001))
                fr = get_reg(0x0001);
            load(fr, &v1);
        }
        if ((ft & 0x000f) == 8) {
            o(0xd9);
            r = 0;
        } else if ((ft & 0x000f) == 9) {
            o(0xdd);
            r = 0;
        } else if ((ft & 0x000f) == 10) {
            o(0xdb);
            r = 5;
        } else if ((ft & (~((0x00001000 | 0x00002000 | 0x00004000 | 0x00008000)|(((1 << (6+6)) - 1) << 20 | 0x0080)))) == 1 || (ft & (~((0x00001000 | 0x00002000 | 0x00004000 | 0x00008000)|(((1 << (6+6)) - 1) << 20 | 0x0080)))) == 11) {
            o(0xbe0f);
        } else if ((ft & (~((0x00001000 | 0x00002000 | 0x00004000 | 0x00008000)|(((1 << (6+6)) - 1) << 20 | 0x0080)))) == (1 | 0x0010)) {
            o(0xb60f);
        } else if ((ft & (~((0x00001000 | 0x00002000 | 0x00004000 | 0x00008000)|(((1 << (6+6)) - 1) << 20 | 0x0080)))) == 2) {
            o(0xbf0f);
        } else if ((ft & (~((0x00001000 | 0x00002000 | 0x00004000 | 0x00008000)|(((1 << (6+6)) - 1) << 20 | 0x0080)))) == (2 | 0x0010)) {
            o(0xb70f);
        } else {
            o(0x8b);
        }
        gen_modrm(r, fr, sv->sym, fc);
    } else {
        if (v == 0x0030) {
            o(0xb8 + r);
            gen_addr32(fr, sv->sym, fc);
        } else if (v == 0x0032) {
            if (fc) {
                o(0x8d);
                gen_modrm(r, 0x0032, sv->sym, fc);
            } else {
                o(0x89);
                o(0xe8 + r);
            }
        } else if (v == 0x0033) {
            oad(0xb8 + r, 0);
            o(0x0f);
            o(fc);
            o(0xc0 + r);
        } else if (v == 0x0034 || v == 0x0035) {
            t = v & 1;
            oad(0xb8 + r, t);
            o(0x05eb);
            gsym(fc);
            oad(0xb8 + r, t ^ 1);
        } else if (v != r) {
            o(0x89);
            o(0xc0 + r + v * 8);
        }
    }
}
static void store(int r, SValue *v)
{
    int fr, bt, ft, fc;
    ft = v->type.t;
    fc = v->c.i;
    fr = v->r & 0x003f;
    ft &= ~(0x0200 | 0x0100);
    bt = ft & 0x000f;
    if (bt == 8) {
        o(0xd9);
        r = 2;
    } else if (bt == 9) {
        o(0xdd);
        r = 2;
    } else if (bt == 10) {
        o(0xc0d9);
        o(0xdb);
        r = 7;
    } else {
        if (bt == 2)
            o(0x66);
        if (bt == 1 || bt == 11)
            o(0x88);
        else
            o(0x89);
    }
    if (fr == 0x0030 ||
        fr == 0x0032 ||
        (v->r & 0x0100)) {
        gen_modrm(r, v->r, v->sym, fc);
    } else if (fr != r) {
        o(0xc0 + fr + r * 8);
    }
}
static void gadd_sp(int val)
{
    if (val == (char)val) {
        o(0xc483);
        g(val);
    } else {
        oad(0xc481, val);
    }
}
static void gen_static_call(int v)
{
    Sym *sym;
    sym = external_global_sym(v, &func_old_type, 0);
    oad(0xe8, -4);
    greloc(cur_text_section, sym, ind-4, 2);
}
static void gcall_or_jmp(int is_jmp)
{
    int r;
    if ((vtop->r & (0x003f | 0x0100)) == 0x0030 && (vtop->r & 0x0200)) {
        greloc(cur_text_section, vtop->sym, ind + 1, 2);
        oad(0xe8 + is_jmp, vtop->c.i - 4);
    } else {
        r = gv(0x0001);
        o(0xff);
        o(0xd0 + r + (is_jmp << 4));
    }
    if (!is_jmp) {
        int rt;
        rt = vtop->type.ref->type.t;
        switch (rt & 0x000f) {
            case 1:
                if (rt & 0x0010) {
                    o(0xc0b60f);
                }
                else {
                    o(0xc0be0f);
                }
                break;
            case 2:
                if (rt & 0x0010) {
                    o(0xc0b70f);
                }
                else {
                    o(0xc0bf0f);
                }
                break;
            default:
                break;
        }
    }
}
static uint8_t fastcall_regs[3] = { TREG_EAX, TREG_EDX, TREG_ECX };
static uint8_t fastcallw_regs[2] = { TREG_ECX, TREG_EDX };
static int gfunc_sret(CType *vt, int variadic, CType *ret, int *ret_align, int *regsize)
{
    *ret_align = 1;
    return 0;
}
static void gfunc_call(int nb_args)
{
    int size, align, r, args_size, i, func_call;
    Sym *func_sym;
    args_size = 0;
    for(i = 0;i < nb_args; i++) {
        if ((vtop->type.t & 0x000f) == 7) {
            size = type_size(&vtop->type, &align);
            size = (size + 3) & ~3;
            oad(0xec81, size);
            r = get_reg(0x0001);
            o(0x89);
            o(0xe0 + r);
            vset(&vtop->type, r | 0x0100, 0);
            vswap();
            vstore();
            args_size += size;
        } else if (is_float(vtop->type.t)) {
            gv(0x0002);
            if ((vtop->type.t & 0x000f) == 8)
                size = 4;
            else if ((vtop->type.t & 0x000f) == 9)
                size = 8;
            else
                size = 12;
            oad(0xec81, size);
            if (size == 12)
                o(0x7cdb);
            else
                o(0x5cd9 + size - 4);
            g(0x24);
            g(0x00);
            args_size += size;
        } else {
            r = gv(0x0001);
            if ((vtop->type.t & 0x000f) == 4) {
                size = 8;
                o(0x50 + vtop->r2);
            } else {
                size = 4;
            }
            o(0x50 + r);
            args_size += size;
        }
        vtop--;
    }
    save_regs(0);
    func_sym = vtop->type.ref;
    func_call = func_sym->f.func_call;
    if ((func_call >= 2 && func_call <= 4) ||
        func_call == 5) {
        int fastcall_nb_regs;
        uint8_t *fastcall_regs_ptr;
        if (func_call == 5) {
            fastcall_regs_ptr = fastcallw_regs;
            fastcall_nb_regs = 2;
        } else {
            fastcall_regs_ptr = fastcall_regs;
            fastcall_nb_regs = func_call - 2 + 1;
        }
        for(i = 0;i < fastcall_nb_regs; i++) {
            if (args_size <= 0)
                break;
            o(0x58 + fastcall_regs_ptr[i]);
            args_size -= 4;
        }
    }
    else if ((vtop->type.ref->type.t & 0x000f) == 7)
        args_size -= 4;
    gcall_or_jmp(0);
    if (args_size && func_call != 1 && func_call != 5)
        gadd_sp(args_size);
    vtop--;
}
static void gfunc_prolog(CType *func_type)
{
    int addr, align, size, func_call, fastcall_nb_regs;
    int param_index, param_addr;
    uint8_t *fastcall_regs_ptr;
    Sym *sym;
    CType *type;
    sym = func_type->ref;
    func_call = sym->f.func_call;
    addr = 8;
    loc = 0;
    func_vc = 0;
    if (func_call >= 2 && func_call <= 4) {
        fastcall_nb_regs = func_call - 2 + 1;
        fastcall_regs_ptr = fastcall_regs;
    } else if (func_call == 5) {
        fastcall_nb_regs = 2;
        fastcall_regs_ptr = fastcallw_regs;
    } else {
        fastcall_nb_regs = 0;
        fastcall_regs_ptr = ((void*)0);
    }
    param_index = 0;
    ind += (9 + 0);
    func_sub_sp_offset = ind;
    func_vt = sym->type;
    func_var = (sym->f.func_type == 3);
    if ((func_vt.t & 0x000f) == 7) {
        func_vc = addr;
        addr += 4;
        param_index++;
    }
    while ((sym = sym->next) != ((void*)0)) {
        type = &sym->type;
        size = type_size(type, &align);
        size = (size + 3) & ~3;
        if (param_index < fastcall_nb_regs) {
            loc -= 4;
            o(0x89);
            gen_modrm(fastcall_regs_ptr[param_index], 0x0032, ((void*)0), loc);
            param_addr = loc;
        } else {
            param_addr = addr;
            addr += size;
        }
        sym_push(sym->v & ~0x20000000, type,
                 0x0032 | lvalue_type(type->t), param_addr);
        param_index++;
    }
    func_ret_sub = 0;
    if (func_call == 1 || func_call == 5)
        func_ret_sub = addr - 8;
    else if (func_vc)
        func_ret_sub = 4;
}
static void gfunc_epilog(void)
{
    Elf32_Addr v, saved_ind;
    v = (-loc + 3) & -4;
    o(0xc9);
    if (func_ret_sub == 0) {
        o(0xc3);
    } else {
        o(0xc2);
        g(func_ret_sub);
        g(func_ret_sub >> 8);
    }
    saved_ind = ind;
    ind = func_sub_sp_offset - (9 + 0);
    {
        o(0xe58955);
        o(0xec81);
        gen_le32(v);
    }
    o(0x53 * 0);
    ind = saved_ind;
}
static int gjmp(int t)
{
    return oad(0xe9,t);
}
static void gjmp_addr(int a)
{
    int r;
    r = a - ind - 2;
    if (r == (char)r) {
        g(0xeb);
        g(r);
    } else {
        oad(0xe9, a - ind - 5);
    }
}
static void gtst_addr(int inv, int a)
{
    int v = vtop->r & 0x003f;
    if (v == 0x0033) {
 inv ^= (vtop--)->c.i;
 a -= ind + 2;
 if (a == (char)a) {
     g(inv - 32);
     g(a);
 } else {
     g(0x0f);
     oad(inv - 16, a - 4);
 }
    } else if ((v & ~1) == 0x0034) {
 if ((v & 1) != inv) {
     gjmp_addr(a);
     gsym(vtop->c.i);
 } else {
     gsym(vtop->c.i);
     o(0x05eb);
     gjmp_addr(a);
 }
 vtop--;
    }
}
static int gtst(int inv, int t)
{
    int v = vtop->r & 0x003f;
    if (nocode_wanted) {
        ;
    } else if (v == 0x0033) {
        g(0x0f);
        t = oad((vtop->c.i - 16) ^ inv,t);
    } else if (v == 0x0034 || v == 0x0035) {
        if ((v & 1) == inv) {
            uint32_t n1, n = vtop->c.i;
            if (n) {
                while ((n1 = read32le(cur_text_section->data + n)))
                    n = n1;
                write32le(cur_text_section->data + n, t);
                t = vtop->c.i;
            }
        } else {
            t = gjmp(t);
            gsym(vtop->c.i);
        }
    }
    vtop--;
    return t;
}
static void gen_opi(int op)
{
    int r, fr, opc, c;
    switch(op) {
    case '+':
    case 0xc3:
        opc = 0;
    gen_op8:
        if ((vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030) {
            vswap();
            r = gv(0x0001);
            vswap();
            c = vtop->c.i;
            if (c == (char)c) {
                if (c==1 && opc==0 && op != 0xc3) {
                    o (0x40 | r);
                } else if (c==1 && opc==5 && op != 0xc5) {
                    o (0x48 | r);
                } else {
                    o(0x83);
                    o(0xc0 | (opc << 3) | r);
                    g(c);
                }
            } else {
                o(0x81);
                oad(0xc0 | (opc << 3) | r, c);
            }
        } else {
            gv2(0x0001, 0x0001);
            r = vtop[-1].r;
            fr = vtop[0].r;
            o((opc << 3) | 0x01);
            o(0xc0 + r + fr * 8);
        }
        vtop--;
        if (op >= 0x92 && op <= 0x9f) {
            vtop->r = 0x0033;
            vtop->c.i = op;
        }
        break;
    case '-':
    case 0xc5:
        opc = 5;
        goto gen_op8;
    case 0xc4:
        opc = 2;
        goto gen_op8;
    case 0xc6:
        opc = 3;
        goto gen_op8;
    case '&':
        opc = 4;
        goto gen_op8;
    case '^':
        opc = 6;
        goto gen_op8;
    case '|':
        opc = 1;
        goto gen_op8;
    case '*':
        gv2(0x0001, 0x0001);
        r = vtop[-1].r;
        fr = vtop[0].r;
        vtop--;
        o(0xaf0f);
        o(0xc0 + fr + r * 8);
        break;
    case 0x01:
        opc = 4;
        goto gen_shift;
    case 0xc9:
        opc = 5;
        goto gen_shift;
    case 0x02:
        opc = 7;
    gen_shift:
        opc = 0xc0 | (opc << 3);
        if ((vtop->r & (0x003f | 0x0100 | 0x0200)) == 0x0030) {
            vswap();
            r = gv(0x0001);
            vswap();
            c = vtop->c.i & 0x1f;
            o(0xc1);
            o(opc | r);
            g(c);
        } else {
            gv2(0x0001, 0x0010);
            r = vtop[-1].r;
            o(0xd3);
            o(opc | r);
        }
        vtop--;
        break;
    case '/':
    case 0xb0:
    case 0xb2:
    case '%':
    case 0xb1:
    case 0xc2:
        gv2(0x0004, 0x0010);
        r = vtop[-1].r;
        fr = vtop[0].r;
        vtop--;
        save_reg(TREG_EDX);
        save_reg_upstack(TREG_EAX, 1);
        if (op == 0xc2) {
            o(0xf7);
            o(0xe0 + fr);
            vtop->r2 = TREG_EDX;
            r = TREG_EAX;
        } else {
            if (op == 0xb0 || op == 0xb1) {
                o(0xf7d231);
                o(0xf0 + fr);
            } else {
                o(0xf799);
                o(0xf8 + fr);
            }
            if (op == '%' || op == 0xb1)
                r = TREG_EDX;
            else
                r = TREG_EAX;
        }
        vtop->r = r;
        break;
    default:
        opc = 7;
        goto gen_op8;
    }
}

static void gen_opf(int op) {
exit(1);
}

static void gen_cvt_itof(int t) {
exit(1);
}

static void gen_cvt_ftoi(int t) {
exit(1);
}

static void gen_cvt_ftof(int t) {
exit(1);
}

static void ggoto(void)
{
    gcall_or_jmp(1);
    vtop--;
}

static void gen_vla_sp_save(int addr) {
exit(1);
}

static void gen_vla_sp_restore(int addr) {
exit(1);
}

static void gen_vla_alloc(CType *type, int align) {
exit(1);
}

static char *pstrcpy(char *buf, int buf_size, const char *s) {
    char *q, *q_end;
    int c;
    if (buf_size > 0) {
        q = buf;
        q_end = buf + buf_size - 1;
        while (q < q_end) {
            c = *s++;
            if (c == '\0')
                break;
            *q++ = c;
        }
        *q = '\0';
    }
    return buf;
}

static char *pstrncpy(char *out, const char *in, size_t num)
{
    memcpy(out, in, num);
    out[num] = '\0';
    return out;
}
 char *tcc_basename(const char *name)
{
    char *p = strchr(name, 0);
    while (p > name && !(p[-1] == '/'))
        --p;
    return p;
}
 char *tcc_fileextension (const char *name)
{
    char *b = tcc_basename(name);
    char *e = strrchr(b, '.');
    return e ? e : strchr(b, 0);
}
 void tcc_free(void *ptr)
{
    free(ptr);
}
 void *tcc_malloc(unsigned long size)
{
    void *ptr;
    ptr = malloc(size);
    if (!ptr && size)
        tcc_error("memory full (malloc)");
    return ptr;
}
 void *tcc_mallocz(unsigned long size)
{
    void *ptr;
    ptr = tcc_malloc(size);
    memset(ptr, 0, size);
    return ptr;
}
 void *tcc_realloc(void *ptr, unsigned long size)
{
    void *ptr1;
    ptr1 = realloc(ptr, size);
    if (!ptr1 && size)
        tcc_error("memory full (realloc)");
    return ptr1;
}
 char *tcc_strdup(const char *str)
{
    char *ptr;
    ptr = tcc_malloc(strlen(str) + 1);
    strcpy(ptr, str);
    return ptr;
}
 void tcc_memcheck(void)
{
}
static void dynarray_add(void *ptab, int *nb_ptr, void *data)
{
    int nb, nb_alloc;
    void **pp;
    nb = *nb_ptr;
    pp = *(void ***)ptab;
    if ((nb & (nb - 1)) == 0) {
        if (!nb)
            nb_alloc = 1;
        else
            nb_alloc = nb * 2;
        pp = tcc_realloc(pp, nb_alloc * sizeof(void *));
        *(void***)ptab = pp;
    }
    pp[nb++] = data;
    *nb_ptr = nb;
}
static void dynarray_reset(void *pp, int *n)
{
    void **p;
    for (p = *(void***)pp; *n; ++p, --*n)
        if (*p)
            tcc_free(*p);
    tcc_free(*(void**)pp);
    *(void**)pp = ((void*)0);
}

static void strcat_vprintf(char *buf, int buf_size, const char *fmt, va_list ap) {
    int len;
    len = strlen(buf);
    vsnprintf(buf + len, buf_size - len, fmt, ap);
}
static void strcat_printf(char *buf, int buf_size, const char *fmt, ...)
{
    va_list ap;
    ap = ((char *)&(fmt)) + ((sizeof(fmt)+3)&~3);
    strcat_vprintf(buf, buf_size, fmt, ap);
    ;
}
static void error1(TCCState *s1, int is_warning, const char *fmt, va_list ap)
{
    char buf[2048];
    BufferedFile **pf, *f;
    buf[0] = '\0';
    for (f = file; f && f->filename[0] == ':'; f = f->prev)
     ;
    if (f) {
        for(pf = s1->include_stack; pf < s1->include_stack_ptr; pf++)
            strcat_printf(buf, sizeof(buf), "In file included from %s:%d:\n",
                (*pf)->filename, (*pf)->line_num);
        if (s1->error_set_jmp_enabled) {
            strcat_printf(buf, sizeof(buf), "%s:%d: ",
                f->filename, f->line_num - !!(tok_flags & 0x0001));
        } else {
            strcat_printf(buf, sizeof(buf), "%s: ",
                f->filename);
        }
    } else {
        strcat_printf(buf, sizeof(buf), "tcc: ");
    }
    if (is_warning)
        strcat_printf(buf, sizeof(buf), "warning: ");
    else
        strcat_printf(buf, sizeof(buf), "error: ");
    strcat_vprintf(buf, sizeof(buf), fmt, ap);
    if (!s1->error_func) {
        if (s1->output_type == 5 && s1->ppfp == stdout)
            printf("\n"), fflush(stdout);
        fflush(stdout);
        fprintf(stderr, "%s\n", buf);
        fflush(stderr);
    } else {
        s1->error_func(s1->error_opaque, buf);
    }
    if (!is_warning || s1->warn_error)
        s1->nb_errors++;
}

void tcc_error_noabort(const char *fmt, ...) {
    TCCState *s1 = tcc_state;
    va_list ap;
    ap = ((char *)&(fmt)) + ((sizeof(fmt)+3)&~3);
    error1(s1, 0, fmt, ap);
    ;
}

void tcc_error(const char *fmt, ...) {
    TCCState *s1 = tcc_state;
    va_list ap;
    ap = ((char *)&(fmt)) + ((sizeof(fmt)+3)&~3);
    error1(s1, 0, fmt, ap);
    ;
    if (s1->error_set_jmp_enabled) {
        longjmp(s1->error_jmp_buf, 1);
    } else {
        exit(1);
    }
}

void tcc_warning(const char *fmt, ...) {
    TCCState *s1 = tcc_state;
    va_list ap;
    if (s1->warn_none)
        return;
    ap = ((char *)&(fmt)) + ((sizeof(fmt)+3)&~3);
    error1(s1, 1, fmt, ap);
    ;
}

static void tcc_open_bf(TCCState *s1, const char *filename, int initlen) {
    BufferedFile *bf;
    int buflen = initlen ? initlen : 8192;
    bf = tcc_mallocz(sizeof(BufferedFile) + buflen);
    bf->buf_ptr = bf->buffer;
    bf->buf_end = bf->buffer + initlen;
    bf->buf_end[0] = '\\';
    pstrcpy(bf->filename, sizeof(bf->filename), filename);
    bf->true_filename = bf->filename;
    bf->line_num = 1;
    bf->ifdef_stack_ptr = s1->ifdef_stack_ptr;
    bf->fd = -1;
    bf->prev = file;
    file = bf;
    tok_flags = 0x0001 | 0x0002;
}

static void tcc_close(void) {
    BufferedFile *bf = file;
    if (bf->fd > 0) {
        close(bf->fd);
        total_lines += bf->line_num;
    }
    file = bf->prev;
    tcc_free(bf);
}

static int tcc_open(TCCState *s1, const char *filename) {
    int fd;
    fd = open(filename, 00 | 0);
    tcc_open_bf(s1, filename, 0);
    file->fd = fd;
    return fd;
}

static int tcc_compile(TCCState *s1) {
    Sym *define_start;
    int filetype, is_asm;
    define_start = define_stack;
    filetype = s1->filetype;
    is_asm = filetype == 2 || filetype == 3;
    tccelf_begin_file(s1);
    if (_setjmp (s1->error_jmp_buf) == 0) {
        s1->nb_errors = 0;
        s1->error_set_jmp_enabled = 1;
        preprocess_start(s1, is_asm);
        tccgen_compile(s1);
    }
    s1->error_set_jmp_enabled = 0;
    preprocess_end(s1);
    free_defines(define_start);
    sym_pop(&global_stack, ((void*)0), 0);
    sym_pop(&local_stack, ((void*)0), 0);
    tccelf_end_file(s1);
    return s1->nb_errors != 0 ? -1 : 0;
}

static void tcc_cleanup(void) {
    if (((void*)0) == tcc_state)
        return;
    tccpp_delete(tcc_state);
    tcc_state = ((void*)0);
    dynarray_reset(&sym_pools, &nb_sym_pools);
    sym_free_first = ((void*)0);
}

TCCState *tcc_new(void) {
    TCCState *s;
    tcc_cleanup();
    s = tcc_mallocz(sizeof(TCCState));
    tcc_state = s;
    ++nb_states;
    s->alacarte_link = 1;
    s->nocommon = 1;
    s->warn_implicit_function_declaration = 1;
    s->ms_extensions = 1;
    s->seg_size = 32;
    tccelf_new(s);
    tccpp_new(s);
    return s;
}
 void tcc_delete(TCCState *s1)
{
    tcc_cleanup();
    tccelf_delete(s1);
    dynarray_reset(&s1->library_paths, &s1->nb_library_paths);
    dynarray_reset(&s1->crt_paths, &s1->nb_crt_paths);
    dynarray_reset(&s1->cached_includes, &s1->nb_cached_includes);
    dynarray_reset(&s1->include_paths, &s1->nb_include_paths);
    dynarray_reset(&s1->sysinclude_paths, &s1->nb_sysinclude_paths);
    dynarray_reset(&s1->cmd_include_files, &s1->nb_cmd_include_files);
    tcc_free(s1->tcc_lib_path);
    tcc_free(s1->soname);
    tcc_free(s1->rpath);
    tcc_free(s1->init_symbol);
    tcc_free(s1->fini_symbol);
    tcc_free(s1->outfile);
    tcc_free(s1->deps_outfile);
    dynarray_reset(&s1->files, &s1->nb_files);
    dynarray_reset(&s1->target_deps, &s1->nb_target_deps);
    dynarray_reset(&s1->pragma_libs, &s1->nb_pragma_libs);
    dynarray_reset(&s1->argv, &s1->argc);
    tcc_free(s1);
    if (0 == --nb_states)
        tcc_memcheck();
}

int tcc_set_output_type(TCCState *s, int output_type) {
    s->output_type = output_type;
    if (output_type == 4)
        s->output_format = 0;
    return 0;
}

static int tcc_add_file_internal(TCCState *s1, const char *filename, int flags) {
    int ret;
    ret = tcc_open(s1, filename);
    dynarray_add(&s1->target_deps, &s1->nb_target_deps,
            tcc_strdup(filename));
    ret = tcc_compile(s1);
    tcc_close();
    return ret;
}

int tcc_add_file(TCCState *s, const char *filename) {
    int filetype = s->filetype;
    int flags = 0x10;
    if (filetype == 0) {
        const char *ext = tcc_fileextension(filename);
        if (ext[0]) {
            ext++;
            filetype = 1;
        }
        s->filetype = filetype;
    }
    return tcc_add_file_internal(s, filename, flags);
}

static int strstart(const char *val, const char **str) {
    const char *p, *q;
    p = *str;
    q = val;
    while (*q) {
        if (*p != *q)
            return 0;
        p++;
        q++;
    }
    *str = p;
    return 1;
}

typedef struct TCCOption {
    const char *name;
    uint16_t index;
    uint16_t flags;
} TCCOption;

enum {
    TCC_OPTION_I,
    TCC_OPTION_c,
    TCC_OPTION_o,
    TCC_OPTION_nostdinc,
};
static const TCCOption tcc_options[] = {
    { "I", TCC_OPTION_I, 0x0001 },
    { "c", TCC_OPTION_c, 0 },
    { "o", TCC_OPTION_o, 0x0001 },
    { "nostdinc", TCC_OPTION_nostdinc, 0 },
    { ((void*)0), 0, 0 },
};

static void args_parser_add_file(TCCState *s, const char* filename, int filetype) {
    struct filespec *f = tcc_malloc(sizeof *f + strlen(filename));
    f->type = filetype;
    f->alacarte = s->alacarte_link;
    strcpy(f->name, filename);
    dynarray_add(&s->files, &s->nb_files, f);
}

int tcc_parse_args(TCCState *s, int *pargc, char ***pargv, int optind) {
    const TCCOption *popt;
    const char *optarg, *r;
    const char *run = ((void*)0);
    int last_o = -1;
    int x;
    CString linker_arg;
    int tool = 0, arg_start = 0, noaction = optind;
    char **argv = *pargv;
    int argc = *pargc;
    cstr_new(&linker_arg);
    while (optind < argc) {
        r = argv[optind];
        optind++;
        if (r[0] != '-' || r[1] == '\0') {
            if (r[0] != '@')
                args_parser_add_file(s, r, s->filetype);
            continue;
        }
        for(popt = tcc_options; ; ++popt) {
            const char *p1 = popt->name;
            const char *r1 = r + 1;
            if (!strstart(p1, &r1))
                continue;
            optarg = r1;
            if (popt->flags & 0x0001) {
                if (*r1 == '\0' && !(popt->flags & 0x0002)) {
                    optarg = argv[optind++];
                }
            }
            break;
        }
        switch(popt->index) {
        case TCC_OPTION_c:
            x = 4;
        set_output_type:
            s->output_type = x;
            break;
        case TCC_OPTION_o:
            s->outfile = tcc_strdup(optarg);
            break;
        }
    }
    *pargc = argc - arg_start;
    *pargv = argv + arg_start;
    if (optind != noaction)
        return 0;
}

int main(int argc0, char **argv0) {
    TCCState *s;
    int ret, opt, n = 0, t = 0;
    unsigned start_time = 0;
    const char *first_file;
    int argc; char **argv;
    FILE *ppfp = stdout;
    argc = argc0, argv = argv0;
    s = tcc_new();
    opt = tcc_parse_args(s, &argc, &argv, 1);
    n = s->nb_files;
    tcc_set_output_type(s, s->output_type);
    s->ppfp = ppfp;
    for (first_file = ((void*)0), ret = 0;;) {
        struct filespec *f = s->files[s->nb_files - n];
        s->filetype = f->type;
        s->alacarte_link = f->alacarte;
        if (!first_file)
            first_file = f->name;
        tcc_add_file(s, f->name);
        s->filetype = 0;
        s->alacarte_link = 1;
        if (--n == 0 || ret) {
            break;
        }
    }
    tcc_output_file(s, s->outfile);
    tcc_delete(s);
    return ret;
}
