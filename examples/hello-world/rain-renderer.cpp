
#include <dali/devel-api/adaptor-framework/gl-window.h>

#include "rain-renderer.h"
#include "simple-vert.h"
#include "water-frag.h"

#include <cairo.h>
#include <stdarg.h>

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 752

using namespace Dali;

RainRenderer *gRainRenderer;

void initCallback()
{
  gRainRenderer->initialize_gl();
}

void renderCallback()
{
  gRainRenderer->renderFrame_gl();
}

void terminateCallback()
{
  gRainRenderer->terminate_gl();
}

void esLogMessage(const char *formatStr, ...)
{
  va_list params;
  char buf[BUFSIZ];

  va_start(params, formatStr);
  vsprintf(buf, formatStr, params);

#ifdef ANDROID
  __android_log_print(ANDROID_LOG_INFO, "esUtil", "%s", buf);
#else
  printf("%s", buf);
#endif

  va_end(params);
}

GLuint esLoadShader(GLenum type, const char *shaderSrc)
{
  GLuint shader;
  GLint compiled;

  // Create the shader object
  shader = glCreateShader(type);

  if (shader == 0)
  {
    return 0;
  }

  // Load the shader source
  glShaderSource(shader, 1, &shaderSrc, NULL);

  // Compile the shader
  glCompileShader(shader);

  // Check the compile status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled)
  {
    GLint infoLen = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen > 1)
    {
      char *infoLog = (char *)malloc(sizeof(char) * infoLen);

      glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
      esLogMessage("Error compiling shader:\n%s\n", infoLog);

      free(infoLog);
    }

    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

GLuint esLoadProgram(const char *vertShaderSrc, const char *fragShaderSrc)
{
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint programObject;
  GLint linked;

  // Load the vertex/fragment shaders
  vertexShader = esLoadShader(GL_VERTEX_SHADER, vertShaderSrc);

  if (vertexShader == 0)
  {
    return 0;
  }

  fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);

  if (fragmentShader == 0)
  {
    glDeleteShader(vertexShader);
    return 0;
  }

  // Create the program object
  programObject = glCreateProgram();

  if (programObject == 0)
  {
    return 0;
  }

  glAttachShader(programObject, vertexShader);
  glAttachShader(programObject, fragmentShader);

  // Link the program
  glLinkProgram(programObject);

  // Check the link status
  glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

  if (!linked)
  {
    GLint infoLen = 0;

    glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen > 1)
    {
      char *infoLog = (char *)malloc(sizeof(char) * infoLen);

      glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
      esLogMessage("Error linking program:\n%s\n", infoLog);

      free(infoLog);
    }

    glDeleteProgram(programObject);
    return 0;
  }

  // Free up no longer needed shader resources
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return programObject;
}

// pullic Callbacks
// intialize callback that gets called once for intialization
void RainRenderer::initialize_gl()
{
  // Load the shaders and get a linked program object
  programObject = esLoadProgram(vShaderStr, fShaderStr);

  positionLocation = glGetAttribLocation(programObject, "a_position");
  texCoordLocation = glGetAttribLocation(programObject, "a_texCoord");

  // Get the sampler location
  // samplerLoc = glGetUniformLocation ( programObject, "s_texture" );
  waterMapLoc = glGetUniformLocation(programObject, "u_waterMap");
  textureShineLoc = glGetUniformLocation(programObject, "u_textureShine");
  textureFgLoc = glGetUniformLocation(programObject, "u_textureFg");
  textureBgLoc = glGetUniformLocation(programObject, "u_textureBg");

  // Load the texture
  // waterMapId = CreateSimpleTexture2D("canvas.png");
  waterMapId = CreateSimpleTexture2D("webcanvas2.png");
  textureFgId = CreateSimpleTexture2D("texture-rain-fg.png");
  textureBgId = CreateSimpleTexture2D("texture-rain-bg.png");
  textureShineId = CreateShineTexture();

  getUniformLoc();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void RainRenderer::renderFrame_gl()
{
  mRainDrops->update();

  GLfloat vVertices[] = {
      -1.0f, 1.0f, 0.0f, // Position 0
      0.0f,
      0.0f, // TexCoord 0
      -1.0f,
      -1.0f,
      0.0f, // Position 1
      0.0f,
      1.0f, // TexCoord 1
      1.0f,
      -1.0f,
      0.0f, // Position 2
      1.0f,
      1.0f, // TexCoord 2
      1.0f,
      1.0f,
      0.0f, // Position 3
      1.0f,
      0.0f // TexCoord 3
  };
  GLushort indices[] = {0, 1, 2, 0, 2, 3};

  // Set the viewport
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  // Clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // Use the program object
  glUseProgram(programObject);

  // Load the vertex position
  glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices);
  // Load the texture coordinate
  glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  // --------------------------------------------
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, waterMapId);
  glUniform1i(waterMapLoc, 0);

  // glBindTexture(GL_TEXTURE_2D, waterMapId);
  // Load the texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, mRainDrops->getCanvas());

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureShineId);
  glUniform1i(textureShineLoc, 1);

  //TODO:
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, textureFgId);
  glUniform1i(textureFgLoc, 2);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, textureBgId);
  glUniform1i(textureBgLoc, 3);

  setUniform();
  // --------------------------------------------
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void RainRenderer::terminate_gl()
{
}

void RainRenderer::getUniformLoc()
{
  u_resolution = glGetUniformLocation(programObject, "u_resolution");
  u_parallax = glGetUniformLocation(programObject, "u_parallax");
  u_parallaxFg = glGetUniformLocation(programObject, "u_parallaxFg");
  u_parallaxBg = glGetUniformLocation(programObject, "u_parallaxBg");
  u_textureRatio = glGetUniformLocation(programObject, "u_textureRatio");
  u_renderShine = glGetUniformLocation(programObject, "u_renderShine");
  u_renderShadow = glGetUniformLocation(programObject, "u_renderShadow");
  u_minRefraction = glGetUniformLocation(programObject, "u_minRefraction");
  u_refractionDelta = glGetUniformLocation(programObject, "u_refractionDelta");
  u_brightness = glGetUniformLocation(programObject, "u_brightness");
  u_alphaMultiply = glGetUniformLocation(programObject, "u_alphaMultiply");
  u_alphaSubtract = glGetUniformLocation(programObject, "u_alphaSubtract");
}

void RainRenderer::setUniform()
{
  glUniform2f(u_resolution, WINDOW_WIDTH, WINDOW_HEIGHT);
  glUniform2f(u_parallax, 0, 0);
  glUniform1f(u_parallaxFg, 20);
  glUniform1f(u_parallaxBg, 5);
  glUniform1f(u_textureRatio, 1.5);
  glUniform1i(u_renderShine, GL_FALSE);
  glUniform1i(u_renderShadow, GL_FALSE);
  glUniform1f(u_minRefraction, 256);
  glUniform1f(u_refractionDelta, 256);
  glUniform1f(u_brightness, 1.04);
  glUniform1f(u_alphaMultiply, 6);
  glUniform1f(u_alphaSubtract, 4);
}

GLuint RainRenderer::CreateSimpleTexture2D(char *filename)
{
  // Texture object handle
  GLuint textureId;

  cairo_surface_t *cairoPng = cairo_image_surface_create_from_png(filename);
  unsigned char *mCanvas = cairo_image_surface_get_data(cairoPng);
  int w = cairo_image_surface_get_width(cairoPng);
  int h = cairo_image_surface_get_height(cairoPng);

  // Use tightly packed data
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Generate a texture object
  glGenTextures(1, &textureId);

  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, textureId);

  // Load the texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, mCanvas);
  // glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

  // Set the filtering mode
  // glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  // glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return textureId;
}

GLuint RainRenderer::CreateShineTexture()
{
  // Texture object handle
  GLuint textureId;

  int temp[] = {0, 0, 0, 0};

  // Use tightly packed data
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Generate a texture object
  glGenTextures(1, &textureId);

  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, textureId);

  // Load the texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp);

  // Set the filtering mode
  // glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  // glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return textureId;
}

RainRenderer::RainRenderer()
{

  RainOptions options;
  options.trailRate = 1;
  options.trailScaleRange[0] = 0.2;
  options.trailScaleRange[1] = 0.45;
  options.collisionRadius = 0.45;
  options.collisionBoostMultiplier = 0.28;
  mRainDrops = new RainDrops(WINDOW_WIDTH, WINDOW_HEIGHT, 1, options);

  gRainRenderer = this;
  mGLWindow = Dali::GlWindow::New(PositionSize(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), "GLWindow", "", false);
  mGLWindow.SetEglConfig(true, true, 0, Dali::GlWindow::GlesVersion::VERSION_3_0);
  mGLWindow.RegisterGlCallback(initCallback, renderCallback, terminateCallback);

  mGLWindow.Show();
}

// void RainRenderer::draw()
// {
// }
// void RainRenderer::updateTextures()
// {
// }
// void RainRenderer::updateTexture()
// {
// }