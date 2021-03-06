FROM hhvm/hhvm-proxygen:latest

RUN apt-get update -y && apt-get install -y curl

# Install sqlite3
RUN apt-get install -y sqlite3 libsqlite3-dev

# Install composer
RUN mkdir /opt/composer
RUN curl -sS https://getcomposer.org/installer | hhvm --php -- --install-dir=/opt/composer

# Install the app
RUN rm -rf /var/www
ADD . /var/www
RUN cd /var/www && hhvm /opt/composer/composer.phar install
RUN cd /var/www && hhvm /opt/composer/composer.phar require hhvm/hhvm-autoload

# Create db
RUN mkdir /var/db
RUN cd /var/db && sqlite3 -init /var/www/createdb.sql db.sqlite

# Reconfigure HHVM
# ADD hhvm.prod.ini /etc/hhvm/site.ini

EXPOSE 80
