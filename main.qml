import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtDataVisualization 1.0

Page {
    id: rootWindow
    visible: true
    width: 640
    height: 480
    property alias colorSliderValue: colorSlider.value
    property alias sizeSliderValue: sizeSlider.value

    property alias interSpaceMax: interSpaceSpin.to
    property alias interSpace: interSpaceSpin.value
    property alias numPts: numPointsSpin.value
    property alias numPtsMax: numPointsSpin.to
    property alias shapeIdx: shapeCombo.currentIndex

    signal sendCreateLine(var num_pts, var inter_space, var shape_idx)

    function createLine() {
        console.log("(onCreateLine) Creating line with " + numPts + " points; spaced at " + interSpace);
        sendCreateLine(numPts, interSpace, shapeIdx)
    }

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

        Button {
            id: createButton
            text: qsTr("CREATE")
            spacing: 3

            onClicked: {
                rootWindow.createLine()
            }

            /*Component.onCompleted: {
                onClicked.connect(rootWindow.createLine)
            }*/
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
        height: parent.height - inputControl.height - ptControl.height

        Scatter3D {
            id: scatterGraph
            width: dataView.width
            height: dataView.height

            theme: themeIsabelle
            shadowQuality: AbstractGraph3D.ShadowQualitySoftLow

            Scatter3DSeries {
                id: scatterSeries
                itemLabelFormat: "Series 1: X:@xLabel Y:@yLabel Z:@zLabel"

                ItemModelScatterDataProxy {
                    objectName: "scatterDataProxy"
                    id: scatterSeriesProxy
                    itemModel: ptsModel
                    xPosRole: "X"
                    yPosRole: "Y"
                    zPosRole: "Z"
                }
            }
        }
    }

    RowLayout{
        id: ptControl

        anchors.top: dataView.bottom

        Label {
            text: qsTr("Point size (pixels)")
            verticalAlignment: Text.AlignVCenter
        }

        Slider {
            id: sizeSlider
            value: 0.5
        }

        Label {
            text: qsTr("Point color")
            verticalAlignment: Text.AlignVCenter
        }

        Slider {
            id: colorSlider
            value: 0.5
        }
    }
}
