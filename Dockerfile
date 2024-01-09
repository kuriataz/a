FROM ubuntu

WORKDIR /app

COPY . .

RUN chmod +x install_env.sh
RUN chmod +x new_lib.sh
RUN /bin/bash ./install_env.sh
