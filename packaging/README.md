# Kestrel Quickshell fork

This is `kestrel-distro/quickshell`, a fork of
[`quickshell-mirror/quickshell`](https://github.com/quickshell-mirror/quickshell).
The fork exists so Kestrel can track upstream; the lean build is defined by
`packaging/local/PKGBUILD` and compiles this fork's own tree.

## Kestrel changes on top of upstream

- `packaging/local/PKGBUILD`: Clang, ThinLTO, x86-64-v3, strip, `Release`, and
  the optional modules the Kestrel shell does not use disabled (X11, I3,
  Screencopy, Greetd). Verified against `kestrel/shell`; everything the shell
  imports stays on.

## Keeping up to date

```
git remote add upstream https://github.com/quickshell-mirror/quickshell.git
git fetch upstream
git merge upstream/master
```

The GitHub "Sync fork" button does the same, merging upstream into `master`.

## Building

```
cd packaging/local
makepkg -s -f --noconfirm --nocheck
```

The package is named `quickshell-kestrel`, and conflicts with/replaces/provides
`quickshell`, so Arch's stock package can never install or overwrite it; updates
come only from the Kestrel `[kestrel]` repository. The installed binary and
library are unchanged.

| Variable | Default | Meaning |
|---|---|---|
| `_kestrel_march` | `x86-64-v3` | CPU target; `x86-64-v4` or `native` to retarget |
| `_kestrel_lto` | `thin` | `thin` or `none` |
| `_kestrel_pgo` | `no` | `no`, `generate` (instrumented), or `use` (optimized); see `PGO.md` |

## Standards

- Changes ship with tests.
- English in commits, code and documentation.
- History is never rewritten.
