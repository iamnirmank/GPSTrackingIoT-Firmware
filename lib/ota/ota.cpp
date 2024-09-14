#include "ota.h"
#include "mqtt.h"
#include "gprs.h"
#include "esp_ota_ops.h"
#include "esp_https_ota.h"

/**
 * For handling the actual firmware download from ota server
 * It is needed because the whole firmware data cannot be stored in buffer as its size maybe up to couple of MBs.
 */
// static esp_err_t ota_fimmware_http_event_handler(esp_http_client_event_t *evt)
// {
//     switch (evt->event_id)
//     {
//     case HTTP_EVENT_ERROR:
//         ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
//         break;
//     case HTTP_EVENT_ON_CONNECTED:
//         ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
//         break;
//     case HTTP_EVENT_HEADER_SENT:
//         ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
//         break;
//     case HTTP_EVENT_ON_HEADER:
//         ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
//         break;
//     case HTTP_EVENT_ON_DATA:
//         ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
//         break;
//     case HTTP_EVENT_ON_FINISH:
//         ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
//         break;
//     case HTTP_EVENT_DISCONNECTED:
//         ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
//         break;
//     }
//     return ESP_OK;
// }

// esp_err_t start_ota(const char *firmware_url)
// {
//     ESP_LOGI(TAG, "Starting OTA upgrade task");

//     esp_http_client_config_t config = {
//         .url = firmware_url, // get from manifest
//         .method = HTTP_METHOD_GET,
// #if USE_CA_CERTIFICATE_BUNDLE
//         .crt_bundle_attach = esp_crt_bundle_attach,
// #else
//         .cert_pem = (char *)server_cert_pem_start, // use in cmake
// #endif /* x */
//         .event_handler = ota_fimmware_http_event_handler,
//         .keep_alive_enable = true,
//         .common_name = SERVER_COMMON_NAME,
//     };

//     esp_https_ota_config_t ota_config = {
//         .http_config = &config,
//     };
//     ESP_LOGI(TAG, "Attempting to download update from %s", config.url);

//     return esp_https_ota(&ota_config);
// }

void OTA::handleOta()
{
    ;
    // if (MQTT::GET().mqtt.connected())
    // {
    //     MQTT::GET().mqtt.disconnect();
    // }
    // start_ota("https://api.omnecal.com/ota/v1/download/file/abc.bin");
}
