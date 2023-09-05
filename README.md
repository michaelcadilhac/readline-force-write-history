# readline-force-write-history
When loaded using LD_PRELOAD, this forces readline sessions to be stored in a history file provided as an env variable. 

## Example usage: `bc`

This was written for GNU `bc`, since GNU `bc` now uses `readline` by default on Arch Linux, and does not play nice with `rlwrap`.  However, `bc` does not save anything in files.  I now use:

```
HISTORY_FILE`/home/micha/.bc_history LD_PRELOAD`./force_write_history.so bc
```
