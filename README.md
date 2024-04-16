# FreeWheel Plugin for Brightcove Player SDK for iOS, version 6.13.0.2599

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

The FreeWheel Plugin for Brightcove Player SDK can be imported into code a few different ways; `@import BrightcoveFW;`, `#import <BrightcoveFW/BrightcoveFW.h>` or `#import <BrightcoveFW/[specific class].h>`.

[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios/releases
[cocoapods]: http://cocoapods.org
[podspecs]: https://github.com/brightcove/BrightcoveSpecs/tree/master/Brightcove-Player-FreeWheel
[release]: https://github.com/brightcove/brightcove-player-sdk-ios-fw/releases

## Quick Start

Playing video with the Brightcove Player SDK for iOS with FreeWheel ads:

```
    @property (nonatomic, strong) id<FWAdManager> adManager;
    @property (nonatomic, weak) IBOutlet UIView *videoContainerView;
    
    -(void)setup
    {
        NSString *policyKey = <your-policy-key>;
        NSString *accountId = <your-account-id>;
        NSString *videoId = <your-video-id>;
    
    [1] self.adManager = newAdManager();
        [self.adManager setNetworkId:90750];
    
        BCOVPlayerSDKManager *sdkManager = [BCOVPlayerSDKManager sharedManager];
    [2] id<BCOVPlaybackController> playbackController =
            [sdkManager createFWPlaybackControllerWithAdContextPolicy:[self adContextPolicy] viewStrategy:nil];
        [self.view addSubview:playbackController.view];
    
        BCOVPlaybackService *service = [[BCOVPlaybackService alloc] initWithAccuntID:accountId
                                                                           policyKey:policyKey];
        NSDictionary *configuration = @{
            kBCOVPlaybackServiceConfigurationKeyAssetID:videoId
        };
        [service findVideoWithConfiguration:configuration
                            queryParameters:nil
                                 completion:^(BCOVVideo    *video,
                                              NSDictionary *jsonResponse,
                                              NSError      *error) {

            [playbackController setVideos:@[ video ]];
            [playbackController play];

        }];
    }
    
    - (BCOVFWSessionProviderAdContextPolicy)adContextPolicy
    {
        __weak typeof(self) weakSelf = self;
        
        return [^ BCOVFWContext*(BCOVVideo *video, BCOVSource *source, NSTimeInterval videoDuration) {
            
            // Prevent retain cycles when using self
            typeof(self) strongSelf = weakSelf;
    
    [3]     id<FWContext> adContext = [strongSelf.adManager newContext];
    
            FWRequestConfiguration *adRequestConfig = [[FWRequestConfiguration alloc] initWithServerURL:@"http://demo.v.fwmrm.net" playerProfile:<player-profile>];
            adRequestConfig.siteSectionConfiguration = [[FWSiteSectionConfiguration alloc] initWithSiteSectionId:<site-section-id> idType:FWIdTypeCustom];
            adRequestConfig.videoAssetConfiguration = [[FWVideoAssetConfiguration alloc] initWithVideoAssetId:<video-asset-id> idType:FWIdTypeCustom duration:videoDuration durationType:FWVideoAssetDurationTypeExact autoPlayType:FWVideoAssetAutoPlayTypeAttended];

            [adContext setVideoDisplayBase:strongSelf.videoContainerView];
            
            [adRequestConfig addSlotConfiguration:[[FWTemporalSlotConfiguration alloc] initWithCustomId:@"preroll" adUnit:FWAdUnitPreroll timePosition:0.0]];
            [adRequestConfig addSlotConfiguration:[[FWTemporalSlotConfiguration alloc] initWithCustomId:@"midroll" adUnit:FWAdUnitMidroll timePosition:<non-zero-time-interval>]];
            [adRequestConfig addSlotConfiguration:[[FWTemporalSlotConfiguration alloc] initWithCustomId:@"postroll" adUnit:FWAdUnitPostroll timePosition:0.0]];
        
            BCOVFWContext *bcovAdContext = [[BCOVFWContext alloc] initWithAdContext:adContext requestConfiguration:adRequestConfig];

            return bcovAdContext;
            
        } copy];
    }
```

The code broken down into steps:

1. You create the same ad manager that you would create if you were using FreeWheel's iOS SDK directly; this will be required later.
1. The Brightcove FreeWheel Plugin adds some category methods to BCOVPlayerSDKManager. The first of these is `-createFWPlaybackControllerWithAdContextPolicy:viewStrategy:`. Use this method to create your playback controller. Alternatively (if you are using more than one session provider), you can create a BCOVFWSessionProvider and pass that to the SDK manager method that creates a playback controller with upstream session providers.\*
1. You create the same ad context that would create if you were using FreeWheel's iOS SDK directly, using the SDK manager created in step 1. This is where you would register for companion slots, turn on default ad controls, or any other settings you need to change. This block will get called before each new session is delivered.

\* Note that BCOVFWSessionProvider is not tested for use with other advertising session providers, such as BCOVIMASessionProvider. Also note that BCOVFWSessionProvider should come after any other session providers in the chain passed to the manager when constructing the playback controller.

## Play and Pause

The Brightcove FreeWheel Plugin implements custom play and pause logic to ensure the smoothest possible ad experience. Therefore, you will need to make sure that you use the play method on the `BCOVPlaybackController` or the `-[BCOVSessionProviderExtension fw_play]` or `-[BCOVSessionProviderExtension fw_pause]` ([BCOVSessionProviderExtension][BCOVFWComponent]), and not the AVPlayer.  

As an example, calling play for the first time on `BCOVPlaybackController` allows the Brightcove FreeWheel Plugin to process preroll ads without any of the content playing before the preroll.

[BCOVFWComponent]: https://github.com/brightcove/brightcove-player-sdk-ios-fw/blob/master/ios/BrightcoveFW.framework/Headers/BCOVFWComponent.h

## Using the Built-In PlayerUI

If you are using version 5.1 or later of the Brightcove Player SDK, you can take advantage of the built-in ad controls with the Brightcove FreeWheel plugin.

To use the PlayerUI, create your playback controller as in the quick start code above.

    BCOVPlayerSDKManager *sdkManager = [BCOVPlayerSDKManager sharedManager];
    id<BCOVPlaybackController> playbackController =
    [sdkManager createFWPlaybackControllerWithAdContextPolicy:[self adContextPolicy] viewStrategy:nil];

Then create your Player View. This view contains both the video player view and the view that displays playback and ad controls. This setup is the same no matter what plugin you are using.

**Note:** The `BrightcovePlayerUI` module is no longer needed and has been removed. (Prior to version 5.1 of the Brightcove Player SDK, the Brightcove PlayerUI plugin was a separate framework and module.) You can remove any imports that reference the Brightcove PlayerUI module. All PlayerUI headers are now found in the `BrightcovePlayerSDK` module.

Create a property in your UIViewController to keep track of the BCOVPUIPlayerView. The BCOVPUIPlayerView will contain both the playback controller's view, and the controls view.

	// PlayerUI's Player View
	@property (nonatomic) BCOVPUIPlayerView *playerView;

Create the BCOVPUIBasicControlView, and then the BCOVPUIPlayerView. This is where we associate the playback controller (and thus all the videos it plays) with the controls.

    // Create and configure Control View.
    BCOVPUIBasicControlView *controlView = [BCOVPUIBasicControlView basicControlViewWithVODLayout];
    self.playerView = [[BCOVPUIPlayerView alloc] initWithPlaybackController:self.playbackController options:nil controlsView:controlView];
    // Add BCOVPUIPlayerView to your video view.
    [self.videoView addSubview:self.playerView];

You'll need to set up the layout for the player view, you can do this with Auto Layout or the older Springs & Struts approach. 

### Springs & Struts

Set the player view to match the video container from your layout (`videoView`) when it resizes.

    self.playerView.frame = self.videoView.bounds;
    self.playerView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;   

### Auto Layout

Set the `translatesAutoresizingMaskIntoConstraints` on BCOVPUIPlayerView to `NO`.

    self.videoView.translatesAutoresizingMaskIntoConstraints = NO;

Then add the constraints for the layout; setting the top, right, left and bottom anchors of your BCOVPUIPlayerView to equal that of `videoView` 

    [NSLayoutConstraint activateConstraints:@[
                                              [self.playerView.topAnchor constraintEqualToAnchor:self.videoView.topAnchor],
                                              [self.playerView.rightAnchor constraintEqualToAnchor:self.videoView.rightAnchor],
                                              [self.playerView.leftAnchor constraintEqualToAnchor:self.videoView.leftAnchor],
                                              [self.playerView.bottomAnchor constraintEqualToAnchor:self.videoView.bottomAnchor],
                                              ]];

The last step is specific to FreeWheel. In your adContextPolicy, be sure to set your video display base to the player view's content overlay view. This allows FreeWheel ads to play over the video, but keeps the ad controls above the FreeWheel ad.

```
- (BCOVFWSessionProviderAdContextPolicy)adContextPolicy
{
    // Prevent retain cycles when using self
    __weak typeof(self) weakSelf = self;
    
    return [^ id<FWContext>(BCOVVideo *video, BCOVSource *source, NSTimeInterval videoDuration) {
        
        typeof(self) strongSelf = weakSelf;

         ... FREEWHEEL SETUP CODE ...

        // Tell FreeWheel to display ads in they content overlay view
        [adContext setVideoDisplayBase:strongSelf.playerView.contentOverlayView];
        
        return adContext;
        
    } copy];
}
```

Now, when playing video with ads, you will see ad markers on the timeline scrubber, plus ad controls on the ad during ad playback.

The PlayerUI is highly customizable. For more information and sample code, please see **Custom Layouts** section in the README file of the [Brightcove Native Player SDK repository][BCOVSDK].

[BCOVSDK]: https://github.com/brightcove/brightcove-player-sdk-ios

## Seek Without Ads

Use `-[BCOVPlaybackController seekWithoutAds:(CMTime)seekToTime completionHandler:(void (^)(BOOL finished))completion]` to resume playback at a specific time without forcing the user to watch ads scheduled before `seekToTime`.

In preparation for `seekWithoutAds:completionHandler:`, disable `autoPlay` when setting up the `BCOVPlaybackController`.

Apple recommends waiting for the status of an AVPlayerItem to change to ready-to-play before using the AVPlayerItem. Therefore, call `seekWithoutAds:completionHandler:` in the `kBCOVPlaybackSessionLifecycleEventReady` handler of the `playbackController:playbackSession:didReceiveLifecycleEvent` method of your `BCOVPlaybackControllerDelegate`.

```objective-c
- (void)playbackController:(NSObject<BCOVPlaybackController>*)controller
           playbackSession:(NSObject<BCOVPlaybackSession>*)session
  didReceiveLifecycleEvent:(BCOVPlaybackSessionLifecycleEvent *)lifecycleEvent
{
  if ([kBCOVPlaybackSessionLifecycleEventReady isEqualToString:lifecycleEvent.eventType])
  {

    // self.resumePlayback is a hypothetical instance variable used here for illustration.
    if (self.resumePlayback)
    {
      __weak typeof(controller) weakController = controller;

      // seek without playing ads which are scheduled before the seek time, i.e. resume playback.
      [controller seekWithoutAds:CMTimeMake(seekWithoutAdsValue, seekWithoutAdsTimescale)
               completionHandler:^(BOOL finished) {

        if (!finished)
        {
          NSLog (@"seekWithoutAds failed to finish");
        }

        typeof(controller) strongController = weakController;
        if (strongController)
        {
          // fade out the shutter to reveal the player view.
          strongController.shutterFadeTime = 0.25;
          strongController.shutter = NO;

          // turn off seek without ads - especially important if this player is being used with a playlist
          self.resumePlayback = NO;
        }

      }];
    }
  }
}
```

The `shutter` and `shutterFadeTime` properties of the `BCOVPlaybackController` can be used along with `seekWithoutAds:completionHandler:` to hide frame-flicker which can occur as the AVPlayer loads assets. In your BCOVPlaybackController set-up code, enable the shutter to hide the player view:

```objective-c
  NSObject<BCOVPlaybackController> *playbackController;
        
  playbackController = [sdkManager createFWPlaybackControllerWithAdContextPolicy:nil
                                                                    viewStrategy:nil];
  playbackController.delegate = self;
        
  if (self.resumePlayback)
  {
    // set the shutter fade time to zero to hide the player view immediately.
    playbackController.shutterFadeTime = 0.0;
    playbackController.shutter = YES;
    
    // disable autoPlay when resuming playback.
    playbackController.autoPlay = NO;
  }
```

Note that when Seek Without Ads is used in your app, you might observe network traffic which normally occurs as part of setting up the IMA plugin. This traffic is necessary for proper plugin setup, and does not affect the Seek Without Ads functionality.

## Customizing Plugin Behavior

You can customize default plugin behavior by creating an instance of `BCOVFWSessionProviderOptions` and overriding the default properties. To use a `BCOVFWSessionProviderOptions` options instance, you need to create the `BCOVFWSessionProvider` using `-[BCOVSDKManager createFWSessionProviderWithAdContextPolicy:upstreamSessionProvider:options:]`.

```
    BCOVFWSessionProviderOptions *options = [[BCOVFWSessionProviderOptions alloc] init];
    options.cuePointProgressPolicy = [BCOVCuePointProgressPolicy progressPolicyProcessingCuePoints:BCOVProgressPolicyProcessFinalCuePoint resumingPlaybackFrom:BCOVProgressPolicyResumeFromContentPlayhead ignoringPreviouslyProcessedCuePoints:YES];
    id<BCOVPlaybackSessionProvider> sessionProvider = [sdkManager createFWSessionProviderWithAdContextPolicy:[self adContextPolicy] upstreamSessionProvider:nil options:options];

    id<BCOVPlaybackController> playbackController = [sdkManager createPlaybackControllerWithSessionProvider:sessionProvider viewStrategy:nil];
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

FreeWheel error events (`FWAdErrorEvent`) encountered via the `FWAdEventNotification` will be passed through to the `playbackController:playbackSession:didReceiveLifecycleEvent:` delegate method with an event type of `kBCOVFWLifecycleEventAdError`. The entire userInfo object that the FreeWheel SDK sends along with the notification is included in the events properties and can be accessed with the `kBCOVFWLifecycleEventPropertyKeyAdError` key. 

Here is an example of how to catch these errors:

```
- (void)playbackController:(id<BCOVPlaybackController>)controller playbackSession:(id<BCOVPlaybackSession>)session didReceiveLifecycleEvent:(BCOVPlaybackSessionLifecycleEvent *)lifecycleEvent
{
    if ([kBCOVFWLifecycleEventAdError isEqualToString:lifecycleEvent.eventType])
    {
        NSDictionary *errorInfo = lifecycleEvent.properties[kBCOVFWLifecycleEventPropertyKeyAdError];
        NSLog(@"FreeWheel Error Encountered: %@", errorInfo[FWInfoKeyErrorInfo]);
    }
}
```

## AVPlayerViewController Support

### Displaying an Ad UI

If you'd like to display your own Ad UI during ad playback you can use the `playbackController:playbackSession:didEnterAdSequence:`  and `playbackController:playbackSession:didExitAdSequence:`  delegate methods. Here is an example:

```
#pragma mark BCOVPlaybackControllerDelegate

- (void)playbackController:(id<BCOVPlaybackController>)controller playbackSession:(id<BCOVPlaybackSession>)session didEnterAd:(BCOVAd *)ad
{
    [self displayAdUI:CMTimeGetSeconds(ad.duration)];
}

- (void)playbackController:(id<BCOVPlaybackController>)controller playbackSession:(id<BCOVPlaybackSession>)session didExitAdSequence:(BCOVAdSequence *)adSequence
{
    ...

    [self hideAdUI];
}
```

## Picture-in-Picture

The FreeWheel plugin can be used with Brightcove's Picture-in-Picture support.

Any ad cue-points encountered after Picture-in-Picture has become active will be skipped. The Picture-in-Picture functionality is disabled during ad playback; the Picture-in-Picture button will be greyed out and putting the app into the background while an ad is playing will not result in a Picture-in-Picture window.

## Support

If you have questions, need help or want to provide feedback, please use the [Support Portal](https://supportportal.brightcove.com/s/login/) or contact your Account Manager. To receive notification of new SDK software releases, subscribe to the Brightcove Native Player SDKs [Google Group](https://groups.google.com/g/brightcove-native-player-sdks). If you are unsure what your ad settings are or have questions regarding what FWContext and other FW classes, please contact FreeWheel support at [https://freewheel.com](https://freewheel.com).




