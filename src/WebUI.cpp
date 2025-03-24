#include "WebUI.h"


void  WebUI::setup(bool conf)
{

}

void WebUI::loop(bool configured)
{
    if(firstLoop)
    {
        firstLoop = false;
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
        config.uri_match_fn = httpd_uri_match_wildcard;

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
                logDebugP("URI handler for %s at %s", service.name.c_str(), service.uri.uri);
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
    return MODULE_WebUI_Version;
}

void WebUI::addService(WebService service)
{
    services.push_back(service);
}

httpd_handle_t WebUI::getHandler()
{
    return server;
}

const char * WebUI::getBaseUri()
{
    return WEBUI_BASE_URI;
}

esp_err_t WebUI::base_handler(httpd_req_t *req)
{
    WebUI *ui = (WebUI *)req->user_ctx;

    if(strcmp(req->uri, WEBUI_BASE_URI) == 0)
    {
        std::string response = index_html;
        
        response += "<h3>Web-Services:</h3><ul>";
        for(auto &service : ui->services)
        {
            if(service.isVisible == false)
                continue;
            response += "<a href=\"";
            response += service.uri.uri;
            response += "\">";
            response += service.name;
            response += "</a><br>";
        }
        
        response += "</ul><h3>Verwendete Module:</h3><ul>";
        for(auto &module : openknx.modules.list)
        {
            if(module == nullptr)
                continue;
            response += "<li>";
            response += module->name();
            response += " - ";
            response += module->version();
            response += "</li>";
        }
        response += "</ul></body></html>";
        
        httpd_resp_send(req, response.c_str(), HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
    else if(strcmp(req->uri, "/") == 0)
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
        return ESP_OK;
    }
}

WebUI openknxWebUI;