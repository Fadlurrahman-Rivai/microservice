FROM php:8.1-apache

# Install PDO + MySQL extension
RUN docker-php-ext-install pdo pdo_mysql \
    && a2enmod rewrite

# Use custom VirtualHost so AllowOverride All is set
COPY apache.conf /etc/apache2/sites-available/000-default.conf

# Copy application source
COPY . /var/www/html/

# Remove apache.conf from web root (not needed there)
RUN rm -f /var/www/html/apache.conf \
    && chown -R www-data:www-data /var/www/html \
    && chmod -R 755 /var/www/html

EXPOSE 80
