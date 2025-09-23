FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Rome

RUN apt-get update && apt-get install -y \
    apache2 \
    libapache2-mod-php \
    curl \
    nano


RUN a2enmod rewrite

COPY ./apache-conf/000-default.conf /etc/apache2/sites-available/000-default.conf
COPY ./apache-conf/ports.conf /etc/apache2/ports.conf

EXPOSE 80

CMD ["apache2ctl", "-D", "FOREGROUND"]
