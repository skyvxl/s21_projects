# Dockerfile for Linux C++ dev/test environment
FROM ubuntu:24.04

# Non-interactive for apt
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    g++ \
    cmake \
    clang-format \
    valgrind \
    git \
    make \
    wget \
    libgtest-dev \
    ca-certificates \
    zsh \
    curl \
    fonts-powerline \
    && rm -rf /var/lib/apt/lists/*

# Build GoogleTest (libgtest-dev only provides sources)
# RUN cd /usr/src/gtest && \
#     cmake . && \
#     make && \
#     cp lib/*.a /usr/lib/

# RUN cd /usr/src/gmock && \
#     cmake . && \
#     make && \
#     cp lib/*.a /usr/lib/

RUN apt-get install -y cmake && \
    mkdir -p $HOME/build && \
    cd $HOME/build && \
    cmake /usr/src/googletest/googletest && \
    make && \
    cp lib/libgtest* /usr/lib/ && \
    cd .. && \
    rm -rf build  && \
    mkdir /usr/local/lib/googletest && \
    ln -s /usr/lib/libgtest.a /usr/local/lib/googletest/libgtest.a && \
    ln -s /usr/lib/libgtest_main.a /usr/local/lib/googletest/libgtest_main.a


# Install Oh My Zsh and plugins
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" --unattended && \
    git clone https://github.com/zsh-users/zsh-syntax-highlighting.git /root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting && \
    git clone https://github.com/zsh-users/zsh-autosuggestions /root/.oh-my-zsh/custom/plugins/zsh-autosuggestions && \
    sed -i 's/plugins=(git)/plugins=(git zsh-syntax-highlighting zsh-autosuggestions)/' /root/.zshrc && \
    echo "ZSH_AUTOSUGGEST_HIGHLIGHT_STYLE='fg=cyan'" >> /root/.zshrc && \
    echo "source /root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh" >> /root/.zshrc && \
    echo "source /root/.oh-my-zsh/custom/plugins/zsh-autosuggestions/zsh-autosuggestions.zsh" >> /root/.zshrc

# Set working directory to project root (will be mounted)
WORKDIR /workspace

# Default command: zsh
CMD ["zsh"]
