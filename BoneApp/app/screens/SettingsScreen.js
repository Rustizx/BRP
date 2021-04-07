import React, { Component } from 'react';
import { View, Text, Button, StyleSheet, TextInput } from "react-native";
import { connect } from "react-redux";
import { setIPAddress, changeCountdownTime, testFlip } from '../redux/actions/settingsActions';
import * as Haptics from "expo-haptics";

import { mainStyles } from "../styles";

class SettingsScreen extends Component {

    constructor (props) {
        super(props);
        this.state = {
            ipTextBox: null,
            timeTextBox: null,
        };
    }

    randomHex = () => {
        let letters = "0123456789ABCDEF";
        let color = "#";
        for (let i = 0; i < 6; i++) {
            color += letters[Math.floor(Math.random() * 16)];
        }
        return color;
    };

    setIP = () => {
        Haptics.notificationAsync(Haptics.NotificationFeedbackType.Success);
        this.props._setIPAddress(this.state.ipTextBox);
    };

    setTime = () => {
        Haptics.notificationAsync(Haptics.NotificationFeedbackType.Success);
        if(parseInt(this.state.timeTextBox) > 10) {
            this.props._changeCountdownTime(this.state.timeTextBox);
        } else {
            this.props._changeCountdownTime("11");
        }
    }

    render() {
        return (
            <View style={settingsStyle.main}>
                <View style={{ marginBottom: 4, height: "3.5%" }}></View>
                <View style={[settingsStyle.infoBoxSingle, { backgroundColor: "tomato"}]}>
                    <View style={[settingsStyle.xyzInfo, { marginHorizontal: 10, flexGrow: 1.5, backgroundColor: "teal"}]}>
                        <Text style={settingsStyle.title}>IP Address:</Text>
                    </View>
                    <View style={[settingsStyle.xyzInfo, { flexGrow: 3, backgroundColor: "skyblue"}]}>
                    <TextInput
                        style={{ height: 40, width: "85%", borderColor: 'gray', borderWidth: 1, borderRadius: 10 }}
                        onChangeText={text => this.setState({ipTextBox: text})}
                        value={this.state.ipTextBox}
                        placeholder={this.props.settingsState.ipAddress}
                    />
                    </View>
                    <View style={[settingsStyle.xyzInfo, { marginHorizontal: 10, flexGrow: 1, backgroundColor: "purple"}]}>
                        <Button onPress={() => {this.setIP()}} color="#FFFFFF" title="SET" />
                    </View>
                </View>
                <View style={[settingsStyle.infoBoxSingle, { backgroundColor: "tomato"}]}>
                    <View style={[settingsStyle.xyzInfo, { marginHorizontal: 10, flexGrow: 1.5, backgroundColor: "teal"}]}>
                        <Text style={settingsStyle.title}>Countdown Time:</Text>
                    </View>
                    <View style={[settingsStyle.xyzInfo, { flexGrow: 3, backgroundColor: "skyblue"}]}>
                    <TextInput
                        style={{ height: 40, width: "85%", borderColor: 'gray', borderWidth: 1, borderRadius: 10 }}
                        onChangeText={text => this.setState({timeTextBox: text})}
                        value={this.state.timeTextBox}
                        placeholder={this.props.settingsState.launchCountdownTime}
                    />
                    </View>
                    <View style={[settingsStyle.xyzInfo, { marginHorizontal: 10, flexGrow: 1, backgroundColor: "purple"}]}>
                        <Button onPress={() => {this.setTime()}} color="#FFFFFF" title="SET" />
                    </View>
                </View>
                <View style={[settingsStyle.infoBoxSingle]}></View>
                <View style={[settingsStyle.infoBoxSingle]}></View>
                <View style={[settingsStyle.infoBoxSingle]}></View>
                <View style={[settingsStyle.infoBoxSingle]}></View>
                <View style={[settingsStyle.infoBoxSingle]}></View>
                <View style={[settingsStyle.infoBoxSingle]}></View>
            </View>
        );
    };
}

const settingsStyle = StyleSheet.create({
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
    infoBoxSingle: {
        marginHorizontal: 5,
        marginVertical: 2.5,
        flexGrow: 1,
        flexBasis: 5,
        flexDirection: 'row',
        borderRadius: 10,
    },
    xyzInfo: {
        marginVertical: 10,
        alignItems: "center",
        justifyContent: "center",
        flexBasis: 5,
        borderRadius: 10,
    },
});

const mapDispatchToProps = dispatch => {
    return {
        _testFlip: () => dispatch(testFlip()),
        _setIPAddress: (ip) => dispatch(setIPAddress(ip)),
        _changeCountdownTime: (time) => dispatch(changeCountdownTime(time)),
    }
};

const mapStateToProps = state => ({
    telemetryState: state.telemetry.telemetry.telemetry,
    configState: state.telemetry.telemetry.config,
    statusState: state.telemetry.status,
    settingsState: state.settings
});

export default connect(mapStateToProps, mapDispatchToProps)(SettingsScreen);