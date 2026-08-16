# This file is loaded everytime the executable is ran. Feel free to edit it!

@window = SDLWindow.new('MRuby SDL Wrapper', 250, 250)
@running = true

while @running
  # Event based programming
  case @window.poll_event
  when SDL_EVENT_QUIT
    @running = false
  when SDL_EVENT_KEY_DOWN
    @running = false if @window.scancode == SDLK_ESC
  end

  @window.render_color(255, 100, 100, 255)
  @window.render_clear
  @window.render_present
end
