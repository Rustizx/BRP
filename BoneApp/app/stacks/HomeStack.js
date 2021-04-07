import React from 'react';
import { createStackNavigator } from '@react-navigation/stack';

import HomeScreen from '../screens/Home/HomeScreen';
import LaunchScreen from '../screens/Home/LaunchScreen';

const HomeStack = createStackNavigator();

export default function HomeStackScreen(){
    return (
        <HomeStack.Navigator>
            <HomeStack.Screen name="Home" component={HomeScreen} options={{ headerShown: false }} />
            <HomeStack.Screen name="Launch" component={LaunchScreen} options={{ headerShown: false }} />
        </HomeStack.Navigator>
    );
}