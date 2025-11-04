#include <stdbool.h>
#include <stdio.h>

#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/dump.h>
#include <mruby/irep.h>
#include <mruby/proc.h>

/* Like mrbc, but simpler and stupid */
int main(int argc, char *argv[]) {
    mrb_state *mrb = mrb_open();
    if (!mrb) {
        fprintf(stderr, "Couldn't initialize MRuby\n");
        return 1;
    }

    // Prepare our files
    FILE *input = fopen("input.rb", "r");
    if (!input) {
        fprintf(stderr, "Couldn't find input.rb current directory, quitting...\n");
        mrb_close(mrb);
        return 1;
    }
    FILE *output = fopen("output.mrb", "wb");

    struct mrbc_context *cxt;

    cxt = mrbc_context_new(mrb);
    cxt->no_exec = true; // To not run the file when loaded

    mrbc_filename(mrb, cxt, "input.rb");
    mrb_value result = mrb_load_file_cxt(mrb, input, cxt);

    fclose(input);
    mrbc_context_free(mrb, cxt);

    struct RProc *proc = mrb_proc_ptr(result);
    const mrb_irep *irep = proc->body.irep;

    // NOTE: DUMP_DEBUG_INFO was deprecated in 3.3.0 and used only for compatibility, in newer
    //       MRuby you should use MRB_DUMP_DEBUG_INFO
    #if MRUBY_RELEASE_MAJOR == 3 && MRUBY_RELEASE_MINOR < 3
    mrb_dump_irep_binary(mrb, irep, DUMP_DEBUG_INFO, output);
    #else
    mrb_dump_irep_binary(mrb, irep, MRB_DUMP_DEBUG_INFO, output);
    #endif

    fclose(output);
    mrb_close(mrb);
    return 0;
}
