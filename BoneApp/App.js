import 'react-native-gesture-handler';
import React, { Component } from 'react';
import {
  Text,
  View,
  LogBox,
} from 'react-native';
import { Provider } from "react-redux";
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import Icon from 'react-native-vector-icons/FontAwesome5';

import store from "./app/redux/store";

import HomeStackScreen from './app/stacks/HomeStack';
import SettingsStackScreen from './app/stacks/SettingsStack';
import TelemetryStackScreen from './app/stacks/TelemetryStack';


LogBox.ignoreLogs(['RTCBridge']);

const Tab = createBottomTabNavigator();

class App extends Component {
  render () {
    return (
      <Provider store={store()}>
        <NavigationContainer>
          <Tab.Navigator
            screenOptions={({ route }) => ({
              tabBarIcon: ({ focused, color, size }) => {
                let iconName;

                if (route.name === 'Home') {
                  iconName = "home";
                } else if (route.name == "Telemetry") {
                  iconName = "sliders-h";
                } else if (route.name === 'Settings') {
                  iconName = "wrench";
                }
                return <Icon name={iconName} size={size} color={color} />;
              },
            })}
            tabBarOptions={{
              activeTintColor: 'tomato',
              inactiveTintColor: 'gray',
          }}>
            <Tab.Screen name="Home" component={HomeStackScreen} />
            <Tab.Screen name="Telemetry" component={TelemetryStackScreen} />
            <Tab.Screen name="Settings" component={SettingsStackScreen} />
          </Tab.Navigator>
        </NavigationContainer>
      </Provider>
    );
  }
};

export default App;
