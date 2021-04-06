import { settings_setIP, test_flip } from "../types";

export const setIPAddress = ip => (
    {
        type: settings_setIP,
        payload: ip,
    }
);

export const testFlip = () => (
    {
        type: test_flip
    }
);