// use enet to add online multiplayer


#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"


int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct ball {
  float x;
  float y;
  float width;
  float height;
}ball;

struct bottom {

  float x;
  float y;
  float width;
  float height;
}bottom;



int initialize_window(void) {
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      fprintf(stderr, "Error initializing SDL.\n");
      return FALSE;
}
window = SDL_CreateWindow(
			  NULL,
			  SDL_WINDOWPOS_CENTERED,
			  SDL_WINDOWPOS_CENTERED,
			  WINDOW_WIDTH,
			  WINDOW_HEIGHT,
			  SDL_WINDOW_BORDERLESS
			  );
if (!window) {
  fprintf(stderr, "Error creating SDL Window.\n");
  return FALSE;
 }
renderer = SDL_CreateRenderer(window,-1,0);
  if(!renderer) {
  fprintf(stderr, "Error creating SDL Renderer.\n");
  return FALSE;
 }

return TRUE;
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    game_is_running  = FALSE;
    break;

  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE) 
      game_is_running = FALSE;
    if (event.key.keysym.sym == SDLK_w)
      ball.y -= 10;
    if (event.key.keysym.sym == SDLK_s)
      ball.y +=10;
    if (event.key.keysym.sym == SDLK_a)
      ball.x -= 10;
    if (event.key.keysym.sym == SDLK_d)
      ball.x += 10;
    break;

      
  }
}

void setup() {
  ball.x = 30;
  ball.y = 30;
  ball.width = 30;
  ball.height = 30;

  bottom.x = 200;
  bottom.y = 300;
  bottom.width = 80;
  bottom.height = 50;

}

void update(void) {

  //waste some time until we reach fram target time
  // while(!SDL_TICKS_PASSED(SDL_GetTicks(),last_frame_time + FRAME_TARGET_TIME));

  // calculate how much we have to wait until we reach target frame time
  //int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

  // only delay is we are too fast to update this frame
  //if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
  //	SDL_Delay(time_to_wait);
  // }
  
  // get a delta time factor converted to seconds to be used to update my objects later
    float delta_time = (SDL_GetTicks() - last_frame_time) / 800.0f;
  
  last_frame_time = SDL_GetTicks();



  if (ball.y < bottom.y) {
    bottom.y -= 30 * delta_time;
  }
  else
    {
      bottom.y += 30 * delta_time;
    };

  if ( ball.x < bottom.x) {
    bottom.x -= 30 * delta_time;
  }
  else
    {
      bottom.x += 30 * delta_time;
    };
  


  // as you move to the right, the x value increases
  // as you move down, the y value increases
  
  // this compares the lower most edge of the rectangle with the upper most edge of the square, and the right most edge of the rectangle with the left most edge of the square
  if (abs(ball.x - bottom.x) < bottom.width && abs(ball.y - bottom.y) < bottom.height && ball.x > bottom.x && ball.y > bottom.y) {
    game_is_running = FALSE;
  };

  // this compares the upper most edge of the rectangle with the lower most edge of the square
  if (abs(ball.x - bottom.x) < bottom.width && abs(ball.y + ball.height - bottom.y) < bottom.height && (ball.y + ball.height) > bottom.y && (ball.x > bottom.x)) {
    game_is_running = FALSE;
  };

  // this compares the left most edge of the rectangle with the right most edge of the square
  if (abs(ball.x + ball.width - bottom.x) < bottom.width && abs(ball.y - bottom.y) < bottom.height && (ball.x + ball.width) > bottom.x && (ball.y + ball.height > bottom.y)) {
    game_is_running = FALSE;
  };


}

void render() {
  SDL_SetRenderDrawColor(renderer, 100, 0, 45, 255);
  SDL_RenderClear(renderer);

  
  //draw a shape
  SDL_Rect ball_square= {
    (int)ball.x,
    (int)ball.y,
    (int)ball.width,
    (int)ball.height
  };

  SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);

  SDL_RenderFillRect(renderer, &ball_square);

  SDL_RenderPresent(renderer);

    //draw a flat rectangle
  SDL_Rect bottom_rectangle= {
    (int)bottom.x,
    (int)bottom.y,
    (int)bottom.width,
    (int)bottom.height
  };

  SDL_SetRenderDrawColor(renderer, 80, 30, 50, 200);

  SDL_RenderFillRect(renderer, &bottom_rectangle);

  SDL_RenderPresent(renderer);


  
}


void destroy_window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main() {
 game_is_running = initialize_window();

 setup();

  while (game_is_running) {
    process_input();
    update();
    render();

  }

  destroy_window();
  
  return 0;
}
