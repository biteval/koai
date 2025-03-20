# Use the latest Debian image
FROM debian:latest

# Install dependencies including wget, tar, OpenJDK 17, and net-tools
RUN apt-get update && apt-get install -y \
    sudo build-essential wget tar \
    ca-certificates gnupg software-properties-common \
    openjdk-17-jdk \
    net-tools \
    cmake nlohmann-json3-dev libuv1-dev libssl-dev zlib1g-dev \
    libxml2-dev libcurl4-openssl-dev libasio-dev \
    logrotate cron

# Set Java 17 as default
ENV JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
ENV PATH=$PATH:$JAVA_HOME/bin

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

# Set environment variables
ENV TERM=xterm-256color
ENV CASS_USERNAME="cassandra"
ENV CASS_PASS="cassandra"

# Set working directory
WORKDIR /koai

# Copy project files into the container
COPY . .

# Create download directory for Cassandra
RUN mkdir -p /koai/tars

# Download and extract Cassandra with reduced memory settings
RUN wget -q --show-progress -P /koai/tars https://dlcdn.apache.org/cassandra/5.0.3/apache-cassandra-5.0.3-bin.tar.gz && \
    tar -xzf /koai/tars/apache-cassandra-5.0.3-bin.tar.gz -C /koai

# Configure Cassandra for low memory usage
RUN echo 'MAX_HEAP_SIZE="256M"' > /koai/apache-cassandra-5.0.3/conf/cassandra-env.sh && \
    echo 'HEAP_NEWSIZE="64M"' >> /koai/apache-cassandra-5.0.3/conf/cassandra-env.sh && \
    cat /koai/apache-cassandra-5.0.3/conf/cassandra-env.sh.orig >> /koai/apache-cassandra-5.0.3/conf/cassandra-env.sh

# Configure direct file logging
RUN mkdir -p /var/log/koai
RUN touch /var/log/koai/koai-server.log
RUN chmod 777 /var/log/koai/koai-server.log

# Create modified startup script
RUN echo '#!/bin/bash' > start_app.sh && \
    echo 'cd /koai' >> start_app.sh && \
    echo 'export CASS_USERNAME="cassandra"' >> start_app.sh && \
    echo 'export CASS_PASS="cassandra"' >> start_app.sh && \
    echo '/koai/apache-cassandra-5.0.3/bin/cassandra -R &' >> start_app.sh && \
    echo 'sleep 60' >> start_app.sh && \
    echo '/koai/apache-cassandra-5.0.3/bin/cqlsh -u cassandra -p cassandra -e "UPDATE system.local SET cluster_name = '\''Ufc Cluster'\'' WHERE key='\''local'\'';" || true' >> start_app.sh && \
    echo 'if [ -f "/koai/database/conf/cassandra.yaml" ]; then' >> start_app.sh && \
    echo '  cp "/koai/database/conf/cassandra.yaml" "/koai/apache-cassandra-5.0.3/conf/cassandra.yaml"' >> start_app.sh && \
    echo 'fi' >> start_app.sh && \
    echo '/koai/apache-cassandra-5.0.3/bin/nodetool flush || true' >> start_app.sh && \
    echo 'cd /koai' >> start_app.sh && \
    echo './koai >> /var/log/koai/koai-server.log 2>&1' >> start_app.sh && \
    chmod +x start_app.sh

# Create build directory and build the application
RUN mkdir -p /koai/build && \
    cd /koai/build && \
    cmake .. && \
    make

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
