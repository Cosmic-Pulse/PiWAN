FROM debian:bullseye-slim AS builder

RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    wget \
    ca-certificates

# Clone BasicStation
WORKDIR /build
RUN git clone https://github.com/lorabasics/basicstation.git
WORKDIR /build/basicstation

ARG BASICSTATION_COMMIT=master
RUN git checkout ${BASICSTATION_COMMIT}

# Build for native Linux
RUN make platform=rpi variant=std ARCH=aarch64-linux-gnu

# Now create a lightweight final image
FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y ca-certificates gpiod

COPY --from=builder /build/basicstation/build-rpi-std/bin/station /usr/local/bin/station
COPY entrypoint.sh /usr/local/bin/entrypoint.sh
COPY station.conf /usr/local/bin/station.conf
COPY gw_reset.sh /usr/local/bin/gw_reset.sh

RUN chmod +x /usr/local/bin/entrypoint.sh

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]