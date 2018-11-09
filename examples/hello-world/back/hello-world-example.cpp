/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

#include <dali/dali.h>
#include <dali/devel-api/actors/actor-devel.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali-toolkit/devel-api/visuals/visual-properties-devel.h>
#include <dali-toolkit/devel-api/visuals/text-visual-properties.h>

#include <my_system.h>
#include <dlog.h>
#include <utility.h>


using namespace Dali;
using namespace Toolkit;

// This example shows the load-time image scaling and filtering features.
//
class MyTester : public ConnectionTracker
{
public:

  MyTester(Application& application) :
      mApplication(application) {
    // Connect to the Application's Init signal
    mApplication.InitSignal().Connect(this, &MyTester::Create);
  }

  ~MyTester() {
    // Nothing to do here;
    DALI_LOGE("~MyTester" << std::endl);
  }

  void ConnectEventSignal(Control control) {
    control.KeyEventSignal().Connect(this, &MyTester::OnControlKeyEvent);
    control.TouchSignal().Connect(this, &MyTester::OnControlTouch);
    control.KeyInputFocusGainedSignal().Connect(this, &MyTester::OnFocusSet);
    control.KeyInputFocusLostSignal().Connect(this, &MyTester::OnFocusUnSet);
  }
  // The Init signal is received once (only) during the Application lifetime
  void Create(Application& application) {
#if !DALI_DEMO_UBUNTU
    Tizen_Cursor_Enable(application.GetWindow());
#endif
    Stage stage = Stage::GetCurrent();
    stage.SetBackgroundColor(Color::RED);
    stage.KeyEventSignal().Connect(this, &MyTester::OnKey);

    mControl11 = Toolkit::Control::New();
    mControl11.SetName("mControl11");
    mControl11.SetParentOrigin(ParentOrigin::TOP_LEFT);
    mControl11.SetAnchorPoint(AnchorPoint::TOP_CENTER);
    mControl11.SetBackgroundColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    mControl11.SetPosition(20, 20);
    mControl11.SetSize(200, 200);
    mControl11.SetProperty(DevelActor::Property::POSITION_USES_ANCHOR_POINT, false);
    mControl11.SetKeyboardFocusable(true);
    stage.Add(mControl11);
    ConnectEventSignal(mControl11);

    mImageView1 = Toolkit::ImageView::New(DEMO_IMAGE_DIR"r_highlight_bg_focus_9patch.png");
    mImageView1.SetName("mImageView1");
    mImageView1.SetParentOrigin(ParentOrigin::TOP_LEFT);
    mImageView1.SetAnchorPoint(AnchorPoint::TOP_CENTER);
    mImageView1.SetBackgroundColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
    mImageView1.SetPosition(240, 20);
    mImageView1.SetSize(200, 200);
    mImageView1.SetProperty(DevelActor::Property::POSITION_USES_ANCHOR_POINT, false);
    //Property::Map map;
    //map[Toolkit::DevelVisual::Property::TYPE] = DevelVisual::N_PATCH;
    //map[ImageVisual::Property::URL] = DEMO_IMAGE_DIR"r_highlight_bg_focus_9patch.png";
    //map[Toolkit::DevelImageVisual::Property::BORDER] = Rect< int >( 10, 10, 10, 10 );
    //mImageView1.SetProperty( ImageView::Property::IMAGE, map );
    mImageView1.SetKeyboardFocusable(true);
    stage.Add(mImageView1);
    ConnectEventSignal(mImageView1);

    mTextLabel1 = TextLabel::New();
    mTextLabel1.SetName("mTextLabel1");
    mTextLabel1.SetParentOrigin(ParentOrigin::TOP_LEFT);
    mTextLabel1.SetAnchorPoint(AnchorPoint::TOP_CENTER);
    mTextLabel1.SetBackgroundColor(Vector4(0.0f, 0.0f, 0.0f, 255.0f));
    mTextLabel1.SetPosition(460, 20);
    mTextLabel1.SetSize(400, 50);
    mTextLabel1.SetProperty(TextLabel::Property::TEXT_COLOR, Color::WHITE);
    mTextLabel1.SetProperty(TextLabel::Property::POINT_SIZE, 30);
    mTextLabel1.SetProperty(TextLabel::Property::FONT_FAMILY, "SamsungOneUI" );
    mTextLabel1.SetProperty(TextLabel::Property::TEXT, "Five little monkeys jumping on the bed");
    mTextLabel1.SetProperty(TextLabel::Property::ENABLE_AUTO_SCROLL, true  );
    mTextLabel1.SetProperty(DevelActor::Property::POSITION_USES_ANCHOR_POINT, false);
    stage.Add(mTextLabel1);

    mEditor = TextEditor::New();
    mEditor.SetName("mEditor");
    mEditor.SetParentOrigin(ParentOrigin::TOP_LEFT);
    mEditor.SetAnchorPoint(AnchorPoint::TOP_CENTER);
    mEditor.SetProperty(DevelActor::Property::POSITION_USES_ANCHOR_POINT, false);
    mEditor.SetPosition(20.0f, 260.0f);
    mEditor.SetSize(200, 50);
    mEditor.SetBackgroundColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    mEditor.SetProperty(TextEditor::Property::TEXT_COLOR, Color::WHITE);
    mEditor.SetProperty(TextEditor::Property::POINT_SIZE, 30);
    mEditor.SetProperty(TextEditor::Property::FONT_FAMILY, "SamsungOneUI" );
    mEditor.SetProperty(TextEditor::Property::TEXT, "Input text...\n");
    mEditor.SetKeyboardFocusable(true);
    stage.Add(mEditor);
    //ConnectEventSignal(mEditor);

    //dump_position_size_information(mEditor);

    //KeyboardFocusManager::Get().SetCurrentFocusActor(mEditor);

  }

  void OnKey(const KeyEvent &event) {
    if (event.state == KeyEvent::Down) {
      DALI_LOGE("Stage key : " << event.keyCode << std::endl);
      //dump_position_size_information(mEditor);
    }
  }

  bool OnControlKeyEvent(Toolkit::Control control, const KeyEvent &event) {
    if (event.state == KeyEvent::Down) {
      DALI_LOGE("Control down key : " << control.GetName() << ", keyCode : " << event.keyCode << std::endl);
    }else {
      DALI_LOGE("Control up key : " << control.GetName() << ", keyCode : " << event.keyCode << std::endl);
    }
    return true;
  }


  bool OnControlTouch(Actor actor, const TouchData& touch) {
    if (touch.GetState(0) == PointState::DOWN) {
        DALI_LOGE("Control touch " << actor.GetName() << ", parent " << actor.GetParent().GetName() << std::endl);
        Control control = Control::DownCast(actor);
        control.SetKeyInputFocus();
      }

      return true;
  }

  void OnFocusSet(Control control) {
    DALI_LOGE("OnFocusSet " << control.GetName() << std::endl);

    Animation mAnimationFocusIn = Animation::New(1.1f);
    mAnimationFocusIn.AnimateTo( Property( control, Actor::Property::SCALE ), Vector3(1.08f, 1.08f, 0.0f), AlphaFunction(Vector2(0.21, 2), Vector2(0.14, 1)));
    mAnimationFocusIn.Play();

  }

  void OnFocusUnSet(Control control) {

    DALI_LOGE("OnFocusUnSet " << control.GetName() << std::endl);

    Animation mAnimationFocusOut = Animation::New(0.85f);
    mAnimationFocusOut.AnimateTo( Property( control, Actor::Property::SCALE ), Vector3(1.0f, 1.0f, 0.0f), AlphaFunction(Vector2(0.19, 1), Vector2(0.22, 1)));
    mAnimationFocusOut.Play();
  }

private:
  Application&  mApplication;

  Control     mControl11;
  Control     mControl12;
  ImageView   mImageView1;
  ImageView   mImageView2;
  TextLabel   mTextLabel1;
  TextLabel   mTextLabel2;
  TextEditor  mEditor;

};

void RunTest( Application& application )
{
  MyTester test( application );

  application.MainLoop();
}

// Entry point for Linux & Tizen applications
int DALI_EXPORT_API main( int argc, char **argv )
{
  Application application = Application::New( &argc, &argv);

  RunTest( application );

  return 0;
}
