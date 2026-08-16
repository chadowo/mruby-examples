#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/variable.h>

#include <stdio.h>
#include <stdlib.h>

/* This sample program shows you how to push various tipes of
 * variables to Ruby
 * TODO: Get the variables and print them */

/* Simply set an integer as ivar */
static mrb_value mrb_foobar_new(mrb_state *mrb, mrb_value self)
{
    mrb_int arg_num;

    mrb_get_args(mrb, "i", &arg_num);

    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@num"), mrb_fixnum_value(arg_num));

    return self;
}

int main(int argc, char *argv[])
{
    mrb_state *mrb = mrb_open();
    if (mrb == NULL) {
        fputs("Couldn't initialize MRuby state", stderr);
        return EXIT_FAILURE;
    }

    // Pushing variables to Ruby is relatively easy.

    // Here we define a constant, 'FOO', of class Integer with a value of 42
    mrb_define_const(mrb, mrb->kernel_module, "FOO", mrb_fixnum_value(42));
    // => FOO = 42

    // Globals are slightly different
    mrb_gv_set(mrb, mrb_intern_lit(mrb, "$bar"), mrb_str_new_lit(mrb, "I'm a global!"));
    // => $bar = "I'm a global!"

    // Instance variables
    struct RClass *foobarKlass = mrb_define_class(mrb, "Foobar", mrb->object_class);
    mrb_define_method(mrb, foobarKlass, "initialize", mrb_foobar_new, MRB_ARGS_REQ(1));

    // Create the object
    mrb_value args[1] = { mrb_fixnum_value(1500) };
    mrb_value obj = mrb_obj_new(mrb, foobarKlass, 1, args);

    // Get the ivar
    mrb_int numIvar = mrb_fixnum(mrb_iv_get(mrb, obj, mrb_intern_lit(mrb, "@num")));
    printf("@num: %ld\n", numIvar);

    // Class variables (We'll use the same class as above)
    mrb_mod_cv_set(mrb, foobarKlass, mrb_intern_lit(mrb, "@@baz"), mrb_float_value(mrb, 50.55));

    // Module variables (class variables but on a module)
    struct RClass *foobazModule = mrb_define_module(mrb, "Foobaz");

    mrb_mod_cv_set(mrb, foobazModule, mrb_intern_lit(mrb, "@@quux"), mrb_float_value(mrb, 15.0));

    // We can also get the variables we defined, for example: getting the
    // module variable @@quux
    mrb_float quuxCvar = mrb_float(mrb_cv_get(mrb, mrb_obj_value(foobazModule), mrb_intern_lit(mrb, "@@quux")));

    printf("@@quux: %f\n", quuxCvar);

    mrb_close(mrb);
    return EXIT_SUCCESS;
}
