#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/compile.h>
#include <mruby/error.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <stdlib.h>
#include <stdbool.h>

/* Custom backtrace printer. */
static void print_backtrace(mrb_state *mrb, struct RObject *exc, struct RArray *backtrace)
{
    mrb_int i;
    mrb_int n = (backtrace ? ARY_LEN(backtrace) : 0);
    mrb_value *loc, mesg;

    if (n != 0) {
        if (n > 1) {
            fputs("trace (most recent call lastback):\n", stderr);
        }
        for (i=n-1,loc=&ARY_PTR(backtrace)[i]; i>0; i--,loc--) {
            if (mrb_string_p(*loc)) {
                fprintf(stderr, "\t[%d] ", (int)i);
                fwrite(RSTRING_PTR(*loc), (int)RSTRING_LEN(*loc), 1, stderr);
                fputc('\n', stderr);
            }
        }
        if (mrb_string_p(*loc)) {
            fwrite(RSTRING_PTR(*loc), (int)RSTRING_LEN(*loc), 1, stderr);
            fputs(": ", stderr);
        }
    }
    else {
        fputs("(unknown):0: ", stderr);
    }

    if (exc == mrb->nomem_err) {
        static const char nomem[] = "Out of memory (NoMemoryError)\n";
        fwrite(nomem, sizeof(nomem)-1, 1, stderr);
    }
    // else {
    //     mesg = mrb_exc_inspect(mrb, mrb_obj_value(exc));
    //     fwrite(RSTRING_PTR(mesg), RSTRING_LEN(mesg), 1, stderr);
    //     fputc('\n', stderr);
    // }
}

/* Returns false if file couldn't be loaded.
   TODO: I'm not sure if routines that fail return false or true, I'm getting confused with 0/1. */
static bool load_ruby_file(mrb_state *mrb, mrbc_context *ctx, const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return false;
    }

    // struct mrb_parser_state *par;
    // par = mrb_parse_file(mrb, file, ctx);
    // mrb_parser_set_filename(par, path);

    // if (0 < par->nerr) {
    //     fprintf(stderr, "Error in %s:%d => %s\n", path, par->error_buffer[0].lineno, par->error_buffer[0].message);
    // } else {
    //     mrb_load_exec(mrb, par, ctx);
    // }

    mrbc_filename(mrb, ctx, "main.rb");
    int arena_index = mrb_gc_arena_save(mrb);
    mrb_load_file_cxt(mrb, file, ctx);
    mrb_gc_arena_restore(mrb, arena_index);

    fclose(file);
    return true;
}

int main(int argc, char *argv[])
{
    mrb_state *mrb = mrb_open();
    if (mrb == NULL) {
        fputs("Couldn't open MRuby state", stderr);
        return EXIT_FAILURE;
    }

    // To have advanced error printing and other facilities, we need a CContext
    mrbc_context *ctx = mrbc_context_new(mrb);
    if (ctx == NULL) {
        fputs("Couldn't create CContext", stderr);

        mrb_close(mrb);
        return EXIT_FAILURE;
    }
    ctx->capture_errors = true;

    if (argc > 1) {
        if (!load_ruby_file(mrb, ctx, argv[1])) {
            fprintf(stderr, "Unable to load file at '%s'", argv[1]);
        }
    } else {
        puts("Usage: backtrace [file]");
    }

    if (mrb->exc) {
        // TODO: This works. I don't know why
        fprintf(stderr, "ERROR: %s", mrb_string_cstr(mrb, mrb_obj_as_string(mrb, mrb_obj_value(mrb->exc))));
        // mrb_p(mrb, mrb_obj_value(mrb->exc));
    }

    mrbc_context_free(mrb, ctx);
    mrb_close(mrb);
    return EXIT_SUCCESS;
}
