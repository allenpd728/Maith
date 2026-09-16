#!/usr/bin/env python3
"""Shared helper for the mutation guards (issue #36).

A mutation guard writes a MUTATED module to disk, runs the tests, then restores
the source. Two things can go wrong, both silently and both in a *later*
process:

1. **Stale bytecode.** CPython caches a `.pyc` keyed on the source's (size,
   mtime, ...). Restoring the source does not delete the mutated `.pyc`; if the
   restore lands in the same mtime second and the file keeps its size, the next
   process loads the MUTATED code from cache while `git diff` shows a clean tree.
   This is how a guard left `axiom-rewrite/candidates.py` compiled with
   `open("w")` instead of `open("a")`, truncating the ledger (DEC-048).

2. **A skipped restore.** If the mutation loop raises -- `KeyboardInterrupt`, or
   a `subprocess.TimeoutExpired` from a guard child -- the restore must still
   run. That is why it belongs in a `finally`.

`guarded_source` makes both impossible: it disables bytecode writing for this
process and its children, and binds the restore and the purge into a single
`finally` that no early exit can bypass. Use it instead of hand-rolling the
try/finally, so the pattern cannot drift again.

This file is a library; it has no entry point. Its own tests live in
`tooling/test_mutation_guard_lib.py`.
"""

import contextlib
import os
import shutil
import subprocess
import sys
from pathlib import Path

# Disable bytecode writing for this process the moment the helper is imported.
# The guard process itself can import the mutated module (e.g. the invariants
# 8/9 guard's in-process probe), so the parent is a source of stale `.pyc` too.
sys.dont_write_bytecode = True


def purge_bytecode(module_path) -> None:
    """Remove the `__pycache__` beside `module_path`.

    The whole directory goes, not just this module's `.pyc`: the interpreter tag
    in the filename varies by version, and discarding sibling caches only costs a
    recompile.
    """
    cache = Path(module_path).parent / "__pycache__"
    if not cache.exists():
        return
    try:
        shutil.rmtree(cache)
    except OSError:
        pass


def invalidate_module(module_path) -> None:
    """Drop `module_path`'s module (and submodules) from `sys.modules`."""
    stem = Path(module_path).stem
    for name in list(sys.modules):
        if name == stem or name.startswith(stem + "."):
            del sys.modules[name]


@contextlib.contextmanager
def guarded_source(module_path):
    """Guarantee source restore + bytecode purge on EVERY exit path.

    Yields the original source text. Mutate the file inside the `with`; the
    restore and the purge both run in the `finally`, so an exception in the body
    (including an interrupt or a child timeout) still leaves the tree clean and
    the bytecode cache purged.

        with guarded_source(SRC) as original:
            for name, old, new in MUTATIONS:
                SRC.write_text(original.replace(old, new, 1))
                run_python([str(TEST)], capture_output=True, text=True)

    Keeping the two steps in one `finally` is the point: separating them -- purge
    after the `try`, restore inside it -- is the bug this helper prevents (#36).

    The purge also runs on entry, so a stale `.pyc` left by an earlier crashed run
    cannot be loaded by this one.
    """
    module_path = Path(module_path)
    original = module_path.read_text()
    invalidate_module(module_path)
    purge_bytecode(module_path)
    try:
        yield original
    finally:
        try:
            module_path.write_text(original)
        finally:
            invalidate_module(module_path)
            purge_bytecode(module_path)


def run_python(args, **kwargs):
    """`subprocess.run` with bytecode writing disabled in the child.

    Children are where the mutated module is usually imported, so `-B` (plus the
    matching env var, for anything the child re-execs) is set here rather than
    left to each guard.
    """
    env = dict(os.environ, PYTHONDONTWRITEBYTECODE="1")
    return subprocess.run([sys.executable, "-B", *args], env=env, **kwargs)
