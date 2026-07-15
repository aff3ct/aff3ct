#!/bin/bash
#
# Format all C/C++ sources with a PINNED clang-format version.
#
# Why pinned: clang-format's output changes between major versions, so a
# developer with clang-format 18 would reformat files in a way CI (v14)
# then rejects. This script bootstraps the exact version CI uses into a
# cached virtualenv via the self-contained clang-format PyPI wheel, so the
# result is byte-identical on every machine and OS. Only python3 is needed.
#
# Usage:
#   ./scripts/format.sh            # reformat the tree in place
#   ./scripts/format.sh --check    # exit non-zero if anything would change
#                                    (no files modified) -- used by the hook/CI
#
set -euo pipefail

# Keep this in lockstep with CI's clang-format (Ubuntu 22.04 ships v14).
CF_VERSION="14.0.6"
VENV_DIR="${AFF3CT_CF_VENV:-$HOME/.cache/aff3ct/clang-format-${CF_VERSION}}"

cd "$(git rev-parse --show-toplevel 2>/dev/null || dirname "$(dirname "$0")")"

if ! command -v python3 >/dev/null 2>&1; then
  echo "format.sh: python3 not found; cannot bootstrap clang-format ${CF_VERSION}." >&2
  exit 127
fi

CF="${VENV_DIR}/bin/clang-format"
if [ ! -x "$CF" ] || ! "$CF" --version 2>/dev/null | grep -q "$CF_VERSION"; then
  echo "format.sh: bootstrapping clang-format ${CF_VERSION} into ${VENV_DIR} ..." >&2
  python3 -m venv "$VENV_DIR"
  "${VENV_DIR}/bin/pip" install -q --disable-pip-version-check "clang-format==${CF_VERSION}"
fi

# Same file selection as scripts/apply_clang_format.sh.
mapfile -d '' -t files < <(
  find include src \
    \( -iname '*.h' -o -iname '*.hpp' -o -iname '*.hxx' -o -iname '*.cpp' \) \
    -type f -print0
)

if [ "${1:-}" = "--check" ]; then
  rc=0
  for f in "${files[@]}"; do
    if ! "$CF" "$f" | diff -q - "$f" >/dev/null 2>&1; then
      echo "would reformat: $f"
      rc=1
    fi
  done
  exit $rc
fi

"$CF" -i "${files[@]}"
echo "format.sh: formatted ${#files[@]} files with clang-format ${CF_VERSION}."
