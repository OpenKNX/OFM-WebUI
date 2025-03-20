#include "WebUI.h"


void  WebUI::setup(bool conf)
{

}

void WebUI::loop(bool configured)
{
    if(!firstLoop)
    {
        firstLoop = true;
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();

        if (httpd_start(&server, &config) == ESP_OK) {
            // Registering the ws handler
            logDebugP("Registering URI handlers");
            logIndentUp();

            #ifndef WEBUI_BASE_URI_USED
            logDebugP("URI handler for /");
            httpd_uri_t baseUri = {
                .uri = "/",
                .method = HTTP_GET,
                .handler = base_handler,
                .user_ctx = this
            };
            httpd_register_uri_handler(server, &baseUri);
            #endif

            logDebugP("URI handler for %s", WEBUI_BASE_URI);
            httpd_uri_t baseUri = {
                .uri = WEBUI_BASE_URI,
                .method = HTTP_GET,
                .handler = base_handler,
                .user_ctx = this
            };
            httpd_register_uri_handler(server, &baseUri);

            for(auto &service : services)
            {
                logDebugP("URI handler for %s", service.name.c_str());
                httpd_register_uri_handler(server, &service.uri);
            }
            logIndentDown();
        } else {
            logErrorP("Failed to start the server");
        }
    }
}

const std::string  WebUI::name()
{
    return "WebUI";
}

const std::string  WebUI::version()
{
    return "";
}

void WebUI::addService(WebService service)
{
    services.push_back(service);
}

esp_err_t WebUI::base_handler(httpd_req_t *req)
{
    WebUI *ui = (WebUI *)req->user_ctx;

    if(strcmp(req->uri, "/") == 0)
    {
        httpd_resp_set_type(req, "text/html");
        httpd_resp_set_status(req, "301 Moved Permanently");
        httpd_resp_set_hdr(req, "Location", WEBUI_BASE_URI);
        httpd_resp_send(req, NULL, 0);
        return ESP_OK;
    }
    else
    {
        httpd_resp_send_404(req);
    }
}

WebUI openknxWebUI;