#!/bin/bash
#
# Point git at the version-controlled hooks in scripts/hooks/.
# Run once per clone. Uses core.hooksPath so hook updates are picked up
# automatically (no copying into .git/hooks).
#
set -euo pipefail
cd "$(git rev-parse --show-toplevel)"

git config core.hooksPath scripts/hooks
chmod +x scripts/hooks/* 2>/dev/null || true

echo "install-hooks.sh: git hooks enabled (core.hooksPath = scripts/hooks)."
echo "  Disable with: git config --unset core.hooksPath"
