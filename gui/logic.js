.import org.ewindow.ua 0.1 as UserAgent
.import org.ewindow 0.1 as EWindow

var ua = UserAgent.UserAgent
var currentCall = null

// advertise the freshly started window as available
ua.setPresence(ua.PRESENCE_OPEN)

ua.onConnected.connect(function (call) {
	console.warn("Connected to", call)
	currentCall = call
	ua.setPresence(ua.PRESENCE_BUSY)
})

ua.onRinging.connect(function(call) {
	console.info("RINGING!!!", call)
})

ua.onIncoming.connect(function (call) {
	// Display a popup dialog
	console.info("!!! Incoming Call !!!", call)

	if (currentCall) {
		console.info("denying call...")
		ua.hangup(call)
	} else {
		console.info("accepting call...")
		ua.accept(call)
	}
})

ua.onInterrupted.connect(function(call) {
	console.info("Call was interrupted, re-dialing ", call)
})


function make_connection(uri) {
	if (currentCall) {
		ua.hangup(currentCall)
	}

	ua.connect(uri)
}
