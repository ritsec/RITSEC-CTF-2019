FROM fauria/lamp

# copy the files over
COPY /www/flag.txt /home/flag.txt
COPY /www/lib.php /var/www/html/lib.php
COPY /www/index.php /var/www/html/index.php
COPY /www/style.css /var/www/html/style.css
COPY /www/photos.php /var/www/html/photos.php
COPY /www/upload.php /var/www/html/upload.php
COPY /www/apache2.conf /etc/apache2/apache2.conf
COPY --chown=www-data /www/uploads /var/www/html/uploads

EXPOSE 8003

CMD /usr/sbin/apache2ctl -D FOREGROUND

