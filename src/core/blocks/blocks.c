#include <stdio.h>

#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/value.h>

/* This method will yield the provided block three times */
static mrb_value mrb_method_with_block(mrb_state *mrb, mrb_value self) {
    mrb_value block;

    mrb_get_args(mrb, "&!", &block);

    mrb_yield(mrb, block, mrb_nil_value());
    mrb_yield(mrb, block, mrb_nil_value());
    mrb_yield(mrb, block, mrb_nil_value());

    return self;
}

/* Here we'll pass an argument to the block, in this case it'll be a name */
static mrb_value mrb_method_with_block_args(mrb_state *mrb, mrb_value self) {
    mrb_value block;

    mrb_get_args(mrb, "&", &block);

    mrb_yield(mrb, block, mrb_str_new_cstr(mrb, "John Doe"));

    return self;
}

int main(int argc, char *argv[]) {
    mrb_state *mrb = mrb_open();
    if (!mrb) {
        fprintf(stderr, "Couldn't initialize MRuby\n");
        return 1;
    }

    // For this example we will use a Ruby file
    FILE *rubyScript = fopen("main.rb", "r");
    if (!rubyScript) {
        fprintf(stderr, "Couldn't find a main.rb file in the current directory, quitting...\n");
        mrb_close(mrb);
        return 1;
    }

    mrb_define_method(mrb, mrb->kernel_module, "method_with_block", mrb_method_with_block, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, mrb->kernel_module, "method_with_block_args", mrb_method_with_block_args, MRB_ARGS_REQ(1));

    mrb_load_file(mrb, rubyScript);

    fclose(rubyScript);
    mrb_close(mrb);
    return 0;
}
