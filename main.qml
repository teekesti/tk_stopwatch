import com.gmail.kesti.tero.stopwatchengine 1.0
//import Button
import QtQuick //1.0

//import QtMobility.sensors 1.0


Rectangle {
    id: window
    width: 800
    height: 480
    property int clockWidth: width > height ? height : width
    property int lapTimeListWidth: width - clockWidth
    property int fontSize: 0.04 * (width > height ? height : width)

    // The following was an attempt to change the layout when the
    // device is rotated between landscape and portrait orientations.
    // It didn't quite work, so it is commented out.

    //    states: [
    //        State {
    //            name: "Vertical"

    //            PropertyChanges {
    //                target: clockbuttonarea
    //                width: window.width
    //                height: width
    //                anchors.top: window.top

    //            }
    //            PropertyChanges {
    //                target: lapTimeListContainer
    //                anchors.top: clockbuttonarea.bottom
    //                width: window.width

    //            }
    //        }
    //    ]

    //    onWidthChanged: {
    //        console.log(" new window width: ", width);
    //        if (window.width < window.height)
    //        {
    //            console.log("entering vertical state")
    //            window.state = "Vertical"
    //        }
    //        else {
    //            window.state = "*"
    //        }
    //    }


    StopwatchEngine {
        id: myStopWatchEngine
        onLapTimesSaved: {

            //            console.log(fileName)
            messageButton.label = "Lap times saved to " + fileName
            messageButton.visible = true
            saveLapTimesButton.buttonEnabled = false
        }
        onErrorOccurred : {
            messageButton.label = errorMsg
            messageButton.visible = true

        }
    }

    Rectangle {
        id: clockbuttonarea
        width: window.clockWidth
        height: width
        anchors {
            right: window.right
        }
        Clock {
            id: clockface
            totalTime: myStopWatchEngine.totalTime
            seconds: myStopWatchEngine.seconds
            z: 1 // keep on top of buttons
            height: parent.height < parent.width ? parent.height : parent.width

            scale: 1
            clip: true
            width:  height
            anchors {horizontalCenter: parent.horizontalCenter}
        }

        Button  {
            id: startButton
            buttonEnabled: !myStopWatchEngine.running
            buttonWidth: clockface.width/2
            buttonHeight: clockface.height/2
            anchors.right: clockface.right
            anchors.top: clockface.top
            SystemPalette { id: myPalette; colorGroup: SystemPalette.Disabled }
            buttonColor: buttonEnabled ? "cyan" : myPalette.button


            label: "START"
            labelSize:  window.fontSize
            labelRotationAngle: 45
            labelVerticalCenterOffset: -buttonHeight/4
            labelHorizontalCenterOffset: -labelVerticalCenterOffset
            onButtonClick: myStopWatchEngine.start()
        }

        Button  {
            id: stopButton
            buttonEnabled: myStopWatchEngine.running
            buttonWidth: clockface.width/2
            buttonHeight: clockface.height/2
            anchors.bottom: clockface.bottom
            anchors.right: clockface.right
            buttonColor: buttonEnabled ? "yellow" : myPalette.button
            label: "STOP"
            labelSize:  window.fontSize
            labelRotationAngle: -45
            labelVerticalCenterOffset: buttonHeight/4
            labelHorizontalCenterOffset: labelVerticalCenterOffset
            onButtonClick: {
                myStopWatchEngine.stop()
                saveLapTimesButton.buttonEnabled = true
                saveLapTimesButton.visible = true

            }
        }

        Button  {
            id: lapButton
            buttonEnabled: myStopWatchEngine.running
            buttonWidth: clockface.width/2
            buttonHeight: clockface.height/2
            anchors.bottom: clockface.bottom
            anchors.left: clockface.left
            buttonColor: buttonEnabled ? "magenta" : myPalette.button
            label: "LAP"
            labelSize:  window.fontSize
            labelRotationAngle: 45
            labelVerticalCenterOffset: buttonHeight/4
            labelHorizontalCenterOffset: -labelVerticalCenterOffset
            onButtonClick: {
                myStopWatchEngine.lap()
            }

        }

        Button  {
            id: resetButton
            buttonEnabled: true
            buttonWidth: clockface.width/2
            buttonHeight: clockface.height/2
            anchors.top: clockface.top
            anchors.left: clockface.left
            buttonColor: buttonEnabled ? "pink" : myPalette.button
            label: "RESET"
            labelSize:  window.fontSize
            labelRotationAngle: -45
            labelVerticalCenterOffset: -buttonHeight/4
            labelHorizontalCenterOffset: labelVerticalCenterOffset
            onButtonClick: {
                myStopWatchEngine.reset()
                saveLapTimesButton.buttonEnabled = false
                saveLapTimesButton.visible = false

            }

        }
    } //clockbuttonarea


    Rectangle {
        id: lapTimeListContainer
        width: window.lapTimeListWidth
        anchors.left: window.left
        anchors.top: window.top
        anchors.bottom: window.bottom
        color: "lightblue"
        border.width: 3
        border.color: "#000000"

        Rectangle {
            id: currentLapRectangle
            width: parent.width
            height: lapLabel.height * 1.1
            anchors.left: parent.left
            anchors.top: parent.top
            color: "lightgreen"
            border.width: 3;
            border.color: "black"
            clip: true
            Text {
                id: lapLabel
                text: "Lap:\t" + myStopWatchEngine.currentLapTime
                font.bold: true
                //                font.underline: true
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                font.pointSize: window.fontSize


            }
        }

        ListView {
            id: lapTimeListView
            clip: true
            highlightRangeMode: ListView.NoHighlightRange
            snapMode: ListView.SnapToItem
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            Component {
                id: lapTimeDelegate
                Text
                {
                    text: (index + 1) + ".\t" + display;
                    font.pointSize: window.fontSize
                    color: lapTimeDelegate.ListView.isCurrentItem ? "red"
                                                                  : "black"
                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                }

            }

            anchors.top: currentLapRectangle.bottom
            anchors.bottom: parent.bottom

            model: myStopWatchEngine.LapTimeModel
            delegate: lapTimeDelegate

        } //ListView

        Button {
            id: saveLapTimesButton
            z: 1

            buttonEnabled: false //!myStopWatchEngine.running
            visible: false
            buttonWidth: parent.width //30//1.1 * label.width
            //            buttonHeight: 10//1.1 * label.height
            //            anchors.horizontalCenter: parent.horizontalCenter
            //            anchors.top: lapTimeListView.bottom

            anchors.bottom: parent.bottom
            buttonColor: buttonEnabled ? "lightgreen" : myPalette.button

            label: "Save Lap Times"
            labelSize:  window.fontSize
            onButtonClick: {
                myStopWatchEngine.saveLapTimes()
                visible = false
            }
        } // saveLapTimesButton

    } // lapTimeListContainer

    Button {
        id: messageButton
        visible: false
        buttonWidth: window.width
        anchors.verticalCenter: window.verticalCenter
        //        buttonHeight: 150

        buttonColor: "lightyellow"
        onButtonClick: visible = false

        //Animate the scale property change
        //        Behavior on visible { NumberAnimation{ duration: 100 } }

    }
} // window









