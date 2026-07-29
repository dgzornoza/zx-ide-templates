# Fixed Shooter Up

Template for upward fixed‑shooter games like Space Invaders to be used with Zx‑IDE.

## Description

...

## Index

1. [Installation](#installation)
2. [Usage](#usage)
3. [Features](#features)
4. [Credits](#credits)
5. [License](#license)

## Installation

This template is designed to run inside the **`zx-ide-templates`** VS Code devcontainer, which ships the full z88dk toolchain (SDCC + `sdcc_iy`), the Spectrum support libraries, and any host utilities required to assemble, link, and produce runnable `.tap` / `.sna` artifacts.

The canonical build parameters live in `.vscode/tasks.json` and are:

```text
COMPILER=sdcc
CLIB=sdcc_iy
CRT=31
C_OPT_FLAGS=-SO3 --opt-code-size
LINKER_FLAGS=
CREATE_SNA=true
CREATE_TAP=true
```

### Building from inside VS Code

Open the template folder in VS Code with the **Dev Containers** extension. Reopen in container when prompted (`Dev Containers: Reopen in Container`). Then press **Ctrl+Shift+B** to run the default `Build` task, or `Terminal → Run Build Task`.

### Building from the host with the Dev Containers CLI

If you have the [`@devcontainers/cli`](https://github.com/devcontainers/cli) installed on the host and the devcontainer is already built, you can run the build without opening VS Code:

```bash
devcontainer exec --workspace-folder "<repo-root>" \
  bash -c "cd /workspaces/zx-ide-templates/templates/fixed-shoot-em-up && \
           make COMPILER=sdcc CLIB=sdcc_iy CRT=31 \
                'C_OPT_FLAGS=-SO3 --opt-code-size' \
                LINKER_FLAGS= \
                CREATE_SNA=true CREATE_TAP=true"
```

Replace `<repo-root>` with the absolute path to your local checkout of `zx-ide-templates` (the parent of `.devcontainer/`). The path inside the container (`/workspaces/zx-ide-templates/...`) is the standard devcontainer mount path and matches the workspace folder the container was built from.

On success the build produces:

- `bin/fixed-shoot-em-up.tap` — loadable tape image (includes the loaded title screen).
- `bin/fixed-shoot-em-up.sna` — 48K snapshot for emulators that prefer `.sna` (e.g. Fuse).

## Usage

ASCII 
https://worldofspectrum.org/ZXBasicManual/zxmanappa.html

...

## Features

- First-boot splash using SP1 + font_1 tiles: blinking `PULSA UNA TECLA` prompt over the loaded title screen, with the loaded pixels preserved until a fresh keypress transitions to the menu.

## Credits

- Author: [dgzornoza](https://github.com/dgzornoza)

Any questions or suggestions, please contact me at [dgzornoza@dgzornoza.com](mailto:dgzornoza@dgzornoza.com).

## License

This project is licensed under the **GNU Affero General Public License v3.0 or later (AGPL-3.0-or-later)**. This means you can use, copy, modify, and distribute this code under the terms of the AGPL-3.0. For more details, see the [LICENSE](./LICENSE) file or visit the [GNU AGPL-3.0](https://www.gnu.org/licenses/agpl-3.0.html) page.
