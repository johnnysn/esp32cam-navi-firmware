#include "navi_control.h"
#include "arduino.h"
#include "globals.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#define TAG ""
#else
#include "esp_log.h"
static const char *TAG = "navigation";
#endif

esp_err_t NaviControl::handleMoveRequest(httpd_req_t *req) {
  char *buf = NULL;
  char _speed_left[16];
  char _speed_right[16];
  char _dir_left[4];
  char _dir_right[4];

  if (parse_get(req, &buf) != ESP_OK) {
      return ESP_FAIL;
  }
  if (
      httpd_query_key_value(buf, "sl", _speed_left, sizeof(_speed_left)) != ESP_OK ||
      httpd_query_key_value(buf, "sr", _speed_right, sizeof(_speed_right)) != ESP_OK ||
      httpd_query_key_value(buf, "dl", _dir_left, sizeof(_dir_left)) != ESP_OK ||
      httpd_query_key_value(buf, "dr", _dir_right, sizeof(_dir_right)) != ESP_OK
    ) {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
  }
  free(buf);

  speedLeft = atoi(_speed_left);
  speedRight = atoi(_speed_right);
  dirLeft = _dir_left[0] != '0';
  dirRight = _dir_right[0] != '0';
  moving = true;
  time_last_move_cmd = millis();

  ESP_LOGW(TAG, "Set navi sl %d, sr %d, dl %d, dr %d", speedLeft, speedRight, dirLeft, dirRight);

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

esp_err_t NaviControl::handleStopRequest(httpd_req_t *req) {
  speedLeft = speedRight = 0;
  dirLeft = dirRight = false;
  moving = false;
  ESP_LOGW(TAG, "Stop movement");

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

void NaviControl::refresh() {
  watchdog();
  ledcWrite(PWM_CHANNEL_L, speedLeft);
  ledcWrite(PWM_CHANNEL_R, speedRight);
  digitalWrite(PIN_DIR_L, dirLeft);
  if (moving) digitalWrite(PIN_DIR_R, dirRight);
}

void NaviControl::watchdog() {
  long current_time = millis();
  if ( 
      (speedLeft > 0 || speedRight > 0) &&      
      (current_time - time_last_move_cmd) > NAVIGATION_TIMEOUT
    ) {
    speedLeft = speedRight = 0;
    dirLeft = dirRight = false;
    moving = false;
    ESP_LOGW(TAG, "Motion watchdog kicks in");
  }
}