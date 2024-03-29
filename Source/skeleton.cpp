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
void InterpolateFloats(float a, float b, vector<float>& result);
void Interpolate(vec3 a, vec3 b, vector<vec3>& result);
void DrawRainbow(screen* screen, vector<vec3> a, vector<vec3> b);
void DrawStars(screen*, vector<vec3>& stars);
void UpdateStars(vector<vec3>& stars);

int main( int argc, char* argv[] )
{
    
  screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );
  t = SDL_GetTicks();	/*Set start value for timer.*/
  
  vector<vec3> stars(3000);
  for(size_t i = 0; i < stars.size(); i ++){
     float x  = -1 + 2*float(rand()) / float(RAND_MAX);
     float y  = -1 + 2*float(rand()) / float(RAND_MAX);
     float z  = float(rand()) / float(RAND_MAX);
     vec3 star(x,y,z);
     stars.at(i) = star; 
  }
  
  static int t = SDL_GetTicks();

  while( NoQuitMessageSDL() )
    {
	  DrawStars(screen, stars);
      UpdateStars(stars);
      SDL_Renderframe(screen);
	}

  SDL_SaveImage( screen, "screenshot.bmp" );

  KillSDL(screen);
  return 0;
}

void UpdateStars(vector<vec3>& stars){
    int t2 = SDL_GetTicks();
    float dt = float(t2-t);
    t = t2;
    float velocity = 0.00001f;
    for(size_t s=0; s<stars.size(); s++){
        vec3 v = stars.at(s);
        v.z -= velocity * dt;
        stars.at(s) = v;
        if(stars.at(s).z <= 0)
            stars.at(s).z += 1;
        if(stars.at(s).z > 1)
            stars.at(s).z -=1;
    }
}

void DrawStars(screen* screen, vector<vec3>& stars){
  float f = SCREEN_HEIGHT/2;

  memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
  for(size_t s=0; s<stars.size(); s++){
     int u = f*(stars.at(s).x/stars.at(s).z) + SCREEN_WIDTH/2;
     int v = f*(stars.at(s).y/stars.at(s).z) + SCREEN_HEIGHT/2;
     vec3 color = 0.2f * stars.at(s)/(stars.at(s).z*stars.at(s).z);

     PutPixelSDL(screen, u, v, color);
  }
}

void Colours(){
  vec3 topLeft(1,0,0);
  vec3 topRight(0,0,1);
  vec3 bottomRight(0,1,0);
  vec3 bottomLeft(1,1,0);
  vector<vec3> leftSide(SCREEN_HEIGHT);
  vector<vec3> rightSide(SCREEN_HEIGHT);
  Interpolate(topLeft, bottomLeft, leftSide);
  Interpolate(topRight, bottomRight,rightSide);
}

void InterpolateFloats(float a, float b, vector<float>& result){
   for(size_t i = 0; i < result.size(); i++){
      	result.at(i) = a + i*(b - a)/(result.size() - 1); 
	printf("%f:", result.at(i));
   }
}

/*Place your drawing here*/
void DrawRainbow(screen* screen, vector<vec3> a, vector<vec3> b)
{
  /* Clear buffer */
  memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
  vector<vec3> results(SCREEN_HEIGHT);
  for(size_t y=0; y<SCREEN_HEIGHT; y++)
    {
      vector<vec3> results(SCREEN_WIDTH);
      Interpolate(a.at(y), b.at(y), results);
      for(size_t x = 0; x < SCREEN_WIDTH; x++){
        vec3 colour = results.at(x);
      	PutPixelSDL(screen, x, y, colour);
      }
    }
}

/*Place your drawing here*/
void Draw(screen* screen)
{
  /* Clear buffer */
  memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
  
  vec3 colour(1.0,0.0,0.0);
  for(int i=0; i<1000; i++)
    {
      uint32_t x = rand() % screen->width;
      uint32_t y = rand() % screen->height;
      PutPixelSDL(screen, x, y, colour);
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
