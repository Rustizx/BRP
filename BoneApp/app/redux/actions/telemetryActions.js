import { api_loadArch, api_loadingArch, api_loadedArch, api_loadArchFailed, telemetry_stopArchUpdate, telemetry_startArchUpdate, 
    api_loadedStatus, api_loadingStatus, api_loadStatus, api_loadStatusFailed, telemetry_startStatusUpdate, telemetry_stopStatusUpdate,
    telemetry_updateDate, telemetry_resetUpdate } from "../types";

// API Actions

// Status API
export const loadStatus = (requestBody) => ({ type: api_loadStatus, payload: { requestBody }});
export const loadingStatus = () => ({ type: api_loadingStatus });
export const loadedStatus = (response) => ({ type: api_loadedStatus, payload: { response }});
export const loadStatusFailed = (error) => ({ type: api_loadStatusFailed, payload: { error }});

export const startStatusUpdates = () => ({ type: telemetry_startStatusUpdate });
export const stopStatusUpdates = () => ({ type: telemetry_stopStatusUpdate });


// Arch API
export const loadArch = (requestBody) => ({ type: api_loadArch, payload: { requestBody }});
export const loadingArch = () => ({ type: api_loadingArch });
export const loadedArch = (response) => ({ type: api_loadedArch, payload: { response }});
export const loadArchFailed = (error) => ({ type: api_loadArchFailed, payload: { error }});

export const startArchUpdates = () => ({ type: telemetry_startArchUpdate });
export const stopArchUpdates = () => ({ type: telemetry_stopArchUpdate });

// Data Actions

export const updateData = (data) => (
    {
        type: telemetry_updateDate,
        payload: data,
    }
);

export const resetData = (mode = "all") => (
    {
        type: telemetry_resetUpdate,
        payload: mode,
    }
);