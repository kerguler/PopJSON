#!/usr/bin/env bash
#
# Population + PopJSON Installer Script
#
# DISCLAIMER:
# This script is provided "AS IS", without warranty of any kind.
# Use at your own risk. The authors assume no responsibility for
# any issues, damages, or losses resulting from its use.
#

set -euo pipefail

# Defaults
INSTALLER_VERSION="0.0.1"
SPOP_VERSION=""
TMPDIR="/tmp/population"
PREFIX=""
VERBOSE=0
LOCAL=0
KEEP_TESTS=0
REPO_URL="https://api.github.com/repos/kerguler/Population/releases/latest"

download_file() {
    url="$1"
    dest="$2"

    if command -v curl >/dev/null 2>&1; then
        log "Trying curl..."
        if curl -L -o "$dest" "$url"; then
            return 0
        fi
    fi

    if command -v wget >/dev/null 2>&1; then
        log "Trying wget..."
        if wget --quiet -O "$dest" "$url"; then
            return 0
        fi
    fi

    echo "[ERROR] Neither curl nor wget could fetch $url" >&2
    exit 1
}

# Usage info
usage() {
    echo "Population Installer Script v$INSTALLER_VERSION"
    echo
    echo "Usage: $0 [-d TMPDIR] [--prefix PREFIX] [--local] [--verbose] [--keep-tests]"
    echo
    echo "  -d TMPDIR        Temporary working directory (default: /tmp/population)"
    echo "  --prefix PREFIX  Installation prefix for configure (default: /usr/local)"
    echo "  --local          Install locally (in \$HOME/.local) instead of system-wide"
    echo "  --verbose        Print detailed process information"
    echo "  --keep-tests     Do not delete temporary test files (default: delete)"
    echo "  -h, --help       Show this help and exit"
    echo "  --version        Print installer version and exit"
    exit 1
}

# Error handler
trap 'echo "[ERROR] Script failed at line $LINENO with exit code $?." >&2' ERR

# Argument parsing
while [[ $# -gt 0 ]]; do
    case "$1" in
        -d|--tmpdir)
            TMPDIR="$2"
            shift 2
            ;;
        --prefix)
            PREFIX="$2"
            shift 2
            ;;
        --local)
            LOCAL=1
            shift
            ;;
        --keep-tests)
            KEEP_TESTS=1
            shift
            ;;
        --verbose)
            VERBOSE=1
            shift
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown argument: $1"
            usage
            ;;
    esac
done

# Ensure temp directory
mkdir -p "$TMPDIR"

TESTFILE="$TMPDIR/test"
LOGFILE="$TMPDIR/install.log"

# Verbose logger
log() {
    local msg="[INFO] $1"
    # Always write to logfile
    echo "$msg" >> "$LOGFILE"
    # Also print to stdout if verbose
    if [[ $VERBOSE -eq 1 ]]; then
        echo "$msg"
    fi
}
log "Population + PopJSON Installer Script version $INSTALLER_VERSION"
log "Using temporary directory: $TMPDIR"

# Check privileges unless local install
if [[ $LOCAL -eq 0 ]]; then
    if [[ $EUID -ne 0 ]]; then
        echo "[ERROR] System-wide installation requires root privileges."
        echo "Re-run with sudo, or use --local for user-only installation."
        exit 1
    fi
fi

# ---- Installation steps ----

log "Installing popjson..."
if [[ $LOCAL -eq 1 ]]; then
    npm install --prefix "$HOME/.local" popjson
    NPM_PREFIX_DIR="$HOME/.local"
else
    npm install -g popjson
    NPM_PREFIX_DIR=$(npm prefix -g)
fi

log "Exporting wrappers to POPJSON_WPATH"
WRAPPER_EXPORT="export POPJSON_WPATH=${NPM_PREFIX_DIR}/lib/node_modules/popjson/wrappers"

if [[ $LOCAL -eq 1 ]]; then
    echo "$WRAPPER_EXPORT" >> "$HOME/.bashrc"
    log "Added POPJSON_WPATH to $HOME/.bashrc"
else
    echo "$WRAPPER_EXPORT" >> /etc/bash.bashrc
    log "Added POPJSON_WPATH to /etc/bash.bashrc"
fi

# Get version from popjson
SPOP_VERSION=$(popjson --version | awk -F= '/^Population=/{print $2}')
log "Population package version: $SPOP_VERSION"

# Construct tarball URL
TARBALL_URL="https://raw.githubusercontent.com/kerguler/Population/main/dist/population-${SPOP_VERSION}.tar.gz"
log "Using version $SPOP_VERSION from $TARBALL_URL"

# Download tarball
TARBALL="$TMPDIR/spop-${SPOP_VERSION}.tar.gz"
log "Downloading tarball..."
download_file "$TARBALL_URL" "$TARBALL"

# Extract tarball
log "Extracting package..."
tar -xzf "$TARBALL" -C "$TMPDIR"

# Detect extracted dir
SRCDIR=$(tar -tzf "$TARBALL" | sed -n '1{s#/.*##;p}')
SRCDIR="$TMPDIR/$SRCDIR"
cd "$SRCDIR"

# Configure
if [[ $LOCAL -eq 1 ]]; then
    if [[ -z "$PREFIX" ]]; then
        PREFIX="$HOME/.local"
    fi
    log "Configuring with prefix=$PREFIX (local install)..."
    ./configure --prefix="$PREFIX"
elif [[ -n "$PREFIX" ]]; then
    log "Configuring with prefix=$PREFIX..."
    ./configure --prefix="$PREFIX"
else
    log "Configuring with default prefix (/usr/local)..."
    ./configure
fi

# Build & install
log "Building..."
make -j"$(nproc)"

log "Installing..."
make install

log "Updating linker..."
ldconfig -v

log "Installation of sPop $SPOP_VERSION completed successfully."
if [[ $LOCAL -eq 1 ]]; then
    log "Installed into user-local: $HOME/.local"
elif [[ -n "$PREFIX" ]]; then
    log "Installed into: $PREFIX"
else
    log "Installed into default: /usr/local"
fi

# Quick test
log "Testing installation..."

set +e  # allow test failures without aborting script

# --- Test PopJSON with a simple model ---
cat > "$TESTFILE.json" <<'EOF'
{
    "model": {
        "title": "Dynamically-structured population model",
        "type": "Population",
        "url": "https://github.com/kerguler/Population",
        "deterministic": false,
        "parameters": {
            "algorithm": "Population",
            "istep": 1e-13
        }
    },
    "populations": [
        {
            "id": "larva",
            "name": "The larva stage",
            "processes": [
                {
                    "id": "larva_dev",
                    "name": "Larva development time",
                    "arbiter": "ACC_ERLANG",
                    "value": [10.0, 5.0]
                }
            ]
        }
    ]
}
EOF

if popjson --verbose "$TESTFILE"; then
    log "PopJSON test passed. Skipping manual C test."
else
    echo "[WARN] PopJSON test failed. Trying manual C test..." >&2

    cat > "$TESTFILE.c" <<'EOF'
#include "population.h"

int main() {
    spop2_random_init();

    char arbiters[2] = {ACC_ERLANG, STOP};
    population pop = spop2_init(arbiters, STOCHASTIC);

    number key[1] = {numZERO};
    number num = {.i=10};
    spop2_add(pop, key, num);

    number sz, cm;
    double par[2] = {20.0, 10.0};
    spop2_step(pop, par, &sz, &cm, 0);

    spop2_free(&pop);
    spop2_random_destroy();
    return 0;
}
EOF
fi

if gcc "$TESTFILE.c" $(pkg-config --cflags --libs population) -o "$TESTFILE" >> "$LOGFILE" 2>&1; then
    if "$TESTFILE" >> "$LOGFILE" 2>&1; then
        log "Population C library test passed."
    else
        echo "[WARN] Compiled test program failed to run." | tee -a "$LOGFILE"
    fi
else
    echo "[WARN] Failed to compile test program." | tee -a "$LOGFILE"
fi

set -e  # restore strict error handling

# Cleanup unless user asked to keep tests
if [[ "${KEEP_TESTS}" -eq 0 ]]; then
    rm -rf "$TMPDIR"
    log "Cleaned up temporary files."
else
    log "Kept temporary files in $TMPDIR for debugging."
fi