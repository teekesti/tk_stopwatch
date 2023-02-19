# tk_stopwatch

This is is a simple stopwatch written in Qt/QML/C++. Its main purpose is to demonstrate how program logic written in C++ communicates with the UI written in QML. 
![tkstopwatch_screenshot_1](https://user-images.githubusercontent.com/118811859/219977107-95066452-b71b-461b-b2f6-c186e020b532.png)

## StopWatchEngine

StopWatchEngine implemented in stopwatchengine.h and *.cpp contains the program logic for a stopwatch. It
keeps track of the total running time and current lap time. Both are
incremented after starting the stopwatch. The engine emits signals as the
times change.

When a lap time is recorded, it is stored to a list and the current lap time
is reset to zero.

When the stopwatch is restarted after stopping, the total running time and
current lap time continue from where they were when stopping the stopwatch.

Resetting the stopwatch sets the total and lap times to zero and empties
the list of lap times.

The lap times can be written to a text file. Even if there are no recorded
lap times, the total running time is recorded, along with the full date and
time of the first starting and last stopping of the stopwatch. The file name
is created automatically based on the starting date and time. The files
are stored in a separate directory under the user's home directory.

![tkstopwatch_screenshot_2](https://user-images.githubusercontent.com/118811859/219977443-edfc4b17-1100-4ad2-952f-7f2a959fa312.png)
![tkstopwatch_screenshot_3](https://user-images.githubusercontent.com/118811859/219977450-e8a7955b-0ad1-4e0a-8287-a062a236a420.png)

## Building
- Requires Qt. Originally it was written in Qt 4.x but now it's ported to Qt 6.
