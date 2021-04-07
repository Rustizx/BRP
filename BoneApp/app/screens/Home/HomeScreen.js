import React, { Component } from 'react';
import { View, Text, Button, ImageBackground, Image, Alert, StyleSheet } from "react-native";
import { TouchableWithoutFeedback } from 'react-native-gesture-handler';
import { connect } from "react-redux";
import * as Haptics from "expo-haptics";
import Constants from 'expo-constants';

import { homeStyles } from "../../styles/home";

import { startStatusUpdates, stopStatusUpdates } from "../../redux/actions/telemetryActions";
import { testFlip } from '../../redux/actions/settingsActions';

class HomeScreen extends Component {
    constructor(props) {
        super(props);
        this.state = {
            debug: true,
            text: "nothing",
            isLaunchLocked: true,
            isPolling: false,

            voltageData: "N/A",
            xPosData: "N/A",
            yPosData: "N/A",
        };
        
        this.launchAlert = this.launchAlert.bind(this);
        this.toggleLaunchButton = this.toggleLaunchButton.bind(this);

        //this.togglePolling = this.togglePolling.bind(this);
    }

    launch = () => {
        console.log("Launching Rocket");
        this.lockLaunch();
        this.props.navigation.navigate("Launch");
    }

    launchAlert = () => {
        if (this.props.statusState.dash || this.state.debug) {
            if(!this.state.isLaunchLocked){
                Haptics.notificationAsync(Haptics.NotificationFeedbackType.Warning);
                Alert.alert(
                    "Launching Rocket",
                    "Are you sure you wish to launch?",
                    [
                        {
                        text: "Cancel",
                        onPress: () => {this.lockLaunch()},
                        style: "cancel"
                        },
                        { text: "Yes", onPress: () => {this.launch()} }
                    ],
                    { cancelable: false }
                    );
            } else {
                Haptics.notificationAsync(Haptics.NotificationFeedbackType.Error);
                Alert.alert(
                    "Launch Error",
                    "The Launch Button is Currently Locked",
                    [
                        { text: "Ok" }
                    ],
                    { cancelable: false }
                    );
            }
        } else {
            Haptics.notificationAsync(Haptics.NotificationFeedbackType.Error);
            Alert.alert(
                "Launch Error",
                "Dash is not online",
                [
                    { text: "Ok" }
                ],
                { cancelable: false }
                );
            this.lockLaunch();
        }
    }

    unlockLaunchAlert = () => {
        Haptics.notificationAsync(Haptics.NotificationFeedbackType.Warning);
        Alert.alert(
            "Unlocking Launch",
            "Are you sure you wish to unlock launch?",
            [
                {
                text: "Cancel",
                onPress: () => {this.lockLaunch()},
                style: "cancel"
                },
                { text: "Yes", onPress: () => {this.unlockLaunch()} }
            ],
            { cancelable: false }
        );
    }

    lockLaunch = () => {
        this.setState({
            isLaunchLocked: true,
        });
        console.log("lock");
    }

    unlockLaunch = () => {
        this.setState({
            isLaunchLocked: false,
        });
        console.log("unlock");
    }

    toggleLaunchButton = () => {
        if (this.props.statusState.dash || this.state.debug) {
            if (!this.state.isLaunchLocked) {
                Haptics.notificationAsync(Haptics.NotificationFeedbackType.Success);
                this.lockLaunch();
            } else {
                this.unlockLaunchAlert();
            }
        } else {
            Haptics.notificationAsync(Haptics.NotificationFeedbackType.Error);
            Alert.alert(
                "Launch Error",
                "Dash is not online",
                [
                    { text: "Ok" }
                ],
                { cancelable: false }
                );
            this.lockLaunch();
        }
    }

    togglePolling = () => {
        Haptics.notificationAsync(Haptics.NotificationFeedbackType.Success);
        if (this.state.isPolling) {
            this.props._stopStatusPoll();
            this.setState({ isPolling: false });
        } else {
            this.props._startStatusPoll();
            this.setState({ isPolling: true });
        }
    }

    render() {
        return(
            <ImageBackground style={style.background} source={require("../../assets/background.png")}>
                { /* <View style={{ marginBottom: 4, flexGrow: 1, flexShrink: 1, flexBasis: 10 }}></View> */}
                <View style={style.logoBox}>
                    <Image style={style.logoImage} source={require("../../assets/logo.png")} />
                </View>
                <View style={style.statusBox}>
                    <View style={{flex: 1, flexDirection:"column", justifyContent: "space-evenly",}}>
                        <Image style={style.statusName} source={require("../../assets/dash.png")} />
                        <Image style={style.statusName} source={require("../../assets/bone.png")} />
                    </View>
                    <View style={{flex: 1, flexDirection:"column", justifyContent: "space-evenly", paddingRight: 35,}}>
                        <Image style={style.status} source={this.props.statusState.dash ? require("../../assets/onlineStatus.png") : require("../../assets/offlineStatus.png")} />
                        <Image style={style.status} source={this.props.statusState.bone ? require("../../assets/onlineStatus.png") : require("../../assets/offlineStatus.png")} />
                    </View>
                    <View style={style.connectButton}>
                        <View style={{flexDirection: "row", justifyContent: "center"}}>
                            <TouchableWithoutFeedback onPress={() => {this.togglePolling()}}>
                                <Image style={style.connectButtonImage} source={this.state.isPolling ? require("../../assets/connect.png") : require("../../assets/stopconnect.png")} />
                            </TouchableWithoutFeedback>
                        </View>
                    </View>
                </View>
                <View style={style.dataBox}>
                    <View style={{flexDirection:"column", alignItems:"center"}}>
                        <Image style={homeStyles.dataName} source={require("../../assets/voltage.png")} />
                        <Text style={homeStyles.dataPoint}>{this.props.telemetryState.temperature}</Text>
                    </View>
                    <View style={{flexDirection:"column", alignItems:"center"}}>
                        <Image style={homeStyles.dataName} source={require("../../assets/xpos.png")} />
                        <Text style={homeStyles.dataPoint}>{this.props.telemetryState.gyro[0]}</Text>
                    </View>
                    <View style={{flexDirection:"column", alignItems:"center"}}>
                    <Image style={homeStyles.dataName} source={require("../../assets/ypos.png")} />
                        <Text style={homeStyles.dataPoint}>{this.props.telemetryState.gyro[1]}</Text>
                    </View>
                </View>
                <View style={style.launchBox}>
                    <View style={style.launchButton}>
                        <View style={{flexDirection: "row", justifyContent: "center"}}>
                            <TouchableWithoutFeedback onPress={this.launchAlert}>
                                <Image style={style.launchButtonImage} source={this.state.isLaunchLocked ? require("../../assets/launchLocked.png") : require("../../assets/launch.png")}/>
                            </TouchableWithoutFeedback>
                        </View>
                    </View>
                    <View style={style.lockLaunchButton}>
                        <View style={{flexDirection: "row", justifyContent: "center"}}>
                            <TouchableWithoutFeedback onPress={this.launchLocked = this.toggleLaunchButton}>
                                <Image style={style.launchButtonImage} source={this.state.isLaunchLocked ? require("../../assets/lockedButton.png") : require("../../assets/unlockedButton.png")}/>
                            </TouchableWithoutFeedback>
                        </View>
                    </View>
                </View>
                <View style={{flex: 0.8}}></View>
            </ImageBackground>
        );
    }
}

const style = StyleSheet.create({
    background: {
        flex: 1,
        justifyContent: "space-evenly",
        width:"100%",
        height:"100%",
    },
    logoBox: {
        paddingTop: Constants.statusBarHeight,
        flex: 0.8,
        alignSelf: "center",
        alignContent: "center",
        justifyContent: "center",
        borderRadius: 10,
    },
    logoImage: {
        width: "100%",
        height: undefined,
        aspectRatio: 1,
        resizeMode: 'contain'
    },
    statusBox: {
        backgroundColor: "tomato",
        marginHorizontal: 10,
        marginVertical: 10,
        flexGrow: 0.5,
        flexShrink: 2,
        flexBasis: 8,
        flexDirection: 'row',
        borderRadius: 20,
        alignContent: "center",
        justifyContent: "space-evenly",
    },
    statusName: {
        height: 50,
        width: 75,
        marginLeft: 15,
        resizeMode: "contain",
        justifyContent: 'center',
        alignItems: 'center',
        justifyContent: "space-evenly",
    },
    status: {
        height: 50,
        width: 120,
        justifyContent: 'center',
        resizeMode: "contain",
        alignItems: 'center',
        position: "relative",
        left: -7,
    },
    connectButton: {
        flex: 1,
        borderRadius: 20,
        marginHorizontal: 20,
        marginVertical: 10,
        padding: 1,
        backgroundColor: "#03a9f4",
        justifyContent: "center",
        alignItems: "center",
    },
    connectButtonImage: {
        width: "100%",
        height: undefined,
        aspectRatio: 1,
        resizeMode: 'contain'
    },
    dataBox: {
        backgroundColor: "gold",
        marginHorizontal: 10,
        marginVertical: 10,
        flexGrow: 0.5,
        flexShrink: 2,
        flexBasis: 8,
        flexDirection: 'row',
        borderRadius: 20,
        alignContent: "center",
        justifyContent: "space-evenly",
    },
    launchBox: {
        backgroundColor: "green",
        marginHorizontal: 10,
        marginVertical: 10,
        flexGrow: 0.5,
        flexShrink: 2,
        flexBasis: 8,
        flexDirection: 'row',
        borderRadius: 20,
        alignContent: "center",
        justifyContent: "space-evenly",
    },
    launchButton: {
        flex: 2,
        backgroundColor: "#03a9f4",
        borderRadius: 25,
        justifyContent: 'center',
        alignItems: 'center',
        alignContent: 'center',
        marginLeft: 10,
        marginRight: 5,
        marginVertical: 10,
    },
    lockLaunchButton: {
        flex: 1,
        backgroundColor: "#03a9f4",
        borderRadius: 25,
        justifyContent: 'center',
        alignItems: 'center',
        alignContent: 'center',
        marginLeft: 5,
        marginRight: 10,
        marginVertical: 10,
    },
    launchButtonImage: {
        width: "80%",
        height: undefined,
        aspectRatio: 1,
        resizeMode: 'contain',
        alignSelf: "center",
    },
});

const mapDispatchToProps = dispatch => {
    return {
        _testflip: () => dispatch(testFlip()),
        _startStatusPoll: () => dispatch(startStatusUpdates()),
        _stopStatusPoll: () => dispatch(stopStatusUpdates())
    }
};

const mapStateToProps = state => ({
    telemetryState: state.telemetry.telemetry.telemetry,
    configState: state.telemetry.telemetry.config,
    statusState: state.telemetry.status,
    settingsState: state.settings
});

export default connect(mapStateToProps, mapDispatchToProps)(HomeScreen);