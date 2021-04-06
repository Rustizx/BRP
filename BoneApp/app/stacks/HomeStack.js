import React from 'react';
import { createStackNavigator } from '@react-navigation/stack';

import HomeScreen from '../screens/HomeScreen';

const HomeStack = createStackNavigator();

export default function HomeStackScreen(){
    return (
        <HomeStack.Navigator>
            <HomeStack.Screen name="Home" component={HomeScreen} options={{ headerShown: false }} />
        </HomeStack.Navigator>
    );
}