# Maith devcontainer — persistent Lean 4 + Mathlib environment.
#
# Protocol ported from PleaNP (2026-09-15). Persistence comes from this image
# plus the community Mathlib olean cache (`lake exe cache get`), NOT from any
# live machine. Building Mathlib from source never happens here.
#
# The toolchain is pinned by lean-toolchain (v4.31.0); Mathlib by
# lakefile.lean (@ "v4.31.0"). Bump both together.

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=C.UTF-8

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates curl git python3 && \
    rm -rf /var/lib/apt/lists/*

# elan (Lean version manager) — matches repo CI exactly.
RUN curl -sSf https://raw.githubusercontent.com/leanprover/elan/master/elan-init.sh \
      | sh -s -- -y --default-toolchain none && \
    /root/.elan/bin/elan --version

ENV PATH="/root/.elan/bin:${PATH}"

# Vanilla image: the repo is mounted at /workspaces/Maith by the devcontainer
# framework; `lake exe cache get` + `lake build` run in postCreateCommand (see
# devcontainer.json) so this image stays small and cache/distro-agnostic.