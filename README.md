# Freewheel Plugin for Brightcove Player SDK for iOS, version 1.0.3.157

Requirements
===========

This plugin supports iOS 6.1+.

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
    * Ensure that BCOVIFW headers are in your application's "Header Search Path".
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

Customizing Plugin Behavior
===========
You can customize default plugin behavior by creating an instance of `BCOVFWSessionProviderOptions` and overriding the default properties. To use a `BCOVFWSessionProviderOptions` options instance, you need to create the `BCOVFWSessionProvider` using `-[BCOVSDKManager createFWSessionProviderWithAdContextPolicy:upstreamSessionProvider:options:]`.

```

    BCOVFWSessionProviderOptions *options = [[BCOVFWSessionProviderOptions alloc] init];
    options.cuePointProgressPolicy = [BCOVCuePointProgressPolicy progressPolicyProcessingCuePoints:BCOVProgressPolicyProcessFinalCuePoint resumingPlaybackFrom:BCOVProgressPolicyResumeFromContentPlayhead ignoringPreviouslyProcessedCuePoints:YES];
    id<BCOVPlaybackSessionProvider> sessionProvider = [playbackManager createFWSessionProviderWithAdContextPolicy:[self adContextPolicy] upstreamSessionProvider:nil options:options];

    id<BCOVPlaybackController> playbackController = [playbackManager createPlaybackControllerWithSessionProvider:sessionProvider viewStrategy:nil];
```




