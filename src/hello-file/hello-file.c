/* Loading Ruby code from a file */

#include <mruby.h>
#include <mruby/compile.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    mrb_state *mrb = mrb_open();
    if (mrb == NULL) {
        fputs("Couldn't initialize MRuby state", stderr);
        return EXIT_FAILURE;
    }

    FILE *ruby_script = fopen("main.rb", "r");
    if (ruby_script == NULL) {
        fputs("Couldn't find a main.rb file in the current directory!", stderr);
        mrb_close(mrb);
        return EXIT_FAILURE;
    }

    mrb_load_file(mrb, ruby_script);

    fclose(ruby_script);
    mrb_close(mrb);

    return EXIT_SUCCESS;
}
