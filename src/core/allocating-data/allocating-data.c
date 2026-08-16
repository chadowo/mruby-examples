#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/compile.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mruby/variable.h>

/* This example will teach you how to use Data_Wrap_Struct
 * (macro for mrb_data_object_alloc) to allocate new objects with cdata, in a way
 * that'll let you put anything you want there (or any other imaginative thing
 * you'd like). For this example we'll use the same program as the one in the
 * using-cdata example, except here we'll make an overload of the Car class
 * initialize method.
 *
 * NOTE: I'm still learning about this topic, so I may or not be correct at all
 *       about this, I can only be certain it works. */

/* Same declarations*/

typedef struct carData {
    char *color;
    int mileage;
} carData;

static const mrb_data_type carType = { "Car", mrb_free };

/* We'll declare the class as a global since it will be needed for
 * Data_Wrap_Struct */
struct RClass *carKlass;

/* Same method from the other example */
mrb_value mrb_new_car(mrb_state *mrb, mrb_value self) {
    char *color;
    mrb_int mileage;

    mrb_get_args(mrb, "zi", &color, &mileage);

    carData *instanceCarData = (carData *)DATA_PTR(self);
    if (instanceCarData) {
        mrb_free(mrb, instanceCarData);
    }

    mrb_data_init(self, NULL, &carType);

    instanceCarData = (carData *)malloc(sizeof(carData));
    instanceCarData->color = color;
    instanceCarData->mileage = mileage;

    mrb_data_init(self, instanceCarData, &carType);
    return self;
}

/* A used car will have a random mileage number */
mrb_value mrb_new_used_car(mrb_state *mrb, mrb_value self) {
    char *color;

    mrb_get_args(mrb, "z", &color);

    // Declare the empty data
    carData *instanceCarData = malloc(sizeof(*instanceCarData));

    instanceCarData->color = color;
    instanceCarData->mileage = rand();

    // Allocate the object based on the data we filled with the respective type
    mrb_value obj = mrb_obj_value(Data_Wrap_Struct(mrb, carKlass, &carType, instanceCarData));

    return obj;
}

mrb_value mrb_summarize_car(mrb_state *mrb, mrb_value self) {
    carData *instanceCarData;
    Data_Get_Struct(mrb, self, &carType, instanceCarData);

    printf("The car color is: %s\nThe car mileage is: %d\n", instanceCarData->color, instanceCarData->mileage);

    return self;
}

int main(int argc, char *argv[]) {
    mrb_state *mrb = mrb_open();
    if (!mrb) {
        fprintf(stderr, "Couldn't initialize MRuby\n");
        return 1;
    }

    srand(time(NULL));

    carKlass = mrb_define_class(mrb, "Car", mrb->object_class);
    MRB_SET_INSTANCE_TT(carKlass, MRB_TT_DATA);

    mrb_define_method(mrb, carKlass, "initialize", mrb_new_car, MRB_ARGS_REQ(2));
    mrb_define_class_method(mrb, carKlass, "initialize_used", mrb_new_used_car, MRB_ARGS_REQ(1));

    mrb_define_method(mrb, carKlass, "summarize", mrb_summarize_car, MRB_ARGS_NONE());

    FILE *fp = fopen("main.rb", "r");

    mrb_load_file(mrb, fp);
    fclose(fp);

    mrb_close(mrb);
    return 0;
}
