/*
    Copyright (C) 2012 Collabora Ltd. <info@collabora.com>
      @author George Kiagiadakis <george.kiagiadakis@collabora.com>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
import QtQuick 1.0
import QtGStreamer 1.0


Rectangle {
    id: window
    width: 800
    height: 800

    Column {

        VideoItem {
            id: video

            width: window.width
            height: 520
            surface: videoSurface1 //bound on the context from main()
        }

        Row {
            id: buttons

            width: window.width
            height: 70
            spacing: 5

            Rectangle {
                id: playButton
                color: "black"

                width: 120
                height: 60

                Text { text: "Play"; color: "white"; anchors.centerIn: parent }
                MouseArea { anchors.fill: parent; onClicked: player.play() }
            }

            Rectangle {
                id: stopButton
                color: "black"

                width: 120
                height: 60

                Text { text: "Stop"; color: "white"; anchors.centerIn: parent }
                MouseArea { anchors.fill: parent; onClicked: player.stop() }
            }

            Rectangle {
                id: openButton
                color: "black"

                width: 120
                height: 60

                Text { text: "Open file"; color: "white"; anchors.centerIn: parent }
                MouseArea { anchors.fill: parent; onClicked: player.open() }
            }

        }
        Row {

            width: window.width
            height: 70
            spacing: 5

            Rectangle {
                id: positionText
                color: "black"

                width: 280
                height: 60

                Text { text: player.position; color: "white"; anchors.centerIn: parent }
            }

            Item {
                id: slider; width: 240; height: 60

                //

                signal positionChanged();

                // value is read/write.
                property real value: player.slider_position
                onValueChanged: updatePos();
                property real maximum: 1.0
                property real minimum: 0.0
                property int xMax: width - handle.width - 4
                onXMaxChanged: updatePos();
                onMinimumChanged: updatePos();

                function updatePos() {
                    if (maximum > minimum) {
                        var pos = 2 + (value - minimum) * slider.xMax / (maximum - minimum);
                        pos = Math.min(pos, width - handle.width - 2);
                        pos = Math.max(pos, 2);
                        handle.x = pos;
                    } else {
                        handle.x = 2;
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    border.color: "white"; border.width: 0; radius: 8
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#66343434" }
                        GradientStop { position: 1.0; color: "#66000000" }
                    }
                }

                Rectangle {
                    id: handle; smooth: true
                    y: 2; width: 10; height: slider.height-4; radius: 6
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "lightgray" }
                        GradientStop { position: 1.0; color: "gray" }
                    }

                    MouseArea {
                        id: mouse
                        anchors.fill: parent; drag.target: parent
                        drag.axis: Drag.XAxis; drag.minimumX: 2; drag.maximumX: slider.xMax+2
                        onPositionChanged: { player.sliderPositionChanged((slider.maximum - slider.minimum) * (handle.x-2) / slider.xMax + slider.minimum) }
                    }
                }
            }

            Rectangle {
                id: durationText
                color: "black"

                width: 280
                height: 60

                Text { text: player.duration; color: "white"; anchors.centerIn: parent }
            }

        }


        Item {
            id: thumbnail_progress; width: window.width; height: 32

            //

            signal positionChanged();

            // value is read/write.
            property real value: player.slider_position
            onValueChanged: updatePos();
            property real maximum: 1.0
            property real minimum: 0.0
            property int xMax: width - handle2.width - 4
            onXMaxChanged: updatePos();
            onMinimumChanged: updatePos();

            function updatePos() {
                if (maximum > minimum) {
                    var pos = 2 + (value - minimum) * thumbnail_progress.xMax / (maximum - minimum);
                    pos = Math.min(pos, width - handle2.width - 2);
                    pos = Math.max(pos, 2);
                    handle2.x = pos;
                } else {
                    handle2.x = 2;
                }
            }

            Rectangle {
                anchors.fill: parent
                border.color: "white"; border.width: 0; radius: 0
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#66343434" }
                    GradientStop { position: 1.0; color: "#66000000" }
                }
            }

            Row {
                width: window.width
                height: 32

                Image {
                    id: thumb0
                    source: "image://thumimgpro/0"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                Image {
                    id: thumb1
                    source: "image://thumimgpro/1"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                Image {
                    id: thumb2
                    source: "image://thumimgpro/2"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                Image {
                    id: thumb3
                    source: "image://thumimgpro/3"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                Image {
                    id: thumb4
                    source: "image://thumimgpro/4"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                Image {
                    id: thumb5
                    source: "image://thumimgpro/5"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                Image {
                    id: thumb6
                    source: "image://thumimgpro/6"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                Image {
                    id: thumb7
                    source: "image://thumimgpro/7"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                Image {
                    id: thumb8
                    source: "image://thumimgpro/8"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }

                Image {
                    id: thumb9
                    source: "image://thumimgpro/9"
                    function reload() {
                        var oldSource = source;
                        source = "";
                        source = oldSource;
                    }
                }
            }


            Rectangle {
                id: handle2; smooth: true
                y: 2; width: 2; height: thumbnail_progress.height-4; radius: 0
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "red" }
                    GradientStop { position: 1.0; color: "red" }
                }

                MouseArea {
                    id: mouse2
                    anchors.fill: parent; drag.target: parent
                    drag.axis: Drag.XAxis; drag.minimumX: 2; drag.maximumX: thumbnail_progress.xMax+2
                    onPositionChanged: { player.sliderPositionChanged((thumbnail_progress.maximum - thumbnail_progress.minimum) * (handle2.x-2) / thumbnail_progress.xMax + thumbnail_progress.minimum) }
                }
            }
        }

        Connections {
            target: thumbnail
            onUpdateThumb0: {
              thumb0.reload();
            }
            onUpdateThumb1: {
              thumb1.reload();
            }
            onUpdateThumb2: {
              thumb2.reload();
            }
            onUpdateThumb3: {
              thumb3.reload();
            }
            onUpdateThumb4: {
              thumb4.reload();
            }
            onUpdateThumb5: {
              thumb5.reload();
            }
            onUpdateThumb6: {
              thumb6.reload();
            }
            onUpdateThumb7: {
              thumb7.reload();
            }
            onUpdateThumb8: {
              thumb8.reload();
            }
            onUpdateThumb9: {
              thumb9.reload();
            }
        }

    }
}
