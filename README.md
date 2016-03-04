# Freewheel Plugin for Brightcove Player SDK for iOS, version 2.0.2.259

Supported Platforms
===================
iOS 7.0 and above.

Installation
===========

The Freewheel Plugin for Brightcove Player SDK provides a static library framework for installation. A dynamic framework will be added in the future when Freewheel releases a dylib version.

CocoaPods
--------------

You can use [Cocoapods][cocoapods] to add the Freewheel Plugin for Brightcove Player SDK to your project. You can find the latest `Brightcove-Player-SDK-FW` podspec [here][podspecs]. To use this spec, add the following to the top of Podfile: `source 'https://github.com/brightcove/BCOVSpecs.git'`. CocoaPods 0.39 or newer is required.

The Freewheel SDK **is not** included in this pod.  You **must** manually add the Freewheel SDK AdManager.framework to your project. The pod will however add all the libraries required by AdManager.framework framework.

Static Framework example:  

    source 'https://github.com/CocoaPods/Specs.git'
    source 'https://github.com/brightcove/BCOVSpecs.git'

    pod 'Brightcove-Player-SDK-FW'
    
Manual
--------------

To add the Freewheel Plugin for Brightcove Player SDK to your project manually:

1. Install the latest version of the [Brightcove Player SDK][bcovsdk].
2. Download the latest zipped release of the plugin from our [release page][release].
3. Add the 'BrightcoveFW.framework' to your project.
4. On the "Build Settings" tab of your application target, ensure that the "Framework Search Paths" include the path to the framework. This should have been done automatically unless the framework is stored under a different root directory than your project.
5. On the "Build Phases" tab of your application target, add the following to the "Link
    Binary With Libraries" phase:
    * `BrightcoveFW.framework`
6. On the "Build Settings" tab of your application target:
    * Ensure that `-ObjC` has been added to the "Other Linker Flags" build setting.
7. Install the Freewheel library, which must be retrieved from your Freewheel account.

Imports
--------------
The Freewheel Plugin for Brightcove Player SDK can be imported into code a few different ways; `@import BrightcoveFW;`, `#import <BrightcoveFW/BrightcoveIMA.h>` or `#import <BrightcoveFW/[specific class].h>`.

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

Play and Pause
===========
The Brightcove Freewheel Plugin implements custom play and pause logic to ensure the smoothest possible ad experience. Therefore, you will need to make sure that you use the play method on the `BCOVPlaybackController` or the `-[BCOVSessionProviderExtension fw_play]` or `-[BCOVSessionProviderExtension fw_pause]` ([BCOVSessionProviderExtension][BCOVFWComponent]), and not the AVPlayer.  

As an example, calling play for the first time on `BCOVPlaybackController` allows BCOVFW to process preroll ads without any of the content playing before the preroll. For more information on how BCOVFW overrides the default `BCOVPlaybackController` methods, please check out [BCOVSessionProviderExtension][BCOVFWComponent].

[BCOVFWComponent]: https://github.com/brightcove/brightcove-player-sdk-ios-fw/blob/master/Headers/BCOVFWComponent.h

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






