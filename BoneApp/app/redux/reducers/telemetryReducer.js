import { api_loadArchFailed, api_loadedArch, api_loadedStatus, api_loadStatusFailed, telemetry_resetUpdate, test_flip } from "../types";

const defaultStatus = {
  bone: false,
  dash: false,
};

const defaultRocketInfo = {
  config : {
    version : null,
    orientationType : null,
    servoX_offset : null,
    servoY_offset : null,
    recalibrateGyro : null,
    gyroX_offset : null,
    gyroY_offset : null,
    gyroZ_offset : null,
    kP :null,
    kI : null,
    kD : null,
    attemptDataRecovery : null,
    tvcMountTroubleshoot : null
  },

  telemetry : {
    gyro : [],
    accel : [],
    ore : [],
    altitude : null,
    realAltitude : null,
    temperature : null,
    state : null,
    tvcPOS : [],
    pyro : [],
    error : [],
    pidx : [],
    pidy : [],
    pid : [],
    pwm : [],
    time : null,
    millis : null
  }
};

const INITIAL_STATE = {
  status: {
    ...defaultStatus
  },
  telemetry: {
    ...defaultRocketInfo
  }
};

export default telemetryReducer = (state = INITIAL_STATE, action) => {
  switch (action.type) {

    case api_loadedStatus:
      console.log("Connection Successful");
      return {
        ...state,
        status: {
          ...state.status,
          bone: action.payload.response.bone,
          dash: action.payload.response.dash,
        }
      }

    case api_loadStatusFailed:
      console.log("Connection Failed");
      return {
        ...state,
        status: {
          ...state.status,
          bone: false,
          dash: false,
        }
      }

    case api_loadedArch:
      console.log("rocketdata");
      return {
        ...state,
        telemetry: {
          ...state.telemetry,
          ...action.payload.response
        }
      }
      
      return {...state}

    case api_loadArchFailed:
      console.log("rocketfail");
      //console.log(action.payload);
      return {...state}

    case telemetry_resetUpdate: // Resets the State to Defaults
      if (action.payload == "telemetry"){
        return {
          ...state,
          telemetry: {
            ...defaultRocketInfo
          }
        };
      } if (action.payload == "status"){
        return {
          ...state,
          status: {
            ...defaultStatus
          }
        };
      } if (action.payload == "all") {
        return {
          ...defaultStatus,
          ...defaultRocketInfo
        };
      }

    default:
      return state
  }
};
