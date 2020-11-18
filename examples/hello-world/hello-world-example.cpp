/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <cairo.h>
#include <dali-toolkit/dali-toolkit.h>

using namespace Dali;
using Dali::Toolkit::TextLabel;

#define WIDTH 600
#define HEIGHT 275
#define STRIDE (WIDTH * 4)

// This example shows how to create and display Hello World! using a simple TextActor
//
class HelloWorldController : public ConnectionTracker
{
public:
  HelloWorldController(Application& application)
  : mApplication(application)
  {
    // Connect to the Application's Init signal
    mApplication.InitSignal().Connect(this, &HelloWorldController::Create);
  }

  ~HelloWorldController() = default; // Nothing to do in destructor

  // The Init signal is received once (only) during the Application lifetime
  void Create(Application& application)
  {
    // Get a handle to the window
    Window window = application.GetWindow();
    window.SetBackgroundColor(Color::WHITE);

    TextLabel textLabel = TextLabel::New("Hello World");
    textLabel.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT);
    textLabel.SetProperty(Dali::Actor::Property::NAME, "helloWorldLabel");
    window.Add(textLabel);

    // Respond to a touch anywhere on the window
    window.GetRootLayer().TouchedSignal().Connect(this, &HelloWorldController::OnTouch);

    // Respond to key events
    window.KeyEventSignal().Connect(this, &HelloWorldController::OnKeyEvent);

    unsigned char image[STRIDE * HEIGHT];
    unsigned char image2[STRIDE * HEIGHT];

    cairo_surface_t *surface, *surface2;
    cairo_surface_t *imagePng, *imagePng2;
    cairo_t *        cr, *cr2;

    surface = cairo_image_surface_create_for_data(image, CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT, STRIDE);

    cr = cairo_create(surface);

    // cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2*M_PI);
    // cairo_clip (cr);
    // cairo_new_path (cr); /* path not consumed by clip()*/

    imagePng = cairo_image_surface_create_from_png("stars.png");
    // w = cairo_image_surface_get_width (image);
    // h = cairo_image_surface_get_height (image);

    cairo_set_source_surface(cr, imagePng, 0, 0);
    cairo_paint(cr);

    cairo_surface_write_to_png(surface, "image.png");
    // ---------------------------------------------
    surface2  = cairo_image_surface_create_for_data(image2, CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT, STRIDE);
    cr2       = cairo_create(surface2);
    imagePng2 = cairo_image_surface_create_from_png("stars.png");
    cairo_set_source_surface(cr2, imagePng2, 0, 0);
    cairo_paint(cr2);
    cairo_surface_write_to_png(surface2, "image2.png");

    // ----------------------------------------------------------
    cairo_destroy(cr);

    cairo_surface_destroy(surface);
  }

  bool OnTouch(Actor actor, const TouchEvent& touch)
  {
    // quit the application
    mApplication.Quit();
    return true;
  }

  void OnKeyEvent(const KeyEvent& event)
  {
    if(event.GetState() == KeyEvent::DOWN)
    {
      if(IsKey(event, Dali::DALI_KEY_ESCAPE) || IsKey(event, Dali::DALI_KEY_BACK))
      {
        mApplication.Quit();
      }
    }
  }

private:
  Application& mApplication;
};

int DALI_EXPORT_API main(int argc, char** argv)
{
  Application          application = Application::New(&argc, &argv);
  HelloWorldController test(application);
  application.MainLoop();
  return 0;
}
