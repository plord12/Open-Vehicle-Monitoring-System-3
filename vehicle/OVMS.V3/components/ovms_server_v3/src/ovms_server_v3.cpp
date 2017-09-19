/*
;    Project:       Open Vehicle Monitor System
;    Date:          14th March 2017
;
;    Changes:
;    1.0  Initial release
;
;    (C) 2011       Michael Stegen / Stegen Electronics
;    (C) 2011-2017  Mark Webb-Johnson
;    (C) 2011        Sonny Chen @ EPRO/DX
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
*/

#include "esp_log.h"
static const char *TAG = "ovms-server-v3";

#include <string.h>
#include "ovms_server_v3.h"
#include "ovms_command.h"

OvmsServerV3 *MyOvmsServerV3 = NULL;

static void OvmsServerV3_task(void *pvParameters)
  {
  while(1)
    {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }

OvmsServerV3::OvmsServerV3(std::string name)
  : OvmsServer(name)
  {
  xTaskCreatePinnedToCore(OvmsServerV3_task, "OVMS V3 Task", 4096, (void*)this, 5, &m_task, 1);
  }

OvmsServerV3::~OvmsServerV3()
  {
  vTaskDelete(m_task);
  }

void OvmsServerV3::SetPowerMode(PowerMode powermode)
  {
  m_powermode = powermode;
  switch (powermode)
    {
    case On:
      break;
    case Sleep:
      break;
    case DeepSleep:
      break;
    case Off:
      break;
    default:
      break;
    }
  }

void ovmsv3_start(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  if (MyOvmsServerV3 != NULL)
    {
    MyOvmsServerV3 = new OvmsServerV3("oscv3");
    }
  }

void ovmsv3_stop(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  if (MyOvmsServerV3 != NULL)
    {
    delete MyOvmsServerV3;
    MyOvmsServerV3 = NULL;
    }
  }

void ovmsv3_status(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  }

class OvmsServerV3Init
    {
    public: OvmsServerV3Init();
  } OvmsServerV3Init  __attribute__ ((init_priority (6200)));

OvmsServerV3Init::OvmsServerV3Init()
  {
  ESP_LOGI(TAG, "Initialising OVMS V3 Server (6200)");

  OvmsCommand* cmd_server = MyCommandApp.FindCommand("server");
  cmd_server->FindCommand("start")->RegisterCommand("v3","Start an OVMS V3 Server Connection",ovmsv3_start, "", 0, 0);
  cmd_server->FindCommand("stop")->RegisterCommand("v3","Stop an OVMS V3 Server Connection",ovmsv3_stop, "", 0, 0);
  cmd_server->FindCommand("status")->RegisterCommand("v3","Show OVMS V3 Server connection status",ovmsv3_status, "", 0, 0);
  }