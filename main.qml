import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Extras 1.4
import QtQuick.Controls 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtMultimedia 5.15
import QtAudioEngine 1.15
import QtGraphicalEffects 1.12
import com.luyaohan1001.speedometer 1.0


Window {
    id: driveByWireDashboard
    visible: true
    width: 640
    height: 480
    title: qsTr("Drive-by-Wire Cockpit Dashboard")
    color: "#000000"
    visibility:  "FullScreen"

    Item {
        x: 0
        y: 0
        Image {
            id: uiBackground
            width: 1920
            height: 1080
            source: "media/elements/UI_Background.jpg"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }

        FastBlur {
            anchors.fill: uiBackground
            source: uiBackground
            radius: 32
        }
    }

    /* Gas Meter */
    CircularGauge {
        objectName: "speedController";

        // value: accelerating ? maximumValue : 0
        value: speedometer.speedValue; /* The value can also be overwridden from the speedometer class. */

        /* Location of the meter on the screen. */
        x: 200;
        y: 200;
        width: 600;
        height: 500;

        /* property... specifies a custom property. */
        property bool accelerating: false

        /* If user press space, the speedometer will start rotate accordingly. */
        Keys.onSpacePressed: accelerating = true

        Keys.onReleased: {
            if (event.key === Qt.Key_Space) {
                accelerating = false;
                event.accepted = true;
            }
        }

        Component.onCompleted: forceActiveFocus()

        Behavior on value {
            NumberAnimation {
                duration: 1000
            }
        }

        /* Customize its style to make it look more realistic. */
        style: CircularGaugeStyle {
            tickmark: Rectangle {
               visible: styleData.value < 80 || styleData.value % 10 == 0
               implicitWidth: outerRadius * 0.02
               antialiasing: true
               implicitHeight: outerRadius * 0.06
               color: styleData.value >= 80 ? "#e34c22" : "#e5e5e5"
           }

           minorTickmark: Rectangle {
               visible: styleData.value < 80
               implicitWidth: outerRadius * 0.01
               antialiasing: true
               implicitHeight: outerRadius * 0.03
               color: "#e5e5e5"

           }
        }
    }


    /* Gas Meter Section. */
    CircularGauge {
        id: gasMeter;
        x: 1200;
        y: 200;
        width: 600;
        height: 500;
        value: 80;

        style: CircularGaugeStyle {
           minimumValueAngle: 140;
           maximumValueAngle: 40;
           tickmarkStepSize: 100;
           labelStepSize: 50;
        }

        minimumValue: 0;
        maximumValue: 300;
    }


    /* Water Temperature Gauge Section. */
    CircularGauge {
        id: waterTempGauge;
        x: 1200;
        y: 200;
        width: 600;
        height: 500;
        value: 80;

        style: CircularGaugeStyle {
           minimumValueAngle: 220;
           maximumValueAngle: 340;
           tickmarkStepSize: 100;
           labelStepSize: 50;
        }

        minimumValue: 0;
        maximumValue: 180;
    }


    /* Gas Meter Label. */
    Text {
        text: "GAS"
        font.family: "Helvetica"
        font.pointSize: 20
        color: "white"
        x: 1500;
        y: 500;
    }

    Image {
        source: "media/elements/Airbag_Warning.png"
        width: 30;
        height: 30;
        x: 850
        y: 550
    }

    Image {
        source: "media/elements/Battery_Warning.png"
        width: 30;
        height: 30;
        x: 900
        y: 550
    }

    Image {
        source: "media/elements/Brake_Warning.png"
        width: 30;
        height: 30;
        x: 950
        y: 550
    }

    Image {
        source: "media/elements/DPF_Warning.png"
        width: 30;
        height: 30;
        x: 1000
        y: 550
    }

    Image {
        source: "media/elements/Engine_Oil_Warning.png"
        width: 30;
        height: 30;
        x: 850
        y: 600
    }

    Image {
        source: "media/elements/Engine_Warning.png"
        width: 30;
        height: 30;
        x: 900
        y: 600
    }

    Image {
        source: "media/elements/Power_Steering_Warning.png"
        width: 30;
        height: 30;
        x: 950
        y: 600
    }

    Image {
        source: "media/elements/Temperature_Pressure.png"
        width: 30;
        height: 30;
        x: 1000
        y: 600
    }

    Image {
        source: "media/elements/Tire_Pressure.png"
        width: 30;
        height: 30;
        x: 1050
        y: 600
    }

    /* Left Blinker Light */
    Image {
        id: leftBlinker
        width: 50
        height: 50
        x: 800
        y: 450
        source: "media/elements/Left_Blinker.png"
        visible: blinkers.leftBlinkerState ? true : false
    }

    /* Right Blinker Light */
    Image {
        id: rightBlinker
        width: 50
        height: 50
        x: 1100
        y: 450
        source: "media/elements/Right_Blinker.png"
        visible: blinkers.rightBlinkerState ? true : false
    }

    /* Hazard Light */
    Image {
        id: emergentBlinker
        width: 50
        height: 50
        x: 950
        y: 400
        source: "media/elements/Hazard_Light.png"
        visible: blinkers.emergentBlinkerState ? true : false
    }

    /* Power Button. */
    DelayButton {
        id: startButton;
        text: "START"
        x: 1700;
        y: 900;
        width: 150;
        height: 150;
        delay: 500;
        onClicked: engineStartSound.play();
    }

    Audio {
         id: engineStartSound
         source: "media/elements/engineStartSound.mp3"
     }

    /* Brake Light Left */
    Rectangle {
        width: 100
        height: 100
        color: "#000000"

        StatusIndicator {
            width: 80
            height: 80
            anchors.centerIn: parent
            color: "red"
            active: brakes.brakesState
        }
    }

    /* Brake Light Right */
    Rectangle {
        width: 100
        height: 100
        x: 1820
        y: 0
        color: "#000000"

        StatusIndicator {
            width: 80
            height: 80
            anchors.centerIn: parent
            color: "red"
            active: brakes.brakesState
        }
    }


    /* Steering Wheel*/
    Image {
        source: "media/elements/Steering_Wheel.png"
        width: 500;
        height: 500;
        transformOrigin: Item.Center
        rotation: steeringWheel.steeringPosition
        x: 710
        y: 600
    }

}
