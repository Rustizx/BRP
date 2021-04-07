import { combineEpics } from "redux-observable";
import apiEpics from "./apiEpics";
import launchEpics from "./launchEpics";

export default combineEpics(apiEpics);