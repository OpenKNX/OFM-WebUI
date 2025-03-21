#pragma once

#include "OpenKNX.h"
#include <esp_http_server.h>

#ifndef WEBUI_BASE_URI
#define WEBUI_BASE_URI "/start"
#endif

struct WebService {
    httpd_uri_t uri;
    std::string name;
    bool isVisible = true;
};

class WebUI : public OpenKNX::Module
{
    bool firstLoop = true;
    httpd_handle_t server = NULL;
    std::vector<WebService> services;

    public:
		void loop(bool configured) override;
		void setup(bool conf) override;
		const std::string name() override;
		const std::string version() override;
        const char * getBaseUri();

        // void addHandler(httpd_uri_t handler);
        void addService(WebService service);
        httpd_handle_t* getHandler();
        static esp_err_t base_handler(httpd_req_t *req);
};


extern WebUI openknxWebUI;