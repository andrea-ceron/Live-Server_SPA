FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Rome

RUN apt-get update && apt-get install -y \
    apache2 \
    curl \
    nano \
    make \
    g++ \
    build-essential

RUN a2enmod rewrite
RUN a2enmod substitute
RUN a2enmod proxy
RUN a2enmod proxy_http

COPY ./apache/apache-conf/000-default.conf /etc/apache2/sites-available/000-default.conf
COPY ./apache/apache-conf/ports.conf /etc/apache2/ports.conf

COPY ./reloader /app/reloader
WORKDIR /app/reloader

RUN make  \
 && cp dev_reloader /usr/local/bin/dev_reloader \
 && chmod +x /usr/local/bin/dev_reloader

WORKDIR /
COPY start.sh /usr/local/bin/start.sh
RUN chmod +x /usr/local/bin/start.sh

EXPOSE 80

CMD ["/usr/local/bin/start.sh"] 
