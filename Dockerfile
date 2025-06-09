FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        clang \
        clang-tidy \
        clang-format \
        cmake \
        curl \
        git \
        pkg-config \
        zip \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg
RUN cd /vcpkg && \
    ./bootstrap-vcpkg.sh

WORKDIR /vots/build
COPY ./ /vots
RUN rm -r *

RUN cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake

CMD ["make test"]
