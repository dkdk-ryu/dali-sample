#include <cairo.h>
#include <chrono>
#include <vector>

using namespace std;

struct RainOptions
{
  int   minR;
  int   maxR;
  int   maxDrops;
  float rainChance;
  int   rainLimit;
  int   dropletsRate;
  int   dropletsSize[3];
  float dropletsCleaningRadiusMultiplier;
  bool  raining;
  int   globalTimeScale;
  int   trailRate;
  bool  autoShrink;
  float spawnArea[2];
  float trailScaleRange[2];
  float collisionRadius;
  float collisionRadiusIncrease;
  int   dropFallMultiplier;
  float collisionBoostMultiplier;
  int   collisionBoost;

  RainOptions()
  {
    minR                             = 10;
    maxR                             = 40;
    maxDrops                         = 900;
    rainChance                       = 0.3;
    rainLimit                        = 3;
    dropletsRate                     = 50;
    dropletsSize[0]                  = 2;
    dropletsSize[1]                  = 3;
    dropletsSize[2]                  = 4;
    dropletsCleaningRadiusMultiplier = 0.43;
    raining                          = true;
    globalTimeScale                  = 1;
    trailRate                        = 1;
    autoShrink                       = true;
    spawnArea[0]                     = -0.1;
    spawnArea[1]                     = 0.95;
    trailScaleRange[0]               = 0.2;
    trailScaleRange[1]               = 0.5;
    collisionRadius                  = 0.65;
    collisionRadiusIncrease          = 0.01;
    dropFallMultiplier               = 1;
    collisionBoostMultiplier         = 0.05;
    collisionBoost                   = 1;
  }
};

struct Drop
{
  double       x;
  double       y;
  double       r;
  int          spreadX;
  int          spreadY;
  int          momentum;
  int          momentumX;
  int          lastSpawn;
  int          nextSpawn;
  struct Drop* parent;
  bool         isNew;
  bool         killed;
  int          shrink;

  Drop()
  {
    x         = 0;
    y         = 0;
    r         = 0;
    spreadX   = 0;
    spreadY   = 0;
    momentum  = 0;
    momentumX = 0;
    lastSpawn = 0;
    nextSpawn = 0;
    parent    = nullptr;
    isNew     = true;
    killed    = false;
    shrink    = 0;
  }
};

class RainDrops
{
public:
  RainDrops(int width, int height, float scale, RainOptions options);
  void update();
  unsigned char* getCanvas();

private:
  void setupWeatherData();
  void          init();
  int           deltaR();
  double        area();
  double        areaMultiplier();
  double        random(double from, double to, unsigned char interpolationType);
  bool          chance(double c);
  void          updateDroplets(double timeScale);
  void          updateDrops(double timeScale);
  void          renderDropsGfx();
  void          clearCanvas();
  double        getRandom();
  void          drawDrop(cairo_t* ctx, Drop* drop);
  void          clearDroplets(double x, double y, double r = 30);
  void          drawDroplet(int x, int y, double r);
  vector<Drop*> updateRain(double timeScale);

private:
  int   mWidth;
  int   mHeight;
  float mScale;

  RainOptions mOptions;
  //TODO: init value
  int              mDropletsPixelDensity;
  int              mDropletsCounter;
  double           mTextureCleaningIterations;
  uint64_t         mLastRenderTime;
  unsigned char*   mCanvas;
  cairo_surface_t* mSurface;
  cairo_t*         mCtx;

  //forPNG
  uint32_t         mPngIndex;

  unsigned char*   mDropletsCanvas;
  cairo_surface_t* mDropletsSurface;
  cairo_t*         mDropletsCtx;

  unsigned char*   mClearDropletsGfx; //canvas - //clearcanvas 한번만 그리고 더이상 업데이트할일이 없어서 ctx는 따로 저장하지 않음.
  cairo_surface_t* mClearDropletsSurface;
  cairo_t*         mClearDropletsCtx;

  vector<Drop*>            mDrops;
  vector<unsigned char*>   mDropsGfx;
  vector<cairo_surface_t*> mDropsSurfaces;
  cairo_surface_t*         mDropAlpha;
  cairo_surface_t*         mDropColor;
};