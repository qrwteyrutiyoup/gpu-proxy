#ifndef GPUPROCESS_COMPILER_PRIVATE_H
#define GPUPROCESS_COMPILER_PRIVATE_H

#include <assert.h>

#define public  __attribute__((visibility("default")))
#define private __attribute__((visibility("hidden")))

#ifndef ENABLE_TESTING
#define exposed_to_tests static
#else
#define exposed_to_tests
#endif

#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#define likely(expr) (__builtin_expect (!!(expr), 1))
#define unlikely(expr) (__builtin_expect (!!(expr), 0))
#else
#define likely(expr) (expr)
#define unlikely(expr) (expr)
#endif

#define UNUSED_PARAM(var) (void)var

#if ENABLE_PROFILING
private unsigned long
get_time_in_milliseconds ();

typedef struct {
    const char *function_name;
    unsigned long start_time;
} instrumentation_t;

private void
start_instrumentation ();

private void
print_instrumentation (instrumentation_t *instrumentation);

#define INSTRUMENT() \
    start_instrumentation (); \
    instrumentation_t __instrumentation __attribute__((cleanup(print_instrumentation))); \
    __instrumentation.function_name = __func__; \
    __instrumentation.start_time = get_time_in_milliseconds (); \

#else
#define INSTRUMENT()
#endif

#endif
