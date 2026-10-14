# PGO variant

Profile-guided optimization is a two-pass build: an instrumented pass records
which code runs, then a second pass compiles using that profile. It is
measurement-gated: ship a PGO package only if it beats the normal build in the
kernel `bench/` harness on real hardware.

## 1. Instrumented pass

```
cd packaging/local
_kestrel_pgo=generate _kestrel_pgo_dir=/tmp/quickshell.profdata \
  makepkg -s -f --noconfirm --nocheck
```

Install the result and use the desktop normally for a while, so the profile
reflects real use (opening panels, the bar, menus, and notifications).
The raw profiles land in `_kestrel_pgo_dir`.

## 2. Optimized pass

```
cd packaging/local
_kestrel_pgo=use _kestrel_pgo_dir=/tmp/quickshell.profdata \
  makepkg -s -f --noconfirm --nocheck
```

`-fprofile-correction` is passed so a profile that is incomplete or slightly
stale does not fail the build.

## Notes

- The profile is host- and workload-specific; a profile gathered on one machine
  is not portable.
- Combine with `_kestrel_lto` as usual; `_kestrel_pgo=no` is the default.
- The instrumented binary is slower and writes profile data on exit; do not
  ship it.
