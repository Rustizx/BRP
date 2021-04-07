import { launch_launchRocket, launch_deploySupport } from "../actions/launchActions";

const INITIAL_STATE = {};

export default launchReducer = (state = INITIAL_STATE, action) => {
    switch (action.type) {

        case launch_launchRocket:
        return {
            ...state,
        }

        case launch_deploySupport: // Sets the IP Address and Saves it into a list of used ones
        return {
            ...state,
        };

        default:
            return state;
    }
};
  