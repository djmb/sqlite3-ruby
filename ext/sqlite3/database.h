#ifndef SQLITE3_DATABASE_RUBY
#define SQLITE3_DATABASE_RUBY

#include <sqlite3_ruby.h>

/* bits in the `flags` field */
#define SQLITE3_RB_DATABASE_READONLY  0x01
#define SQLITE3_RB_DATABASE_DISCARDED 0x02

struct _sqlite3Ruby;

/* Everything sqlite calls back into gets one of these instead of a VALUE.
 * malloc owns the box, so its address is stable, and the VALUE inside is
 * rewritten when the garbage collector moves the object. */
typedef struct _sqlite3RubyCallback {
    struct _sqlite3Ruby *db;
    VALUE value;
    struct _sqlite3RubyCallback *next;
} sqlite3RubyCallback;

struct _sqlite3Ruby {
    sqlite3 *db;
    VALUE busy_handler;
    VALUE trace_handler;
    VALUE authorizer;
    sqlite3RubyCallback *callbacks;
    int stmt_timeout;
    struct timespec stmt_deadline;
    rb_pid_t owner;
    int flags;
};

typedef struct _sqlite3Ruby sqlite3Ruby;
typedef sqlite3Ruby *sqlite3RubyPtr;

sqlite3RubyCallback *rb_sqlite3_callback_new(sqlite3RubyPtr ctx, VALUE value);
void rb_sqlite3_callback_free(sqlite3RubyCallback *callback);

void init_sqlite3_database();
void set_sqlite3_func_result(sqlite3_context *ctx, VALUE result);

sqlite3RubyPtr sqlite3_database_unwrap(VALUE database);
VALUE sqlite3val2rb(sqlite3_value *val);

#endif
