#include "WebUI.h"

#include "versions.h"

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
                .uri = (std::string(webui_base_uri) + "*").c_str(),
                .method = HTTP_GET,
                .handler = base_handler,
                .user_ctx = this
            };
            httpd_register_uri_handler(server, &baseUri);

            for(auto &handle : handler)
            {
                logDebugP("URI handler for %s at %s", handle.name.c_str(), handle.uri);
                httpd_register_uri_handler(server, &handle.httpd);
            }

            for(auto &page : pages)
            {
                logDebugP("URI page for %s at %s", page.name.c_str(), page.uri);
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

void WebUI::addHandler(WebHandler h)
{
    handler.push_back(h);
}

void WebUI::addPage(WebPage p)
{
    pages.push_back(p);
}

httpd_handle_t WebUI::getHandler()
{
    return server;
}

const char * WebUI::getBaseUri()
{
    return webui_base_uri;
}

esp_err_t WebUI::base_handler(httpd_req_t *req)
{
    WebUI *ui = (WebUI *)req->user_ctx;

    if(strcmp(req->uri, WEBUI_BASE_URI) == 0)
    {
        std::string response = index_html;
        
        response += "<h3>Web-Services:</h3><ul>";
        for(auto &handle : ui->handler)
        {
            if(handle.isVisible == false)
                continue;
            response += "<a href=\"";
            response += handle.uri;
            response += "\">";
            response += handle.name;
            response += "</a><br>";
        }
        for(auto &page : ui->pages)
        {
            if(page.isVisible == false)
                continue;
            response += "<a href=\"";
            response += webui_base_uri;
            response += page.uri;
            response += "\">";
            response += page.name;
            response += "</a><br>";
        }
        
        response += "</ul><h3>Verwendete Module:</h3><ul>";

        #ifdef KNX_Version
        response += "<li>KNX - ";
        response += KNX_Version;
        response += "</li>";
        #endif
        #ifdef MODULE_Common_Version
        response += "<li>Common - ";
        response += MODULE_Common_Version;
        response += "</li>";
        #endif

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
        for(auto &page : ui->pages)
        {
            if(strncmp(req->uri + webui_base_uri_len, page.uri.c_str(), page.uri.length()) == 0)
            {
                return page.handler(req->uri + webui_base_uri_len, req, page.arg);
            }
        }
    }

    httpd_resp_send_404(req);
    return ESP_OK;
}

WebUI openknxWebUI;