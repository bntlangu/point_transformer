import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtDataVisualization 1.0

Window {
    visible: true
    width: 640
    height: 480

    property alias interSpaceMax: interSpaceSpin.to
    property alias interSpace: interSpaceSpin.value
    property alias numPts: numPointsSpin.value
    property alias numPtsMax: numPointsSpin.to
    property alias shape: shapeCombo.currentText

    title: qsTr("Hello World")

    RowLayout{

        id: inputControl

        Label {
            id: label
            text: qsTr("Line length (#points)")
            verticalAlignment: Text.AlignVCenter
        }

        SpinBox {
            id: numPointsSpin
            from: 1
            value: 1
        }

        Label {
            id: label1
            text: qsTr("Inter point dist (pixels)")
            verticalAlignment: Text.AlignVCenter
        }

        SpinBox {
            id: interSpaceSpin
            from: 1
            value: 1
        }

        ComboBox {
            id: shapeCombo
            displayText: "Shape"
            model:ListModel {
                id: shapeModel

                ListElement {
                    name: "Horizontal Line"
                }
                ListElement {
                    name: "Vertical Line"
                }
            }

            onCurrentIndexChanged: {
                displayText = shapeModel.index(currentIndex, currentIndex).name
            }

        }
    }

    Theme3D {
        id: themeIsabelle
        type: Theme3D.ThemeIsabelle
        font.family: "Lucida Handwriting"
        font.pointSize: 40
    }

    Item {
        id: dataView
        anchors.top: inputControl.bottom
        width: parent.width
        height: parent.height - inputControl.height

        Scatter3D {
            id: scatterGraph
            width: dataView.width
            height: dataView.height

            theme: themeIsabelle
            shadowQuality: AbstractGraph3D.ShadowQualitySoftLow

//            axisX.segmentCount: 3
//            axisX.subSegmentCount: 2
//            axisX.labelFormat: "%.2f"
//            axisZ.segmentCount: 2
//            axisZ.subSegmentCount: 2
//            axisZ.labelFormat: "%.2f"
//            axisY.segmentCount: 2
//            axisY.subSegmentCount: 2
//            axisY.labelFormat: "%.2f"

            Scatter3DSeries {
                id: scatterSeries
                itemLabelFormat: "Series 1: X:@xLabel Y:@yLabel Z:@zLabel"

                ItemModelScatterDataProxy {
                    itemModel: dataModel
                    xPosRole: "xPos"
                    yPosRole: "yPos"
                    zPosRole: "zPos"
                }
            }
        }

        ListModel {
            id: dataModel
            ListElement{ xPos: -10.0; yPos: 5.0; zPos: -5.0 }
            ListElement{ xPos: -9.0; yPos: 3.0; zPos: -4.5 }
            ListElement{ xPos: -8.5; yPos: 4.1; zPos: -4.0 }
            ListElement{ xPos: -8.0; yPos: 4.75; zPos: -3.9 }
            ListElement{ xPos: -9.5; yPos: 4.9; zPos: -4.2 }
            ListElement{ xPos: -9.9; yPos: 3.42; zPos: -3.5 }
            ListElement{ xPos: -7.8; yPos: 3.1; zPos: -4.9 }
            ListElement{ xPos: -7.3; yPos: 2.91; zPos: -4.1 }
            ListElement{ xPos: -7.1 ; yPos: 3.68 ; zPos: -4.52 }
            ListElement{ xPos: -8.8 ; yPos: 2.96 ; zPos: -3.6 }
            ListElement{ xPos: -6.94 ; yPos: 2.4 ; zPos: -2.92 }
            ListElement{ xPos: -9.02 ; yPos: 4.74 ; zPos: -4.18 }
            ListElement{ xPos: -9.54 ; yPos: 3.1 ; zPos: -3.8 }
            ListElement{ xPos: -6.86 ; yPos: 3.66 ; zPos: -3.58 }
            ListElement{ xPos: -8.16 ; yPos: 1.82 ; zPos: -4.64 }
            ListElement{ xPos: -7.4 ; yPos: 3.18 ; zPos: -4.22 }
            ListElement{ xPos: -7.9 ; yPos: 3.06 ; zPos: -4.3 }
            ListElement{ xPos: -8.98 ; yPos: 2.64 ; zPos: -4.44 }
            ListElement{ xPos: -6.36 ; yPos: 3.96 ; zPos: -4.38 }
            ListElement{ xPos: -7.18 ; yPos: 3.32 ; zPos: -4.04 }
            ListElement{ xPos: -7.9 ; yPos: 3.4 ; zPos: -2.78 }
            ListElement{ xPos: -7.4 ; yPos: 3.12 ; zPos: -3.1 }
            ListElement{ xPos: -7.54 ; yPos: 2.8 ; zPos: -3.68 }
        }

    }

}
