#syntax=docker/dockerfile:1.5

ARG DEBIAN_VERSION=bullseye
ARG DEBIAN_VERSION_NUMBER=11
ARG PROJECT=task
ARG GCC_VERSION=11.4

# ====================================================================================================
# Base
FROM gcc:${GCC_VERSION}-${DEBIAN_VERSION} AS base


WORKDIR /app

RUN <<EOT
#!/usr/bin/env bash
set -eu

apt -q update
apt -qy --no-install-recommends install libczmq-dev libzmq3-dev
rm -rf /var/lib/apt/lists/*
EOT

COPY src/ src/
COPY Makefile Makefile

RUN make OPT_LEVEL='-O3 -flto'

# ==================================================
FROM debian:${DEBIAN_VERSION}-slim AS release

WORKDIR /app

RUN <<EOT
#!/usr/bin/env bash
set -eu

apt -q update
apt -qy --no-install-recommends install libczmq-dev libzmq3-dev
rm -rf /var/lib/apt/lists/*
EOT

COPY --from=base /app/build/${PROJECT} ./${PROJECT}

ENTRYPOINT [ "./task" ]
