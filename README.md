# FreeWheel Plugin for Brightcove Player SDK for iOS, version 2.1.1.298

Supported Platforms
===================
iOS 7.0 and above.

Installation
===========

The FreeWheel Plugin for Brightcove Player SDK provides a static library framework for installation. A dynamic framework will be added in the future when FreeWheel releases a dylib version.

CocoaPods
--------------

You can use [CocoaPods][cocoapods] to add the FreeWheel Plugin for Brightcove Player SDK to your project. You can find the latest `Brightcove-Player-SDK-FW` podspec [here][podspecs]. To use this spec, add the following to the top of Podfile: `source 'https://github.com/brightcove/BCOVSpecs.git'`. CocoaPods version 0.39.0 or higher is required.

The FreeWheel SDK **is not** included in this pod.  You **must** manually add the FreeWheel SDK AdManager.framework to your project. The pod will however add all the libraries required by AdManager.framework framework.

Static Framework example:  

    source 'https://github.com/CocoaPods/Specs.git'
    source 'https://github.com/brightcove/BCOVSpecs.git'

    pod 'Brightcove-Player-SDK-FW'
    
Manual
--------------

To add the FreeWheel Plugin for Brightcove Player SDK to your project manually:

1. Install the latest version of the [Brightcove Player SDK][bcovsdk].
2. Download the latest zipped release of the plugin from our [release page][release].
3. Add the 'BrightcoveFW.framework' to your project.
4. On the "Build Settings" tab of your application target, ensure that the "Framework Search Paths" include the path to the framework. This should have been done automatically unless the framework is stored under a different root directory than your project.
5. On the "Build Phases" tab of your application target, add the following to the "Link
    Binary With Libraries" phase:
    * `BrightcoveFW.framework`
6. On the "Build Settings" tab of your application target:
    * Ensure that `-ObjC` has been added to the "Other Linker Flags" build setting.
7. Install the FreeWheel library, which must be retrieved from your FreeWheel account.

Imports
--------------
The FreeWheel Plugin for Brightcove Player SDK can be imported into code a few different ways; `@import BrightcoveFW;`, `#import <BrightcoveFW/BrightcoveFW.h>` or `#import <BrightcoveFW/[specific class].h>`.

[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios
[cocoapods]: http://cocoapods.org
[podspecs]: https://github.com/brightcove/BCOVSpecs/tree/master/Brightcove-Player-SDK-FW
[release]: https://github.com/brightcove/brightcove-player-sdk-ios-fw/releases

Quick Start
===========
Playing video with the Brightcove Player SDK for iOS with FreeWheel ads:

```
    @property (nonatomic, strong) id<FWAdManager> adManager;
    @property (nonatomic, weak) IBOutlet UIView *videoContainerView;
    
    -(void)setup
    {
        NSString *token;      // (Brightcove Media API token with URL access)
        NSString *playlistID; // (ID of the playlist you wish to use)
    
    [1] self.adManager = newAdManager();
        [self.adManager setNetworkId:90750];
        [self.adManager setServerUrl:@"http://demo.v.fwmrm.net"];
    
        BCOVPlayerSDKManager *sdkManager = [BCOVPlayerSDKManager sharedManager];
    [2] id<BCOVPlaybackController> playbackController = [sdkManager createFWPlaybackControllerWithAdContextPolicy:[self adContextPolicy] viewStrategy:nil];
        [self.view addSubview:playbackController.view];
    
        BCOVCatalogService *catalog = [[BCOVCatalogService alloc] initWithToken:token];
        [catalog findPlaylistWithPlaylistID:playlistID
                                 parameters:nil
                                 completion:^(BCOVPlaylist *playlist,
                                              NSDictionary *jsonResponse,
                                              NSError      *error) {
    
                                     [playbackController setVideos:playlist];
                                     [playbackController play];
                                     
                                 }];
    }
    
    - (BCOVFWSessionProviderAdContextPolicy)adContextPolicy
    {
        __weak typeof(self) weakSelf = self;
        
        return [^ id<FWContext>(BCOVVideo *video, BCOVSource *source, NSTimeInterval videoDuration) {
            
            // Prevent retain cycles when using self
            typeof(self) strongSelf = weakSelf;
    
    [3]     id<FWContext> adContext = [strongSelf.adManager newContext];
            [adContext setPlayerProfile:<player-profile> defaultTemporalSlotProfile:nil defaultVideoPlayerSlotProfile:nil defaultSiteSectionSlotProfile:nil];
            [adContext setSiteSectionId:<site-section-id> idType:FW_ID_TYPE_CUSTOM pageViewRandom:0 networkId:0 fallbackId:0];
            [adContext setVideoAssetId:<video-asset-id> idType:FW_ID_TYPE_CUSTOM duration:videoDuration durationType:FW_VIDEO_ASSET_DURATION_TYPE_EXACT location:nil autoPlayType:true videoPlayRandom:0 networkId:0 fallbackId:0];
            [adContext setVideoDisplayBase:strongSelf.videoContainerView];
            
            return adContext;
            
        } copy];
    }
```
Let's break this code down into steps, to make it a bit simpler to digest:

1. You create the same ad manager that you would create if you were using FreeWheel's iOS SDK directly, and this will be required later.
1. BCOVFW adds some category methods to BCOVPlayerSDKManager. The first of these is `-createFWPlaybackControllerWithAdContextPolicy:viewStrategy:`. Use this method to create your playback controller. Alternatively (if you are using more than one session provider), you can create a BCOVFWSessionProvider and pass that to the SDK manager method that creates a playback controller with upstream session providers.\*
1. You create the same ad context that would create if you were using FreeWheel's iOS SDK directly, using the SDK manager created in step 1. This is where you would register for companion slots, turn on default ad controls, or any other settings you need to change. This block will get called before each new session is delivered.

\* Note that BCOVFWSessionProvider is not tested for use with other advertising session providers, such as BCOVIMASessionProvider. Also note that BCOVFWSessionProvider should come after any other session providers in the chain passed to the manager when constructing the playback controller.

If you have questions or need help, visit the [Brightcove Native Player SDK support forum](https://groups.google.com/forum/#!forum/brightcove-native-player-sdks). If you are unsure what your ad settings are or have questions regarding what FWContext and other FW classes, please contact FreeWheel support at [http://freewheel.tv](http://freewheel.tv).

Play and Pause
===========
The Brightcove FreeWheel Plugin implements custom play and pause logic to ensure the smoothest possible ad experience. Therefore, you will need to make sure that you use the play method on the `BCOVPlaybackController` or the `-[BCOVSessionProviderExtension fw_play]` or `-[BCOVSessionProviderExtension fw_pause]` ([BCOVSessionProviderExtension][BCOVFWComponent]), and not the AVPlayer.  

As an example, calling play for the first time on `BCOVPlaybackController` allows BCOVFW to process preroll ads without any of the content playing before the preroll.

[BCOVFWComponent]: https://github.com/brightcove/brightcove-player-sdk-ios-fw/blob/master/ios/BrightcoveFW.framework/Headers/BCOVFWComponent.h

Using the Built-In PlayerUI
===========================
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
Set the player view to match the video container from your layout (`videoView`) when it resizes.

    // Create and configure Control View.
    BCOVPUIBasicControlView *controlView = [BCOVPUIBasicControlView basicControlViewWithVODLayout];
    self.playerView = [[BCOVPUIPlayerView alloc] initWithPlaybackController:self.playbackController options:nil controlsView:controlView];
    self.playerView.frame = self.videoView.bounds;
    self.playerView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;

Then, add the BCOVPUIPlayerView to your video container, `videoView`.

    // Add BCOVPUIPlayerView to your video view.
    [self.videoView addSubview:self.playerView];

The last step is specific to FreeWheel. In your adContextPolicy, be sure to set your video display base to the player view's content overlay view. This allows FreeWheel ads to play over the video, but keeps the ad controls above the FreeWheel ad.

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

Now, when playing video with ads, you will see ad markers on the timeline scrubber, plus ad controls on the ad during ad playback.

The PlayerUI is highly customizable. For more information and sample code, please see **Custom Layouts** section in the README file of the [Brightcove Native Player SDK repository][BCOVSDK].

[BCOVSDK]: https://github.com/brightcove/brightcove-player-sdk-ios

Seeking Without Ads
===========
Use `-[BCOVPlaybackController seekWithoutAds:(CMTime)time completionHandler:(void (^)(BOOL finished))completion]` to resume playback at a specific time without forcing the user to watch ads scheduled before `time`. When using `seekWithoutAds:completionHandler:`, `autoPlay` should be disabled in the `BCOVPlaybackController`. `seekWithoutAds:completionHandler:` should be called on or after receiving `kBCOVPlaybackSessionLifecycleEventReady` in your `playbackController:playbackSession:didReceiveLifecycleEvent` delegate method.

The `shutter` and `shutterFadeTime` properties of the `BCOVPlaybackController` can be used along with `seekWithoutAds:completionHandler:` to hide frame-flicker which can occur as the AVPlayer loads assets. In your BCOVPlaybackController setup code, close the shutter to hide the player view:

```
  NSObject<BCOVPlaybackController> *playbackController;
        
  playbackController = [sdkManager createFWPlaybackControllerWithAdContextPolicy:nil
                                                                    viewStrategy:nil];
  playbackController.delegate = self;
        
  if (self.willSeekWithoutAds)
  {
    // set the shutter fade time to zero to hide the player view immediately.
    playbackController.shutterFadeTime = 0.0;
    playbackController.shutter = YES;
    
    // disable autoPlay when resuming playback.
    playbackController.autoPlay = NO;
  }
```

Apple recommends waiting for the status of an AVPlayerItem to change to ready-to-play before using the AVPlayerItem; call `seekWithoutAds:completionHandler:` in the `playbackController:playbackSession:didReceiveLifecycleEvent` method of your `BCOVPlaybackControllerDelegate` delegate.

```
- (void)playbackController:(NSObject<BCOVPlaybackController>*)controller
           playbackSession:(NSObject<BCOVPlaybackSession>*)session
  didReceiveLifecycleEvent:(BCOVPlaybackSessionLifecycleEvent *)lifecycleEvent
{
  if ([kBCOVPlaybackSessionLifecycleEventReady isEqualToString:lifecycleEvent.eventType])
  {

    if (self.willSeekWithoutAds)
    {
      __weak typeof(controller) weakController = controller;

      // seek without playing ads which are scheduled before the seek time, i.e. resume playback.
      [controller seekWithoutAds:CMTimeMake(seekWithoutAdsValue, seekWithoutAdsTimescale)
             completionHandler:^(BOOL finished){

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

          // turn off seeking without ads - especially important if this player is being used with a playlist
          self.willSeekWithoutAds = NO;
        }

      }];
    }
  }
}
```

Note that with Seeking Without Ads enabled in your app, you will still see the network traffic that normally occurs as part of setting up the FreeWheel plugin. This traffic is necessary for proper plugin setup, and does not affect the Seeking Without Ads functionality.

Customizing Plugin Behavior
===========
You can customize default plugin behavior by creating an instance of `BCOVFWSessionProviderOptions` and overriding the default properties. To use a `BCOVFWSessionProviderOptions` options instance, you need to create the `BCOVFWSessionProvider` using `-[BCOVSDKManager createFWSessionProviderWithAdContextPolicy:upstreamSessionProvider:options:]`.

```
    BCOVFWSessionProviderOptions *options = [[BCOVFWSessionProviderOptions alloc] init];
    options.cuePointProgressPolicy = [BCOVCuePointProgressPolicy progressPolicyProcessingCuePoints:BCOVProgressPolicyProcessFinalCuePoint resumingPlaybackFrom:BCOVProgressPolicyResumeFromContentPlayhead ignoringPreviouslyProcessedCuePoints:YES];
    id<BCOVPlaybackSessionProvider> sessionProvider = [sdkManager createFWSessionProviderWithAdContextPolicy:[self adContextPolicy] upstreamSessionProvider:nil options:options];

    id<BCOVPlaybackController> playbackController = [sdkManager createPlaybackControllerWithSessionProvider:sessionProvider viewStrategy:nil];
```

Preloading Slots
--------------------------
Depending on network conditions, there can be a delay in the time it takes for ads to play, once the ad position has been hit. In order to improve ad performance, it is possible to preload the FreeWheel slots. To turn on preloading, enable `preloadSlots` on `BCOVFWSessionProviderOptions`.

When enabled, pre roll slots will be loaded as soon as possible. For midrolls, slots will be preloaded a configurable amount of seconds, determined by `timeBeforeAdToPreload` on `BCOVFWSessionProviderOptions`.

Using an accurate duration
--------------------------
Depending on your FreeWheel configuration, supplying an accurate duration in calls to `setVideoId:` is the difference between receiving the correct number of midrolls or not.

By default, the SDK will wait to call the `BCOVFWSessionProviderAdContextPolicy` until the AVPlayerItem has an accurate duration, and this value will be provided to the block call. If you do not want to wait until the AVPlayerItem duration is ready and you have this information available from another source (like the BCOVVideo/BCOVSource properties or your own CMS), `waitForAVPlayerDuration` can be used to disable waiting.

Handling Seeks
--------------------------
When seeking over multiple ad pods (like two midrolls slots, at different positions), the SDK provides a convenience mechanism to determine which pods get played. This can be modified by changing the default `BCOVCuePointProgressPolicy` on `BCOVFWSessionProviderOptions`. For more information on the `BCOVCuePointProgressPolicy`, please consult the `BCOVFWSessionProvider.h` file.






