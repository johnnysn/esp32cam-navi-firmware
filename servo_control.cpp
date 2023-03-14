#include "servo_control.h"
#include "server.h"
#include "globals.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#define TAG ""
#else
#include "esp_log.h"
static const char *TAG = "servo";
#endif

esp_err_t ServoControl::handleRequest(httpd_req_t *req) 
{
  char *buf = NULL;
  char _hor[16];
  char _ver[16];

  if (parse_get(req, &buf) != ESP_OK) {
      return ESP_FAIL;
  }
  if (
      httpd_query_key_value(buf, "hor", _hor, sizeof(_hor)) != ESP_OK ||
      httpd_query_key_value(buf, "ver", _ver, sizeof(_ver)) != ESP_OK
    ) {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
  }
  free(buf);

  horizontalAngle = atoi(_hor);
  verticalAngle = atoi(_ver);
  ESP_LOGW(TAG, "Set servo hor %d, ver %d", horizontalAngle, verticalAngle);

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

void ServoControl::refresh() {
  servoH.write(horizontalAngle);
  servoV.write(verticalAngle);
}