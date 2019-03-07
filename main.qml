import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtDataVisualization 1.0

Page {
    id: rootWindow
    visible: true
    width: implicitWidth
    height: 480

    property alias colorSliderValue: colorSlider.value
    property alias sizeSliderValue: sizeSlider.value
    property alias ptSize: scatterSeries.itemSize // vary this from 0.02 to 1
    property alias ptColor: scatterSeries.baseColor // vary this from 1 to 16777216 and convert to hex

    property alias interSpaceMax: interSpaceSpin.to
    property alias interSpace: interSpaceSpin.value
    property alias numPts: numPointsSpin.value
    property alias numPtsMax: numPointsSpin.to
    property alias shapeIdx: shapeCombo.currentIndex

    signal sendCreateLine(var num_pts, var inter_space, var shape_idx)

    signal requestSizeTransform(var size)
    signal requestColorTransform(var color)
    signal requestRotationTransform(var angle)

    function createLine() {
        console.log("(onCreateLine) Creating line with " + numPts + " points; spaced at " + interSpace);
        sendCreateLine(numPts, interSpace, shapeIdx)
    }

    function onApplySizeUpdate(size){
        console.log("(onApplySizeUpdate) Size: " + size)
        ptSize = size
    }

    function onApplyColorUpdate(color){
        console.log("(onApplyColorUpdate) Color: " + color)
        ptColor = color
    }

    title: qsTr("Point Manipulator")

    ColumnLayout{

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

            width: inputControl.width
            height: rootWindow.height - inputControl.height - ptControls.height

            Scatter3D {
                id: scatterGraph
                width: dataView.width
                height: dataView.height

                theme: themeIsabelle
                shadowQuality: AbstractGraph3D.ShadowQualitySoftLow

                Scatter3DSeries {
                    id: scatterSeries
                    itemLabelFormat: "Series 1: X:@xLabel Y:@yLabel Z:@zLabel"
                    itemSize: 0.05
                    baseColor: "#ff00ff"

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

        ColumnLayout{
            id: ptControls

            RowLayout{
                id: sizeControl

                Label {
                    text: qsTr("Point size (pixels)")
                    verticalAlignment: Text.AlignVCenter
                }

                Slider {
                    id: sizeSlider
                    stepSize: 0.01
                    value: 0.5

                    onValueChanged: {
                        requestSizeTransform(value)
                    }
                }
            }

            RowLayout{
                id: colorControl

                Label {
                    text: qsTr("Point color")
                    verticalAlignment: Text.AlignVCenter
                }

                Slider {
                    id: colorSlider
                    stepSize: 100
                    to: 1677721
                    value: 1

                    onValueChanged: {
                        requestColorTransform(value)
                    }
                }
            }

            RowLayout{
                id: rotControl

                Label {
                    text: qsTr("Point rotation")
                    verticalAlignment: Text.AlignVCenter
                }

                Slider {
                    id: rotSlider
                    stepSize: 1
                    to: 360
                    value: 0.5

                    onValueChanged: {
                        requestRotationTransform(value)
                    }
                }
            }
        }
    }
}
