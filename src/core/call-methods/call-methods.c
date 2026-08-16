#include <stdio.h>

#include <mruby.h>
#include <mruby/compile.h>

int main(int argc, char *argv[]) {
    mrb_state *mrb = mrb_open();
    if (!mrb) {
        fprintf(stderr, "Couldn't initialize MRuby\n");
        return 1;
    }

    // Define method foo, that puts a single string
    mrb_load_string(mrb, "def foo; puts 'foo called' end");

    // Define method bar, that requires one argument
    mrb_load_string(mrb, "def bar(x); puts \"X: #{x}\" end");

    // NOTE: Since the above methods aren't defined inside a class, they're in the
    //       main object, so we gotta use mrb_top_self() to get that object
    mrb_funcall(mrb, mrb_top_self(mrb), "foo", 0);
    mrb_funcall(mrb, mrb_top_self(mrb), "bar", 1, mrb_fixnum_value(49));

    // Doing the same but for an instance object is quite similar,
    // first we create our object, in this case a descendant of the string class

    // We need an array of mrb_values to pass it to the constructor of the class
    mrb_value args[1] = { mrb_str_new_cstr(mrb, "Test string") };
    mrb_value obj = mrb_obj_new(mrb, mrb->string_class, 1, args);

    // And here it comes the important bit, calling the function
    mrb_value reversedString = mrb_funcall(mrb, obj, "reverse", 0);

    // NOTE: mrb_format is a nifty function provided by MRuby that lets us format
    //       a string with MRuby internal types, under the hood it uses mrb_vformat.
    //       You can find a table with the formats specifier here:
    //       https://github.com/mruby/mruby/blob/master/src/error.c#L264
    mrb_p(mrb, mrb_format(mrb, "Reversed string: %v", reversedString));

    // For the sake of completeness let's also call a class method, like
    // Math#sqrt
    mrb_value mathMod = mrb_obj_value(mrb_module_get(mrb, "Math"));
    mrb_value result = mrb_funcall(mrb, mathMod, "sqrt", 1, mrb_fixnum_value(16));
    mrb_p(mrb, mrb_format(mrb, "Result of Math.sqrt(16): %v", result));

    mrb_close(mrb);
    return 0;
}
