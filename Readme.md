## Defines
|Define|Description|
|---|---|
|WEBUI_BASE_URI|Set the base uri. default: /start
|WEBUI_BASE_URI_USED|If set WebUI wont create a redirect from / to /<WEBUI_BASE_URI>|


## Webdateien
Um automatisch html/js/css zu komprimieren und als Headerdatei  
zur Verfügung zu stellen, muss folgendes in der .custom.ini eingetragen werden:
```ini
extra_scripts =
  ${env.extra_scripts}
  lib/OFM-WebUI/scripts/pio/minimize.py
```
Die Dateien müssen im Ordner www sein und werden dann in den include Ordner als  
char array gespeichert.