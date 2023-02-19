#include <QFile>
#include <QDir>
#include "stopwatchengine.h"
#include "eoqttrace.h"




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
StopwatchEngine::StopwatchEngine(QObject *parent) :
    QObject(parent), mTotalTime(0, 0), mCurrentLapTime(0, 0),
    mTimerInterval(100), mTimerID(0)
{
//    mLapTimeModel.setHeaderData(0, Qt::Horizontal, "Lap nr");
    connect(this, SIGNAL(totalTimeChanged(QTime)),
            this, SLOT(onTotalTimeChanged()));

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
QString StopwatchEngine::getTotalTime() const
{
    return mTotalTime.toString(KTimeFormat);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
QString StopwatchEngine::getCurrentLapTime() const
{
    return mCurrentLapTime.toString(KTimeFormat);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
QStringList StopwatchEngine::getLapTimes() const
{
    return mLapTimeModel.stringList();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
float StopwatchEngine::getSeconds() const
{
    return mTotalTime.second() + mTotalTime.msec()*0.001;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
bool StopwatchEngine::running() const
{
    return (mTimerID != 0);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
QStringListModel* StopwatchEngine::getLapTimeModel()
{
    return &mLapTimeModel;
}

void StopwatchEngine::start()
{
    // Inherited from QObject. startTimer returns zero if it cannot start
    // a timer. Only start the timer if mTimerID is zero, i.e. the timer is not
    // already running.
    FUTR();
    if (!running())
    {
        mStartTime.start(); //= QTime::currentTime();
        mTimerID = startTimer(mTimerInterval);
        emit runStatusChanged(running());
        if (mFirstStartTime.isNull())
        {
            mFirstStartTime = QDateTime::currentDateTime();
        }
        mLastStopTime = QDateTime(); // null
#ifdef ENABLE_FUNC_TRACE
        mDebugTimer.start();
#endif
    }

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void StopwatchEngine::stop()
{   FUTR();
    if (running())
    {
        killTimer(mTimerID);
        mTimerID = 0; // reset to zero
        int mSecsSinceLastTimerEvent = mStartTime.elapsed();
        mTotalTime = mTotalTime.addMSecs(mSecsSinceLastTimerEvent);
        mCurrentLapTime = mCurrentLapTime.addMSecs(mSecsSinceLastTimerEvent);
        emit totalTimeChanged(mTotalTime);
        emit currentLapTimeChanged(mCurrentLapTime);
        // When the clock is stopped, emit the secondsChanged signal
        // so that the second pointer will be in sync with the numeric
        // display.
        emit secondsChanged();
        TRACE1("Total time: %1", mTotalTime.toString(KTimeFormat));
        emit runStatusChanged(running());
        TRACE1("Milliseconds elapsed: %1", mDebugTimer.elapsed());
        mLastStopTime = QDateTime::currentDateTime();
    }

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void StopwatchEngine::lap()
{
    FUTR();
    if (running()) // timer is running
    {
        mCurrentLapTime = mCurrentLapTime.addMSecs(mStartTime.elapsed());
//        TRACE1("Lap time: %1", mCurrentLapTime.toString("hh:mm:ss.zzz"));
        addLapTime();
        mCurrentLapTime.setHMS(0, 0, 0);
        emit currentLapTimeChanged(mCurrentLapTime);

    }


}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void StopwatchEngine::reset()
{
    FUTR();
    stop();
    //mTotalTime.elapsed();
    mTotalTime.setHMS(0, 0, 0);
    emit totalTimeChanged(mTotalTime);
    mCurrentLapTime.setHMS(0, 0, 0);
    emit currentLapTimeChanged(mCurrentLapTime);
    mLapTimeModel.removeRows(0, mLapTimeModel.rowCount());
    mFirstStartTime = QDateTime(); // set to a null date time

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void StopwatchEngine::timerEvent(QTimerEvent */* event */)
{
    // The mTotalTime and mCurrentLapTime are incremented
    // at every timer event. In practise the events do not occur exactly at
    // the set interval (default 1000 ms). Therefore, we can't increment the
    // time counters by the fixed interval, but need to measure the really
    // elapsed time.

    // The time (in milliseconds) elapsed since last timer event
    int elapsedMsecs = mStartTime.elapsed();

    mStartTime.restart();
    mTotalTime = mTotalTime.addMSecs(elapsedMsecs);

    mCurrentLapTime = mCurrentLapTime.addMSecs(elapsedMsecs);
    emit totalTimeChanged(mTotalTime);
    emit currentLapTimeChanged(mCurrentLapTime);

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void StopwatchEngine::addLapTime()
{
    // append a new row to the end
    int newIdx = mLapTimeModel.rowCount();
    mLapTimeModel.insertRow(newIdx);
    QModelIndex index = mLapTimeModel.index(newIdx);
    mLapTimeModel.setData(index, mCurrentLapTime.toString(KTimeFormat));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void StopwatchEngine::onTotalTimeChanged(){
    // Emit secondsChanged signal only once per second,
    // so that the second pointer ticks once per second.
    // This is implemented so that if the seconds part of the total
    // time is different than at the previous timer event, the integer second
    // has changed and the secondsChanged signal is emitted.

    static int previousSec = 0;
    int currentSec = mTotalTime.second();
    if (currentSec !=  previousSec)
    {
        emit secondsChanged();
//        qDebug() << mTotalTime.second() << mTotalTime.msec();
    }
    previousSec = currentSec;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void StopwatchEngine::saveLapTimes()
{


    if (setupLogFile()) // log file successfully opened
    {
        QTextStream out(mLapTimeFile);
        out << "Record from TKStopWatch.\n";
        out << "Stopwatch first started at " << mFirstStartTime.toString()
            << Qt::endl;
        QDateTime stopTime = mLastStopTime.isNull() ?
                    QDateTime::currentDateTime() : mLastStopTime;
        out << "Stopwatch last stopped at "  <<  stopTime.toString()
            << Qt::endl;
        out << "Total running time: " << mTotalTime.toString(KTimeFormat)
            << Qt::endl;
        if (mLapTimeModel.rowCount() > 0) // There are recorded lap times
        {
            out << "Lap times:" << Qt::endl;
            out << "Lap nr\tTime" << Qt::endl;
            int row = 1;
            foreach(QString lapTime, mLapTimeModel.stringList())
            {
                out << row++ <<".\t" << lapTime << Qt::endl;
            }
            out << "Last lap: " << mCurrentLapTime.toString(KTimeFormat)
                << Qt::endl;
        }

        emit lapTimesSaved(mLapTimeFile->fileName());
        mLapTimeFile->close();
        delete mLapTimeFile;
        mLapTimeFile = NULL;
    }
    else
    {
        TRACE("Lap times could not be saved.");
        // No need to emit errorOccurred signal here,
        // because it has already been emitted if setupLogFile()
        // returned false.
    }

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
bool StopwatchEngine::setupLogFile()
{
    // Create a directory for the lap times if it doesnt exist
    QString logPath = QDir::homePath() + "/" + "TKStopwatch_laptimes";
    QDir logDirectory(logPath);
    if (!logDirectory.exists())
    {
        if (logDirectory.mkdir(logPath))
        {
            qDebug() << "Created log directory " << logPath;
        }
        else
        {
            emit errorOccurred("Could not create log directory " + logPath);
            return false;
        }
    }

    if (mFirstStartTime.isNull()) // The stopwatch hasn't been started
    {
        return false;
    }

    QString logFileName = QString("%1/%2.txt")
            .arg(logPath)
            .arg(mFirstStartTime.toString("yyMMdd_hhmmss"));
    if (NULL == mLapTimeFile)
    {
        mLapTimeFile = new QFile(logFileName, this);
    }

    // Return true if file open successful, false otherwise
    if (mLapTimeFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        TRACE1("Log file: %1", logFileName);
        return true;
    }
    else
    {
        emit errorOccurred("Could not open log file: " + logFileName);
        return false;
    }

}


