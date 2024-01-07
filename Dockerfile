FROM ubuntu

# Create a directory for scripts
RUN mkdir /scripts

# Copy the scripts to the /scripts directory
COPY install_env.sh install_env.sh
COPY install_lib.sh install_lib.sh

# Set permissions
RUN chmod +x install_env.sh
RUN chmod +x install_lib.sh

# Run the install_env.sh script
RUN /bin/bash ./install_env.sh

# Run the install_lib.sh script with default source directory
CMD ["/bin/bash", "./install_lib.sh", "<directory_to_install_lib>", "<source_dir>"]
