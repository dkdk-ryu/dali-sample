#include "rain-drops.h"
#include <algorithm>
#include <cmath>
#include <random>

using namespace chrono;

const int STRIDE   = 4;
const int DROPSIZE = 64;

RainDrops::RainDrops(int width, int height, float scale, RainOptions options)
: mWidth(width),
  mHeight(height),
  mScale(scale)
{
  init();
}

void RainDrops::init()
{
  //TODO: Check memset
  mCanvas  = new unsigned char[mWidth * mHeight * STRIDE];
  mSurface = cairo_image_surface_create_for_data(mCanvas, CAIRO_FORMAT_ARGB32, mWidth, mHeight, STRIDE);
  mCtx     = cairo_create(mSurface);

  mDropletsCanvas  = new unsigned char[mWidth * mHeight * STRIDE];
  mDropletsSurface = cairo_image_surface_create_for_data(mDropletsCanvas, CAIRO_FORMAT_ARGB32, mWidth, mHeight, STRIDE);
  mDropletsCtx     = cairo_create(mDropletsSurface);

  mDropColor = cairo_image_surface_create_from_png("drop-color.png");
  mDropAlpha = cairo_image_surface_create_from_png("drop-alpha.png");

  //   this.drops    = [];
  //   this.dropsGfx = [];

  renderDropsGfx();

  update();
}

void RainDrops::renderDropsGfx()
{
  //TODO: Check canvas format
  // I think
  // HTML canvas : RGBA
  // cairo surface : ARGB
  // clearRect(): transparent black : rgba(0,0,0,0)
  unsigned char    dropBufferCanvas[DROPSIZE * DROPSIZE * STRIDE];
  cairo_surface_t* dropBufferSurface = cairo_image_surface_create_for_data(dropBufferCanvas, CAIRO_FORMAT_ARGB32, DROPSIZE, DROPSIZE, STRIDE);
  cairo_t*         dropBufferCtx     = cairo_create(dropBufferSurface);

  for(int i = 0; i < 255; i++)
  {
    unsigned char*   dropCanvas  = new unsigned char[DROPSIZE * DROPSIZE * STRIDE];
    cairo_surface_t* dropSurface = cairo_image_surface_create_for_data(dropCanvas, CAIRO_FORMAT_ARGB32, DROPSIZE, DROPSIZE, STRIDE);
    cairo_t*         dropCtx     = cairo_create(dropSurface);

    //Clear dropBuffer
    cairo_save(dropBufferCtx);
    cairo_set_source_rgba(dropBufferCtx, 0.0, 0.0, 0.0, 0.0);
    cairo_paint(dropBufferCtx);
    cairo_restore(dropBufferCtx);

    // color
    cairo_save(dropBufferCtx);
    cairo_set_operator(dropBufferCtx, CAIRO_OPERATOR_OVER);
    cairo_set_source_surface(dropBufferCtx, mDropColor, 0, 0);
    cairo_paint(dropBufferCtx);

    // blue overlay, for depth
    cairo_set_operator(dropBufferCtx, CAIRO_OPERATOR_SCREEN);
    //Check: RGBA parameters are double type;
    cairo_set_source_rgba(dropBufferCtx, 0.0, 0.0, (double)i / 255, 1);
    cairo_paint(dropBufferCtx);
    cairo_restore(dropBufferCtx);

    // alpha
    cairo_set_operator(dropCtx, CAIRO_OPERATOR_OVER);
    cairo_set_source_surface(dropCtx, mDropAlpha, 0, 0);

    cairo_set_operator(dropCtx, CAIRO_OPERATOR_IN);
    cairo_set_source_surface(dropCtx, dropBufferSurface, 0, 0);
    cairo_paint(dropCtx);

    cairo_destroy(dropCtx);
    cairo_surface_destroy(dropSurface);

    mDropsGfx.push_back(dropCanvas);
    mDropsSurfaces.push_back(dropSurface);
  }

  cairo_destroy(dropBufferCtx);
  cairo_surface_destroy(dropBufferSurface);

  // create circle that will be used as a brush to remove droplets
  mClearDropletsGfx                     = new unsigned char[128 * 128 * STRIDE];
  cairo_surface_t* clearDropletsSurface = cairo_image_surface_create_for_data(mClearDropletsGfx, CAIRO_FORMAT_ARGB32, 128, 128, STRIDE);
  cairo_t*         clearDropletsCtx     = cairo_create(clearDropletsSurface);

  //TODO: Check arc() between cairo_arc and arc(). In cairo the parameters are double.
  //   void cairo_arc(cairo_t * cr, double xc, double yc, double radius, double angle1, double angle2);
  //   clearDropletsCtx.arc(64, 64, 64, 0, Math.PI * 2);
  cairo_arc(clearDropletsCtx, 64, 64, 64, 0, M_PI * 2);
  cairo_clip(clearDropletsCtx);
  cairo_new_path(clearDropletsCtx);
  cairo_set_source_rgba(clearDropletsCtx, 0.0, 0.0, 0.0, 1.0);
  cairo_paint(clearDropletsCtx);

  cairo_destroy(clearDropletsCtx);
  cairo_surface_destroy(clearDropletsSurface);
}

void RainDrops::clearCanvas()
{
  cairo_save(mCtx);
  cairo_set_source_rgba(mCtx, 0.0, 0.0, 0.0, 0.0);
  cairo_paint(mCtx);
  cairo_restore(mCtx);
}

int RainDrops::deltaR()
{
  return mOptions.maxR - mOptions.minR;
}

double RainDrops::area()
{
  return (mWidth * mHeight) / mScale;
}

double RainDrops::areaMultiplier()
{
  return sqrt(area() / (1024 * 768));
}

double RainDrops::random(double from, double to, unsigned char interpolationType)
{
  //TODO: check double 0
  // TODO: check type all case
  if(from == 0)
  {
    from = 0;
    to   = 1;
  }
  else if(from != 0 && to == 0)
  {
    to   = from;
    from = 0;
  }
  double delta = to - from;

  if(interpolationType == 1)
  { //n
    return from + (getRandom() * delta);
  }
  else if(interpolationType == 2)
  { //n^2
    double n = getRandom();
    return from + (pow(n, 2) * delta);
  }
  else
  { // n^3
    double n = getRandom();
    return from + (pow(n, 3) * delta);
  }
}

bool RainDrops::chance(double c)
{
  return random(0, 0, 1) <= c;
}

double RainDrops::getRandom()
{
  std::random_device randDeivce;
  // std::mt19937 gen(randDeivce());
  std::default_random_engine             randEngine{randDeivce()};
  std::uniform_real_distribution<double> dist(0, 1);

  return dist(randEngine);
}

// TODO: check return type. pointer? instance?

// void drawDrop(ctx, Drop drop)
void RainDrops::drawDrop(Drop drop)
{
  if(mDropsGfx.size() > 0)
  {
    double x       = drop.x;
    double y       = drop.y;
    double r       = drop.r;
    int    spreadX = drop.spreadX;
    int    spreadY = drop.spreadY;

    double scaleX = 1;
    double scaleY = 1.5;

    double d = std::max((double)0, std::min((double)1, ((r - mOptions.minR) / (deltaR())) * 0.9));
    d *= 1 / (((drop.spreadX + drop.spreadY) * 0.5) + 1);
    d = floor(d * (mDropsGfx.size() - 1));

    // TODO: Check globalAlpha meaning.
    cairo_save(mCtx);
    cairo_set_operator(mCtx, CAIRO_OPERATOR_OVER);
    //TODO: Check How to set(scale) image size
    // void cairo_set_source_surface(cairo_t * cr, cairo_surface_t * surface, double x, double y);

    // int w = cairo_image_surface_get_width(mDropsGfx[d]);
    // int h = cairo_image_surface_get_height(mDropsGfx[d]);
    int w = DROPSIZE;
    int h = DROPSIZE;
    cairo_scale(mCtx, ((r * 2 * scaleX * (spreadX + 1)) * mScale) / w, ((r * 2 * scaleY * (spreadY + 1)) * mScale) / h);

    // TODO:check x, y
    cairo_set_source_surface(mCtx, mDropsSurfaces[d], ((x - (r * scaleX * (spreadX + 1))) * mScale) / w, ((y - (r * scaleY * (spreadY + 1))) * mScale) / h);
    cairo_paint_with_alpha(mCtx, 1);
    cairo_restore(mCtx);
  }
}

void RainDrops::drawDroplet(int x, int y, double r)
{
  Drop drop;
  drop.x = x * mDropletsPixelDensity;
  drop.y = y * mDropletsPixelDensity;
  drop.r = r * mDropletsPixelDensity;
  drawDrop(drop);
}

void RainDrops::updateDroplets(double timeScale)
{
  if(mTextureCleaningIterations > 0)
  {
    mTextureCleaningIterations -= 1 * timeScale;

    cairo_save(mDropletsCtx);
    cairo_set_operator(mDropletsCtx, CAIRO_OPERATOR_DEST_OUT);
    cairo_set_source_rgba(mDropletsCtx, 0.0, 0.0, 0.0, (0.05 * timeScale) / 255);
    //TODO: check x,y,w,h are double or int.
    cairo_rectangle(mDropletsCtx, 0, 0, mWidth * mDropletsPixelDensity, mHeight * mDropletsPixelDensity);
    cairo_fill(mDropletsCtx);
    cairo_restore(mDropletsCtx);
  }

  if(mOptions.raining)
  {
    mDropletsCounter += mOptions.dropletsRate * timeScale * areaMultiplier();

    for(int i = 0; i < mDropletsCounter; i++)
    {
      mDropletsCounter--;
      drawDroplet(random((double)mWidth / mScale, 0, 1),
                  random((double)mHeight / mScale, 0, 1),
                  random(mOptions.dropletsSize[0], mOptions.dropletsSize[1], 2));
    }
  }

  cairo_save(mCtx);
  cairo_set_source_surface(mCtx, mDropletsSurface, 0, 0);
  cairo_paint(mCtx);
  cairo_restore(mCtx);
}

vector<Drop*> RainDrops::updateRain(double timeScale)
{
  vector<Drop*> rainDrops;

  if(mOptions.raining)
  {
    double limit = mOptions.rainLimit * timeScale * areaMultiplier();
    int    count = 0;
    while(chance(mOptions.rainChance * timeScale * areaMultiplier()) && count < limit)
    {
      count++;
      double r = random(mOptions.minR, mOptions.maxR, 3);

      { //createDrop
        if(mDrops.size() < mOptions.maxDrops * areaMultiplier())
        {
          Drop* rainDrop     = new Drop();
          rainDrop->x        = random(mWidth / mScale, 0, 1);
          rainDrop->y        = random((mHeight / mScale) * mOptions.spawnArea[0], (mHeight / mScale) * mOptions.spawnArea[1], 1);
          rainDrop->r        = r;
          rainDrop->momentum = 1 + ((r - mOptions.minR) * 0.1) + random(2, 0, 1);
          rainDrop->spreadX  = 1.5;
          rainDrop->spreadY  = 1.5;
          rainDrops.push_back(rainDrop);
        }
      }
    }
  }

  return rainDrops;
}

bool compare(Drop a, Drop b)
{
  // if(a.name == b.name)
  // { //이름이 같으면, 나이가 적은순
  //   return a.age < b.age;
  // }
  // else
  // { //이름 다르면, 이름 사전순
  //   return a.name < b.name;
  // }

  // TODO: Can I use member variable?
  // TODO: equal?
  // return -1 is Okay?
  // double va = (a.y * (mWidth / mScale)) + a.x;
  // double vb = (b.y * (mWidth / mScale)) + b.x;
  // // return va > vb ? 1 : va == vb ? 0 : -1;
  // return va > vb ? true : false;

  return true;
}

void RainDrops::updateDrops(double timeScale)
{
  vector<Drop> newDrops;

  updateDroplets(timeScale);
  vector<Drop> rainDrops = updateRain(timeScale);
  // vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
  newDrops.insert(newDrops.end(), rainDrops.begin(), rainDrops.end());

  sort(mDrops.begin(), mDrops.end(), compare);

  // vector<Drop>::iterator iter = mDrops.begin();

  // for(iter = mDrops.begin(); iter != mDrops.end(); ++iter)
  for(unsigned int i = 0; i < mDrops.size(); i++)
  {
    if(!mDrops[i].killed)
    {
      // update gravity
      // (chance of drops "creeping down")
      if(chance((mDrops[i].r - (mOptions.minR * mOptions.dropFallMultiplier)) * (0.1 / deltaR()) * timeScale))
      {
        mDrops[i].momentum += random((mDrops[i].r / mOptions.maxR) * 4, 0, 1);
      }
      // clean small drops
      if(mOptions.autoShrink && mDrops[i].r <= mOptions.minR && chance(0.05 * timeScale))
      {
        mDrops[i].shrink += 0.01;
      }
      //update shrinkage
      mDrops[i].r -= mDrops[i].shrink * timeScale;
      if(mDrops[i].r <= 0) mDrops[i].killed = true;

      // update trails
      if(mOptions.raining)
      {
        mDrops[i].lastSpawn += mDrops[i].momentum * timeScale * mOptions.trailRate;
        if(mDrops[i].lastSpawn > mDrops[i].nextSpawn)
        {
          // CreateDrop
          if(mDrops.size() < mOptions.maxDrops * areaMultiplier())
          {
            Drop trailDrop;
            trailDrop.x       = mDrops[i].x + (random(-mDrops[i].r, mDrops[i].r, 1) * 0.1);
            trailDrop.y       = mDrops[i].y - (mDrops[i].r * 0.01);
            trailDrop.r       = mDrops[i].r * random(mOptions.trailScaleRange[0], mOptions.trailScaleRange[1], 1);
            trailDrop.spreadY = mDrops[i].momentum * 0.1;
            trailDrop.parent  = mDrops[i];
            newDrops.push_back(trailDrop);

            mDrops[i].r *= pow(0.97, timeScale);
            mDrops[i].lastSpawn = 0;
            mDrops[i].nextSpawn = random(mOptions.minR, mOptions.maxR, 1) - (mDrops[i].momentum * 2 * mOptions.trailRate) + (mOptions.maxR - mDrops[i].r);
          }
        }
      }

      //normalize spread
      mDrops[i].spreadX *= pow(0.4, timeScale);
      mDrops[i].spreadY *= pow(0.7, timeScale);

      //update position
      bool moved = mDrops[i].momentum > 0;
      if(moved && !mDrops[i].killed)
      {
        mDrops[i].y += mDrops[i].momentum * mOptions.globalTimeScale;
        mDrops[i].x += mDrops[i].momentumX * mOptions.globalTimeScale;
        if(mDrops[i].y > (mHeight / mScale) + mDrops[i].r)
        {
          mDrops[i].killed = true;
        }
      }

      // collision
      bool checkCollision = (moved || mDrops[i].isNew) && !mDrops[i].killed;
      mDrops[i].isNew     = false;

      if(checkCollision)
      {
        // Example:
        // v2 = std::vector<int>(v1.begin() + 1, v1.end());
        //TODO: Check bound again
        vector<Drop> tempDrops = vector<Drop>(mDrops.at(i + 1), (i + 70) >= mDrops.size() ? mDrops.end() : mDrops.at(i + 70));
        for(unsigned int j = 0; j < tempDrops.size(); j++)
        {
          if(mDrops[i] != tempDrops[j] &&
             mDrops[i].r > tempDrops[j].r &&
             mDrops[i].parent != tempDrops[j] &&
             tempDrops[j].parent != mDrops[i] &&
             !tempDrops[j].killed)
          {
            double dx = tempDrops[j].x - mDrops[i].x;
            double dy = tempDrops[j].y - mDrops[i].y;
            double d  = sqrt((dx * dx) + (dy * dy));
            //if it's within acceptable distance
            // TODO: I can't find this.maxR. I used mOptions.maxR
            if(d < (mDrops[i].r + tempDrops[j].r) * (mOptions.collisionRadius + (mDrops[i].momentum * mOptions.collisionRadiusIncrease * timeScale)))
            {
              double pi      = M_PI;
              double r1      = mDrops[i].r;
              double r2      = tempDrops[j].r;
              double a1      = pi * (r1 * r1);
              double a2      = pi * (r2 * r2);
              double targetR = sqrt((a1 + (a2 * 0.8)) / pi);
              if(targetR > mOptions.maxR)
              {
                targetR = mOptions.maxR;
              }
              mDrops[i].r = targetR;
              mDrops[i].momentumX += dx * 0.1;
              mDrops[i].spreadX   = 0;
              mDrops[i].spreadY   = 0;
              tempDrops[j].killed = true;
              mDrops[i].momentum  = std::max(tempDrops[j].momentum, std::min((double)40, mDrops[i].momentum + (targetR * mOptions.collisionBoostMultiplier) + mOptions.collisionBoost));
            }
          }
        }

        //slowdown momentum
        mDrops[i].momentum -= std::max((double)1, (mOptions.minR * 0.5) - mDrops[i].momentum) * 0.1 * timeScale;
        if(mDrops[i].momentum < 0) mDrops[i].momentum = 0;
        mDrops[i].momentumX *= pow(0.7, timeScale);

        if(!mDrops[i].killed)
        {
          newDrops.push_back(mDrops[i]);
          if(moved && mOptions.dropletsRate > 0) clearDroplets(mDrops[i].x, mDrops[i].y, mDrops[i].r * mOptions.dropletsCleaningRadiusMultiplier);
          drawDrop(mDrops[i]);
        }
      }
    }
  }

  mDrops = newDrops;
};

void RainDrops::update()
{
  clearCanvas();

  // milliseconds now = duration_cast<milliseconds>(system_clock::now()).count();
  uint64_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

  if(mLastRenderTime == 0)
  {
    mLastRenderTime = now;
  }

  uint64_t deltaT = now - mLastRenderTime;

  double timeScale = deltaT / ((1. / 60) * 1000);
  if(timeScale > 1.1)
  {
    timeScale = 1.1;
  }
  timeScale *= mOptions.globalTimeScale;
  mLastRenderTime = now;

  updateDrops(timeScale);

  // requestAnimationFrame(this.update.bind(this));
}
