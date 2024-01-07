FROM ubuntu

# Create a directory for the project
WORKDIR /app

# Copy the source code and scripts to the container
COPY . .

# Install dependencies and build the project
RUN chmod +x install_env.sh
RUN chmod +x install_lib.sh
RUN /bin/bash ./install_env.sh
RUN /bin/bash ./install_lib.sh

# Set the entry point to your application
CMD ["./build/tests"]
