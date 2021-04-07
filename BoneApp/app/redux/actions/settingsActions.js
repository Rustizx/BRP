import { settings_setIP, test_flip, settings_changeCountdownTime } from "../types";

export const setIPAddress = ip => (
    {
        type: settings_setIP,
        payload: ip,
    }
);

export const changeCountdownTime = time => (
    {
        type: settings_changeCountdownTime,
        payload: time,
    }
);

export const testFlip = () => (
    {
        type: test_flip
    }
);