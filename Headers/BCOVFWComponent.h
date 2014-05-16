//
// BCOVFWComponent.h
// BCOVFW
//
// Copyright (c) 2014 Brightcove, Inc. All rights reserved.
// License: https://accounts.brightcove.com/en/terms-and-conditions
//

#import <Foundation/Foundation.h>

#import "BCOVPlayerSDK.h"
#import "BCOVFWSessionProvider.h"


/**
 * Category methods added to BCOVPlayerSDKManager to support FW.
 */
@interface BCOVPlayerSDKManager (BCOVFWAdditions)

- (id<BCOVPlaybackController>)createFWPlaybackControllerWithAdContextPolicy:(BCOVFWSessionProviderAdContextPolicy)adContextPolicy viewStrategy:(BCOVPlaybackControllerViewStrategy)strategy;

- (id<BCOVPlaybackSessionProvider>)createFWSessionProviderWithAdContextPolicy:(BCOVFWSessionProviderAdContextPolicy)adContextPolicy upstreamSessionProvider:(id<BCOVPlaybackSessionProvider>)provider options:(BCOVFWSessionProviderOptions *)options;

@end
