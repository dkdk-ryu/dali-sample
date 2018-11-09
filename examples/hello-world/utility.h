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


#ifndef __DALI_DEMO_UILITY__
#define __DALI_DEMO_UILITY__

#include <dali/dali.h>
#include <dali/devel-api/actors/actor-devel.h>
#include <dali-toolkit/dali-toolkit.h>
#include <iostream>
#include "dlog.h"


using namespace Dali;
using namespace Toolkit;

#if !DALI_DEMO_UBUNTU

#include <Ecore.h>
#include <Ecore_Wayland.h>
#include <cursor_module.h>


void Tizen_Cursor_Enable(Window window)
{
  Ecore_Wl_Global *global_data = NULL;
  Ecore_Wl_Input *wlInput;
  unsigned int cursormgr_id = 0;
  struct wl_registry *wl_registry;
  struct wl_display *display;
  struct wl_seat *seat;

  Ecore_Wl_Window* wl_win = AnyCast<Ecore_Wl_Window*>(window.GetNativeHandle());
  ecore_wl_sync();

// ecore_wl_init() or elm_win_add(or elm_win_util_standard_add) should be done to before calling below functions
  Eina_Inlist* global_list = ecore_wl_globals_get(); // to get the list of global objects of wayland
  wl_registry = ecore_wl_registry_get();             // to get the registry structure of wayland
  display = ecore_wl_display_get();                  // to get wl_display structure
  wlInput = ecore_wl_input_get();                    // to get wl_input structure
  seat = ecore_wl_input_seat_get(wlInput);
  EINA_INLIST_FOREACH(global_list, global_data)
  {
    if (0 == strcmp(global_data->interface, "tizen_cursor")) {
      printf("Found cursormgr Object \n");
      cursormgr_id = global_data->id;  // to get the id of cursormgr object
      break;
    }
  }
  int ret_cursormod = CursorModule_Initialize(display, wl_registry, seat, cursormgr_id);

// Set CURSOR AVAILABLE config
  if (ret_cursormod) {
    Cursor_Set_Config(ecore_wl_window_surface_get(wl_win), TIZEN_CURSOR_CONFIG_CURSOR_AVAILABLE,
        NULL);
  }
}

#endif


void dump_position_size_information(Control control)
{
  Vector3 pposition = control.GetCurrentParentOrigin();
  DALI_LOGE("GetCurrentParentOrigin " << pposition.x << ", " << pposition.y << std::endl);
  Vector3 aposition = control.GetCurrentAnchorPoint();
  DALI_LOGE("GetCurrentAnchorPoint " << aposition.x << ", " << aposition.y << std::endl);
  Vector3 tSize = control.GetTargetSize();
  DALI_LOGE("GetTargetSize " << tSize.x << ", " << tSize.y << std::endl);
  Vector3 cSize = control.GetCurrentSize();
  DALI_LOGE("GetCurrentSize " << cSize.x << ", " << cSize.y << std::endl);
  Vector3 nSize = control.GetNaturalSize();
  DALI_LOGE("GetNaturalSize " << nSize.x << ", " << nSize.y << std::endl);
  Vector3 position = control.GetCurrentPosition();
  DALI_LOGE("GetCurrentPosition " << position.x << ", " << position.y << std::endl);
  Vector3 wposition = control.GetCurrentWorldPosition();
  DALI_LOGE("GetCurrentWorldPosition " << wposition.x << ", " << wposition.y << std::endl);
  Vector3 cScale = control.GetCurrentScale();
  DALI_LOGE("GetCurrentScale " << cScale.x << ", " << cScale.y << std::endl);
  Vector3 wScale = control.GetCurrentWorldScale();
  DALI_LOGE("GetCurrentWorldScale " << wScale.x << ", " << wScale.y << std::endl);
  Vector3 mFactor = control.GetSizeModeFactor();
  DALI_LOGE("GetSizeModeFactor " << mFactor.x << ", " << mFactor.y << std::endl);
  Vector3 SIZE = control.GetProperty<Vector3>(DevelActor::Property::SIZE);
  DALI_LOGE("SIZE " << SIZE.x << ", " << SIZE.y << std::endl);
  Vector3 POSITION = control.GetProperty<Vector3>(DevelActor::Property::POSITION);
  DALI_LOGE("POSITION " << POSITION.x << ", " << POSITION.y << std::endl);
  Vector3 WORLD_POSITION = control.GetProperty<Vector3>(DevelActor::Property::WORLD_POSITION);
  DALI_LOGE("WORLD_POSITION " << WORLD_POSITION.x << ", " << WORLD_POSITION.y << std::endl);
  Vector2 sPosition = control.GetProperty<Vector2>(DevelActor::Property::SCREEN_POSITION);
  DALI_LOGE("SCREEN_POSITION " << sPosition.x << ", " << sPosition.y << std::endl);

}

#endif /* __DALI_DEMO_UILITY__ */
