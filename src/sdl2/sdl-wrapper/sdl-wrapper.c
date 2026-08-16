#include <stdio.h>

#include <SDL2/SDL.h>

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/compile.h>
#include <mruby/data.h>
#include <mruby/value.h>
#include <mruby/version.h>

/* Declarations */

void freeWindow(mrb_state *mrb, void *ptr);

typedef struct {
    SDL_Window *win;
    SDL_Renderer *ren;
    SDL_Event e; // Event handler
} SDLWindowData;

static const mrb_data_type SDLWindowType = { "Window", freeWindow };

struct RClass *windowKlass;

/* Functions  */

static mrb_value mrb_window_new(mrb_state *mrb, mrb_value self) {
    char *title;
    mrb_int w, h;
    mrb_get_args(mrb, "zii", &title, &w, &h);

    SDLWindowData *data = (SDLWindowData *)DATA_PTR(self);
    if (data != NULL) {
        freeWindow(mrb, data);
    }

    // Init stuff ...
    mrb_data_init(self, NULL, &SDLWindowType);
    data = (SDLWindowData *)malloc(sizeof(SDLWindowData));

    data->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    data->ren = SDL_CreateRenderer(data->win, -1, SDL_RENDERER_ACCELERATED);

    mrb_data_init(self, data, &SDLWindowType);

    return self;
}

/* This functions polls the events from SDL and returns the type of the current event */
static mrb_value mrb_window_get_events(mrb_state *mrb, mrb_value self) {
    SDLWindowData *idata;
    Data_Get_Struct(mrb, self, &SDLWindowType, idata);

    SDL_PollEvent(&idata->e);

    return mrb_fixnum_value(idata->e.type);
}

static mrb_value mrb_window_get_scancode(mrb_state *mrb, mrb_value self) {
    SDLWindowData *idata;
    Data_Get_Struct(mrb, self, &SDLWindowType, idata);

    return mrb_fixnum_value(idata->e.key.keysym.sym);
}

static mrb_value mrb_window_render_set_color(mrb_state *mrb, mrb_value self) {
    mrb_int r, g, b, a;
    mrb_get_args(mrb, "iiii", &r, &g, &b, &a);

    SDLWindowData *idata;
    Data_Get_Struct(mrb, self, &SDLWindowType, idata);

    SDL_SetRenderDrawColor(idata->ren, r, g, b, a);

    return mrb_nil_value();
}

static mrb_value mrb_window_render_clear(mrb_state *mrb, mrb_value self) {
    SDLWindowData *idata;
    Data_Get_Struct(mrb, self, &SDLWindowType, idata);

    SDL_RenderClear(idata->ren);

    return mrb_nil_value();
}

static mrb_value mrb_window_render_present(mrb_state *mrb, mrb_value self) {
    SDLWindowData *idata;
    Data_Get_Struct(mrb, self, &SDLWindowType, idata);

    SDL_RenderPresent(idata->ren);

    return mrb_nil_value();
}

static void appendMRubyFunctions(mrb_state *mrb) {
    windowKlass = mrb_define_class(mrb, "SDLWindow", mrb->object_class);
    MRB_SET_INSTANCE_TT(windowKlass, MRB_TT_DATA);

    mrb_define_method(mrb, windowKlass, "initialize", mrb_window_new, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, windowKlass, "poll_event", mrb_window_get_events, MRB_ARGS_NONE());
    mrb_define_method(mrb, windowKlass, "scancode", mrb_window_get_scancode, MRB_ARGS_NONE());
    mrb_define_method(mrb, windowKlass, "render_color", mrb_window_render_set_color, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, windowKlass, "render_clear", mrb_window_render_clear, MRB_ARGS_NONE());
    mrb_define_method(mrb, windowKlass, "render_present", mrb_window_render_present, MRB_ARGS_NONE());
}

static void appendSDLConstants(mrb_state *mrb) {
    // Events
    mrb_define_const(mrb, mrb->kernel_module, "SDL_EVENT_QUIT", mrb_fixnum_value(SDL_QUIT));
    mrb_define_const(mrb, mrb->kernel_module, "SDL_EVENT_KEY_DOWN", mrb_fixnum_value(SDL_KEYDOWN));

    // Scancodes
    mrb_define_const(mrb, mrb->kernel_module, "SDLK_ESC", mrb_fixnum_value(SDLK_ESCAPE));
}

int main(void) {
    fprintf(stdout, "MRuby version %s", MRUBY_VERSION);

    mrb_state *mrb = mrb_open();
    if (!mrb) {
        fprintf(stderr, "Couldn't initialize MRuby\n");
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL2: Failed to initialize!\n");
        fprintf(stderr, "Error: %s\n", SDL_GetError());
    }

    appendMRubyFunctions(mrb);
    appendSDLConstants(mrb);

    // Open our file
    FILE *rubyScript = fopen("window.rb", "r");
    if (!rubyScript) {
        fprintf(stderr, "Couldn't find a main.rb file in the current directory, quitting...\n");
        mrb_close(mrb);
        return 1;
    }

    // Pass it to MRuby
    mrb_load_file(mrb, rubyScript);

    fclose(rubyScript);

    if (mrb->exc) {
        mrb_print_error(mrb);
    }

    SDL_Quit();
    mrb_close(mrb);
    return 0;
}

void freeWindow(mrb_state *mrb, void *ptr) {
    SDLWindowData *data = (SDLWindowData *)ptr;
    if (data != NULL) {
        if (data->ren != NULL) {
            SDL_DestroyRenderer(data->ren);
        }

        if (data->win != NULL) {
            SDL_DestroyWindow(data->win);
        }

        mrb_free(mrb, data);
    }
}
