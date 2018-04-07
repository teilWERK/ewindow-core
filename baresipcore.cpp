#include <rtc_base/thread.h>
#include <api/peerconnectioninterface.h>
#include <api/audio_codecs/builtin_audio_decoder_factory.h>
#include <api/audio_codecs/builtin_audio_encoder_factory.h>
#include "media/engine/webrtcvideocapturerfactory.h"
#include "modules/video_capture/video_capture_factory.h"


#include <api/jsep.h>

#include "api/test/fakeconstraints.h"
#include "pc/test/peerconnectiontestwrapper.h"

#include <QDebug>

#include "baresipcore.h"
#include "baresipvidisp.h"

using namespace webrtc;

class MyPeerConnectionObserver
  : public webrtc::PeerConnectionObserver
{
	void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override {
		qInfo() << "signallingChange: new_state=" << new_state;
	}

	// Triggered when media is received on a new stream from remote peer.
	virtual void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {
		//qInfo() << "addStream: " << stream;
	}

  // Triggered when a remote peer close a stream.
  // Deprecated: This callback will no longer be fired with Unified Plan
  // semantics.
  virtual void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {
		//qInfo() << "removeStream: " << stream;
	}

  // Triggered when a remote peer opens a data channel.
  virtual void OnDataChannel(
      rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel)  override {
		qInfo() << "dataChannel: ";// << data_channel;
	};

  // Triggered when renegotiation is needed. For example, an ICE restart
  // has begun.
  virtual void OnRenegotiationNeeded()  override {
		qInfo() << "renegotiationNeeded: ";
	};

  // Called any time the IceConnectionState changes.
  //
  // Note that our ICE states lag behind the standard slightly. The most
  // notable differences include the fact that "failed" occurs after 15
  // seconds, not 30, and this actually represents a combination ICE + DTLS
  // state, so it may be "failed" if DTLS fails while ICE succeeds.
  virtual void OnIceConnectionChange(
      webrtc::PeerConnectionInterface::IceConnectionState new_state)  override {
		qInfo() << "iceConnectionChange: ";// << new_state;
	};

  // Called any time the IceGatheringState changes.
  virtual void OnIceGatheringChange(
      webrtc::PeerConnectionInterface::IceGatheringState new_state)  override {
		qInfo() << "iceGathingChange: ";// << new_state;
	};

  // A new ICE candidate has been gathered.
  virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate)  override {
		qInfo() << "iceCandidate: " ;//<< candidate;
	};

  // Ice candidates have been removed.
  // TODO(honghaiz): Make this a pure virtual method when all its subclasses
  // implement it.
  virtual void OnIceCandidatesRemoved(
      const std::vector<cricket::Candidate>& candidates) override {
		qInfo() << "iceCandidatesRemoved: " ;//<< candidates;
	}

  // Called when the ICE connection receiving status changes.
  virtual void OnIceConnectionReceivingChange(bool receiving) override {
		qInfo() << "iceConnectioNReceivingChange " ;//<< receiving;
	}

  // This is called when a receiver and its track are created.
  // TODO(zhihuang): Make this pure virtual when all subclasses implement it.
  // Note: This is called with both Plan B and Unified Plan semantics. Unified
  // Plan users should prefer OnTrack, OnAddTrack is only called as backwards
  // compatibility (and is called in the exact same situations as OnTrack).
  virtual void OnAddTrack(
      rtc::scoped_refptr<RtpReceiverInterface> receiver,
      const std::vector<rtc::scoped_refptr<MediaStreamInterface>>& streams) override {
		qInfo() << "addTrack: " << receiver, streams;
	}

  // This is called when signaling indicates a transceiver will be receiving
  // media from the remote endpoint. This is fired during a call to
  // SetRemoteDescription. The receiving track can be accessed by:
  // |transceiver->receiver()->track()| and its associated streams by
  // |transceiver->receiver()->streams()|.
  // Note: This will only be called if Unified Plan semantics are specified.
  // This behavior is specified in section 2.2.8.2.5 of the "Set the
  // RTCSessionDescription" algorithm:
  // https://w3c.github.io/webrtc-pc/#set-description
  virtual void OnTrack(
      rtc::scoped_refptr<RtpTransceiverInterface> transceiver) override {
		qInfo() << "track: ";// << transceiver;
	}

  // Called when a receiver is completely removed. This is current (Plan B SDP)
  // behavior that occurs when processing the removal of a remote track, and is
  // called when the receiver is removed and the track is muted. When Unified
  // Plan SDP is supported, transceivers can change direction (and receivers
  // stopped) but receivers are never removed, so this is never called.
  // https://w3c.github.io/webrtc-pc/#process-remote-track-removal
  // TODO(hbos,deadbeef): When Unified Plan SDP is supported and receivers are
  // no longer removed, deprecate and remove this callback.
  // TODO(hbos,deadbeef): Make pure virtual when all subclasses implement it.
  virtual void OnRemoveTrack(
      rtc::scoped_refptr<RtpReceiverInterface> receiver) override {
		qInfo() << "removeTrack: ";// << receiver;
	}
};

class SDPObserver : public webrtc::CreateSessionDescriptionObserver
{
	void OnSuccess(webrtc::SessionDescriptionInterface* desc) override {
		std::string sdp;
		desc->ToString(&sdp);
		qInfo() << "SDP success" << sdp.c_str();
		//assert(0);
	}

	void OnFailure(const std::string& error) override {
		qInfo() << "SDP Error: " << error.c_str();
		assert(0);
  }
};

static void sighandler(int signal) {
	exit(signal);
}


std::unique_ptr<cricket::VideoCapturer> OpenVideoCaptureDevice() {
  std::vector<std::string> device_names;
  {
    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
        webrtc::VideoCaptureFactory::CreateDeviceInfo());
    if (!info) {
      return nullptr;
    }
    int num_devices = info->NumberOfDevices();
    for (int i = 0; i < num_devices; ++i) {
      const uint32_t kSize = 256;
      char name[kSize] = {0};
      char id[kSize] = {0};
      if (info->GetDeviceName(i, name, kSize, id, kSize) != -1) {
        device_names.push_back(name);
      qInfo() << "Found device " << i << name;
      }
    }
  }

  cricket::WebRtcVideoDeviceCapturerFactory factory;
  std::unique_ptr<cricket::VideoCapturer> capturer;
  for (const auto& name : device_names) {
    capturer = factory.Create(cricket::Device(name, 0));
    if (capturer) {
      break;
    }
  }
  return capturer;
}




void BaresipCore::run()
{
    /*
    //BaresipVidisp::m_vidisp->moveToThread(this);
    int ret;
    const char* conf_path = "/etc/ewindow";
    libre_init();
    conf_path_set(conf_path);
    conf_configure();

    //log_enable_debug(true);
    log_enable_info(true);

    bool enable_tls = false;
    bool prefer_ipv6 = true;

    ret = baresip_init(conf_config(), prefer_ipv6);
    ret |= ua_init("baresip title", true, true, enable_tls, prefer_ipv6);
    if (ret) {
        puts("Error in ua_init(), exiting...");
        QCoreApplication::exit(ret);
        return;
    }

    conf_modules();

    // TODO: overwrite some basic conf/settings, like video_display?

	BaresipVidisp::register_vidisp();

//        tmr_init(&m_timer);
//        tmr_start(&m_timer, 100, re_callback, this);

    re_main(sighandler);
    * */
}


void BaresipCore::initWebRTC() {
	// Create PC Factory. Stuff like Video codecs plugs here
	static rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> pcfi =
		webrtc::CreatePeerConnectionFactory(
			webrtc::CreateBuiltinAudioEncoderFactory(),
			webrtc::CreateBuiltinAudioDecoderFactory()
		);

	// Prepare the media stream, our "payload" (audio and video)
	static rtc::scoped_refptr<webrtc::MediaStreamInterface> stream =
      pcfi->CreateLocalMediaStream("my_fancy_stream");


    cricket::AudioOptions ao;
    qInfo() << ao.ToString().c_str();
	rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
		pcfi->CreateAudioTrack(
		"audio_label", pcfi->CreateAudioSource(NULL /**/)));


	rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track;
	auto video_device(OpenVideoCaptureDevice());
	if (video_device) {
		video_track =
        pcfi->CreateVideoTrack(
            "video_label",
            pcfi->CreateVideoSource(std::move(video_device), NULL));
	}

	stream->AddTrack(audio_track);
	stream->AddTrack(video_track);

	static BaresipVidisp vidisp(video_track);
	emit BaresipCore::instance().newVideo(&vidisp);

	webrtc::PeerConnectionInterface::RTCConfiguration rtcconf;

	webrtc::FakeConstraints constraints;
	constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
							"false");

	MyPeerConnectionObserver observer;
	auto pci = pcfi->CreatePeerConnection(rtcconf, &constraints, 0 /*port allocator*/, 0/*cert generator*/, &observer);


	pci->AddStream(stream);

	auto sdpo = new rtc::RefCountedObject<SDPObserver>();
	pci->CreateOffer(sdpo, 0);

	assert(pci);

}
