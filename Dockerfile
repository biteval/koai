# Start with latest Debian as the base image
FROM debian:latest

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Set working directory
WORKDIR /koai

# Copy the entire project into the container
COPY . .

# Add deadsnakes PPA for Python 3.11
RUN apt-get update && apt-get install -y \
    gnupg \
    software-properties-common \
    curl 
    
# Install dependencies and Python 3.11
RUN apt-get update && apt-get install -y \
    cmake \
    wget \
    sudo \
    build-essential \
    nlohmann-json3-dev \
    libuv1-dev \
    libssl-dev \
    zlib1g-dev \
    libxml2-dev \
    libcurl4-openssl-dev \
    libasio-dev \
    net-tools \
    openjdk-17-jdk \
    tar \
    python3.11 \
    python3.11-dev \
    python3.11-venv \
    python3-pip \
    libncurses5-dev \
    libgdbm-dev \
    libnss3-dev \
    libsqlite3-dev \
    libreadline-dev \
    libffi-dev \
    libbz2-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set Python 3.11 as the default Python version
RUN update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.11 1 && \
    update-alternatives --set python3 /usr/bin/python3.11 && \
    update-alternatives --install /usr/bin/python python /usr/bin/python3.11 1 && \
    update-alternatives --set python /usr/bin/python3.11 && \
    ln -sf /usr/bin/pip3 /usr/bin/pip

# Download and set up Cassandra
RUN mkdir -p /koai/tars \
    && wget -q -P /koai/tars https://dlcdn.apache.org/cassandra/5.0.3/apache-cassandra-5.0.3-bin.tar.gz \
    && tar -xzf /koai/tars/apache-cassandra-5.0.3-bin.tar.gz -C /koai \
    && rm -rf /koai/tars

# Set up Cassandra configuration if it exists
RUN if [ -f "/koai/database/conf/cassandra.yaml" ]; then \
    cp /koai/database/conf/cassandra.yaml /koai/apache-cassandra-5.0.3/conf/cassandra.yaml; \
    fi

# Create build directory and build the project
RUN mkdir -p /koai/build \
    && cd /koai/build \
    && cmake .. \
    && make

# Set environment variables for Cassandra
ENV CASS_USERNAME=cassandra
ENV CASS_PASS=cassandra

# Define the PORT environment variable with a default value
# This will be overridden by the cloud platform's assigned port
ENV PORT=18080

# Expose necessary ports
# Port 9042 for Cassandra
# PORT for the KoAi application (will be dynamically assigned by the cloud platform)
EXPOSE 9042 ${PORT}

# Create startup script file
RUN echo '#!/bin/bash' > /koai/startup.sh && \
    echo '# Ensure the PORT environment variable is set' >> /koai/startup.sh && \
    echo 'export PORT=${PORT:-18080}' >> /koai/startup.sh && \
    echo 'echo "Application will listen on port: $PORT"' >> /koai/startup.sh && \
    echo '' >> /koai/startup.sh && \
    echo 'cd /koai/apache-cassandra-5.0.3/bin && ./cassandra -R &' >> /koai/startup.sh && \
    echo 'echo "Waiting for Cassandra to start..."' >> /koai/startup.sh && \
    echo 'max_attempts=30' >> /koai/startup.sh && \
    echo 'attempt=0' >> /koai/startup.sh && \
    echo 'while [ $attempt -lt $max_attempts ]; do' >> /koai/startup.sh && \
    echo '    if netstat -an | grep "LISTEN" | grep ":9042" > /dev/null; then' >> /koai/startup.sh && \
    echo '        echo "Cassandra started successfully."' >> /koai/startup.sh && \
    echo '        break' >> /koai/startup.sh && \
    echo '    fi' >> /koai/startup.sh && \
    echo '    attempt=$((attempt+1))' >> /koai/startup.sh && \
    echo '    echo "Attempt $attempt of $max_attempts. Still waiting..."' >> /koai/startup.sh && \
    echo '    sleep 5' >> /koai/startup.sh && \
    echo 'done' >> /koai/startup.sh && \
    echo '' >> /koai/startup.sh && \
    echo 'if [ $attempt -eq $max_attempts ]; then' >> /koai/startup.sh && \
    echo '    echo "Cassandra did not start within the expected time frame."' >> /koai/startup.sh && \
    echo '    exit 1' >> /koai/startup.sh && \
    echo 'fi' >> /koai/startup.sh && \
    echo '' >> /koai/startup.sh && \
    echo '# Change cluster name' >> /koai/startup.sh && \
    echo 'cd /koai/apache-cassandra-5.0.3/bin' >> /koai/startup.sh && \
    echo './cqlsh -u $CASS_USERNAME -p $CASS_PASS -e "UPDATE system.local SET cluster_name = '\''Ufc Cluster'\'' WHERE key='\''local'\'';"' >> /koai/startup.sh && \
    echo '' >> /koai/startup.sh && \
    echo '# Run nodetool flush' >> /koai/startup.sh && \
    echo './nodetool flush' >> /koai/startup.sh && \
    echo '' >> /koai/startup.sh && \
    echo '# Start the KoAi application' >> /koai/startup.sh && \
    echo 'cd /koai' >> /koai/startup.sh && \
    echo './koai' >> /koai/startup.sh && \
    chmod +x /koai/startup.sh

# Set the entrypoint
CMD ["/koai/startup.sh"]
