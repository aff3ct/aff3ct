# Working in AFF3CT

## Before committing any C++ change

Two `analysis` CI checks are pure functions of the source tree, so they fail
the same way locally and in CI. Run them before you commit (or install the
hook below and it does it for you):

1. **Format the code.**
   ```bash
   ./scripts/format.sh            # reformat in place
   ./scripts/format.sh --check    # dry-run: lists files that would change
   ```
   CI diffs the whole tree against a clang-format pass and fails on any
   difference. `format.sh` bootstraps the *exact* clang-format version CI uses
   (14.x, pinned) into a cached virtualenv via the clang-format PyPI wheel, so
   its output is identical on every machine — only `python3` is required. Do
   **not** rely on your system `clang-format`: a different major version
   reformats differently and will fight CI.

2. **Regenerate the aggregate header** whenever you add, remove, or rename a
   file under `include/`:
   ```bash
   ./scripts/generate_aff3ct_header.sh          # rewrites include/aff3ct.hpp
   ```
   CI regenerates `include/aff3ct.hpp` and fails if the committed copy differs.
   Every header is auto-included there, so a new header must compile standalone
   in *any* build configuration — guard SIMD includes behind the right macros
   (e.g. `#ifdef __AVX2__ ... #include <immintrin.h> ... #endif`).

## Install the pre-commit hook (recommended, once per clone)

```bash
./scripts/install-hooks.sh
```

The hook (`scripts/hooks/pre-commit`) **blocks** the commit if
`include/aff3ct.hpp` is stale (cheap, no dependencies) and **warns without
blocking** if clang-format would change files. It never prevents a commit over
missing tooling — CI remains the hard gate.

## CI

- **GitLab CI** (`.gitlab-ci.yml`) is the canonical CI and must keep working.
- Shared logic lives under `ci/`; changes there can affect both GitLab and the
  GitHub Actions workflows, so check both.
