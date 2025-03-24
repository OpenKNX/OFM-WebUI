#pragma once

#include "OpenKNX.h"
#include <esp_http_server.h>

#ifndef WEBUI_BASE_URI
#define WEBUI_BASE_URI "/start"
#endif

struct WebService {
    httpd_uri_t httpd;
    std::string uri;
    std::string name;
    bool isVisible = true;
};

static const char index_html[] = "<html><head><title>OpenKNX WebUI</title></head><body style='font-family:Arial, sans-serif;margin:20px;background-color:#f4f4f4;'><header style='position:relative;padding:10px 0;'><img src='https://raw.githubusercontent.com/OpenKNX/.github/main/profile/images/weiss.svg' alt='Logo' style='width:150px;height:auto;top:20px;right:20px;position:absolute;'><h1>Startseite</h1></header>";

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

        void addService(WebService service);
        httpd_handle_t getHandler();
        static esp_err_t base_handler(httpd_req_t *req);
};

extern WebUI openknxWebUI;