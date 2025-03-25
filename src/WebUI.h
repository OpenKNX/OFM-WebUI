#pragma once

#include "OpenKNX.h"
#include <esp_http_server.h>
#include <esp_err.h>

#ifndef WEBUI_BASE_URI
// CHANGE ALSO IN MINIMIZE.PY
#define WEBUI_BASE_URI "/openknx"
#endif

struct WebHandler {
    httpd_uri_t httpd;
    std::string uri;
    std::string name;
    bool isVisible = true;
};

struct WebPage {
    std::string uri;
    std::string name;
    bool isVisible = true;
    esp_err_t (*handler)(const char *uri, httpd_req_t *r, void *arg);
    void *arg;
};

static const char index_html[] = "<html><head><title>OpenKNX WebUI</title></head><body style='font-family:Arial, sans-serif;margin:20px;background-color:#f4f4f4;'><header style='position:relative;padding:10px 0;'><img src='https://raw.githubusercontent.com/OpenKNX/.github/main/profile/images/weiss.svg' alt='Logo' style='width:150px;height:auto;top:20px;right:20px;position:absolute;'><h1>Startseite</h1></header>";
static const char webui_base_uri[] = WEBUI_BASE_URI;
static const uint8_t webui_base_uri_len = strlen(webui_base_uri);

class WebUI : public OpenKNX::Module
{
    bool firstLoop = true;
    httpd_handle_t server = NULL;
    std::vector<WebHandler> handler;
    std::vector<WebPage> pages;

    

    public:
		void loop(bool configured) override;
		void setup(bool conf) override;
		const std::string name() override;
		const std::string version() override;
        const char * getBaseUri();

        void addHandler(WebHandler handler);
        void addPage(WebPage p);
        httpd_handle_t getHandler();
        static esp_err_t base_handler(httpd_req_t *req);
};

extern WebUI openknxWebUI;