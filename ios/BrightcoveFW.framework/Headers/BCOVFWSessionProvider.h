//
// BCOVFWSessionProvider.h
// BrightcoveFW
//
// Copyright (c) 2016 Brightcove, Inc. All rights reserved.
// License: https://accounts.brightcove.com/en/terms-and-conditions
//

#import <Foundation/Foundation.h>

#import <BrightcovePlayerSDK/BrightcovePlayerSDK.h>


@protocol FWContext;


/**
 * @typedef BCOVFWSessionProviderAdContextPolicy
 *
 * Policy that returns a Freewheel context object for the given video and
 * source. Clients of the BCOVFW component must implement this policy to return
 * an appropriately-configured Freewheel context for each video/source/duration. The same
 * context object may be returned for all video/source combinations, if desired.
 *
 * The videoDuration in seconds is provided for convenience when calling setVideoAssetId: on
 * the ad context. By default, the sdk will wait for the AVPlayerItem to report
 * a duration before executing this block.
 *
 * If you would like to have this block called as soon as possible, either because
 * the duration is available on the BCOVVideo/Source properties dictionary (if retreived
 * from Video Cloud) or you are playing a live stream, set `BCOVFWSessionProviderOptions.waitForAVPlayerDuration = NO`
 * when creating the plugin.
 */
typedef id<FWContext> (^BCOVFWSessionProviderAdContextPolicy)(BCOVVideo *video, BCOVSource *source, NSTimeInterval videoDuration);


/**
 * Session provider implementation that delivers playback sessions with support
 * for Freewheel ads.
 *
 * Instances of this class should not be created directly by clients of the
 * Brightcove Player SDK for iOS; instead use the `-[BCOVPlayerSDKManager createFWSessionProviderWithAdContextPolicy:upstreamSessionProvider:options:]`
 * factory method (which is added as a category method).
 */
@interface BCOVFWSessionProvider : NSObject <BCOVPlaybackSessionProvider>

@end


/**
 * Optional configuration for FW session providers.
 */
@interface BCOVFWSessionProviderOptions : NSObject

/**
 * Whether or not to wait for the AVPlayer item to report a duration before calling
 * the adContextPolicy. When set to NO, the duration passed into the BCOVFWSessionProviderAdContextPolicy
 * block may not be accurate. Defaults to Yes.
 */
@property (nonatomic, assign, getter=shouldWaitForAVPlayerDuration) BOOL waitForAVPlayerDuration;

/**
 * The number of seconds the session provider will wait for an ad request
 * before delivering a session without using ads. Defaults to 3 seconds.
 */
@property (nonatomic, assign) NSTimeInterval adTimeoutInterval;

/**
 * A policy that determines whether a Freewheel ad should be played when a
 * given cue point event occurs, and if so, where playback should resume.
 * Defaults to BCOVProgressPolicyProcessFinalCuePoint, BCOVProgressPolicyResumeFromContentPlayhead, YES.
 */
@property (nonatomic, strong) BCOVCuePointProgressPolicy *cuePointProgressPolicy;

/**
 * Determines whether or not slots are preloaded so they are ready to play when hit.
 * When set to YES, preroll slots will load as soon as possible. Midrolls slots 
 * will load X seconds before the slot is to be played, determined by timeBeforeAdToPreload.
 * Defaults to NO.
 */
@property (nonatomic, assign, getter=shouldPreloadSlots) BOOL preloadSlots;

/**
 * If slot preloading is enabled, this is the amount of seconds before the ad to
 * preload it. Defaults to 10 seconds.
 */
@property (nonatomic, assign) NSTimeInterval timeBeforeAdToPreload;

@end
