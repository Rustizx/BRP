import React, { Component } from 'react';
import { View, Text, StyleSheet, ImageBackground, Animated, TouchableWithoutFeedback, } from "react-native";
import { connect } from "react-redux";
import { CountdownCircleTimer } from 'react-native-countdown-circle-timer'
import Constants from 'expo-constants';
import SoundPlayer from 'react-native-sound-player'
import fetchT from "../../functions/fetchWithTimeout";

import { startStatusUpdates, stopStatusUpdates } from "../../redux/actions/telemetryActions";
import { launchRocket, deploySupport } from "../../redux/actions/launchActions";
import { testFlip } from '../../redux/actions/settingsActions';


class LaunchScreen extends Component {
    constructor(props) {
        super(props);
        this.state = {
            timeLeft: parseInt(this.props.settingsState.launchCountdownTime),
        };

        this.timeLeft = parseInt(this.props.settingsState.launchCountdownTime);
    }

    back = () => {
      SoundPlayer.stop();
      this.props.navigation.navigate("Home");
    }

    updatetime = (time) => {
      //this.setState({timeLeft: (parseInt(time))});
      this.timeLeft = parseInt(time);
      if(this.timeLeft == 10){
        try {
          SoundPlayer.playSoundFile('tensecondcountdown', 'mp3')
        } catch (e) {
            console.log(`cannot play the sound file`, e)
        }
      }
      else if (this.timeLeft == 3) {
        this.sendSupportCommand();
      }
      else if (this.timeLeft == 1) {
        this.sendLaunchCommand();
      }
    }

    sendLaunchCommand = () => {
      fetchT(("http://" + this.props.settingsState.ipAddress + "/data"), {
        method: 'POST',
        body: JSON.stringify({
          action: "launch"
        }),
        headers: {
          'Content-type': 'application/json; charset=UTF-8',
        },
      }, 500)
        .then((response) => response.json())
        .then((json) => console.log(json))
        .catch((err) => {
            console.log(err);
        });
    }

    sendSupportCommand = () => {
      fetchT(("http://" + this.props.settingsState.ipAddress + "/data"), {
        method: 'POST',
        body: JSON.stringify({
          action: "support"
        }),
        headers: {
          'Content-type': 'application/json; charset=UTF-8',
        },
      }, 500)
        .then((response) => response.json())
        .then((json) => console.log(json))
        .catch((err) => {
            console.log(err);
        });
    }

    render() {
        return(
            <ImageBackground style={style.backgroundImage} source={require("../../assets/background.png")}>
                <View style={style.background}>
                    <View style={style.CountDownContainer}>
                      <CountdownCircleTimer
                        isPlaying
                        duration={parseInt(this.props.settingsState.launchCountdownTime)}
                        colors={[
                            ['#004777', 0.4],
                            ['#F7B801', 0.4],
                            ['#A30000', 0.2],
                        ]}
                      >
                        {({ remainingTime, animatedColor }) => {
                          if(!(this.timeLeft == parseInt(remainingTime))){
                            this.updatetime(remainingTime);
                            console.log(this.timeLeft);
                          }

                          return (
                            <Animated.Text
                              style={{ ...style.remainingTime, color: animatedColor }}>
                              {remainingTime}
                            </Animated.Text>
                          );
                        }}
                      </CountdownCircleTimer>
                    </View>
                    <View style={{flex: 1,}}>
                      <Text style={style.text}> Launching </Text>
                    </View>
                    <View style={style.backButton}>
                      <View style={style.backButtonButton}>
                        <TouchableWithoutFeedback onPress={this.back}>
                            <Text style={style.textButton}>CANCEL</Text>
                        </TouchableWithoutFeedback>
                      </View>
                    </View>
                </View>
            </ImageBackground>
        );
    }
}

const style = StyleSheet.create({
    backgroundImage: {
        flex: 1,
        justifyContent: "space-evenly",
        alignItems: "center",
        width:"100%",
        height:"100%",
    },
    background: {
      paddingTop: Constants.statusBarHeight, 
      flex: 1,
      justifyContent: "space-around",
    },  
    backButton: {
      flex: 1,
      justifyContent: "center", 
      borderRadius: 25, 
      margin: 20,
    },
    backButtonButton: {
      justifyContent: "center", 
      borderRadius: 25, 
      margin: 20,
      flexDirection: "row",
      backgroundColor: "#03a9f4"
    },
    text: {
      flex: 1,
      fontSize: 40,
      color: "white",
      fontFamily: "rubikmonoone-regular",
    },
    textButton: {
        flex: 1,
        fontSize: 40,
        color: "white",
        textAlign: "center",
        fontFamily: "rubikmonoone-regular",
    },
    CountDownContainer: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        padding: 8,
      },
      remainingTime: {
        fontSize: 50,
        fontWeight: "bold",
        textShadowColor:'#ffffff',
        textShadowOffset:{width: 10, height: 10},
        textShadowRadius:20,
      },
});

const mapDispatchToProps = dispatch => {
    return {
        _testflip: () => dispatch(testFlip()),
        _startStatusPoll: () => dispatch(startStatusUpdates()),
        _stopStatusPoll: () => dispatch(stopStatusUpdates()),
        _launchRocket: () => dispatch(launchRocket()),
        _deploySupport: () => dispatch(deploySupport()),
    }
};

const mapStateToProps = state => ({
    telemetryState: state.telemetry.telemetry.telemetry,
    configState: state.telemetry.telemetry.config,
    statusState: state.telemetry.status,
    settingsState: state.settings
});

export default connect(mapStateToProps, mapDispatchToProps)(LaunchScreen);