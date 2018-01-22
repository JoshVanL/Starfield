#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModel.h"
#include <stdint.h>

using namespace std;
using glm::vec3;
using glm::mat3;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define FULLSCREEN_MODE false


/* ----------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                            */
int t;

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update();
void Draw(screen* screen);
void Interpolate( vec3 a, vec3 b, vector<vec3>& result );
vector<vec3> stars( 1000 );

float VELOCITY = 0.001;

int main( int argc, char* argv[] )
{

    for(int i=0; i<1000; i++)
    {
        stars[i].x =  ((float)rand())/RAND_MAX * 2 - 1;
        stars[i].y =  ((float)rand())/RAND_MAX * 2 - 1;
        stars[i].z =  ((float)rand())/RAND_MAX;
    }



    screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );
    t = SDL_GetTicks();	/*Set start value for timer.*/
    SDL_GetTicks();	/*Set start value for timer.*/

  while( NoQuitMessageSDL() )
    {
      Draw(screen);
      Update();
      SDL_Renderframe(screen);
    }

  SDL_SaveImage( screen, "screenshot.bmp" );

  KillSDL(screen);
  return 0;
}

/*Place your drawing here*/
void Draw(screen* screen)
{
  /* Clear buffer */
  memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

  float H = screen->height;
  float W = screen->width;

    for ( size_t s=0; s<stars.size(); ++s )
    {
        float u = ((H/2) * (stars[s].x / stars[s].z)) + (W/2);
        float v = ((H/2) * (stars[s].y / stars[s].z)) + (H/2);

        PutPixelSDL(screen, u, v, vec3(1,1,1));
    }
}

void Update()
{
    //static int t;// = SDL_GetTicks();
    int t2 = SDL_GetTicks();
    float dt = float(t2-t);
    t = t2;

    for( uint s=0; s<stars.size(); ++s )
    {
        stars[s].z -= VELOCITY * dt;

        if( stars[s].z <= 0 )
            stars[s].z += 1;
        if( stars[s].z > 1 )
            stars[s].z -= 1;
    }
}

void Interpolate( vec3 a, vec3 b, vector<vec3>& result )
{

    if (result.size() == 1) {
        for (uint i=0; i < 3; i++) {
            result[0][i] = a[i] + ((b[i] - a[i]) / 2);
        }
        return;
    }

    vec3 step;
    for (uint i=0; i < 3; i++) {
        step[i]  = (b[i] - a[i]) / (result.size() -1);
        result[0][i] = a[i];
        result[result.size()-1][i] = b[i];
    }


    for (uint i=1; i<result.size() - 1; i++) {
        for (uint j=0; j < 3; j++) {
            result[i][j] = (i * step[j]) + a[j];
        }
    }
}
