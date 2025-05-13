FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
        git \
        clang \
        clang-tidy \
        clang-format \
        cmake \
        build-essential \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /vots/build
COPY . /vots
RUN rm -r *

RUN cmake ..

CMD ["make test"]
