FROM ubuntu

# Install.
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y git 

# Adad files.
COPY . /sisop

# Define default command.
CMD ["bash"]

