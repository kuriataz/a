FROM ubuntu

# Create a directory for the project
WORKDIR /app

# Copy the source code and scripts to the container
COPY . .

# Install dependencies and build the project
RUN chmod +x install_env.sh
RUN chmod +x install_lib.sh
RUN /bin/bash ./install_env.sh
# RUN /bin/bash ./install_lib.sh ./new
# Change the working directory to /app/new
# WORKDIR /app/new
# RUN g++ -o try try.cpp -I./ ./build/liballocator_lib.a
# # RUN g++ -o try try.cpp -I./ -L./build -lallocator_lib


# # Set the entry point to your application
# CMD ["./try"]
