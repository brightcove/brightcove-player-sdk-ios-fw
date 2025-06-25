# FreeWheel Plugin for Brightcove Player SDK for iOS, version 7.0.6.18

## Installation

The FreeWheel Plugin for Brightcove Player SDK provides a dynamic library framework and an XCFramework for installation.

The FreeWheel Plugin for Brightcove Player SDK has been integrated with version 6.52.0 of the FreeWheel AdManager framework which is available from the [AdManager SDK Integration Downloads](https://hub.freewheel.tv/display/techdocs/AdManager+SDK+Integration+Downloads) page.

### CocoaPods

You can use [CocoaPods][cocoapods] to add the FreeWheel Plugin for Brightcove Player SDK to your project. You can find the latest `Brightcove-Player-FreeWheel` podspec [here][podspecs]. To use this spec, add the following to the top of your Podfile: `source 'https://github.com/brightcove/BrightcoveSpecs.git'`.

The FreeWheel SDK **is not** included in this Pod.  You **must** manually add the FreeWheel SDK AdManager.framework to your project. The Pod will, however, add all the libraries required by AdManager.framework.

#### Framework example

```bash
source 'https://github.com/CocoaPods/Specs'
source 'https://github.com/brightcove/BrightcoveSpecs.git'

platform :ios, '12.0'
use_frameworks!

target 'MyVideoPlayer' do
  pod 'Brightcove-Player-FreeWheel'
end
```

#### XCFramework example

XCFrameworks can be installed by appending the `/XCFramework` subspec to the pod name.

```bash
source 'https://github.com/CocoaPods/Specs'
source 'https://github.com/brightcove/BrightcoveSpecs.git'

platform :ios, '12.0'
use_frameworks!

target 'MyVideoPlayer' do
  pod 'Brightcove-Player-FreeWheel/XCFramework'
end
```

When updating your installation, it's a good idea to refresh the local copy of your BrightcoveSpecs repository so that you have the latest podspecs locally, just like you would update your CococaPods master repository.

Typically if you use `pod update` in Terminal this will happen automatically, or alternatively you can update explicitly with `pod repo update brightcove`. (Your BrightcoveSpecs repository may have a different name if you explicitly added it to your list of podspecs repos.)

### Manual

To add the FreeWheel Plugin for Brightcove Player SDK to your project manually:

1. Install the latest version of the [Brightcove Player SDK][bcovsdk].
1. Download the latest zipped release of the plugin from our [release page][release].
1. Add the 'BrightcoveFW.framework' to your project.  You can do this by right-clicking on the Frameworks folder and choose "Add Files To" option and select the BrightcoveFW.framework from the path where it is stored.
1. On the "Build Settings" tab of your application target, ensure that the "Framework Search Paths" include the path to the framework. This should have been done automatically unless the framework is stored under a different root directory than your project.
1. On the "Build Phases" tab of your application target, add the following to the "Link
    Binary With Libraries" phase:
    * `BrightcoveFW.framework`
1. On the "Build Settings" tab of your application target:
    * Ensure that `-ObjC` has been added to the "Other Linker Flags" build setting.
1. Install the FreeWheel library, which must be retrieved from your FreeWheel account.
1. (**Universal Framework** only) On the "Build Phases" tab, add a "Run Script" phase with the command `bash ${BUILT_PRODUCTS_DIR}/${FRAMEWORKS_FOLDER_PATH}/BrightcoveFW.framework/strip-frameworks.sh`. Check "Run script only when installing". This will remove unneeded architectures from the build, which is important for App Store submission.
1. (**Apple Silicon** only) On the "Build Settings" tab of your application target:
    * Ensure that `arm64` has been added to your "Excluded Architectures" build setting for `Any iOS Simulator SDK`.
    
### Swift Package Manager

To add the FreeWheel Plugin for Brightcove Player SDK to your project with Swift Package Manager: 

1. First [follow the steps][corespm] to add the Core XCFramework with Swift Package Mananger.
1. Add the FreeWheel package to Swift Package Manager using `https://github.com/brightcove/brightcove-player-sdk-ios-fw.git`.
1. Link to the following libraries:
    * libxml2.tbd
    * CoreLocation
    * MessageUI
    * WebKit
1. On the "Build Settings" tab of your application target:
    * Ensure that `-ObjC` has been added to the "Other Linker Flags" build setting.
1. Install the FreeWheel library, which must be retrieved from your FreeWheel account.

[corespm]: https://github.com/brightcove/brightcove-player-sdk-ios#swift-package-manager

### Imports

The FreeWheel Plugin for Brightcove Player SDK can be imported using:

```swift
import BrightcoveFW
```

[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios/releases
[cocoapods]: http://cocoapods.org
[podspecs]: https://github.com/brightcove/BrightcoveSpecs/tree/master/Brightcove-Player-FreeWheel
[release]: https://github.com/brightcove/brightcove-player-sdk-ios-fw/releases

## Quick Start

Playing video with the Brightcove Player SDK for iOS with FreeWheel ads:

```swift
var adManager: FWAdManager?
@IBOutlet weak var videoContainerView: UIView!

func setup() {
    let policyKey = "<your-policy-key>"
    let accountId = "<your-account-id>"
    let videoId = "<your-video-id>"
    
[1] adManager = newAdManager()
    adManager?.setNetworkId(90750)
    
    let sdkManager = BCOVPlayerSDKManager.sharedManager()
[2] guard let playbackController = sdkManager.createFWPlaybackController(adContextPolicy: adContextPolicy(),
                                                                         viewStrategy: nil) else {
        return
    }
    view.addSubview(playbackController.view)
    
    let playbackService = BCOVPlaybackService(withAccountId: accountId,
                                              policyKey: policyKey)
    let configuration = [
        BCOVPlaybackService.ConfigurationKeyAssetID: videoId
    ]
    playbackService.findVideo(withConfiguration: configuration,
                              queryParameters: nil) { (video: BCOVVideo?,
                                                       jsonResponse: Any?,
                                                       error: Error?) in
        if let video {
            playbackController.setVideos([video])
            playbackController.play()
        }
    }
}

func adContextPolicy() -> BCOVFWSessionProviderAdContextPolicy {
    return { [weak self] (video: BCOVVideo?, source: BCOVSource?, videoDuration: TimeInterval) in
        
        guard let self,
              let adManager = self.adManager,
[3]           let adContext = adManager.newContext() else {
            return nil
        }

        let adRequestConfig = FWRequestConfiguration(serverURL: "http://demo.v.fwmrm.net",
                                                     playerProfile: "<player-profile>",
                                                     playerDimensions: videoContainerView.frame.size)
        adRequestConfig.siteSectionConfiguration = FWSiteSectionConfiguration(siteSectionId: "<site-section-id>",
                                                                              idType: .custom)
        adRequestConfig.videoAssetConfiguration = FWVideoAssetConfiguration(videoAssetId: "<video-asset-id>",
                                                                            idType: .custom, duration: videoDuration,
                                                                            durationType: .exact,
                                                                            autoPlayType: .attended)

        adContext.setVideoDisplayBase(videoContainerView)

        adRequestConfig.add(FWTemporalSlotConfiguration(customId: "preroll", adUnit: FWAdUnitPreroll, timePosition: 0.0))
        adRequestConfig.add(FWTemporalSlotConfiguration(customId: "midroll", adUnit: FWAdUnitPreroll, timePosition: videoDuration / 2))
        adRequestConfig.add(FWTemporalSlotConfiguration(customId: "postroll", adUnit: FWAdUnitPostroll, timePosition: 0.0))

        let bcovAdContext = BCOVFWContext(adContext: adContext,
                                          requestConfiguration: adRequestConfig)

        return bcovAdContext
    }
}
```

The code broken down into steps:

1. You create the same ad manager that you would create if you were using FreeWheel's iOS SDK directly; this will be required later.
1. The Brightcove FreeWheel Plugin adds some category methods to BCOVPlayerSDKManager. The first of these is `createFWPlaybackController(adContextPolicy:viewStrategy:)`. Use this method to create your playback controller. Alternatively (if you are using more than one session provider), you can create a BCOVFWSessionProvider and pass that to the SDK manager method that creates a playback controller with upstream session providers.
1. You create the same ad context that would create if you were using FreeWheel's iOS SDK directly, using the SDK manager created in step 1. This is where you would register for companion slots, turn on default ad controls, or any other settings you need to change. This block will get called before each new session is delivered.

**Note that BCOVFWSessionProvider is not tested for use with other advertising session providers, such as BCOVIMASessionProvider. Also note that BCOVFWSessionProvider should come after any other session providers in the chain passed to the manager when constructing the playback controller.**

## Play and Pause

The Brightcove FreeWheel Plugin implements custom play and pause logic to ensure the smoothest possible ad experience. Therefore, you will need to make sure that you use the play method on the `BCOVPlaybackController` or the `-[BCOVSessionProviderExtension fw_play]` or `-[BCOVSessionProviderExtension fw_pause]` ([BCOVSessionProviderExtension][BCOVFWComponent]), and not the AVPlayer.  

As an example, calling play for the first time on `BCOVPlaybackController` allows the Brightcove FreeWheel Plugin to process preroll ads without any of the content playing before the preroll.

[BCOVFWComponent]: https://github.com/brightcove/brightcove-player-sdk-ios-fw/blob/master/ios/BrightcoveFW.framework/Headers/BCOVFWComponent.h

## Seek Without Ads

Use `playbackController.seekWithoutAds(_,completionHandler:)` to resume playback at a specific time without forcing the user to watch ads scheduled before `seekToTime`.

In preparation for `seekWithoutAds(_,completionHandler:)`, disable `autoPlay` when setting up the `BCOVPlaybackController`.

Apple recommends waiting for the status of an AVPlayerItem to change to ready-to-play before using the AVPlayerItem. Therefore, call `seekWithoutAds(_,completionHandler:)` in the `kBCOVPlaybackSessionLifecycleEventReady` handler of the `playbackController:playbackSession:didReceiveLifecycleEvent` method of your `BCOVPlaybackControllerDelegate`.

```swift
func playbackController(_ controller: BCOVPlaybackController,
                        playbackSession session: BCOVPlaybackSession,
                        didReceive lifecycleEvent: BCOVPlaybackSessionLifecycleEvent) {

    if kBCOVPlaybackSessionLifecycleEventReady == lifecycleEvent.eventType {

        // self.resumePlayback is a hypothetical instance variable used here for illustration.
        if resumePlayback {
            // seek without playing ads which are scheduled before the seek time, i.e. resume playback.
            controller.seekWithoutAds(seekWithoutAdsValue) { [weak playbackController, weak self] finished in
                if !finished {
                    print("seekWithoutAds failed to finish")
                }

                // fade out the shutter to reveal the player view.
                playbackController?.shutterFadeTime = 0.25
                playbackController?.shutter = false

                // turn off seek without ads - especially important if this player is being used with a playlist
                self?.resumePlayback = false
            }
        }

    }
}
```

The `shutter` and `shutterFadeTime` properties of the `BCOVPlaybackController` can be used along with `seekWithoutAds:completionHandler:` to hide frame-flicker which can occur as the AVPlayer loads assets. In your BCOVPlaybackController set-up code, enable the shutter to hide the player view:

```swift
let sdkManager = BCOVPlayerSDKManager.sharedManager()
guard let playbackController = sdkManager.createFWPlaybackController(adContextPolicy: adContextPolicy(),
                                                                     viewStrategy: nil) else {
    return
}

playbackController.delegate = self

if resumePlayback {
    // set the shutter fade time to zero to hide the player view immediately.
    playbackController.shutterFadeTime = 0
    playbackController.shutter = true
    
    // disable autoPlay when resuming playback.
    playbackController.isAutoPlay = false
}
```

Note that when Seek Without Ads is used in your app, you might observe network traffic which normally occurs as part of setting up the FW plugin. This traffic is necessary for proper plugin setup, and does not affect the Seek Without Ads functionality.

## Customizing Plugin Behavior

You can customize default plugin behavior by creating an instance of `BCOVFWSessionProviderOptions` and overriding the default properties. To use a `BCOVFWSessionProviderOptions` options instance, you need to create the `BCOVFWSessionProvider` using `sdkManager.createFWSessionProvider(adContextPolicy:upstreamSessionProvider:options:)`.

```swift
let options = BCOVFWSessionProviderOptions()
options.cuePointProgressPolicy = BCOVCuePointProgressPolicy(processingCuePoints: .processFinalCuePoint,
                                                            resumingPlaybackFrom: .fromContentPlayhead,
                                                            ignoringPreviouslyProcessedCuePoints: true)
let sessionProvider = sdkManager.createFWSessionProvider(adContextPolicy: adContextPolicy(),
                                                         upstreamSessionProvider: nil,
                                                         options: options)

let playbackController = sdkManager.createPlaybackController(withSessionProvider: sessionProvider,
                                                             viewStrategy: nil)
```

### Preloading Slots

Depending on network conditions, there can be a delay in the time it takes for ads to play, once the ad position has been hit. In order to improve ad performance, it is possible to preload the FreeWheel slots. To turn on preloading, enable `preloadSlots` on `BCOVFWSessionProviderOptions`.

When enabled, pre roll slots will be loaded as soon as possible. For midrolls, slots will be preloaded a configurable amount of seconds, determined by `timeBeforeAdToPreload` on `BCOVFWSessionProviderOptions`.

### Using an Accurate Duration

Depending on your FreeWheel configuration, supplying an accurate duration in calls to `setVideoId:` is the difference between receiving the correct number of midrolls or not.

By default, the SDK will wait to call the `BCOVFWSessionProviderAdContextPolicy` until the AVPlayerItem has an accurate duration, and this value will be provided to the block call. If you do not want to wait until the AVPlayerItem duration is ready and you have this information available from another source (like the BCOVVideo/BCOVSource properties or your own CMS), `waitForAVPlayerDuration` can be used to disable waiting.

### Handling Seeks

When seeking over multiple ad pods (like two midrolls slots, at different positions), the SDK provides a convenience mechanism to determine which pods get played. This can be modified by changing the default `BCOVCuePointProgressPolicy` on `BCOVFWSessionProviderOptions`. For more information on the `BCOVCuePointProgressPolicy`, please consult the `BCOVFWSessionProvider.h` file.

## Tracking FreeWheel Errors

FreeWheel error events (`FWAdErrorEvent`) encountered via the `FWAdEventNotification` will be passed through to the `playbackController(_ controller:,playbackSession session:,didReceive lifecycleEvent:)` delegate method with an event type of `kBCOVFWLifecycleEventAdError`. The entire userInfo object that the FreeWheel SDK sends along with the notification is included in the events properties and can be accessed with the `kBCOVFWLifecycleEventPropertyKeyAdError` key.

Here is an example of how to catch these errors:

```swift
func playbackController(_ controller: BCOVPlaybackController,
                        playbackSession session: BCOVPlaybackSession,
                        didReceive lifecycleEvent: BCOVPlaybackSessionLifecycleEvent) {

    if lifecycleEvent.eventType == kBCOVFWLifecycleEventAdError {
        if let adError = lifecycleEvent.properties[kBCOVFWLifecycleEventPropertyKeyAdError] as? [AnyHashable:Any],
           let errorInfo = adError[FWInfoKeyErrorInfo] {
            print("FreeWheel Error Encountered: \(errorInfo)")
        }
    }

}
```

## AVPlayerViewController Support

### Displaying an Ad UI

If you'd like to display your own Ad UI during ad playback you can use the `playbackController:playbackSession:didEnterAdSequence:`  and `playbackController:playbackSession:didExitAdSequence:`  delegate methods. Here is an example:

```swift
// MARK: BCOVPlaybackControllerDelegate

func playbackController(_ controller: BCOVPlaybackController,
                        playbackSession session: BCOVPlaybackSession,
                        didEnter ad: BCOVAd) {
    displayAdUI(withAdDuration: CMTimeGetSeconds(ad.duration))
}

func playbackController(_ controller: BCOVPlaybackController,
                        playbackSession session: BCOVPlaybackSession,
                        didExitAdSequence adSequence: BCOVAdSequence) {
    hideAdUI()
}
```

## Picture-in-Picture

The FreeWheel plugin can be used with Brightcove's Picture-in-Picture support.

Any ad cue-points encountered after Picture-in-Picture has become active will be skipped. The Picture-in-Picture functionality is disabled during ad playback; the Picture-in-Picture button will be greyed out and putting the app into the background while an ad is playing will not result in a Picture-in-Picture window.

## Support

If you have questions, need help or want to provide feedback, please use the [Support Portal](https://supportportal.brightcove.com/s/login/) or contact your Account Manager. To receive notification of new SDK software releases, subscribe to the Brightcove Native Player SDKs [Google Group](https://groups.google.com/g/brightcove-native-player-sdks). If you are unsure what your ad settings are or have questions regarding what FWContext and other FW classes, please contact FreeWheel support at [https://freewheel.com](https://freewheel.com).
