import { combineEpics } from "redux-observable";
import apiEpics from "./apiEpics";

export default combineEpics(apiEpics);