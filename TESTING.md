# Verification of this maintenance package

Source compiled in Linux with GCC using:

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined C_project/*.c -lm -o easycharge
```

Compilation completed without warnings. Runtime checks used AddressSanitizer and UndefinedBehaviorSanitizer with leak detection disabled. These checks do not certify absence of all memory leaks or all defects. The newly edited package has not been compiled in Visual Studio in this environment.

Tests used isolated copies of data; packaged data were not changed. Tests for the earlier car-deletion and port-numbering bugs used the original course dataset because the uploaded working dataset already contains those operations' results.

## Completed checks

- Display all stations, cars at a station, station statistics and top customers.
- Reject numeric input with trailing letters; accept 32.79, 34.99 and find Haifa Port at distance zero.
- Reject NaN and positive/negative infinity, then accept valid coordinates.
- Delete car 25497284 from the original tree without damaging the queue; restart and find car 25529962 still waiting.
- Remove port 3 at station 307, add a SLOW port, and verify saved numbers 1, 2, 4, 5, 6.
- Reject a request to charge queued car 25529962 at station 205.
- Charge and disconnect idle car 22112748 at station 205.
- Disconnect car 12345678 and advance the first compatible waiting car.
- Release overdue charging ports at station 101.
- Close station 101 and verify queued car 25529962 is saved with stationId=0 and InQueue=0.
- Add a new SLOW car at station 623, where that charging type is unavailable; verify saved stationId=0.
- Remove each of the four data files separately: startup exits with code 1, does not report success, does not create the missing file and does not modify remaining data files.
- Supply an incomplete station record containing only an ID: startup exits with code 1, without a sanitizer error or modifying the input files.
- Verify the packaged data files remain identical to the uploaded working data.

## Windows check to perform

1. Extract this archive into a new folder and open C_project.sln.
2. Select Debug and x64, then Build > Rebuild Solution.
3. Run with Ctrl+F5; select 5 to display stations and 0 to exit.
4. With the program closed, temporarily rename C_project/Cars.txt to Cars.backup.txt.
5. Run again. Expect a startup error and the statement that no files were saved.
6. Restore the filename Cars.txt before continuing.

Run with C_project as the working directory, where all four .txt files are located. Keep a copy before manual tests, since successful operations save changes to those files.
