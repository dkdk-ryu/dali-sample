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
#include <iostream>
// #include "rain-drops.h"
#include "rain-renderer.h"

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
  // RainDrops*    mRainDrops;
  RainRenderer* mRainRenderer;
  Timer         mTimer;

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
    // Window window = application.GetWindow();
    // window.SetBackgroundColor(Color::WHITE);

    // TextLabel textLabel = TextLabel::New("Hello World");
    // textLabel.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT);
    // textLabel.SetProperty(Dali::Actor::Property::NAME, "helloWorldLabel");
    // window.Add(textLabel);

    // // Respond to a touch anywhere on the window
    // window.GetRootLayer().TouchedSignal().Connect(this, &HelloWorldController::OnTouch);

    // // Respond to key events
    // window.KeyEventSignal().Connect(this, &HelloWorldController::OnKeyEvent);

    // RainOptions options;
    // options.trailRate                = 1;
    // options.trailScaleRange[0]       = 0.2;
    // options.trailScaleRange[1]       = 0.45;
    // options.collisionRadius          = 0.45;
    // options.collisionBoostMultiplier = 0.28;
    // mRainDrops                       = new RainDrops(1000, 1000, 1, options);

    mRainRenderer = new RainRenderer();

    // mTimer = Timer::New(100); // ms
    // mTimer.TickSignal().Connect(this, &HelloWorldController::OnTimer);
    // mTimer.Start();
  }

  // bool OnTimer(void)
  // {
  //   mRainDrops->update();

  //   return true;
  // }

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
