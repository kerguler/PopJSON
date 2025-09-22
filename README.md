# PopJSON

JSON-based representation of population dynamics models.  
Includes a parser for the [Population](https://github.com/kerguler/Population) model and its `Python` wrapper.

Please check [this page](https://kerguler.github.io/PopJSON/) for usage instructions.  
We are working on the specifications.

---

## Installation

PopJSON has two parts:

1. The **PopJSON package** (Node.js, installed via `npm`).  
2. The **Population C library**, which provides the backend engine.

### Install PopJSON (Node.js package)

```bash
npm install -g popjson
```

This installs the `popjson` CLI globally.

---

### Install Population (C library)

PopJSON depends on the [Population](https://github.com/kerguler/Population) library.  
For convenience, this repository includes an installer script in the root folder:

```bash
./install.population.sh
```

#### System-wide installation (recommended)

```bash
sudo ./install.population.sh
```

This builds and installs Population under `/usr/local` and updates your environment.

#### Local installation (no sudo required)

```bash
./install.population.sh --local
```

This installs Population under `~/.local` and updates your `~/.bashrc`.

#### Options

Run with `--help` for the full list of options:

```bash
./install.population.sh --help
```

Key options:

- `--prefix PREFIX` : custom installation prefix (default: `/usr/local`)  
- `--tmpdir TMPDIR` : temporary working directory (default: `/tmp/population`)  
- `--local` : user-local installation under `~/.local`  
- `--verbose` : print detailed process information  
- `--keep-tests` : keep temporary test files instead of deleting them  
- `--version` : print installer script version  

Installation logs are saved as `population-install.log` in the working directory.

---

## Quick Start

Install both **PopJSON** and **Population** in one go:

```bash
# Install PopJSON CLI and Population backend (system-wide)
sudo ./install.population.sh

# Verify installation
popjson --version
```

If you prefer a local (non-sudo) installation:

```bash
./install.population.sh --local
```

---

## License & Disclaimer

This project is distributed under the terms of the license in `LICENSE`.  

> **Disclaimer:**  
> The installer script (`install.population.sh`) is provided *“AS IS”*, without warranty of any kind.  
> Use at your own risk. The authors assume no responsibility for any issues, damages, or losses resulting from its use.