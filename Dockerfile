# Start with Ubuntu as the base image
FROM ubuntu:22.04

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Set working directory
WORKDIR /app

# Copy the entire project into the container
COPY . .

# Install required dependencies
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
    curl \
    libncurses5-dev \
    libgdbm-dev \
    libnss3-dev \
    libsqlite3-dev \
    libreadline-dev \
    libffi-dev \
    libbz2-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install Python 3.11
RUN mkdir -p /tmp/python_install \
    && cd /tmp/python_install \
    && wget https://www.python.org/ftp/python/3.11.11/Python-3.11.11.tgz \
    && tar -xf Python-3.11.11.tgz \
    && cd Python-3.11.11 \
    && ./configure --enable-optimizations \
    && make -j $(nproc) \
    && make altinstall \
    && ln -sf /usr/local/bin/python3.11 /usr/local/bin/python3 \
    && ln -sf /usr/local/bin/pip3.11 /usr/local/bin/pip3 \
    && cd / \
    && rm -rf /tmp/python_install

# Download and set up Cassandra
RUN mkdir -p /app/tars \
    && wget -q -P /app/tars https://dlcdn.apache.org/cassandra/5.0.3/apache-cassandra-5.0.3-bin.tar.gz \
    && tar -xzf /app/tars/apache-cassandra-5.0.3-bin.tar.gz -C /app \
    && rm -rf /app/tars

# Set up Cassandra configuration if it exists
RUN if [ -f "/app/database/conf/cassandra.yaml" ]; then \
    cp /app/database/conf/cassandra.yaml /app/apache-cassandra-5.0.3/conf/cassandra.yaml; \
    fi

# Create build directory and build the project
RUN mkdir -p /app/build \
    && cd /app/build \
    && cmake .. \
    && make

# Set environment variables for Cassandra
ENV CASS_USERNAME=cassandra
ENV CASS_PASS=cassandra

# Define the PORT environment variable with a default value
# This will be overridden by the cloud platform's assigned port
ENV PORT=8080

# Expose necessary ports
# Port 9042 for Cassandra
# PORT for the KoAi application (will be dynamically assigned by the cloud platform)
EXPOSE 9042 ${PORT}

# Create a startup script that uses the PORT environment variable
RUN echo '#!/bin/bash\n\
# Ensure the PORT environment variable is set\n\
export PORT=${PORT:-8080}\n\
echo "Application will listen on port: $PORT"\n\
\n\
cd /app/apache-cassandra-5.0.3/bin && ./cassandra -R &\n\
echo "Waiting for Cassandra to start..."\n\
max_attempts=30\n\
attempt=0\n\
while [ $attempt -lt $max_attempts ]; do\n\
    if netstat -an | grep "LISTEN" | grep ":9042" > /dev/null; then\n\
        echo "Cassandra started successfully."\n\
        break\n\
    fi\n\
    attempt=$((attempt+1))\n\
    echo "Attempt $attempt of $max_attempts. Still waiting..."\n\
    sleep 5\n\
done\n\
\n\
if [ $attempt -eq $max_attempts ]; then\n\
    echo "Cassandra did not start within the expected time frame."\n\
    exit 1\n\
fi\n\
\n\
# Change cluster name\n\
cd /app/apache-cassandra-5.0.3/bin\n\
./cqlsh -u cassandra -p cassandra -e "UPDATE system.local SET cluster_name = '\''Ufc Cluster'\'' WHERE key='\''local'\';"\n\
\n\
# Run nodetool flush\n\
./nodetool flush\n\
\n\
# Start the KoAi application\n\
# Pass the PORT environment variable to your application if it accepts it as a command-line parameter\n\
# If your application uses a configuration file for the port, you may need to modify it here\n\
cd /app\n\
# Assuming your application can take a port flag; modify as needed for your specific application\n\
./koai\n' > /app/startup.sh \
    && chmod +x /app/startup.sh

# Set the entrypoint
ENTRYPOINT ["/app/startup.sh"]
