ARG ES_VERSION
FROM elasticsearch:${ES_VERSION}
LABEL authors="takiido"

COPY init-users.sh /usr/local/bin/init-users.sh

USER root

RUN chmod +x /usr/local/bin/init-users.sh

USER elasticsearch

ENTRYPOINT ["/usr/local/bin/init-users.sh"]