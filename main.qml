import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Extras 1.4
import QtQuick.Controls 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import com.luyaohan1001.speedometer 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Drive-by-Wire Cockpit Dashboard")
    color: "#000000"
    visibility:  "FullScreen"

    /* Gas Meter */
    CircularGauge {
        objectName: "speedController";

        // value: accelerating ? maximumValue : 0
        value: speedometer.speedValue; /* The value can also be overwridden from the speedometer class. */

        /* Location of the meter on the screen. */
        x: 400;
        y: 400;
        width: 600;
        height: 600;

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
        y: 400;
        value: 80;

        style: CircularGaugeStyle {
           minimumValueAngle: 180;
           maximumValueAngle: 0;
           tickmarkStepSize: 100;
           labelStepSize: 30;
        }

        minimumValue: 0;
        maximumValue: 300;
    }

    /* Gas Meter Label. */
    Text {
        text: "GAS"
        font.family: "Helvetica"
        font.pointSize: 10
        color: "white"
        x: 1300;
        y: 550;
    }






}
