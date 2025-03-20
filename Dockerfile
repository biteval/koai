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
    logrotate cron procps lsof

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

# Use PORT environment variable from Render.com
ENV PORT=18080

# Set working directory
WORKDIR /koai

# Copy project files into the container
COPY . .

# Create download directory for Cassandra
RUN mkdir -p /koai/tars

# Download and extract Cassandra
RUN wget -q --show-progress -P /koai/tars https://dlcdn.apache.org/cassandra/5.0.3/apache-cassandra-5.0.3-bin.tar.gz && \
    tar -xzf /koai/tars/apache-cassandra-5.0.3-bin.tar.gz -C /koai

# Configure logging directories
RUN mkdir -p /var/log/koai
RUN touch /var/log/koai/koai-server.log
RUN chmod 777 /var/log/koai/koai-server.log

# Create startup script
RUN cat > /koai/start_app.sh << 'EOF'
#!/bin/bash
set -e

# Initial setup
cd /koai
export CASS_USERNAME="cassandra"
export CASS_PASS="cassandra"

# Listen on the correct port
export PORT=${PORT:-18080}
echo "Server will listen on PORT: $PORT"

# Start Cassandra
echo "Starting Cassandra database..."
/koai/apache-cassandra-5.0.3/bin/cassandra -R &

# Wait for Cassandra to initialize with health check
echo "Waiting for Cassandra to initialize..."
ATTEMPTS=0
MAX_ATTEMPTS=10
while [ $ATTEMPTS -lt $MAX_ATTEMPTS ]; do
    ATTEMPTS=$((ATTEMPTS+1))
    echo "Checking Cassandra status (attempt $ATTEMPTS/$MAX_ATTEMPTS)..."
    
    if /koai/apache-cassandra-5.0.3/bin/cqlsh -u $CASS_USERNAME -p $CASS_PASS -e "DESCRIBE KEYSPACES;" &>/dev/null; then
        echo "Cassandra is ready!"
        break
    fi
    
    if [ $ATTEMPTS -eq $MAX_ATTEMPTS ]; then
        echo "WARNING: Cassandra may not be fully initialized, but continuing..."
    else
        echo "Cassandra not ready yet. Waiting 15 seconds..."
        sleep 15
    fi
done

# Configure Cassandra cluster
echo "Configuring Cassandra cluster..."
/koai/apache-cassandra-5.0.3/bin/cqlsh -u $CASS_USERNAME -p $CASS_PASS -e "UPDATE system.local SET cluster_name = 'Ufc Cluster' WHERE key='local';" || echo "Cluster configuration failed but continuing..."

# Apply custom Cassandra configuration if available
if [ -f "/koai/database/conf/cassandra.yaml" ]; then
    echo "Applying custom Cassandra configuration..."
    cp "/koai/database/conf/cassandra.yaml" "/koai/apache-cassandra-5.0.3/conf/cassandra.yaml"
    
    # Restart Cassandra after configuration change
    echo "Restarting Cassandra with new configuration..."
    /koai/apache-cassandra-5.0.3/bin/nodetool flush || echo "Flush failed but continuing..."
    /koai/apache-cassandra-5.0.3/bin/nodetool drain || echo "Drain failed but continuing..."
    pkill -f CassandraDaemon || echo "No Cassandra process found to kill"
    sleep 5
    /koai/apache-cassandra-5.0.3/bin/cassandra -R &
    sleep 30
fi

# Start the application
echo "Starting the koai-server application..."
cd /koai
./koai >> /var/log/koai/koai-server.log 2>&1 &

# Print confirmation message
echo "Container is now running and services have been started."
echo "Cassandra: 127.0.0.1:9042"
echo "Application: 0.0.0.0:$PORT"

# Keep the container running
tail -f /var/log/koai/koai-server.log
EOF

# Make startup script executable
RUN chmod +x /koai/start_app.sh

# Create build directory and build the application
RUN mkdir -p /koai/build && \
    cd /koai/build && \
    cmake .. && \
    make

# Make configure.sh executable
#RUN chmod +x configure.sh

# Create a non-root user
RUN useradd -m appuser
RUN chown -R appuser:appuser /koai
RUN chown -R appuser:appuser /var/log/koai

# Explicitly expose port for Render.com
EXPOSE 18080

# Switch to the non-root user
USER appuser

# Run the application
CMD ["/koai/start_app.sh"]
