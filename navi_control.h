#ifndef NAVI_CONTROL_H
#define NAVI_CONTROL_H

#define NAVIGATION_TIMEOUT 2000

#include "server.h"

class NaviControl {

  public:
    int speedLeft = 0;
    int speedRight = 0;
    bool dirLeft = false;
    bool dirRight = false;
    bool moving = false;

    esp_err_t handleMoveRequest(httpd_req_t *req);
    esp_err_t handleStopRequest(httpd_req_t *req);
    void refresh();

    long time_last_move_cmd = 0;
  
  private:
    void watchdog();
};

#endif