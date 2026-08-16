/* Basic hello world with MRuby. */

#include <mruby.h>
#include <mruby/compile.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Initiate MRuby
    mrb_state *mrb = mrb_open();
    if (mrb == NULL) {
        fputs("Couldn't initialize MRuby state", stderr);
        return EXIT_FAILURE;
    }

    // Load and run Ruby program from literal string
    mrb_load_string(mrb, "puts 'Hello, World!'");

    mrb_close(mrb);
    return EXIT_SUCCESS;
}
