# EasyCharge maintenance changes — 25 September 2026

These changes were made after the original course submission while preparing the portfolio. They are not part of the original graded submission.

The four earlier fixes are retained: coordinate sscanf arguments, identity-preserving car BST deletion, unique port numbering, and rejection of duplicate charging requests for queued cars.

## Additional fixes in this package

- Cars_BST.c: initialize a new car's stationId to zero before attempting assignment.
- Files.c / Files.h: loaders now return bool; missing files, incomplete records and the detected loading errors abort startup instead of silently continuing. Zero-initialize temporary stations to make cleanup safe. Release temporary station structs after insertion. Detect duplicate car/station IDs and failed insertion. Propagate stream read/close errors.
- SystemCoreFuncs.c / SystemCoreFuncs.h: propagate loading failure and print success only after all loaders succeed.
- main.c: free partially loaded state and return failure without invoking the saving exit path when loading fails. Free allocated managers if initialization is incomplete.
- Station_BST.c: initialize an empty port-list wrapper with every station; fix findStationOfPortRec to start at the list head; release port-list wrappers when deleting stations.
- LineOfCars.c: when clearing a queue, reset the station ID of cars that are not charging.
- GenFuncs.c: reject NaN and infinity for coordinate input.

The four uploaded data files are preserved byte-for-byte. Generated build output and machine-specific .vcxproj.user settings are excluded. Original uploaded files have not been modified.

## Scope and remaining limitations

This is a focused maintenance pass, not a complete redesign of input or persistence handling. CSV numeric fields and cross-file relationships do not yet have exhaustive validation. Saving still writes the files individually rather than as an atomic transaction. End-of-input handling in interactive loops is unchanged. Historic sample charging timestamps can produce large elapsed times and simulated bills. No license has been added.

See TESTING.md for the checks performed. See changes.patch for the exact changes relative to EasyCharge fixed.zip.
