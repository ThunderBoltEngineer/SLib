package slib.platform.android.call;

import android.annotation.TargetApi;
import android.os.Build;
import android.telecom.Call;
import android.telecom.InCallService;

import slib.platform.android.Logger;
import slib.platform.android.device.PhoneCall;

@TargetApi(Build.VERSION_CODES.M)
public class CallService extends InCallService {

	private static Call.Callback callback = new Call.Callback() {
		@Override
		public void onStateChanged(Call call, int state) {
			super.onStateChanged(call, state);
			try {
				if (state == Call.STATE_DISCONNECTED) {
					removeCall(call);
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	};

	@Override
	public void onCallAdded(Call call) {
		super.onCallAdded(call);
		try {
			int state = call.getState();
			if (state == Call.STATE_RINGING) {
				String callId = PhoneCall.registerCall(call);
				PhoneCall.onIncomingCall(callId, getPhoneNumber(call));
				call.registerCallback(callback);
			} else if (state == Call.STATE_CONNECTING) {
				String callId = PhoneCall.registerCall(call);
				PhoneCall.onOutgoingCall(callId, getPhoneNumber(call));
				call.registerCallback(callback);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	@Override
	public void onCallRemoved(Call call) {
		super.onCallRemoved(call);
		try {
			removeCall(call);
			call.unregisterCallback(callback);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static void removeCall(Call call) {
		String callId = PhoneCall.unregisterCall(call);
		if (callId != null) {
			PhoneCall.onEndCall(callId, getPhoneNumber(call));
		}
	}

	private static String getPhoneNumber(Call call) {
		Call.Details details = call.getDetails();
		return details.getHandle().getSchemeSpecificPart();
	}

}