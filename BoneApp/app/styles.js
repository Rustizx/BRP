import { StyleSheet, Dimensions } from "react-native";

const { screenWidth, screenHeight } = Dimensions.get('window');

const mainStyles = StyleSheet.create({
  background: {
    flex: 10,
    flexDirection: "column",
    alignItems: 'center',
    alignContent: "space-between",
    justifyContent: "flex-start",
  },
  backgroundTele: {
    width: screenWidth,
    height: screenHeight,
    flexDirection: "column",
    alignItems: 'center',
    alignContent: "space-between",
    justifyContent: "flex-start",
  },
  container: {
    flex: 1,
    alignItems: "center",
    justifyContent: "center",
  }
});

export { mainStyles, screenWidth, screenHeight };