/* import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
*/
import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import AGL.Demo.Controls 1.0
import QtWebSockets 1.0
import io.qt.BackendStuff 1.0


ApplicationWindow {
        id: _root

         // WINDOW PROPERTIES
        visible: true
        width: 520
        height: 680
        color:"steelblue"

        //child-object will be displayed inside root-window
        Text {
                id: _MoTalk
                text: "This is Motalk-App"
                x: 100
                y: 10
                anchors.horizontalCenter: _root.horizontalCenter
                font.pointSize: 24; font.bold: true
        }


        Image {
                id: _backgroundFreq
                anchors.fill: parent
                width: parent.width
                height: parent.height
                source: "background_frequency.jpg"
                fillMode: Image.Tile; opacity: 0.3
        }
/*
        //declaring the root-object WebSocket, id "websocket"
        WebSocket {
            id: _websocket
            url: ws_binding     //aus main.cpp übernommen
            active: true
      }
      */
/*
        TextField{
            id: _recvtext
            x: 90; y: 400
            text: backend.recvtxt
        }
*/
        /*
                ColumnLayout {              //two ways for layouts: itemPositioner and QtQucikLayouts, last one more dynamic
                    anchors.fill: parent
                    //anchors.top: parent

                    ListView {
                        id: _listView
                        Layout.fillWidth: true      //take/fill as much space as available
                        Layout.fillHeight: true
                        Layout.margins: _pane.leftPadding + _textfield.leftPadding
                        displayMarginBeginning: 40      //these properties ensure that delegates outside the bounds of view don't disappear
                        displayMarginEnd: 40
                        //verticalLayoutDirection: ListView.BottomToTop       //make first item/message etc. appear at bottom of page
                        spacing: 10
                        model: 8
                        /*
                        delegate: Row {
                            readonly property bool sentByMe: index % 2 == 0     //dummy property using index of delegate to alternate btw authors

                            anchors.right: sentByMe ? _listView.contentItem.right : undefined   //messages from user apper on the right
                            spacing: 6

                            Rectangle {
                                id: _avatar
                                width: height
                                height: parent.height
                                color: "grey"
                                visible: !sentByMe      //only show if message was sent by contact
                            }

                            Rectangle {
                                width: 80
                                height: 40
                                color: sentByMe ? "lightgrey" : "steelblue" //change color depending on author

                                Label {
                                    anchors.centerIn: parent
                                    text: index
                                    color: sentByMe ? "black" : "white"
                                }
                            }
                        }

                        ScrollBar.vertical: ScrollBar{}
                    }
                } */
        BackendStuff {
            id: _afbBackend
            //onBufferChanged: { }
        }

                    Keyboard{
                        id: _keyboard
                        height: 200
                        width: 500
                        x: 90
                        y: 150
                        visible: true
                    }


                    Rectangle{
                        id: _textRect
                        Layout.fillWidth: true
                        x: 90
                        y: 400
                        Text{
                            //text: "blablub"
                            text: _afbBackend.receiveBuffer
                            font.pointSize: 14
                            color: "black"
                        }

                    }

                    Rectangle {  //Pane is a rectangle whose color comes from the application's style, similar to Frame, without stroke around its border.
                        id: _rect
                        Layout.fillWidth: true
                        x: 90
                        y: 600


                        RowLayout {
                            width: parent.width

                            TextField {
                                id: _textfield
                                y: 640
                                width: 120
                                height: 45
                                //topPadding: 8
                                font.pointSize: 14
                                //bottomPadding: 16

                                placeholderText: "User Input"
                                renderType: Text.QtRendering
                            }

                            Button {
                                id: _sendButton
                                text: "Send"
                                //enabled: _textfield.length > 0   //only enable when message to send
                                enabled: true
                                onClicked:
                                {
                                    //afbBackend.text = _textfield.text;
                                    //afbBackend.sendTextMessage(afbBackend.text)
                                    _afbBackend.sendMessage("helloworld","ping", true);
                                }
                            }

                            Button {
                                id: _connectButton
                                text: "connect AFB-ws"
                                enabled: true
                                onClicked:
                                {
                                    _afbBackend.connectClicked();
                                    _afbBackend.sendClicked("GET /api?token=123456 HTTP/1.1\r\nHost: 127.0.0.1:1234\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: MfpIVG082jFTV7SxTNNijQ==\r\nSec-WebSocket-Protocol: x-afb-ws-json1\r\nContent-Length: 0\r\n\r\n");
                                }
                            }

                            Button {
                                id: _disconnectButton
                                text: "disconnect AFB-ws"
                                enabled: true
                                onClicked:
                                {
                                    _afbBackend.disconnectClicked();
                                }

                            }
                        }

                }
              }


/*
Window {

    TextField {
        id: serverField1
        x: 15
        y: 46
        width: 120
        height: 45
        topPadding: 8
        font.pointSize: 14
        bottomPadding: 16
        placeholderText: "Server Ip"
        renderType: Text.QtRendering
        onTextChanged: backend.text = text
    }
} */
