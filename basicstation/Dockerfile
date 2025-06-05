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
ARG BASICSTATION_PLATFORM=rpi
RUN make platform=${BASICSTATION_PLATFORM} variant=std ARCH=aarch64-linux-gnu deps s-clean s-all

# Now create a lightweight final image
FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y ca-certificates gpiod

ARG BASICSTATION_PLATFORM=rpi
# Select the correct station.conf at build time
COPY station.conf /usr/local/bin/station.conf.rpi
COPY station_corecell.conf /usr/local/bin/station.conf.corecell
RUN if [ "$BASICSTATION_PLATFORM" = "corecell" ]; then \
    cp /usr/local/bin/station.conf.corecell /usr/local/bin/station.conf; \
  else \
    cp /usr/local/bin/station.conf.rpi /usr/local/bin/station.conf; \
  fi

COPY --from=builder /build/basicstation/build-${BASICSTATION_PLATFORM}-std/bin/station /usr/local/bin/station
COPY entrypoint.sh /usr/local/bin/entrypoint.sh
COPY gw_reset.sh /usr/local/bin/gw_reset.sh

RUN chmod +x /usr/local/bin/entrypoint.sh

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
