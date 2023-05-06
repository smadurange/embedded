#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"

#include "freertos/event_groups.h"

#include "wifi.h"

#define WIFI_SSID       CONFIG_ESP_WIFI_SSID
#define WIFI_PASS       CONFIG_ESP_WIFI_PASSWORD
#define WIFI_MAX_RETRY  CONFIG_ESP_MAXIMUM_RETRY

#define WIFI_CONNECTED_BIT BIT0

static const char *TAG = "wifi station";
static EventGroupHandle_t wifi_event_group;

static void event_handler(void *arg, esp_event_base_t eb, int32_t id, void *data)
{
	ip_event_got_ip_t *event = (ip_event_got_ip_t *) data;
	ESP_LOGI(TAG, "ip: %s", ip4addr_ntoa(&event->ip_info.ip));
	xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
}

void wifi_sta_init(void)
{
	wifi_event_group = xEventGroupCreate();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = WIFI_SSID,
			.password = WIFI_PASS,
			.threshold.authmode = WIFI_AUTH_WPA2_PSK
		},
	};
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,
		IP_EVENT_STA_GOT_IP, &event_handler, 0));

	ESP_ERROR_CHECK(esp_wifi_start());
	ESP_ERROR_CHECK(esp_wifi_connect());

	xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
}
