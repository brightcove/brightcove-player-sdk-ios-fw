//
// BCOVFWSessionProvider.h
// BCOVFW
//
// Copyright (c) 2014 Brightcove, Inc. All rights reserved.
// License: https://accounts.brightcove.com/en/terms-and-conditions
//

#import <Foundation/Foundation.h>

#import "BCOVPlayerSDK.h"


@protocol FWContext;


/**
 * Policy that returns a Freewheel context object for the given video and
 * source. Clients of the BCOVFW component must implement this policy to return
 * an appropriately-configured Freewheel context for each video/source/duration. The same
 * context object may be returned for all video/source combinations, if desired.
 *
 * It is possible that the videoDuration will not be known at block execution time
 * and it may contain incorrect data.
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
 * The number of seconds the session provider will wait for an ad request
 * before delivering a session without using ads.
 */
@property (nonatomic, assign) NSTimeInterval adTimeoutInterval;

/**
 * A policy that determines whether a Freewheel ad should be played when a
 * given cue point event occurs, and if so, where playback should resume.
 */
@property (nonatomic, strong) BCOVCuePointProgressPolicy *cuePointProgressPolicy;

@end
