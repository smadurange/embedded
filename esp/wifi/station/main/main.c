#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "wifi.h"
#include "http.h"
#include "servo.h"

static httpd_handle_t server = 0;

void app_main()
{
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	servo_init();
	wifi_sta_init();
	server = http_serv_init();
}

