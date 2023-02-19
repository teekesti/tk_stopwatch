#ifndef STOPWATCHENGINE_H
#define STOPWATCHENGINE_H

#include "qqmlintegration.h"
#include <QObject>
#include <QTime>
#include <QElapsedTimer>
#include <QStringList>
#include <QStringListModel>

class QFile;

/*! Time format string for representing elapsed time. */
const QString KTimeFormat = "HH:mm:ss.zzz";

/*! StopWatchEngine contains the program logic for a stopwatch. It
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

    */

class StopwatchEngine : public QObject
{
    Q_OBJECT
    /*! The total running time as a QString */
    Q_PROPERTY(QString totalTime READ getTotalTime NOTIFY totalTimeChanged)
    /*! The currently running lap time as a QString */
    Q_PROPERTY(QString currentLapTime READ getCurrentLapTime
               NOTIFY currentLapTimeChanged)
    /*! The lap time model. For example, a QML ListView element can use
    this directly. */
    Q_PROPERTY(QStringListModel* LapTimeModel READ getLapTimeModel
               NOTIFY lapTimesChanged)
    /*! The seconds part (including milliseconds) of the total running time */
    Q_PROPERTY(float seconds READ getSeconds NOTIFY secondsChanged)
    /*! A boolean indicating whether the clock is running or not. */
    Q_PROPERTY(bool running READ running NOTIFY runStatusChanged)
    QML_ELEMENT


public:
    /*! Create a new StopwatchEngine object.*/
    explicit StopwatchEngine(QObject *parent = 0);


signals:
    /*! Emitted when the running time changes */
    void totalTimeChanged(QTime newTotalTime);
    /*! Emitted when the current lap time changes */
    void currentLapTimeChanged(QTime newLapTime);
    /*! Emitted when the integer seconds part of the running time changes.
    This is mostly useful for an analog clock that is updated once per
    second. */
    void secondsChanged();
    /*! Not really used, but it serves as a notifier signal for the LapTimeModel
    property. (If there is no notifier, there will be some error messages.) */
    void lapTimesChanged();
    /*! Emitted when the stopwatch is started or stopped. */
    void runStatusChanged(bool running);
    /*! Emitted when the lap times are saved to a file
    @param fileName The full path of the file to which the lap times
    were saved. */
    void lapTimesSaved(QString fileName);
    /*! Emitted when there was some error saving the lap times.
    @param errorMsg A string describing the error */
    void errorOccurred(QString errorMsg);

public:
    /*! Return the time elapsed after starting, as a string. */
    QString getTotalTime() const;
    /*! Return the time elapsed after last lap (or start, if this is the first
    lap).*/
    QString getCurrentLapTime() const;
    /*! Get a list of lap times */
    QStringList getLapTimes() const;
    /*! Get the seconds and milliseconds part of the total running time,
    as a floating point number */
    float getSeconds() const;
    /*! Return true if the clock is running, false otherwise.*/
    bool running() const;
    /*! Return a pointer to the model that stores the lap times, needed
    for the property LapTimeModel that is used in the QML ui. */
    QStringListModel* getLapTimeModel();


public slots:
    /*! Start the stopwatch. If the stopwatch was started earlier and then
    stopped, re-starting will increment the time values from the previous ones.
    */
    void start();
    /*! Stop the stopwatch.*/
    void stop();

    /*! Add the current lap time to the lap time list and reset the current
    lap time to zero.*/
    void lap();

    /*! Stop the stopwatch, reset the total and lap times and clear the
    lap time list. */
    void reset();

    /*! Save the lap times to a file in a separate directory under the user's
    home directory */
    void saveLapTimes();



private: // data
    QElapsedTimer mStartTime;
    QTime mTotalTime;
    QTime mCurrentLapTime;
    /*! Used only for checking the accuracy of the clock during debugging.*/
    QElapsedTimer mDebugTimer;
    QStringListModel mLapTimeModel;
    int mTimerInterval; /*!< in milliseconds */
    int mTimerID;
    QFile* mLapTimeFile;
    /*! The date and time when the stopwatch was first started
    and stopped. These are written to the log file, if the user saves it.*/
    QDateTime mFirstStartTime;
    QDateTime mLastStopTime;

private: //methods
    /*! Add a new lap time to the list */
    void addLapTime();
    /*! Create and open a file (mLapTimeFile) for writing the lap times.
    @return true if successfull, false otherwise.*/
    bool setupLogFile();

private slots:
    void onTotalTimeChanged();

protected:
    void timerEvent(QTimerEvent *);

};

#endif // STOPWATCHENGINE_H
