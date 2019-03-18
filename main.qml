import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtDataVisualization 1.0

Page {
    id: rootWindow
    visible: true
    height: 720
    width: 800

    property alias simProgressValue: simProgress.value
    property alias numCyclesSpinValue: numCyclesSpin.value
    property alias angleSpinValue: angleSpin.value
    property alias axisComboCurrentIndex: axisCombo.currentIndex

    property alias colorSliderValue: colorSlider.value
    property alias sizeSliderValue: sizeSlider.value
    property alias ptSize: scatterSeries.itemSize // vary this from 0.02 to 1
    property alias ptColor: scatterSeries.baseColor // vary this from 1 to 16777216 and convert to hex

    property alias interSpaceMax: originSpin.to
    property alias interSpace: originSpin.value
    property alias numLinePts: numPointsSpin.value
    property alias numRandPts: numRandPtsSpin.value
    property alias axisLimit: maxValSpin.value
    property alias numPtsMax: numPointsSpin.to
    property alias shapeIdx: shapeCombo.currentIndex

    signal sendCreateLine(var num_pts, var inter_space, var shape_idx)
    signal sendGeneratePts(var numPts, var maxValue)

    signal sendPlay(var angle, var axis, var num_cycles)
    signal sendPause()
    signal sendCancel()

    signal requestSizeTransform(var size)
    signal requestColorTransform(var color)
    signal requestRotationTransform(var angle, var axis)

    function createLine() {
        console.log("(onCreateLine) Creating line with " + numLinePts + " points; spaced at " + interSpace);
        sendCreateLine(numLinePts, interSpace, shapeIdx)
        if(axisLimit < numLinePts*1.2)
            axisLimit = numLinePts*1.2
    }

    function generatePts() {
        console.log("(generatePts) Generating " + numRandPts + " random points with maximum " + axisLimit );
        sendGeneratePts(numRandPts, axisLimit)
    }

    function onApplySizeUpdate(size){
        console.log("(onApplySizeUpdate) Size: " + size)
        ptSize = size
    }

    function onApplyColorUpdate(color){
        console.log("(onApplyColorUpdate) Color: " + color)
        ptColor = color
    }

    function onUpdateSimStatus(numThreads, progressVal, simStatus){

        console.log("(onUpdateSimStatus) sim. status: " + simStatus)

        if(simStatus === 1){//RUNNING
            playBtn.text = "||"
            progressText.visible = true
            simProgress.visible = true
            console.log("(onUpdateSimStatus) num threads: " + numThreads + " progress: " + progressVal*100 + "% sim. status: " + simStatus)
            simProgressValue = progressVal
        }else if(simStatus === 2){//PAUSED
            playBtn.text = ">"
        }else if(simStatus === 3){//FINISHED
            playBtn.text = ">"
        }else{
            playBtn.text = ">"
        }
    }

    title: qsTr("Point Manipulator")

    ColumnLayout{

        Layout.fillWidth: true

        RowLayout{
            id: inputControl
            Layout.fillWidth: true

            Label {
                text: qsTr("Line length (#points)")
                verticalAlignment: Text.AlignVCenter
            }

            SpinBox {
                id: numPointsSpin
                to: 9999
                editable: true
                from: 1
                value: 100
            }

            Label {
                text: qsTr("Origin")
                verticalAlignment: Text.AlignVCenter
            }

            SpinBox {
                id: originSpin
                editable: true
                from: -99
                value: 0
            }

            ComboBox {
                id: shapeCombo
                displayText: "Orientation"
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
                text: qsTr("Generate")
                spacing: 3

                onClicked: {
                    rootWindow.createLine()
                }
            }
        }

        RowLayout{
            id: inputControl2
            Layout.fillWidth: true

            Label {
                text: qsTr("Number of points")
                verticalAlignment: Text.AlignVCenter
            }

            SpinBox {
                id: numRandPtsSpin
                to: 9999
                editable: false
                from: 1
                value: 100
            }

            Label {
                text: qsTr("Maximum value")
                verticalAlignment: Text.AlignVCenter
            }

            SpinBox {
                id: maxValSpin
                to: 9999
                editable: true
                from: 1
                value: 10
            }

            Button {
                id: randPtsButton
                text: qsTr("Generate")
                spacing: 3

                onClicked: {
                    rootWindow.generatePts()
                }
            }
        }

        Item {
            id: dataView
            height: rootWindow.height/2
            Layout.fillWidth: true

            Theme3D {
                id: themeIsabelle
                type: Theme3D.ThemeIsabelle
                font.family: "Lucida Handwriting"
                font.pointSize: 40
            }

            Scatter3D {
                id: scatterGraph
                width: dataView.width
                height: dataView.height
                theme: themeIsabelle
                shadowQuality: AbstractGraph3D.ShadowQualitySoftLow

                axisX: ValueAxis3D{max: axisLimit;  min: -axisLimit}
                axisY: ValueAxis3D{max: axisLimit;  min: -axisLimit}
                axisZ: ValueAxis3D{max: axisLimit;  min: -axisLimit}

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
            Layout.fillHeight: true
            Layout.fillWidth: true

            RowLayout{
                id: manualAppControls

                RowLayout{
                    id: sizeControl
                    width: inputControl.width/2
                    height: 20

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
                    width: inputControl.width/2
                    height: 20

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
            }

            RowLayout{
                id: manualRotControls

                RowLayout{
                    id: xRotControl
                    width: inputControl.width/3
                    height: 20

                    Label {
                        text: qsTr("Rotate X")
                        verticalAlignment: Text.AlignVCenter
                    }

                    Slider {
                        id: xRotSlider
                        stepSize: 1
                        to: 360
                        value: 0.1

                        onValueChanged: {
                            requestRotationTransform(value, 0)
                        }
                    }
                }

                RowLayout{
                    id: yRotControl
                    width: inputControl.width/3
                    height: 20

                    Label {
                        text: qsTr("Rotate Y")
                        verticalAlignment: Text.AlignVCenter
                    }

                    Slider {
                        id: yRotSlider
                        stepSize: 1
                        to: 360
                        value: 0.1

                        onValueChanged: {
                            requestRotationTransform(value, 1)
                        }
                    }
                }

                RowLayout{
                    id: zRotControl
                    width: inputControl.width/3
                    height: 20

                    Label {
                        text: qsTr("Rotate Z")
                        verticalAlignment: Text.AlignVCenter
                    }

                    Slider {
                        id: zRotSlider
                        stepSize: 1
                        to: 360
                        value: 0.1

                        onValueChanged: {
                            requestRotationTransform(value, 2)
                        }
                    }
                }

            }

            RowLayout {
                id: autoRotControls
                width: 100
                height: 100

                ComboBox {
                    id: axisCombo

                    model: ListModel {
                        id: axisModel

                        ListElement { name: "X-AXIS" }
                        ListElement { name: "Y-AXIS" }
                        ListElement { name: "Z-AXIS" }
                        ListElement { name: "ALL-AXES" }
                    }
                    displayText: "Rotation axis"
                }

                Label {
                    text: qsTr("Rotation angle")
                    verticalAlignment: Text.AlignVCenter
                }

                SpinBox {
                    id: angleSpin
                    to: 360
                    editable: true
                    from: 1
                    value: 15
                }

                Label {
                    text: qsTr("Num cycles")
                    verticalAlignment: Text.AlignVCenter
                }

                SpinBox {
                    id: numCyclesSpin
                    to: 1000
                    editable: true
                    from: 1
                    value: 1
                }

                Button{
                    id:playBtn
                    text: ">"

                    onClicked: {
                        if (text === ">"){
                            sendPlay(angleSpinValue, axisComboCurrentIndex, numCyclesSpinValue)
                        }
                        else if ( text === "||"){
                            sendPause()
                        }
                    }
                }

                Button {
                    id: stopBtn
                    text: "[]"

                    onClicked: {
                        sendCancel()
                    }
                }

            }

            RowLayout{
                id: progressRow

                Text {
                    id: progressText
                    text: qsTr("Simulating...")
                    visible: false
                }

                ProgressBar{
                    id: simProgress
                    value: 0
                    width: parent.width
                    height: 12
                    visible: false
                }
            }
        }
    }
}
