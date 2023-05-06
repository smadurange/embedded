#include <stdlib.h>
#include <string.h>

#include "esp_log.h"

#include "http.h"
#include "servo.h"

#define PWM_DUTY_CYCLE_LEN 5
#define PWM_DUTY_CYCLE_MIN 500
#define PWM_DUTY_CYCLE_MAX 2500

static const char *TAG = "http";

static esp_err_t http_res_err_pwmdc(httpd_req_t *req)
{
	const char *msg = "Bad duty cycle\r\n";
	httpd_resp_set_status(req, "400 Bad Request");
	httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
	return httpd_resp_send(req, msg, strlen(msg));
}

esp_err_t http_servo_post_cb(httpd_req_t *req)
{
	int rc, dc;
	char buf[PWM_DUTY_CYCLE_LEN];

	if (req->content_len > PWM_DUTY_CYCLE_LEN - 1)
		return http_res_err_pwmdc(req);

	rc = httpd_req_recv(req, buf, PWM_DUTY_CYCLE_LEN - 1);
	buf[PWM_DUTY_CYCLE_LEN - 1] = 0;
	if (rc == HTTPD_SOCK_ERR_TIMEOUT)
		return httpd_resp_send_408(req);

	dc = atoi(buf);
	if (dc < PWM_DUTY_CYCLE_MIN || dc > PWM_DUTY_CYCLE_MAX)
		return http_res_err_pwmdc(req);
	
	servo_pwmdc_set(dc);	

	return httpd_resp_send_chunk(req, 0, 0);
}

httpd_uri_t servo = {
	.uri = "/servo",
	.method = HTTP_POST,
	.handler = http_servo_post_cb,
	.user_ctx = 0
};

httpd_handle_t http_serv_init(void)
{
	httpd_handle_t server = 0;
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();

	if (httpd_start(&server, &config) == ESP_OK) {
		httpd_register_uri_handler(server, &servo);
		ESP_LOGI(TAG, "started server on port: '%d'", config.server_port);
		return server;
	}
	ESP_LOGI(TAG, "failed to start server");
	return 0;
}
