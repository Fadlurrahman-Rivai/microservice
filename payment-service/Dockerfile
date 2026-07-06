FROM php:8.1-apache
RUN docker-php-ext-install pdo pdo_mysql \
    && a2enmod rewrite
COPY apache.conf /etc/apache2/sites-available/000-default.conf
COPY . /var/www/html/
RUN rm -f /var/www/html/apache.conf \
    && chown -R www-data:www-data /var/www/html \
    && chmod -R 755 /var/www/html
EXPOSE 80
