
import QtQuick// 1.0

Item {
    id: clock
    width: 200; height: width

    property real seconds
    property string totalTime
    property int fontSize: width * 0.05

//    property bool running:  true

    Rectangle{

        id: clockface
        x: 0
        y: 0
        height: parent.width - 4


        width:  height
        radius: height/2
        border.width: 3
        border.color: "#000000"
        Image { z: 1; anchors.fill: parent; fillMode: Image.PreserveAspectFit;
            source: "circlegr.svg"}

        Repeater { // tick marks
            model: 60
            Rectangle {
                // extra width and height for every 5 second ticks
                width: (index % 5 === 0) ? 3 : 1
                height: (index % 5 === 0) ? 15 : 10
                color: "black"
                smooth: true
                x: clockface.width/2 - width/2

                transform: Rotation {
                    origin.x: 0; origin.y: clockface.height/2
                    angle: index * 6 //- parent.angleCorrection // -1 to accommodate line width
                }

            }
        }

        Repeater { // numbers
            model: 12
            Rectangle { // Dummy placeholder for text
                x:clockface.width/2
                y:clockface.height/2

                transform: Translate { // shift from the center
                x: (parent.radius -35 ) * Math.sin(index*30/180*Math.PI)
                y: -(parent.radius - 35) * Math.cos(index*30/180*Math.PI)
                }
                Text{
                    text: String(index*5)
                    font.pointSize: clock.fontSize
                    anchors.centerIn: parent

                }
            }
        }

        }

    Rectangle {
        id: secondPointer
        height:0.9 * clockface.radius; width:3
        color: "red"
        anchors.top: clockface.verticalCenter
        anchors.horizontalCenter: clockface.horizontalCenter
        transformOrigin: Item.Bottom
        smooth: true
        transform: Rotation {
            id: secondRotation
            angle: clock.seconds * 6 + 180
            Behavior on angle {
                SpringAnimation { spring: 2; damping: 0.2; modulus: 360;
                epsilon: 0.5 }
//                SmoothedAnimation { duration:100 }
            }
        }
    }

    Rectangle {
        id: centerDisk
        //        height: 0.40 * clockface.height
        width: 0.5 * clockface.width
        height: 1.2 * centerText.height
        //        color: "#fffefd"
        //        width:  height
        //        radius:  height/2
        border.width: 3
        border.color: "silver" //"#ff0b0b"
        anchors.centerIn: clockface
        Text {
            id: centerText
            text: clock.totalTime
            font.family: "DejaVu Sans Mono"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: clock.fontSize
            anchors.centerIn: parent
            z: 2

        }

    }

}

