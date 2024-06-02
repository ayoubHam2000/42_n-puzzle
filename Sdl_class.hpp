#ifndef SDL_CLASS_HPP
#define SDL_CLASS_HPP

#include "n_puzzle.h"

constexpr unsigned int BUFFER_INPUT_SIZE = 4;

class Sdl_class
{
public:
  int           mouse_x;
  int           mouse_y;
  bool          quit;
  const unsigned int  width;
  const unsigned int  height;
  int           buffer_input[BUFFER_INPUT_SIZE];

private:
  const char*   title;
  SDL_Window*   win;
  SDL_Surface*  win_surface;
  SDL_Event     e;

private:
  Sdl_class();

public:

  Sdl_class(const char *title, unsigned int width, unsigned int height) :
    mouse_x(0),
    mouse_y(0),
    quit(false),
    width(width), 
    height(height),
    title(title)
  {
    this->win = this->_init_sdl();
    this->win_surface = this->_get_win_surface();
    memset(buffer_input, -1, sizeof(int) * BUFFER_INPUT_SIZE);
  }

public:

  void  flush()
  {
    SDL_UpdateWindowSurface(this->win);
  }

  void  clear()
  {
    SDL_FillRect(this->win_surface, NULL, SDL_MapRGB(this->win_surface->format, 0, 0, 0));
  }

  void  ft_put_pixel(unsigned int x, unsigned int y, unsigned int color){
    this->_ft_put_pixel(this->win_surface, x, y, color);
  }

  void   exit()
  {
    SDL_DestroyWindow(this->win);
    IMG_Quit();
    SDL_Quit();
  }

  void   events()
  {
    while (SDL_PollEvent(&this->e) != 0)
    {
      //std::cout << "Events" << std::endl;
      if (e.type == SDL_QUIT)
        this->quit = true;
      else if (e.type == SDL_KEYDOWN)
        this->_on_key_down(e.key.keysym.scancode);
      else if (e.type == SDL_KEYUP)
        this->_on_key_up(e.key.keysym.scancode);
      else if (e.type == SDL_MOUSEMOTION)
        this->_on_mouse_move(e.motion.x, e.motion.y);
      // else if (e.type == SDL_MOUSEBUTTONUP)
      //   on_mouse_up(e.button.button, g);
      // else if (e.type == SDL_MOUSEWHEEL)
      //   on_mouse_wheel(e.wheel.y);
    }
  }

  bool  is_in_buffer(int key) {
    for (unsigned int i = 0; i < BUFFER_INPUT_SIZE; i++) {
      if (this->buffer_input[i] == key)
        return true;
    }
    return false;
  }

  void  print_input_buffer() {
    for (unsigned int i = 0; i < BUFFER_INPUT_SIZE; i++) {
      std::cout << this->buffer_input[i] << ", ";
    }
    std::cout << std::endl;
  }

private:

  SDL_Window*   _init_sdl()
  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
      __exit_fatal("Can't initialize SDL.");
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
      return NULL;
    SDL_Window* window = SDL_CreateWindow(
        this->title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        this->width, this->height, SDL_WINDOW_SHOWN);
    if (window == NULL)
      __exit_fatal("Can't create sdl window.");
    return window;
  }

  SDL_Surface*  _get_win_surface()
  {
    SDL_Surface*  s = SDL_GetWindowSurface(this->win);
    if (!s) {
      this->__exit_fatal("Can't get window surface");
    }
    return s;
  }

  static void	_ft_put_pixel(SDL_Surface* surface, unsigned int x, unsigned int y, unsigned int color){
    if (y >= (unsigned int)surface->h || x >= (unsigned int)surface->w)
      return ;
    *((Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel)) = color;
  }


  void  __exit_fatal(const char *msg)
  {
    std::cout << msg << std::endl;
    std::cout << "error: " << SDL_GetError() << std::endl;
    std::exit(1);
  }

 
  void  _on_key_down(int key)
  {
    unsigned int i;

    for (i = 0; i < BUFFER_INPUT_SIZE; i++) {
      if (this->buffer_input[i] == key) {
        return;
      }
    }
    for (i = 0; i < BUFFER_INPUT_SIZE; i++) {
      if (this->buffer_input[i] == -1) {
        this->buffer_input[i] = key;
        break;
      }
    }
  }

  void  _on_key_up(int key)
  {
    unsigned int i = 0;

    if (key == SDL_SCANCODE_ESCAPE) {
      this->quit = true;
    }
    for (; i < BUFFER_INPUT_SIZE; i++) {
      if (this->buffer_input[i] == key) {
        this->buffer_input[i] = -1;
        break;
      }
    }
  }

  void  _on_mouse_move(int x, int y)
  {
    this->mouse_x = x;
    this->mouse_y = y;
  }

};


#endif

