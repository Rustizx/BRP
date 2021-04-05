import React, { Component } from 'react';
import { Provider } from "react-redux";
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from "@react-navigation/bottom-tabs";
import Icon from '@expo/vector-icons/FontAwesome5';

import HomeScreen from "./app/screens/HomeScreen";
import TelemetryScreen from "./app/screens/TelemetryScreen";
import SettingsScreen from "./app/screens/SettingsScreen";

import store from "./app/redux/store";

const Tab = createBottomTabNavigator();

class App extends Component {
  render() {
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
            <Tab.Screen name="Home" component={HomeScreen} />
            <Tab.Screen name="Telemetry" component={TelemetryScreen} />
            <Tab.Screen name="Settings" component={SettingsScreen} />
          </Tab.Navigator>
        </NavigationContainer>
      </Provider>
    );
  };
}

export default App;
