# Use the latest Debian image
FROM debian:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    sudo build-essential rsyslog

# Set working directory
WORKDIR /koai

# Copy project files into the container
COPY . .

# Set environment variables
ENV TERM=xterm-256color
ENV CASS_USERNAME="cassandra"
ENV CASS_PASS="cassandra"

# Create a non-root user
RUN useradd -m appuser && \
    echo "appuser ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Configure rsyslog to log to a file
RUN echo "if \$programname == 'koai-server' then /var/log/koai-server.log" >> /etc/rsyslog.d/koai.conf && \
    echo '& stop' >> /etc/rsyslog.d/koai.conf

# Disable imklog module to avoid /proc/kmsg error
RUN sed -i 's/^module(load="imklog"*/#module(load="imklog"/' /etc/rsyslog.conf

# (Optional) Disable immark to clean up config
RUN sed -i 's/^#\(\$ModLoad immark\)/#\1/' /etc/rsyslog.conf

# Set up log rotation
RUN cat <<EOF > /etc/logrotate.d/koai
/var/log/koai-server.log {
    rotate 3
    size 10M
    missingok
    notifempty
    compress
    delaycompress
    copytruncate
}
EOF

RUN echo "0 0 * * * find /var/log/koai-server.log* -mtime +7 -exec rm {} \;" | crontab -

# Create a wrapper script to start rsyslog and the app
RUN echo '#!/bin/bash' > conf_wrapper.sh && \
    echo 'rsyslogd -n &' >> conf_wrapper.sh && \
    echo './configure.sh 2>&1 | logger -t koai-server' >> conf_wrapper.sh && \
    chmod +x conf_wrapper.sh

# Set permissions
RUN chown -R appuser:appuser /koai
RUN echo "appuser ALL=(ALL) NOPASSWD: /usr/sbin/service rsyslog start" >> /etc/sudoers.d/appuser

# Switch to the non-root user
USER appuser

# Make configure.sh executable
RUN chmod +x configure.sh

# Expose the Crow server port (adjust if needed)
EXPOSE 18080

# Run the wrapper script
CMD ["./conf_wrapper.sh"]
