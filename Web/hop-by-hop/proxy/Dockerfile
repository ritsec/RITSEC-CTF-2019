from owasp/modsecurity-crs

RUN sed -i 's/Listen 80/Listen 81/g' /usr/local/apache2/conf/httpd.conf

RUN sed -i -e 's/^#\(LoadModule .*mod_proxy.so\)/\1/' -e 's/^#\(LoadModule .*mod_proxy_http.so\)/\1/'  conf/httpd.conf;

RUN printf "include conf/extra/httpd-proxy.conf\n" >> /usr/local/apache2/conf/httpd.conf;

RUN printf "ProxyPass / \${PROXYLOCATION}\nProxyPassReverse / \${PROXYLOCATION}\n" > /usr/local/apache2/conf/extra/httpd-proxy.conf;

RUN sed -i 's/deny/redirect:http:\/blocked/g' /etc/modsecurity.d/owasp-crs/rules/RESPONSE-959-BLOCKING-EVALUATION.conf
RUN sed -i 's/deny/redirect:http:\/blocked/g' /etc/modsecurity.d/owasp-crs/rules/REQUEST-949-BLOCKING-EVALUATION.conf
