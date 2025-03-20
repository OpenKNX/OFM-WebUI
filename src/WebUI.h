#pragma once

#include "OpenKNX.h"
#include <esp_http_server.h>

#ifndef WEBUI_BASE_URI
#define WEBUI_BASE_URI "/start"
#endif

struct WebService {
    httpd_uri uri;
    String name;
};

class WebUI : public OpenKNX::Module
{
    bool firstLoop = false;
    httpd_handle_t server = NULL;
    std::vector<WebService> services;

    public:
		void loop(bool configured) override;
		void setup(bool conf) override;
		const std::string name() override;
		const std::string version() override;

        void addService(WebService service);

        static esp_err_t base_handler(httpd_req_t *req);


};


extern WebUI openknxWebUI;