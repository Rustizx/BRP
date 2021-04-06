import { applyMiddleware, combineReducers, createStore } from "redux";
import telemetryReducer from "./reducers/telemetryReducer";
import settingsReducer from "./reducers/settingsReducer";
import { createEpicMiddleware } from "redux-observable";
import rootEpic from "./epics/rootEpic";

const epicMiddleware = createEpicMiddleware();

const rootReducer = combineReducers({
    telemetry : telemetryReducer,
    settings : settingsReducer,
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