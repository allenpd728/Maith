# maith-docs Usage Notes

`maith-docs` is a local ollama model defined in `~/Projects/Maith/Modelfile` (not committed — see `.gitignore`).
It is based on `qwen3:8b` and constrained to documentation and formatting tasks only.

## Creating the model

```bash
ollama create maith-docs -f ~/Projects/Maith/Modelfile
```

## Running and parsing output

qwen3:8b emits a `Thinking...` / `...done thinking.` block before every response, plus ANSI
cursor-control sequences (`[?25l`/`[?25h`) interleaved with every streamed character.
Standard ANSI stripping misses the `[?25` sequences. Use this pipeline:

```bash
ollama run maith-docs "your prompt" 2>&1 | cat \
  | sed 's/\x1b\[[?]*[0-9;]*[a-zA-Z]//g' \
  | sed 's/\r//g' \
  | awk '/\.\.\.done thinking\./,0' \
  | tail -n +2 \
  | tr -s ' \n'
```

Key details:
- `[?]*` in the sed pattern catches `[?25l`/`[?25h` in addition to standard colour codes
- `awk` splits on the thinking delimiter, discarding everything before and including `...done thinking.`
- `tr -s` collapses repeated whitespace from terminal line-wrap artifacts

## What maith-docs will and won't do

**Will do:**
- Draft and reformat markdown documentation from facts explicitly given in the prompt
- Write boilerplate Python scripts and test scaffolding when given clear specs
- Formatting, clarity edits, and consistency checks on text shown to it

**Will not do:**
- Modify or suggest changes to Lean pipeline files
- Modify training or evaluation logic
- State any numeric result or claim not explicitly provided in the prompt
- Determine what is currently true about the codebase — that requires direct repo inspection

## Notes

- ollama version tested: 0.30.6
- Thinking cannot be disabled in this version (`PARAMETER think false` and `/no_think` in system prompt both fail)
- The thinking block content can be ignored — the response after `...done thinking.` is always clean
