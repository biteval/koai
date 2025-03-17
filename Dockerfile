FROM debian:latest

# Install dependencies
RUN apt-get update && apt-get install -y sudo build-essential  ncurses-bin

# Set working directory
WORKDIR /app

# Copy project files into the container
COPY . .


ENV TERM=xterm-256color
ENV CASS_USERNAME="cassandra"
ENV CASS_PASS="cassandra"


# Create a non-root user
RUN useradd -m appuser && \
    echo "appuser ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Set permissions for the user
RUN chown -R appuser:appuser /app

# Switch to the non-root user
USER appuser

# Make configure.sh executable
RUN chmod +x configure.sh

# Expose the Crow server port (adjust if needed)
EXPOSE 18080

# Run the configure script as the non-root user
CMD ["./configure.sh"]
