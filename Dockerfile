# Use the latest Debian image
FROM debian:latest

# Install dependencies including wget, tar, and Python 3.11
RUN apt-get update && apt-get install -y \
    sudo build-essential wget tar \
    ca-certificates gnupg software-properties-common \
    logrotate cron

# Add Python 3.11 repository and install it
RUN apt-get update && \
    apt-get install -y lsb-release && \
    echo "deb http://deb.debian.org/debian $(lsb_release -cs)-backports main" > /etc/apt/sources.list.d/backports.list && \
    apt-get update && \
    apt-get install -y python3.11 python3.11-venv python3.11-dev python3-pip

# Set Python 3.11 as default
RUN update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.11 1 && \
    update-alternatives --set python3 /usr/bin/python3.11 && \
    ln -sf /usr/bin/python3 /usr/bin/python

# Set working directory
WORKDIR /koai

# Copy project files into the container
COPY . .

# Set environment variables
ENV TERM=xterm-256color
ENV CASS_USERNAME="cassandra"
ENV CASS_PASS="cassandra"

# Configure direct file logging instead of syslog
RUN mkdir -p /var/log/koai
RUN touch /var/log/koai/koai-server.log
RUN chmod 777 /var/log/koai/koai-server.log

# Set up log rotation
RUN cat <<EOF > /etc/logrotate.d/koai
/var/log/koai/koai-server.log {
    rotate 3
    size 10M
    missingok
    notifempty
    compress
    delaycompress
    copytruncate
}
EOF

# Create a wrapper script that logs directly to file
RUN echo '#!/bin/bash' > start_app.sh && \
    echo './configure.sh 2>&1 >> /var/log/koai/koai-server.log' >> start_app.sh && \
    chmod +x start_app.sh

# Make configure.sh executable
RUN chmod +x configure.sh

# Create a non-root user
RUN useradd -m appuser
RUN chown -R appuser:appuser /koai
RUN chown -R appuser:appuser /var/log/koai

# Expose the Crow server port
EXPOSE 18080

# Switch to the non-root user
USER appuser

# Run the application
CMD ["./start_app.sh"]
