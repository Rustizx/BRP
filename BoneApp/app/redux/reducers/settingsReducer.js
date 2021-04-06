import { settings_setIP, settings_storage_clearIPs, test_flip } from "../types";

const INITIAL_STATE = {
  test: "PING",
  ipAddress: "192.168.0.195",
  pastAddresses: [],
};

export default settingsReducer = (state = INITIAL_STATE, action) => {
  switch (action.type) {

    case settings_storage_clearIPs: // Clears the previously used IPs from state and the local storage
      return {
        ...state,
        pastAddresses: [],
      }

    case settings_setIP: // Sets the IP Address and Saves it into a list of used ones
      return {
        ...state,
        ipAddress: action.payload,
      };

    default:
      return state;
  }
};
