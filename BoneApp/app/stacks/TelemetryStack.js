import React from 'react';
import { createStackNavigator } from '@react-navigation/stack';

import TelemetryScreen from '../screens/TelemetryScreen';

const TelemetryStack = createStackNavigator();

export default function TelemetryStackScreen(){
    return (
        <TelemetryStack.Navigator>
            <TelemetryStack.Screen name="Telemetry" component={TelemetryScreen} options={{ headerShown: false }} />
        </TelemetryStack.Navigator>
    );
}