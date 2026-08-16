#include <stdio.h>

#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>

/* Keyword arguments (also called keyword parameters) are a type of arguments
 * added back in CRuby 2.0 that let's us specify arguments like so:
 *
 * foo(keyword1: 'hi', keyword2: false)
 *
 * We can shuffle the order of args too:
 *
 * foo(keyword2: false, keyword:1 'hi')
 *
 * It doesn't really matter as long as the keyword's present.
 */

/* This method uses 2 required kwargs (both expected to be integers):
 *
 * kwargs_method(foo:, bar:)
 */
static mrb_value mrb_kwargs_method(mrb_state *mrb, mrb_value self) {
    mrb_int kwNum = 2;
    mrb_int kwRequired = 2;
    mrb_sym kwNames[] = { mrb_intern_cstr(mrb, "foo"), mrb_intern_cstr(mrb, "bar") };

    mrb_value kwValues[kwNum];
    const mrb_kwargs kwArgs = { kwNum, kwRequired, kwNames, kwValues, NULL };

    mrb_get_args(mrb, ":", &kwArgs);

    printf("Keyword argument foo: %ld\nKeyword argument bar: %ld\n", mrb_fixnum(kwValues[0]), mrb_fixnum(kwValues[1]));

    return mrb_nil_value();
}

/* Positional and (optional) keyword arguments:
 *
 * positional_kwargs_method(foo, bar, baz:, quux:)*/
static mrb_value mrb_positional_kwargs_method(mrb_state *mrb, mrb_value self) {
    mrb_value foo;
    mrb_float bar;

    mrb_int kwNum = 2;
    mrb_int kwRequired = 0;
    mrb_sym kwNames[] = { mrb_intern_cstr(mrb, "baz"), mrb_intern_cstr(mrb, "quux") };

    mrb_value kwValues[kwNum];
    const mrb_kwargs kwArgs = { kwNum, kwRequired, kwNames, kwValues, NULL };

    mrb_get_args(mrb, "Sf:", &foo, &bar, &kwArgs);

    printf("\nPositional argument foo: %s\nPositional argument bar: %f\n", mrb_str_to_cstr(mrb, foo), bar);
    if (!mrb_undef_p(kwValues[0]) && !mrb_undef_p(kwValues[1])) {
        printf("Optional keyword argument baz: %ld\nOptional keyword argument quux: %ld\n", mrb_fixnum(kwValues[0]),
               mrb_fixnum(kwValues[1]));
    }

    return mrb_nil_value();
}

/* Keyword arguments with default values:
 *
 * default_kwargs_method(foo: "Hello", bar: "World")
 */
static mrb_value mrb_default_kwargs_method(mrb_state *mrb, mrb_value self) {
    mrb_int kwNum = 2;
    mrb_int kwRequired = 0;
    mrb_sym kwNames[] = { mrb_intern_cstr(mrb, "foo"), mrb_intern_cstr(mrb, "bar") };

    mrb_value kwValues[kwNum];
    const mrb_kwargs kwArgs = { kwNum, kwRequired, kwNames, kwValues, NULL };

    mrb_get_args(mrb, ":", &kwArgs);

    if (mrb_undef_p(kwValues[0])) {
        kwValues[0] = mrb_str_new_cstr(mrb, "Hello");
    }
    if (mrb_undef_p(kwValues[1])) {
        kwValues[1] = mrb_str_new_cstr(mrb, "World");
    }

    printf("\nKeyword argument foo: %s\nKeyword argument bar: %s\n", mrb_str_to_cstr(mrb, kwValues[0]),
           mrb_str_to_cstr(mrb, kwValues[1]));

    return mrb_nil_value();
}

int main(int argc, char *argv[]) {
    mrb_state *mrb = mrb_open();
    if (!mrb) {
        fprintf(stderr, "Couldn't initialize MRuby\n");
        return 1;
    }

    mrb_define_method(mrb, mrb->kernel_module, "kwargs_method", mrb_kwargs_method, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, mrb->kernel_module, "positional_kwargs_method", mrb_positional_kwargs_method,
                      MRB_ARGS_REQ(2) | MRB_ARGS_OPT(2));
    mrb_define_method(mrb, mrb->kernel_module, "default_kwargs_method", mrb_default_kwargs_method, MRB_ARGS_OPT(2));

    mrb_load_string(mrb, "kwargs_method(foo: 25, bar: 10)");
    mrb_load_string(mrb, "positional_kwargs_method('hi', 2.5, baz: 100, quux: 200)");
    mrb_load_string(mrb, "default_kwargs_method(bar: 'To you!')");

    mrb_close(mrb);
    return 0;
}
