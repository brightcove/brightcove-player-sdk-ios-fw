//
// BCOVFWComponent.h
// BrightcoveFW
//
// Copyright (c) 2025 Brightcove, Inc. All rights reserved.
// License: https://accounts.brightcove.com/en/terms-and-conditions
//

#import <Foundation/Foundation.h>

@import BrightcovePlayerSDK;

#import "BCOVFWSessionProvider.h"

extern NSString * const kBCOVFWLifecycleEventAdError;
extern NSString * const kBCOVFWLifecycleEventPropertyKeyAdError;


/**
 * Category methods added to BCOVPlayerSDKManager to support FW.
 */
@interface BCOVPlayerSDKManager (BCOVFWAdditions)

/**
 * Creates a playback controller with the given adContextPolicy and viewStrategy.
 * This method uses a FreeWheel plugin with its default settings. To customize
 * FreeWheel plugin, use 
 * createFWSessionProviderWithAdContextPolicy:upstreamSessionProvider:options.
 *
 * @param adContextPolicy The block that constructs the FWAdContext to be used per video.
 * @param strategy        The view strategy for the playbackController.view property.
 *
 * @return A playback controller.
 */
- (id<BCOVPlaybackController>)createFWPlaybackControllerWithAdContextPolicy:(BCOVFWSessionProviderAdContextPolicy)adContextPolicy viewStrategy:(BCOVPlaybackControllerViewStrategy)strategy;

/**
 * Creates a session provider that can be used to create a playback controller.
 *
 *  @param adContextPolicy The block that constructs the FWAdContext to be used per video.
 *  @param provider        Another provider to be used, if nil, default will be used.
 *  @param options         The options to customize the FreeWheel plugin.
 *
 *  @return A session provider.
 */
- (id<BCOVPlaybackSessionProvider>)createFWSessionProviderWithAdContextPolicy:(BCOVFWSessionProviderAdContextPolicy)adContextPolicy upstreamSessionProvider:(id<BCOVPlaybackSessionProvider>)provider options:(BCOVFWSessionProviderOptions *)options;

@end

/**
 * FreeWheel specific methods for the plugin context.
 */
@interface BCOVSessionProviderExtension (BCOVFWAdditions)

/**
 * If FreeWheel has requested that the content video be in the paused state,
 * this will be YES. Otherwise it will be NO.
 */
@property (nonatomic, assign, readonly, getter=isPausedOnFreeWheelRequest) BOOL pausedOnFreeWheelRequest;

/**
 * Plays the video and updates FreeWheel with a video state of FW_VIDEO_STATE_PLAYING.
 * This method will only play the video if isPausedOnFreewheelRequest returns NO.
 *
 * On first play, before playing content, the session will process all preroll ads before starting
 * the content.
 */
- (void)fw_play;

/**
 * Pauses the video and updates FreeWheel with a video state of FW_VIDEO_STATE_PAUSED.
 */
- (void)fw_pause;

@end
