#include <GLES2/gl2.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali/devel-api/adaptor-framework/gl-window.h>
#include "rain-drops.h"


class RainRenderer
{
public:
  RainRenderer();
  void initialize_gl();
  void renderFrame_gl();
  void terminate_gl();

private:
  // GLuint CreateSimpleTexture2D(char* filename);
  GLuint CreateSimpleTexture2D(string filename);
  GLuint CreateShineTexture();
  void   getUniformLoc();
  void   setUniform();

private:
  Dali::GlWindow mGLWindow;
  RainDrops* mRainDrops;

  GLuint programObject;

  // Sampler location
  // GLint samplerLoc;
  GLint waterMapLoc;
  GLint textureShineLoc;
  GLint textureFgLoc;
  GLint textureBgLoc;

  GLint positionLocation;
  GLint texCoordLocation;

  // Texture handle
  // GLuint textureId;
  GLuint waterMapId;
  GLuint textureShineId;
  GLuint textureFgId;
  GLuint textureBgId;

  //uniform
  GLint u_resolution;
  GLint u_parallax;
  GLint u_parallaxFg;
  GLint u_parallaxBg;
  GLint u_textureRatio;
  GLint u_renderShine;
  GLint u_renderShadow;
  GLint u_minRefraction;
  GLint u_refractionDelta;
  GLint u_brightness;
  GLint u_alphaMultiply;
  GLint u_alphaSubtract;
};