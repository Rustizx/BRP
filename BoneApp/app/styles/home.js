import { StyleSheet, Dimensions } from "react-native";

const { width, height } = Dimensions.get('window');
const boxWidth = width-(width/7);

const homeStyles = StyleSheet.create({
    logo: {
        width: '100%',
        height: undefined,
        aspectRatio: 1,
        resizeMode: 'contain'
    },
    homeContainer: {
        flex: 8,
        position: "relative",
        justifyContent: "center",
        alignItems: "center",
    },

    logoBox: {
        flex: 2,
        alignSelf: "center",
        alignContent: "center",
        justifyContent: "center",
        borderRadius: 10,
        marginTop: 30,
    },

    statusBox: {
        flex: 1,
        flexDirection: "row",
        width: boxWidth,
        backgroundColor: "tomato",
        alignSelf: "center",
        borderRadius: 10,
        marginTop: 5,
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
        width: 90,
        height: 85,
        resizeMode: "contain",
        alignSelf: "flex-start",
        position: "relative",
        left: 13,
        bottom: 3,
    },
    dataBox: {
        flex: 1,
        flexDirection: "row",
        width: boxWidth,
        backgroundColor: "gold",
        alignSelf: "center",
        alignItems: "flex-start",
        justifyContent: "space-around",
        borderRadius: 10,
        marginTop: 25,
    },
    dataName: {
        height: 50,
        width: 90,
        marginLeft: 15,
        resizeMode: "contain",
    },
    dataPoint: {
        color: "white",
        fontStyle: "normal",
        fontSize: 20,
    },
    launchBox: {
        flex: 1,
        flexDirection: "row",
        width: boxWidth,
        backgroundColor: "green",
        alignSelf: "center",
        borderRadius: 10,
        marginTop: 25,
    },
    launchButton: {
        flex: 1,
        backgroundColor: "#0095ff",
        borderRadius: 5,
        height: 15,
        width: 210,
        justifyContent: 'center',
        resizeMode: "contain",
        alignItems: 'center',
        margin: 15,
    },
    lockLaunchButton: {
        flex: 1,
        height: 50,
        width: 100,
        alignItems: 'flex-start',
    },
});

export { homeStyles };