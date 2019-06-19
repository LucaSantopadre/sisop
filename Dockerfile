FROM ubuntu

# Install.
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y git && \
    apt-get install -y vim && \
    apt-get install -y gcc

# Adad files.
COPY . /sisop

# Define default command.
CMD ["bash"]

