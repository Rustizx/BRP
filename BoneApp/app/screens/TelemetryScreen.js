import React, { Component } from 'react';
import { View, Text, StyleSheet, ImageBackground } from "react-native";
import { connect } from "react-redux";

import { mainStyles } from "../styles";



class TelemetryScreen extends Component {

    randomHex = () => {
        let letters = "0123456789ABCDEF";
        let color = "#";
        for (let i = 0; i < 6; i++) {
            color += letters[Math.floor(Math.random() * 16)];
        }
        return color;
    };

    round = (num) => {
        return num.toFixed(2);
    }


    render() {
        return (
            <View style={teleStyle.main}>
                    <View style={{ marginBottom: 4, flexGrow: 1, flexShrink: 1, flexBasis: 10 }}></View>
                    <View style={[ teleStyle.infoBoxSingle, { backgroundColor: "tomato" }, ]}>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                            <Text style={ teleStyle.title }>Gyro: </Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.gyro[0]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.gyro[1]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.gyro[2]}</Text>
                        </View>
                    </View>
                    <View style={[ teleStyle.infoBoxSingle, { backgroundColor: "tomato" }, ]}>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                            <Text style={ teleStyle.title }>Accel: </Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.accel[0]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.accel[1]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.accel[2]}</Text>
                        </View>
                    </View>
                    <View style={[ teleStyle.infoBoxSingle, { backgroundColor: "tomato" }, ]}>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                            <Text style={ teleStyle.title }>Ore: </Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.ore[0]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.ore[1]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.ore[2]}</Text>
                        </View>
                    </View>
                    <View style={[ teleStyle.infoBoxQuad,]}>
                        <View style={[ teleStyle.render, { backgroundColor: "#1ED670" },]}>
                            <View style={[ teleStyle.xyzInfo ]}>
                                <Text style={ teleStyle.title }> render </Text> 
                            </View>
                        </View>
                        <View style={[ teleStyle.halfInfo,]}>
                            <View style={[ teleStyle.smallInfoBoxTop, { backgroundColor: "#D42F76" }, ]}>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                                    <Text style={ teleStyle.title }>Alt: </Text>
                                </View>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                                    <Text>{this.props.telemetryState.altitude}</Text>
                                </View>
                            </View>
                            <View style={[ teleStyle.smallInfoBoxTop, { backgroundColor: "#D42F76" }, ]}>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                                    <Text style={ teleStyle.title }>R-Alt: </Text>
                                </View>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                                    <Text>{this.props.telemetryState.realAltitude}</Text>
                                </View>
                            </View>
                            <View style={[ teleStyle.smallInfoBoxTop, { backgroundColor: "#D42F76" }, ]}>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                                    <Text style={ teleStyle.title }>Temp: </Text>
                                </View>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                                    <Text>{this.props.telemetryState.temperature}</Text>
                                </View>
                            </View>
                            <View style={[ teleStyle.smallInfoBoxBottom, { backgroundColor: "#D42F76" }, ]}>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                                    <Text style={ teleStyle.title }>Volt: </Text>
                                </View>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                                    <Text>N/A</Text>
                                </View>
                            </View>
                        </View>
                    </View>
                    <View style={[ teleStyle.infoBoxTriple]}>
                        <View style={[ teleStyle.render, { backgroundColor: "#0054AD" },]}>
                            <View style={[ teleStyle.smallInfoBoxTop, { backgroundColor: "teal" }, ]}>
                                <View style={[ teleStyle.xyzInfo]}>
                                    <Text style={ teleStyle.title }>TVC POS </Text>
                                </View>
                            </View>
                            <View style={[ teleStyle.smallInfoBoxTop]}>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                                    <Text style={ teleStyle.title }>X: </Text>
                                </View>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                                    <Text>{this.props.telemetryState.tvcPOS[0]}</Text>
                                </View>
                            </View>
                            <View style={[ teleStyle.smallInfoBoxTop]}>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                                    <Text style={ teleStyle.title }>Y: </Text>
                                </View>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                                    <Text>{this.props.telemetryState.tvcPOS[1]}</Text>
                                </View>
                            </View>
                        </View>
                        <View style={[ teleStyle.halfInfo, { backgroundColor: "#0054AD" },]}>
                            <View style={[ teleStyle.smallInfoBoxTop, { backgroundColor: "teal" }, ]}>
                                <View style={[ teleStyle.xyzInfo]}>
                                    <Text style={ teleStyle.title }>PWM</Text>
                                </View>
                            </View>
                            <View style={[ teleStyle.smallInfoBoxTop]}>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                                    <Text style={ teleStyle.title }>X: </Text>
                                </View>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                                    <Text>{this.props.telemetryState.pwm[0]}</Text>
                                </View>
                            </View>
                            <View style={[ teleStyle.smallInfoBoxTop]}>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                                    <Text style={ teleStyle.title }>Y: </Text>
                                </View>
                                <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                                    <Text>{this.props.telemetryState.pwm[1]}</Text>
                                </View>
                            </View>
                        </View>
                    </View>
                    <View style={[ teleStyle.infoBoxSingle, { backgroundColor: "#6A4FD1" }, ]}>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                            <Text style={ teleStyle.title }>PIDX: </Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.pidx[0]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.pidx[1]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.pidx[2]}</Text>
                        </View>
                    </View>
                    <View style={[ teleStyle.infoBoxSingle, { backgroundColor: "#6A4FD1" }, ]}>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                            <Text style={ teleStyle.title }>PIDY: </Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.pidy[0]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.pidy[1]}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>{this.props.telemetryState.pidy[2]}</Text>
                        </View>
                    </View>
                    <View style={[ teleStyle.infoBoxSingle, { backgroundColor: "#6A4FD1" }, ]}>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                            <Text style={ teleStyle.title }>Pyro: </Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>1 / { this.props.telemetryState.pyro[0] ? "on" : "off"}</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "skyblue" }, ]}>
                            <Text>2 / { this.props.telemetryState.pyro[1] ? "on" : "off"}</Text>
                        </View>
                    </View>
                    <View style={[ teleStyle.infoBoxSingle, { backgroundColor: "#5829CF" }, ]}>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                            <Text style={ teleStyle.title }>State / { this.props.telemetryState.state }</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                            <Text style={ teleStyle.title }>Time / { this.props.telemetryState.time }</Text>
                        </View>
                        <View style={[ teleStyle.xyzInfo, { backgroundColor: "teal" }, ]}>
                            <Text style={ teleStyle.title }>Millis / { this.props.telemetryState.millis }</Text>
                        </View>
                    </View>
            </View>
        );
    };
}

const teleStyle = StyleSheet.create({
    title: {
        color: "white", 
        fontWeight: "bold"
    },
    main: {
        flex: 1,
        alignContent: "stretch",
        alignItems: "stretch",
        justifyContent: "space-evenly",
    },
    infoBoxQuad: {
        marginHorizontal: 5,
        marginVertical: 2.5,
        flexGrow: 10,
        flexShrink: 10,
        flexBasis: 28,
        flexDirection: 'row',
        borderRadius: 10,
    },
    infoBoxTriple: {
        marginHorizontal: 5,
        marginVertical: 2.5,
        flexGrow: 5,
        flexShrink: 4,
        flexBasis: 28,
        flexDirection: 'row',
        borderRadius: 10,
    },
    infoBoxSingle: {
        marginHorizontal: 5,
        marginVertical: 2.5,
        flexGrow: 1.5,
        flexShrink: 2,
        flexBasis: 28,
        flexDirection: 'row',
        borderRadius: 10,
    },
    smallInfoBoxTop: {
        marginBottom: 5,
        flexGrow: 2,
        flexShrink: 2,
        flexBasis: 28,
        flexDirection: 'row',
        borderRadius: 10,
    },
    smallInfoBoxBottom: {
        flexGrow: 2,
        flexShrink: 2,
        flexBasis: 28,
        flexDirection: 'row',
        borderRadius: 10,
    },
    xyzInfo: {
        margin: 10,
        alignItems: "center",
        justifyContent: "center",
        flexGrow: 1,
        flexShrink: 1,
        borderRadius: 10,
    },
    render: {
        alignItems: "center",
        justifyContent: "center",
        flexGrow: 1,
        flexShrink: 1,
        flexBasis: 2,
        borderRadius: 10,
    },
    halfInfo: {
        marginLeft: 5,
        alignItems: "center",
        justifyContent: "center",
        flexGrow: 1,
        flexShrink: 1,
        flexBasis: 2,
        borderRadius: 10,
    },
});

const mapDispatchToProps = dispatch => {
    return {
        _testFlip: () => dispatch(testFlip()),
    }
};

const mapStateToProps = state => ({
    telemetryState: state.telemetry.telemetry.telemetry,
    configState: state.telemetry.telemetry.config,
    statusState: state.telemetry.status,
    settingsState: state.settings
});

export default connect(mapStateToProps, mapDispatchToProps)(TelemetryScreen);