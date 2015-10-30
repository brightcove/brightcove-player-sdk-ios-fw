# Freewheel Plugin for Brightcove Player SDK for iOS, version 1.1.1.220

Requirements
===========

This plugin supports iOS 7.0+.

Installation
===========

You can use [Cocoapods][cocoapods] to add the Freewheel Plugin for Brightcove Player SDK to your project.  You can find the latest `Brightcove-Player-SDK-FW` podspec [here][podspecs].  To use this spec, using Cocoapods 0.34.1+, add the following to the top of Podfile: `source 'https://github.com/brightcove/BCOVSpecs.git'`.

The Freewheel SDK **is not** included in this pod.  You **must** manually add the Freewheel SDK AdManager.framework to your project. The pod will however add all the libraries required by this framework.

To add the Freewheel Plugin for Brightcove Player SDK to your project manually:

1. Install the latest version of the [Brightcove Player SDK][bcovsdk].
1. Download the latest zip'ed release of the plugin from our [release page][release].
1. Add the contents of Library and Headers to the project.
1. On the "Build Phases" tab of your application target, add the following to the "Link
    Binary With Libraries" phase:
    * `libBCOVFW.a`
1. On the "Build Settings" tab of your application target:
    * Ensure that BCOVFW headers are in your application's "Header Search Path".
    * Ensure that `-ObjC` has been added to the "Other Linker Flags" build setting.
1. Install the Freewheel library, which must be retrieved from your Freewheel account.

[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios
[cocoapods]: http://cocoapods.org
[podspecs]: https://github.com/brightcove/BCOVSpecs/tree/master/Brightcove-Player-SDK-FW
[release]: https://github.com/brightcove/brightcove-player-sdk-ios-fw/releases

Quick Start
===========
Playing video with the Brightcove Player SDK for iOS with Freewheel ads:

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
    
        BCOVPlayerSDKManager *manager = [BCOVPlayerSDKManager sharedManager];
    [2] id<BCOVPlaybackController> controller = [manager createFWPlaybackControllerWithAdContextPolicy:[self adContextPolicy] viewStrategy:nil];
        [self.view addSubview:controller.view];
    
        BCOVCatalogService *catalog = [[BCOVCatalogService alloc] initWithToken:token];
        [catalog findPlaylistWithPlaylistID:playlistID
                                 parameters:nil
                                 completion:^(BCOVPlaylist *playlist,
                                              NSDictionary *jsonResponse,
                                              NSError      *error) {
    
                                     [controller setVideos:playlist];
                                     [controller play];
                                     
                                 }];
    }
    
    - (BCOVFWSessionProviderAdContextPolicy)adContextPolicy
    {
        __weak typeof(self) weakSelf = self;
        
        return [^ id<FWContext>(BCOVVideo *video, BCOVSource *source, NSTimeInterval videoDuration) {
            
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

1. You create the same ad manager that you would create if you were using Freewheel's iOS SDK directly, and this will be required later.
1. BCOVFW adds some category methods to BCOVPlaybackManager. The first of these is `-createFWPlaybackControllerWithAdContextPolicy:viewStrategy:`. Use this method to create your playback controller. Alternatively (if you are using more than one session provider), you can create a BCOVFWSessionProvider and pass that to the manager method that creates a playback controller with upstream session providers.\*
1. You create the same ad context that would create if you were using Freewheel's iOS SDK directly, using the manager created in step 1. This is where you would register for companion slots, turn on default ad controls, or any other settings you need to change. This block will get called before each new session is delivered.

\* Note that BCOVFWSessionProvider is not tested for use with other advertising session providers, such as BCOVIMASessionProvider. Also note that BCOVFWSessionProvider should come after any other session providers in the chain passed to the manager when constructing the playback controller.

If you have questions or need help, we have a support forum for Brightcove's native Player SDKs at https://groups.google.com/forum/#!forum/brightcove-native-player-sdks . If you are unsure what your ad settings are or have questions regarding what FWContext and other FW classes, please contact Freewheel support.

Playing and Pausing
===========
The Brightcove Freewheel Plugin will pause and play the content video automatically when playing a `FWSlot` Ad. However, in order for this to work reliably, our Freewheel plugin must provide accurate video state information to the Freewheel library. In order to ensure accurate video state information, we recommend the following options:

* Using `-[BCOVPlaybackController pause]` and `-[BCOVPlaybackController play]` will update Freewheel with the correct state automatically.
* Using `-[BCOVPlaybackSession.providerExtension fw_pause]` and  `-[BCOVPlaybackSession.providerExtension fw_play]` will update Freewheel with the correct state automatically.
* Manually update the `-[FWContext setVideoState:]` with either `FW_VIDEO_STATE_PLAYING` or `FW_VIDEO_STATE_PAUSED` anytime you call play/pause on the AVPlayer directly. Example:

```
    [self.adContext setVideoState:FW_VIDEO_STATE_PAUSED];
    [session.player pause];
```

In addition to providing accurate video state, it is important not to accidentally play content while an ad is playing. In order to prevent this, we recommend the following:

* Using `-[BCOVPlaybackController pause]` and `-[BCOVPlaybackController play]` will not call '-[AVPlayer play]' when an ad is playing.
* Using `-[BCOVPlaybackSession.providerExtension fw_pause]` and  `-[BCOVPlaybackSession.providerExtension fw_play]` will not call '-[AVPlayer play]' when an ad is playing.
* If using the AVPlayer directly, check `BCOVPlaybackSession.providerExtension.isPausedOnFreewheelsRequest` to determine whether Freewheel has requested a pause in content, and disables calls to `-[AVPlayer play]`.
* Register listeners with `NSNotificationCenter` for `FW_TIME_POSITION_CLASS_PREROLL` and `FW_NOTIFICATION_SLOT_ENDED`. You will need to verify that the slot has a time position class of `FW_TIME_POSITION_CLASS_MIDROLL`, `FW_TIME_POSITION_CLASS_MIDROLL`, or `FW_TIME_POSITION_CLASS_POSTROLL`. Keep track of when a slot is playing, and disables calls to `-[AVPlayer play]`.

Customizing Plugin Behavior
===========
You can customize default plugin behavior by creating an instance of `BCOVFWSessionProviderOptions` and overriding the default properties. To use a `BCOVFWSessionProviderOptions` options instance, you need to create the `BCOVFWSessionProvider` using `-[BCOVSDKManager createFWSessionProviderWithAdContextPolicy:upstreamSessionProvider:options:]`.

```
    BCOVFWSessionProviderOptions *options = [[BCOVFWSessionProviderOptions alloc] init];
    options.cuePointProgressPolicy = [BCOVCuePointProgressPolicy progressPolicyProcessingCuePoints:BCOVProgressPolicyProcessFinalCuePoint resumingPlaybackFrom:BCOVProgressPolicyResumeFromContentPlayhead ignoringPreviouslyProcessedCuePoints:YES];
    id<BCOVPlaybackSessionProvider> sessionProvider = [playbackManager createFWSessionProviderWithAdContextPolicy:[self adContextPolicy] upstreamSessionProvider:nil options:options];

    id<BCOVPlaybackController> playbackController = [playbackManager createPlaybackControllerWithSessionProvider:sessionProvider viewStrategy:nil];
```

Preloading Slots
--------------------------
Depending on network conditions, there can be a delay in the time it takes for ads to play, once the ad position has been hit. In order to improve ad performance, it is possible to preload the Freewheel slots. To turn on preloading, enable `preloadSlots` on `BCOVFWSessionProviderOptions`.

When enabled, pre roll slots will be loaded as soon as possible. For midrolls, slots will be preloaded a configurable amount of seconds, determined by `timeBeforeAdToPreload` on `BCOVFWSessionProviderOptions`.

Using an accurate duration
--------------------------
Depending on your Freewheel configuration, supplying an accurate duration in calls to `setVideoId:` is the difference between receiving the correct number of midrolls or not.

By default, the SDK will wait to call the `BCOVFWSessionProviderAdContextPolicy` until the AVPlayerItem has an accurate duration, and this value will be provided to the block call. If you do not want to wait until the AVPlayerItem duration is ready and you have this information available from another source (like the BCOVVideo/BCOVSource properties or your own CMS), `waitForAVPlayerDuration` can be used to disable waiting.

Handling Seeks
--------------------------
When seeking over multiple ad pods (like two midrolls slots, at different positions), the SDK provides a convenience mechanism to determine which pods get played. This can be modified by changing the default `BCOVCuePointProgressPolicy` on `BCOVFWSessionProviderOptions`. For more information on the `BCOVCuePointProgressPolicy`, please consult the `BCOVFWSessionProvider.h` file.






