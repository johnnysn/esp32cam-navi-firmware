#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include "esp_http_server.h"

class ServoControl {

  public:
    int horizontalAngle = 0;
    int verticalAngle = 0;

    esp_err_t handleRequest(httpd_req_t *req);
    void refresh();
};

#endif