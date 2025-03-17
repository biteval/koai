#!/bin/bash

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check if a specific Java version is installed
java_version_installed() {
    if command_exists java; then
        java_version=$(java -version 2>&1 | head -n 1 | cut -d'"' -f2)
        if [[ $java_version == *"$1"* ]]; then
            return 0
        fi
    fi
    return 1
}

# Function to check if Python 3.11 is installed
python_311_installed() {
    if command_exists python3.11; then
        python_version=$(python3.11 --version 2>&1)
        if [[ $python_version == *"3.11"* ]]; then
            return 0
        fi
    fi
    return 1
}

# Function to wait for Cassandra to start
wait_for_cassandra() {
    echo "Waiting for Cassandra to start..."
    max_attempts=30
    attempt=0
    while [ $attempt -lt $max_attempts ]; do
        if netstat -an | grep "LISTEN" | grep ":9042" > /dev/null; then
            echo " Ok."
            return 0
        fi
        attempt=$((attempt+1))
        echo "Attempt $attempt of $max_attempts. Still waiting..."
        sleep 5
    done
    echo "Cassandra did not start within the expected time frame."
    return 1
}

#update 
echo "Updating package lists..."
sudo apt-get update
# Install required dependencies
echo "Installing required dependencies..."
sudo apt-get install -y cmake nlohmann-json3-dev libuv1-dev libssl-dev zlib1g-dev libxml2-dev libcurl4-openssl-dev libasio-dev net-tools

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CASSANDRA_DIR="$SCRIPT_DIR/apache-cassandra-5.0.3/bin"

# Install OpenJDK 17 if not already installed
echo "Checking for OpenJDK 17..."
if ! java_version_installed "17"; then
    echo "Installing OpenJDK 17..."
    sudo apt-get install -y openjdk-17-jdk
    
    # Set OpenJDK 17 as default
    if [ -x "$(command -v update-alternatives)" ]; then
        java_path=$(update-alternatives --list java | grep "java-17" || echo "")
        if [ -n "$java_path" ]; then
            sudo update-alternatives --set java "$java_path"
        fi
        
        javac_path=$(update-alternatives --list javac | grep "java-17" || echo "")
        if [ -n "$javac_path" ]; then
            sudo update-alternatives --set javac "$javac_path"
        fi
    else
        echo "update-alternatives command not found. Setting JAVA_HOME manually."
        echo 'export JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64' >> ~/.bashrc
        echo 'export PATH=$JAVA_HOME/bin:$PATH' >> ~/.bashrc
        source ~/.bashrc
    fi
    
    echo "OpenJDK 17 installed and set as default."
else
    echo "OpenJDK 17 is already installed."
fi

# Verify Java version
java -version

# Check if Python 3.11 is already installed
if python_311_installed; then
    echo "Python 3.11 is already installed."
    
    # Ensure Python 3.11 is set as default
    if command_exists update-alternatives; then
        sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.11 1 || true
        sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.11 1 || true
        sudo update-alternatives --set python /usr/bin/python3.11 || true
        sudo update-alternatives --set python3 /usr/bin/python3.11 || true
    else
        # Create symbolic links if alternatives system is not available
        sudo ln -sf $(which python3.11) /usr/local/bin/python3 || true
    fi
else
    # Ask user if they want to install Python 3.11.11
    read -t 10 -p "Python 3.11 is not installed. Do you want to install Python 3.11.11 from source? (y/n): " INSTALL_PYTHON
    INSTALL_PYTHON_LOWER=$(echo "$INSTALL_PYTHON" | tr '[:upper:]' '[:lower:]')
    
    if [[ "$INSTALL_PYTHON_LOWER" != *"n"* ]]; then  
        echo "Installing Python 3.11.11..."
        
        # Install build dependencies
        sudo apt-get update
        sudo apt-get install -y build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libsqlite3-dev libreadline-dev libffi-dev curl libbz2-dev wget
        
        # Create temporary directory for Python installation
        mkdir -p ~/python_install
        cd ~/python_install
        
        # Download and extract Python 3.11.11
        wget https://www.python.org/ftp/python/3.11.11/Python-3.11.11.tgz
        tar -xf Python-3.11.11.tgz
        cd Python-3.11.11
        
        # Configure and install Python
        ./configure --enable-optimizations
        make -j $(nproc)
        sudo make altinstall
        
        # Create symbolic links to make it the default Python version
        sudo ln -sf /usr/local/bin/python3.11 /usr/local/bin/python3
        sudo ln -sf /usr/local/bin/pip3.11 /usr/local/bin/pip3
        
        # Add to alternatives system if it exists
        if command_exists update-alternatives; then
            sudo update-alternatives --install /usr/bin/python python /usr/local/bin/python3.11 1
            sudo update-alternatives --install /usr/bin/python3 python3 /usr/local/bin/python3.11 1
            sudo update-alternatives --set python /usr/local/bin/python3.11
            sudo update-alternatives --set python3 /usr/local/bin/python3.11
        fi
        
        # Clean up
        cd ~/
        rm -rf ~/python_install
        
        echo "Python 3.11.11 installed and set as default."
    else
        echo "Skipping Python 3.11.11 installation."
    fi
fi

# Verify Python version
python3 --version
which python3

# Check for nlohmann-json3-dev, which might be named differently
if ! apt-cache show nlohmann-json3-dev > /dev/null 2>&1; then
    echo "nlohmann-json3-dev package not found, trying alternatives..."
    sudo apt-get install -y nlohmann-json-dev || echo "Installing JSON library from alternative sources..."
fi

#download cassandra db

# Check if Cassandra is already installed
if [ -d "$CASSANDRA_DIR" ]; then
    echo "Apache Cassandra 5.0.3 bin folder found at $CASSANDRA_DIR"
else
    # Create download directory
    DOWNLOAD_DIR="${SCRIPT_DIR}/tars"
    mkdir -p "$DOWNLOAD_DIR"

    # Cassandra download URL
    CASSANDRA_URL="https://dlcdn.apache.org/cassandra/5.0.3/apache-cassandra-5.0.3-bin.tar.gz"
    CASSANDRA_FILENAME="apache-cassandra-5.0.3-bin.tar.gz"

    echo "Downloading Apache Cassandra 5.0.3..."
    wget -q --show-progress -P "$DOWNLOAD_DIR" "$CASSANDRA_URL"

    if [ $? -ne 0 ]; then
        echo "Download failed. Please check your internet connection and try again."
        exit 1
    fi

    echo "Download completed. Extracting to $SCRIPT_DIR..."
    tar -xzf "$DOWNLOAD_DIR/$CASSANDRA_FILENAME" -C "$SCRIPT_DIR"

    if [ $? -ne 0 ]; then
        echo "Extraction failed."
        exit 1
    fi

    echo "Ok."
fi


# Check for Cassandra in the script directory
if [ -d "$CASSANDRA_DIR" ]; then
    echo "Starting Cassandra..."
    cd "$CASSANDRA_DIR" && ./cassandra -f &
    cassandra_pid=$!

    # Wait for Cassandra to start
    wait_for_cassandra

    # Connect to Cassandra and change cluster name
    echo "changing cluster name to Ufc Cluster..."
    cd "$CASSANDRA_DIR"
    ./cqlsh -u cassandra -p cassandra -e "UPDATE system.local SET cluster_name = 'Ufc Cluster' WHERE key='local';"

    # Replace Cassandra configuration file
    if [ -f "$SCRIPT_DIR/database/conf/cassandra.yaml" ]; then
        echo "Replacing cassandra.yaml configuration file..."
        cp "$SCRIPT_DIR/database/conf/cassandra.yaml" "$SCRIPT_DIR/apache-cassandra-5.0.3/conf/cassandra.yaml"
    else
        echo "Warning: Source configuration file '$SCRIPT_DIR/database/conf/cassandra.yaml' not found."
    fi

    # Run nodetool flush
    echo "Running nodetool flush..."
    cd "$CASSANDRA_DIR" && ./nodetool flush
else
    echo "Error: Cassandra directory not found at: $CASSANDRA_DIR"
    echo "Please ensure the apache-cassandra-5.0.3 directory is in the same location as this script."
    exit 1
fi

# Create the build directory
BUILD_DIR="${SCRIPT_DIR}/build"
echo "Creating build directory ..."
mkdir -p "${BUILD_DIR}"

# Navigate to the build directory
cd "${BUILD_DIR}" || { echo "Failed to change directory to ${BUILD_DIR}"; exit 1; }

# Run CMake
cmake .. || { echo "CMake configuration failed"; exit 1; }

# Run Make
make || { echo "Build failed"; exit 1; }

#Export cassandra default username and password
export CASS_USERNAME="cassandra" && export CASS_PASS="cassandra"

# choose Start koai server
read -t 10 -p "Start koAi server? (y/n): " response
response_lower=$(echo "$response" | tr '[:upper:]' '[:lower:]')

# Check if the response is NOT "n"
if [[ "$response_lower" != *"n"* ]]; then
    clear
    echo "Starting koai server..."
    cd "$SCRIPT_DIR" && ./koai
else
    echo "Run ./koai to start koai server."
fi
