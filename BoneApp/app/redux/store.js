import { applyMiddleware, combineReducers, createStore } from "redux";
import telemetryReducer from "./reducers/telemetryReducer";
import settingsReducer from "./reducers/settingsReducer";
import launchReducer from "./reducers/launchReducer";
import { createEpicMiddleware } from "redux-observable";
import rootEpic from "./epics/rootEpic";

const epicMiddleware = createEpicMiddleware();

const rootReducer = combineReducers({
    telemetry : telemetryReducer,
    settings : settingsReducer,
    launch : launchReducer,
});

export default function configureStore () {
    const store = createStore(
      rootReducer,
      {},
      applyMiddleware(epicMiddleware)
    );

    epicMiddleware.run(rootEpic);
  
    return store;
};