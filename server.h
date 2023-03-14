#ifndef SERVER_H
#define SERVER_H
#include "esp_http_server.h"

esp_err_t parse_get(httpd_req_t *req, char **obuf);
void startCameraServer();

#endif