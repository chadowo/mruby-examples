#include <stdio.h>
#include <stdlib.h>

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/value.h>

/* This sample program will teach you how to work with data in MRuby
 * Based on this marvelous article: https://dev.to/roryo/storing-c-data-in-an-mruby-class-50k4 */

/* For demonstration purposes we'll work with a imaginary car
 * struct, it accepts a char* for both color and type */
typedef struct carData {
    char *color;
    char *type;
} carData;

/* We need to note the datatype and the function we'll use
 * when the object is GC'ed, The "Car" bit is the unique
 * identifier, the class name suffices for that, if you use
 * custom allocated resources for your data then you'll need
 * to define a custom free function that frees your resources,
 * however since we don't do that we can just call mrb_free */
static const mrb_data_type carType = { "Car", mrb_free };

/* Car constructor */
static mrb_value mrb_car_new(mrb_state *mrb, mrb_value self) {
    char *color, *type;

    mrb_get_args(mrb, "zz", &color, &type);

    // We initialize our data structure, DATA_PTR is a macro that gives us
    // a void*, we want that pointer to point to ourselves, and
    // cast it to a carData
    carData *instanceCarData = (carData *)DATA_PTR(self);

    // If there's a pointer already associated to this instance, free it
    if (instanceCarData) {
        mrb_free(mrb, instanceCarData);
    }

    // Initialize the data
    mrb_data_init(self, NULL, &carType);

    // Allocate heap memory for the data
    instanceCarData = (carData *)malloc(sizeof(carData));

    // Set the data
    instanceCarData->color = color;
    instanceCarData->type = type;

    // And now we populate the data
    mrb_data_init(self, instanceCarData, &carType);

    return self;
}

/* This function will summarize the instance car attributes,
 * color and type. Thus demonstrating how to get the data
 * back */
static mrb_value mrb_car_summarize(mrb_state *mrb, mrb_value self) {
    // We initialize the struct that will hold our data
    carData *instanceCarData;

    // Get the data
    Data_Get_Struct(mrb, self, &carType, instanceCarData);

    printf("The car's color is: %s\nThe car's type is: %s\n", instanceCarData->color, instanceCarData->type);

    return self;
}

int main(int argc, char *argv[]) {
    mrb_state *mrb = mrb_open();
    if (!mrb) {
        fprintf(stderr, "Couldn't initialize MRuby\n");
        return 1;
    }

    struct RClass *carKlass = mrb_define_class(mrb, "Car", mrb->object_class);
    MRB_SET_INSTANCE_TT(carKlass, MRB_TT_DATA);

    mrb_define_method(mrb, carKlass, "initialize", mrb_car_new, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, carKlass, "summarize", mrb_car_summarize, MRB_ARGS_NONE());

    // Now we're gonna create a Car object and call its 'summarize' method.
    // The args for the car's initialize
    const mrb_value args[2] = { mrb_str_new_cstr(mrb, "Red"), mrb_str_new_cstr(mrb, "Sedan") };

    // Our car object
    mrb_value carObj = mrb_obj_new(mrb, carKlass, 2, args);

    // Call the summarize method
    mrb_funcall(mrb, carObj, "summarize", 0);

    mrb_close(mrb);
    return 0;
}
